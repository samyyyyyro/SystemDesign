#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class Customer {
private:
    std::string name;
    std::string contactInfo;
    std::string driversLicenseNumber;

public:
    Customer(std::string name, std::string contactInfo, std::string driversLicenseNumber)
        : name(name), contactInfo(contactInfo), driversLicenseNumber(driversLicenseNumber) {}

    std::string getName() const { return name; }
    std::string getContactInfo() const { return contactInfo; }
    std::string getDriversLicenseNumber() const { return driversLicenseNumber; }
};

#endif // CUSTOMER_H
