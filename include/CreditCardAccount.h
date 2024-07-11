#ifndef CreditCardAccount_H
#define CreditCardAccount_H

#include <iostream>

#include "include/Transaction.h"
#include "include/BankUser.h"
#include "include/CustomException.h"
#include "include/CurrentTime.h"

class CreditCardAccount : public BankUser {
private:
    double creditLimit;
    double currentSpending;
    std::string dueDate;
    double interestRate;
    TransactionHistory transactionHistory;

public:
    CreditCardAccount(const std::string& accNum, const std::string& accHolderName, const std::string& pin, double limit, double interest);

    void makePurchase(double amount);

    void makePayment(double amount);

    void calculateInterest();

    void getCreditCardDetails() const;
};

#endif