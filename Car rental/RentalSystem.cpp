#ifndef RENTALSYSTEM_H
#define RENTALSYSTEM_H

#include "Car.cpp"
#include "Customer.cpp"
#include "Reservation.cpp"
#include "CreditCardPaymentProcessor.cpp"
#include "PaymentProcessor.cpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <ctime>
#include <iostream>

class RentalSystem {
private:
    static RentalSystem* instance;
    std::unordered_map<std::string, Car> cars;
    std::unordered_map<std::string, Reservation> reservations;
    PaymentProcessor* paymentProcessor;

    RentalSystem() : paymentProcessor(new CreditCardPaymentProcessor()) {}

public:
    static RentalSystem* getInstance() {
        if (!instance) {
            instance = new RentalSystem();
        }
        return instance;
    }

    void addCar(const Car& car) {
        cars[car.getLicensePlate()] = car;
    }

    void removeCar(const std::string& licensePlate) {
        cars.erase(licensePlate);
    }

    std::vector<Car> searchCars(const std::string& make, const std::string& model,
                                const std::chrono::system_clock::time_point& startDate,
                                const std::chrono::system_clock::time_point& endDate) {
        std::vector<Car> availableCars;
        for (const auto& [licensePlate, car] : cars) {
            if (car.getMake() == make && car.getModel() == model && car.isAvailable()) {
                if (isCarAvailable(car, startDate, endDate)) {
                    availableCars.push_back(car);
                }
            }
        }
        return availableCars;
    }

    bool isCarAvailable(const Car& car, const std::chrono::system_clock::time_point& startDate,
                        const std::chrono::system_clock::time_point& endDate) {
        for (const auto& [reservationId, reservation] : reservations) {
            if (reservation.getCar().getLicensePlate() == car.getLicensePlate()) {
                auto resStart = reservation.getStartDate();
                auto resEnd = reservation.getEndDate();
                if (startDate < resEnd && endDate > resStart) {
                    return false;
                }
            }
        }
        return true;
    }

    Reservation* makeReservation(const Customer& customer, const Car& car,
                                 const std::chrono::system_clock::time_point& startDate,
                                 const std::chrono::system_clock::time_point& endDate) {
        if (isCarAvailable(car, startDate, endDate)) {
            std::string reservationId = generateReservationId();
            Reservation reservation(reservationId, customer, car, startDate, endDate);
            reservations[reservationId] = reservation;
            const_cast<Car&>(car).setAvailable(false); // We need to cast away const for setting availability
            return &reservations[reservationId];
        }
        return nullptr;
    }

    void cancelReservation(const std::string& reservationId) {
        auto it = reservations.find(reservationId);
        if (it != reservations.end()) {
            Reservation& reservation = it->second;
            reservation.getCar().setAvailable(true);
            reservations.erase(it);
        }
    }

    bool processPayment(const Reservation& reservation) {
        return paymentProcessor->processPayment(reservation.getTotalPrice());
    }

private:
    std::string generateReservationId() {
        static std::mt19937 rng(std::time(nullptr));
        static std::uniform_int_distribution<int> dist(0, 99999999);
        return "RES" + std::to_string(dist(rng));
    }
};

RentalSystem* RentalSystem::instance = nullptr;

#endif // RENTALSYSTEM_H
