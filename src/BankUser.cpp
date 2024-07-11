#include "include/BankUser.h"

BankUser::BankUser(std::string accNum, std::string accHolderName, std::string pin)
    : accountNumber(accNum), accountHolderName(accHolderName), atmPin(pin) {}

std::string BankUser::getAccountNumber() const
{
        return accountNumber;
}

std::string BankUser::getAccountHolderName() const
{
        return accountHolderName;
}

void BankUser::getAccountHolderDetails(std::string inputPin)
{
        if (inputPin == atmPin)
        {
                std::cout << "Account Holder Name: " << accountHolderName << "\nAccount Number: " << accountNumber << std::endl;
        }
        else
        {
                std::cout << "Invalid PIN." << std::endl;
        }
}