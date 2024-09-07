#ifndef CREDITCARDBPAYMENTPROCESSOR_H
#define CREDITCARDBPAYMENTPROCESSOR_H

#include "PaymentProcessor.cpp"
#include<bits/stdc++.h>
class CreditCardPaymentProcessor : public PaymentProcessor {
public:
    bool processPayment(double amount) override {
        // Process credit card payment
        // ...
        std::cout << "Processing credit card payment of amount: " << amount << std::endl;
        return true;
    }
};

#endif // CREDITCARDBPAYMENTPROCESSOR_H
