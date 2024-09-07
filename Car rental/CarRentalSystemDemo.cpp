#include "RentalSystem.cpp"
#include "Car.cpp"
#include "Customer.cpp"
#include "Reservation.cpp"
#include <iostream>
#include <chrono>

void runCarRentalSystemDemo() {
    RentalSystem* rentalSystem = RentalSystem::getInstance();

    // Add cars to the rental system
    rentalSystem->addCar(Car("Toyota", "Camry", 2022, "ABC123", 50.0));
    rentalSystem->addCar(Car("Honda", "Civic", 2021, "XYZ789", 45.0));
    rentalSystem->addCar(Car("Ford", "Mustang", 2023, "DEF456", 80.0));

    // Create customers
    Customer customer1("John Doe", "john@example.com", "DL1234");

    // Make reservations
    auto now = std::chrono::system_clock::now();
    auto startDate = now;
    auto endDate = now + std::chrono::hours(72); // 3 days later

    auto availableCars = rentalSystem->searchCars("Toyota", "Camry", startDate, endDate);
    if (!availableCars.empty()) {
        Car selectedCar = availableCars.front();
        Reservation* reservation = rentalSystem->makeReservation(customer1, selectedCar, startDate, endDate);
        if (reservation) {
            bool paymentSuccess = rentalSystem->processPayment(*reservation);
            if (paymentSuccess) {
                std::cout << "Reservation successful. Reservation ID: " << reservation->getReservationId() << std::endl;
            } else {
                std::cout << "Payment failed. Reservation canceled." << std::endl;
                rentalSystem->cancelReservation(reservation->getReservationId());
            }
        } else {
            std::cout << "Selected car is not available for the given dates." << std::endl;
        }
    } else {
        std::cout << "No available cars found for the given criteria." << std::endl;
    }
}

int main() {
    runCarRentalSystemDemo();
    return 0;
}
