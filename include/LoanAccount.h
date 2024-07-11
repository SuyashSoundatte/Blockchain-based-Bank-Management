#ifndef LoanAccount_H
#define LoanAccount_H

#pragma once

#include "BankUser.h"
#include "Transaction.h"
#include "CurrentTime.h"

class LoanAccount : public BankUser {
private:
    double loanAmount;
    double interestRate;
    int tenure; // in months
    double emi;
    TransactionHistory transactionHistory;

public:
    LoanAccount(const std::string& accNum, const std::string& accHolderName, const std::string& pin, double loanAmt, double interest, int tenure);

    void calculateEMI();

    void makePayment(double amount);

    void getLoanDetails() const;
};

#endif