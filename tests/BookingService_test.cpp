#define BOOST_TEST_MODULE BookingServiceTest
#include <boost/test/included/unit_test.hpp>
#include "BookingService.h"

BOOST_AUTO_TEST_CASE(ViewMoviesTest) {
    BookingService service;
    auto movies = service.viewMovies();
    BOOST_CHECK_EQUAL(movies.size(), 2);
    BOOST_CHECK_EQUAL(movies[0].title, "Avengers");
    BOOST_CHECK_EQUAL(movies[1].title, "Inception");
}

BOOST_AUTO_TEST_CASE(ViewTheatersTest) {
    BookingService service;
    auto theaters = service.viewTheaters(1);
    BOOST_CHECK_EQUAL(theaters.size(), 2);
    BOOST_CHECK(theaters[0].name.find("Theater") != std::string::npos);
    BOOST_CHECK(theaters[1].name.find("Theater") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(ViewSeatsTest) {
    BookingService service;
    auto seats = service.viewSeats(1, 1);
    BOOST_CHECK_EQUAL(seats.size(), 20);
    BOOST_CHECK(seats.find("A1") != seats.end());
}

BOOST_AUTO_TEST_CASE(BookAndCancelTest) {
    BookingService service;

    // Mock a session ID
    std::string sessionId = "test-session-123";

    std::vector<std::string> seats = { "A1", "A2" };

    // Book seats with sessionId
    bool booked = service.book(1, 1, seats, sessionId);
    BOOST_CHECK(booked);

    auto updatedSeats = service.viewSeats(1, 1);
    BOOST_CHECK(updatedSeats.at("A1") == true);
    BOOST_CHECK(updatedSeats.at("A2") == true);

    // Cancel seats with same sessionId
    bool canceled = service.cancel(1, 1, seats, sessionId);
    BOOST_CHECK(canceled);

    updatedSeats = service.viewSeats(1, 1);
    BOOST_CHECK(updatedSeats.at("A1") == false);
    BOOST_CHECK(updatedSeats.at("A2") == false);
}

BOOST_AUTO_TEST_CASE(BookFailOnAlreadyBookedSeat) {
    BookingService service;

    std::string session1 = "session1";
    std::string session2 = "session2";
    std::vector<std::string> seats = { "A3" };

    // Session1 books seat A3
    bool booked1 = service.book(1, 1, seats, session1);
    BOOST_CHECK(booked1);

    // Session2 tries to book same seat it should fail
    bool booked2 = service.book(1, 1, seats, session2);
    BOOST_CHECK(!booked2);
}

BOOST_AUTO_TEST_CASE(CancelFailIfNotOwner) {
    BookingService service;

    std::string session1 = "session1";
    std::string session2 = "session2";
    std::vector<std::string> seats = { "A4" };

    // Session1 books seat A4
    bool booked1 = service.book(1, 1, seats, session1);
    BOOST_CHECK(booked1);

    // Session2 tries to cancel seat A4 it should fail as session2 does not own it
    bool canceled = service.cancel(1, 1, seats, session2);
    BOOST_CHECK(!canceled);
}
