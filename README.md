# Movie Ticket Booking System

This is a cross-platform **movie ticket booking system** developed using a **C++14 backend** and a **Qt 6 QML frontend**. The backend provides a RESTful API, while the frontend allows users to view movies, select theaters, and book seats.

---

## Prerequisites

Ensure the following tools are installed before building and running the project:

- **Visual Studio 2017** or newer (with MSVC v141 or later) – for Windows
- **Qt Creator** 6.9.0 or newer (with MinGW or GCC)
- **CMake** 3.16 or newer
- **g++** and **make** (for Linux)
- **boost_1_88_0.zip*** (Download boost_1_88_0.zip from https://www.boost.org/releases/latest/, extract it, and place the entire folder inside the third_party/ directory (i.e., third_party/boost_1_88_0/)

---

## Backend Setup (C++ Server)

### On **Windows**

1. Open a terminal and navigate to the root directory of the project:
    ```bash
    cd path\to\project
    ```

2. Create a build directory and enter it:
    ```bash
    mkdir build && cd build
    ```

3. Generate Visual Studio solution files using CMake:
    ```bash
    cmake ..
    ```

4. Open the generated `MovieBooking.sln` file in Visual Studio.

5. Set the `movie_booking` project as the **Startup Project**.

6. Build and run the project.

---

### On **Linux (Ubuntu)**

1. Open a terminal and navigate to the root directory of the project:
    ```bash
    cd ~/path/to/project
    ```

2. Create a build directory and enter it:
    ```bash
    mkdir build && cd build
    ```

3. Generate Makefiles using CMake:
    ```bash
    cmake ..
    ```

4. Build the project:
    ```bash
    make -j$(nproc)
    ```

5. Run the executable:
    ```bash
    ./movie_booking
    ```

> By default, the backend server runs on **port 8080**. You can change it in `main.cpp`.

---

## Frontend Setup (Qt QML App)

1. Navigate to `Examples/MovieBookingApp` and open the project in **Qt Creator**.

2. Select an appropriate kit:
    - Use **MinGW** on Windows
    - Use **GCC** on Linux

3. Configure, build, and run the application.

---

## Application UI

The application consists of three sections:

- **Movie List**
- **Theater List** (based on selected movie)
- **Seat Grid** (click to book)

### User Flow

1. The app fetches a list of available movies from the backend.
2. The user selects a movie and a theater.
3. The app displays available seats.
4. Clicking on a seat sends a booking request to the REST API.

---

## Running Unit Tests (Boost Test)

### On **Windows**

1. Navigate to the `test` directory and create a build directory:
    ```bash
    cd test
    mkdir build && cd build
    ```

2. Generate the test solution using CMake:
    ```bash
    cmake ..
    ```

3. Open `BookingService_test.sln` in Visual Studio.

4. Set `BookingService_test` as the **Startup Project**.

5. Build and run to execute the tests.

---

### On **Linux (Ubuntu)**

1. Navigate to the `test` directory:
    ```bash
    cd test
    mkdir build && cd build
    ```

2. Generate Makefiles using CMake:
    ```bash
    cmake ..
    ```

3. Build the test executable:
    ```bash
    make -j$(nproc)
    ```

4. Run the test binary:
    ```bash
    ./BookingService_test
    ```

---

## Backend API Testing with `curl`

API Testing with curl

Use these curl commands to interact with the Ticket Booking REST API:

Fetch all movies
This also returns a sessionId you need for booking or canceling seats.
curl http://localhost:8080/movies

Fetch theaters for a movie (Movie ID: 1)
curl http://localhost:8080/theaters/1

Fetch seat availability for a movie-theater pair (Movie ID: 1, Theater ID: 1)
curl http://localhost:8080/seats/1/1

Book seats
Replace user123 with the actual sessionId from the /movies response.
curl -X POST http://localhost:8080/book/1/1 -H "Content-Type: application/json" -d "{\"sessionId\":\"user123\",\"seats\":[\"A1\",\"A2\"]}"

Cancel seats
Use the same sessionId you used when booking.
curl -X POST http://localhost:8080/cancel/1/1 -H "Content-Type: application/json" -d "{\"sessionId\":\"user123\",\"seats\":[\"A1\",\"A2\"]}"


---

## Project Structure

include/ → Header files
src/ → Backend source files
Examples/MovieBookingApp/ → Qt QML frontend application
test/ → Unit tests using Boost
thirdparty/ → External libraries (e.g., Boost)
build/ → Build directory (created manually)
CMakeLists.txt → CMake build script
README.md → Project documentation

## Summary

- Start the backend with `movie_booking`.
- Launch the frontend QML app in Qt Creator.
- Select a movie, theater, and book seats through the UI.
- Use unit tests to validate backend logic.
- Use `curl` for lightweight API testing.