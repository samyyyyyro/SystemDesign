#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <random>
#include <ctime>
#include <memory>
#include <iomanip>
#include <sstream>

// PaymentProcessor.h
class PaymentProcessor {
public:
    virtual ~PaymentProcessor() = default;
    virtual bool processPayment(double amount) = 0;
};

// CreditCardPaymentProcessor.h
class CreditCardPaymentProcessor : public PaymentProcessor {
public:
    bool processPayment(double amount) override {
        std::cout << "Processing credit card payment of amount: " << amount << std::endl;
        return true;
    }
};

// Car.h
class Car {
private:
    std::string make;
    std::string model;
    int year;
    std::string licensePlate;
    double rentalPricePerDay;
    bool available;

public:
    Car() : make(""), model(""), year(0), licensePlate(""), rentalPricePerDay(0.0), available(true) {}
    
    Car(std::string make, std::string model, int year, std::string licensePlate, double rentalPricePerDay)
        : make(make), model(model), year(year), licensePlate(licensePlate), rentalPricePerDay(rentalPricePerDay), available(true) {}

    double getRentalPricePerDay() const { return rentalPricePerDay; }
    std::string getLicensePlate() const { return licensePlate; }
    std::string getMake() const { return make; }
    std::string getModel() const { return model; }
    bool isAvailable() const { return available; }
    void setAvailable(bool available) { this->available = available; }
};

// Customer.h
class Customer {
private:
    std::string name;
    std::string contactInfo;
    std::string driversLicenseNumber;

public:
    Customer() : name(""), contactInfo(""), driversLicenseNumber("") {}
    
    Customer(std::string name, std::string contactInfo, std::string driversLicenseNumber)
        : name(name), contactInfo(contactInfo), driversLicenseNumber(driversLicenseNumber) {}

    std::string getName() const { return name; }
    std::string getContactInfo() const { return contactInfo; }
    std::string getDriversLicenseNumber() const { return driversLicenseNumber; }
};

// Reservation.h
class Reservation {
private:
    std::string reservationId;
    Customer customer;
    Car car;
    std::chrono::system_clock::time_point startDate;
    std::chrono::system_clock::time_point endDate;
    double totalPrice;

    double calculateTotalPrice() const {
        auto duration = std::chrono::duration_cast<std::chrono::hours>(endDate - startDate).count() / 24 + 1;
        return car.getRentalPricePerDay() * duration;
    }

public:
    Reservation() : reservationId(""), customer(), car(), startDate(), endDate(), totalPrice(0.0) {}

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

// RentalSystem.h
class RentalSystem {
private:
    static RentalSystem* instance;
    std::unordered_map<std::string, Car> cars;
    std::unordered_map<std::string, Reservation> reservations;
    std::unique_ptr<PaymentProcessor> paymentProcessor;

    RentalSystem() : paymentProcessor(std::make_unique<CreditCardPaymentProcessor>()) {}

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
            const_cast<Car&>(car).setAvailable(false);
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

// Main program
int main() {
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

    return 0;
}
