#ifndef Transction_H
#define Transaction_H

#pragma once

#include "TransactionIDGenerator.h"
#include <vector>
#include <iostream>
#include <string>

// TransactionHistory class
class TransactionHistory {
private:
    std::vector<Transaction> transactions;

public:
    void addTransaction(const Transaction& transaction);

    void displayTransactionHistory() const;
};

// Transaction class
class Transaction {
private:
    std::string transactionId;
    std::string date;
    std::string typeOfTransaction;
    double amount;
    std::string description;

public:
    Transaction(const std::string transactionId, const std::string& date, const std::string& type, double amt, const std::string& desc);

    std::string getTransactionDetails() const {
        return "Transaction ID: " + transactionId + "\nDate: " + date + "\nType: " + typeOfTransaction + "\nAmount: ₹" + std::to_string(amount) + "\nDescription: " + description;
    }
};

#endif