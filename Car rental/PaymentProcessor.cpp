#ifndef PAYMENTPROCESSOR_H
#define PAYMENTPROCESSOR_H
#include<bits/stdc++.h>

class PaymentProcessor {
public:
    virtual ~PaymentProcessor() = default; // Virtual destructor
    virtual bool processPayment(double amount) = 0; // Pure virtual function
};

#endif // PAYMENTPROCESSOR_H
