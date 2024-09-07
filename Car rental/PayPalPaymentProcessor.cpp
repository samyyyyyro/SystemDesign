#ifndef PAYPALPAYMENTPROCESSOR_H
#define PAYPALPAYMENTPROCESSOR_H

#include "PaymentProcessor.cpp"

class PayPalPaymentProcessor : public PaymentProcessor {
public:
    bool processPayment(double amount) override {
        // Process PayPal payment
        // ...
        std::cout << "Processing PayPal payment of amount: " << amount << std::endl;
        return true;
    }
};

#endif // PAYPALPAYMENTPROCESSOR_H
