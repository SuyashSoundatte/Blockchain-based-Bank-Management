#ifndef BankUser_H
#define BankUser_H

#pragma once

#include <string>
#include <iostream>

class BankUser {
private:
    std::string accountNumber;
    std::string accountHolderName;
    std::string atmPin;

public:
    BankUser(std::string accNum, std::string accHolderName, std::string pin);

    std::string getAccountNumber() const;

    std::string getAccountHolderName() const;

    void getAccountHolderDetails(std::string inputPin);
};

#endif