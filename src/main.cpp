/**

@file main.cpp

@brief Entry point for the Movie Ticket Booking REST API server.

This file sets up a RESTful server using httplib that allows users to:

View movies

View theaters for a movie

View seats in a theater

Book seats

Cancel bookings

It also manages session IDs for secure seat booking and cancellation.
*/


#include "BookingService.h"
#include "../third_party/httplib.h"
#include "../third_party/json/json.hpp"
#include <iostream>
#include <unordered_set>
#include <random>
#include <chrono>

using json = nlohmann::json;
std::mutex sessionMutex;

/**
 * @brief Generates a random alphanumeric session ID.
 *
 * @return A 12-character session ID string.
 */
std::string generateSessionId() {
    static const char charset[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);
    std::string sid;
    for (int i = 0; i < 12; ++i)
        sid += charset[dist(rng)];
    return sid;
}

/**
 * @brief Main function to initialize and run the booking server.
 *
 * Sets up REST API endpoints and starts the HTTP server.
 *
 * @return int Exit code.
 */

int main() {
    BookingService service;
    httplib::Server svr;

    /// Holds valid session IDs for active clients
    std::unordered_set<std::string> validSessions;

    /**
     * @brief Endpoint: GET /movies
     *
     * Returns a list of movies along with a new session ID.
     */
    svr.Get("/movies", [&](const httplib::Request& req, httplib::Response& res) {
        std::string sessionId;

        // Check for existing session
        auto it = req.headers.find("X-Session-Id");
        {
            std::lock_guard<std::mutex> lock(sessionMutex);
            if (it != req.headers.end() && validSessions.count(it->second)) {
                sessionId = it->second;
            }
            else {
                sessionId = generateSessionId();
                validSessions.insert(sessionId);
            }
        }

        json response;
        for (const auto& movie : service.viewMovies()) {
            response["movies"].push_back({ {"id", movie.id}, {"title", movie.title} });
        }
        response["sessionId"] = sessionId;
        res.set_content(response.dump(), "application/json");
        });

    /**
    * @brief Endpoint: GET /theaters/{movieId}
    *
    * Returns a list of theaters showing the specified movie.
    */
    svr.Get(R"(/theaters/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        int movieId = std::stoi(req.matches[1]);
        auto theaters = service.viewTheaters(movieId);

        json response = json::array();
        for (const auto& theater : theaters) {
            response.push_back({ {"id", theater.id}, {"name", theater.name} });
        }

        res.set_content(response.dump(), "application/json");
        });

    /**
     * @brief Endpoint: GET /seats/{movieId}/{theaterId}
     *
     * Returns a map of seat availability for the given movie and theater.
     */
    svr.Get(R"(/seats/(\d+)/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        int movieId = std::stoi(req.matches[1]);
        int theaterId = std::stoi(req.matches[2]);

        auto seats = service.viewSeats(movieId, theaterId);
        res.set_content(json(seats).dump(), "application/json");
        });

    /**
     * @brief Endpoint: POST /book/{movieId}/{theaterId}
     *
     * Books the specified seats if the session ID is valid and seats are available.
     *
     * @param sessionId Session identifier
     * @param seats Array of seat identifiers
     */
    svr.Post(R"(/book/(\d+)/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        int movieId = std::stoi(req.matches[1]);
        int theaterId = std::stoi(req.matches[2]);

        auto j = json::parse(req.body);
        if (!j.contains("sessionId") || !j.contains("seats")) {
            res.status = 400;
            res.set_content(R"({"error":"Missing sessionId or seats"})", "application/json");
            return;
        }

        std::string sessionId = j["sessionId"];
        if (validSessions.find(sessionId) == validSessions.end()) {
            res.status = 401;
            res.set_content(R"({"error":"Invalid sessionId"})", "application/json");
            return;
        }

        std::vector<std::string> seats = j["seats"].get<std::vector<std::string>>();
        bool success = service.book(movieId, theaterId, seats, sessionId);  // pass sessionId here

        json response = {
            {"success", success}
        };

        if (success) {
            std::string bookingId = "BK" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
            response["bookingId"] = bookingId;
        }

        res.set_content(response.dump(), "application/json");
        });

    /**
     * @brief Endpoint: POST /cancel/{movieId}/{theaterId}
     *
     * Cancels the specified seats if the session ID is valid.
     *
     * @param sessionId Session identifier
     * @param seats Array of seat identifiers
     */
    svr.Post(R"(/cancel/(\d+)/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        int movieId = std::stoi(req.matches[1]);
        int theaterId = std::stoi(req.matches[2]);

        auto j = json::parse(req.body);
        if (!j.contains("sessionId") || !j.contains("seats")) {
            res.status = 400;
            res.set_content(R"({"error":"Missing sessionId or seats"})", "application/json");
            return;
        }

        std::string sessionId = j["sessionId"];
        if (validSessions.find(sessionId) == validSessions.end()) {
            res.status = 401;
            res.set_content(R"({"error":"Invalid sessionId"})", "application/json");
            return;
        }

        std::vector<std::string> seats = j["seats"].get<std::vector<std::string>>();
        bool success = service.cancel(movieId, theaterId, seats, sessionId);  // pass sessionId here

        res.set_content(json({ {"success", success} }).dump(), "application/json");
        });

    std::cout << "Booking server running on http://localhost:8080\n";
    /// Start listening on all network interfaces on port 8080
    svr.listen("0.0.0.0", 8080);

    return 0;
}
