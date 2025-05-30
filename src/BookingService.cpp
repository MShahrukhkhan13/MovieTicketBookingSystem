#include "BookingService.h"
#include <algorithm>

BookingService::BookingService() {
    initSampleData();
}

void BookingService::initSampleData() {
    std::lock_guard<std::mutex> lock(mtx_);

    movies_ = {
        {1, "Avengers"},
        {2, "Inception"}
    };

    std::vector<std::pair<int, int>> movieTheaterPairs = {
        {1, 1},
        {1, 2},
        {2, 2}
    };

    for (const auto& [movieId, theaterId] : movieTheaterPairs) {
        TheaterSeats ts;
        for (int i = 1; i <= 20; ++i) {
            std::string seatName = "A" + std::to_string(i);
            ts.seatBooked[seatName] = false;            
        }
        data_[movieId][theaterId] = ts;
    }
}

std::vector<Movie> BookingService::viewMovies() {
    return movies_;
}

std::vector<Theater> BookingService::viewTheaters(int movieId) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<Theater> theaters;
    if (data_.count(movieId)) {
        for (const auto& [theaterId, seats] : data_[movieId]) {
            theaters.push_back({ theaterId, "Theater " + std::to_string(theaterId) });
        }
    }
    return theaters;
}

std::unordered_map<std::string, bool> BookingService::viewSeats(int movieId, int theaterId) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::unordered_map<std::string, bool> seatMap;

    if (data_.count(movieId) && data_[movieId].count(theaterId)) {
        const auto& ts = data_[movieId][theaterId];
        seatMap = ts.seatBooked;
    }
    return seatMap;
}

bool BookingService::book(int movieId, int theaterId, const std::vector<std::string>& seats, const std::string& sessionId) {
    std::lock_guard<std::mutex> lock(mtx_);

    if (!data_.count(movieId) || !data_[movieId].count(theaterId))
        return false;

    auto& ts = data_[movieId][theaterId];

    // Check availability: if seat exists, not booked
    for (const auto& s : seats) {
        if (ts.seatBooked.count(s) == 0 || ts.seatBooked[s])
            return false;
    }
    // Book all seats and record session ownership
    for (const auto& s : seats) {
        ts.seatBooked[s] = true;
        ts.ownerSession[s] = sessionId;
    }

    return true;
}

bool BookingService::cancel(int movieId, int theaterId, const std::vector<std::string>& seats, const std::string& sessionId) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (!data_.count(movieId) || !data_[movieId].count(theaterId))
        return false;

    auto& ts = data_[movieId][theaterId];

    // Check: seat exists, booked, and owned by sessionId
    for (const auto& s : seats) {
        if (ts.seatBooked.count(s) == 0 || !ts.seatBooked[s] || ts.ownerSession[s] != sessionId)
            return false;
    }

    // Cancel seats and remove session ownership
    for (const auto& s : seats) {
        ts.seatBooked[s] = false;
        ts.ownerSession.erase(s);
    }

    return true;
}
