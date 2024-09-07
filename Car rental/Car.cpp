#ifndef CAR_H
#define CAR_H

#include <string>

class Car {
private:
    std::string make;
    std::string model;
    int year;
    std::string licensePlate;
    double rentalPricePerDay;
    bool available;

public:
    Car(std::string make, std::string model, int year, std::string licensePlate, double rentalPricePerDay)
        : make(make), model(model), year(year), licensePlate(licensePlate), rentalPricePerDay(rentalPricePerDay), available(true) {}

    double getRentalPricePerDay() const { return rentalPricePerDay; }
    std::string getLicensePlate() const { return licensePlate; }
    std::string getMake() const { return make; }
    std::string getModel() const { return model; }
    bool isAvailable() const { return available; }
    void setAvailable(bool available) { this->available = available; }
};

#endif // CAR_H
