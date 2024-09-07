#ifndef RESERVATION_H
#define RESERVATION_H

#include "Car.cpp"
#include "Customer.cpp"
#include <chrono>
#include <string>

class Reservation {
private:
    std::string reservationId;
    Customer customer;
    Car car;
    std::chrono::system_clock::time_point startDate;
    std::chrono::system_clock::time_point endDate;
    double totalPrice;

    double calculateTotalPrice() const {
        auto duration = std::chrono::duration_cast<std::chrono::days>(endDate - startDate).count() + 1;
        return car.getRentalPricePerDay() * duration;
    }

public:
    Reservation(std::string reservationId, Customer customer, Car car,
                std::chrono::system_clock::time_point startDate, std::chrono::system_clock::time_point endDate)
        : reservationId(reservationId), customer(customer), car(car),
          startDate(startDate), endDate(endDate), totalPrice(calculateTotalPrice()) {}

    std::chrono::system_clock::time_point getStartDate() const { return startDate; }
    std::chrono::system_clock::time_point getEndDate() const { return endDate; }
    Car getCar() const { return car; }
    double getTotalPrice() const { return totalPrice; }
    std::string getReservationId() const { return reservationId; }
};

#endif // RESERVATION_H
