#ifndef CurrentAccount_H
#define CurrentAccount_H

#include <iostream>
#include <vector>

#include "include/BankUser.h"
#include "include/Transaction.h"
#include "include/CustomException.h"
#include "include/CurrentTime.h"


class CurrentAccount : public BankUser {
private:
    std::vector<Transaction> transactions;
    static int transactionCounter;
    double minBalance;

protected:
    double balance;

public:
    CurrentAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal);

    virtual double checkBalance() const;

    void deposit(double amount);

    virtual void withdraw(double amount);

    std::string getAccountInfo() const;

    void recordTransaction(std::string transactionId, const std::string date ,const std::string& type, double amount, const std::string& description);

    void displayTransactionHistory() const;

    virtual ~CurrentAccount();
};


#endif