/**
 * @file BookingService.h
 * @brief Declares the data structures and logic for managing movie booking services.
 *
 * Provides functionality to:
 * - View movies and theaters
 * - View and manage seat availability
 * - Book and cancel seats using session-based tracking
 */

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

 /**
  * @struct Movie
  * @brief Represents a movie with an ID and title.
  */
struct Movie {
    int id;
    std::string title;
};

/**
 * @struct Theater
 * @brief Represents a theater with an ID and name.
 */
struct Theater {
    int id;
    std::string name;
};

/**
 * @class BookingService
 * @brief Provides the core functionality for movie booking and seat management.
 */
class BookingService {
public:
    /**
     * @brief Constructs the booking service and loads sample data.
     */
    BookingService();

    /**
     * @brief Returns the list of all available movies.
     *
     * @return A vector of Movie structs.
     */
    std::vector<Movie> viewMovies();

    /**
     * @brief Returns the list of theaters showing a given movie.
     *
     * @param movieId The ID of the movie.
     * @return A vector of Theater structs.
     */
    std::vector<Theater> viewTheaters(int movieId);

    /**
     * @brief Returns the availability status of seats for a movie-theater pair.
     *
     * @param movieId The ID of the movie.
     * @param theaterId The ID of the theater.
     * @return A map of seat labels to availability (true = available, false = booked).
     */
    std::unordered_map<std::string, bool> viewSeats(int movieId, int theaterId);

    /**
     * @brief Attempts to book the specified seats for a given movie and theater.
     *
     * Only books if all seats are available. Associates booking with the session ID.
     *
     * @param movieId The ID of the movie.
     * @param theaterId The ID of the theater.
     * @param seats List of seat labels to book.
     * @param sessionId Session ID of the client booking the seats.
     * @return True if all seats were booked successfully, false otherwise.
     */
    bool book(int movieId, int theaterId, const std::vector<std::string>& seats, const std::string& sessionId);

    /**
     * @brief Cancels the booking for the specified seats.
     *
     * Only the session that originally booked the seats can cancel them.
     *
     * @param movieId The ID of the movie.
     * @param theaterId The ID of the theater.
     * @param seats List of seat labels to cancel.
     * @param sessionId Session ID of the client requesting the cancellation.
     * @return True if all seats were canceled successfully, false otherwise.
     */
    bool cancel(int movieId, int theaterId, const std::vector<std::string>& seats, const std::string& sessionId);

private:
    std::mutex mtx_; ///< Mutex for thread-safe access to booking data

    /**
    * @struct TheaterSeats
    * @brief Stores seat status and booking ownership for a theater.
    */
    struct TheaterSeats {
        std::unordered_map<std::string, bool> seatBooked;              // seat -> booked or not
        std::unordered_map<std::string, std::string> ownerSession;     // seat -> sessionId that owns booking
    };

    /**
     * @brief Booking data structured as: movieId -> theaterId -> seat information.
     */
    std::unordered_map<int, std::unordered_map<int, TheaterSeats>> data_;
    std::vector<Movie> movies_; ///< List of available movies

    /**
     * @brief Initializes sample data for movies, theaters, and seats.
     */
    void initSampleData();
};
