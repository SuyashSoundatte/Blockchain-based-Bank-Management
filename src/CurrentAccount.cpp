#include "include/CurrentAccount.h"

CurrentAccount::CurrentAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal)
        : BankUser(accNum, accHolderName, atmPin), balance(bal), minBalance(50000) {}

double CurrentAccount::checkBalance() const {
        return balance;
    }  

void CurrentAccount::deposit(double amount) {
        if (amount < 0) {
            throw DepositAmountNegative();
        }
        balance += amount;
        recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(),"Deposit", amount, "Deposited amount");
    }

void CurrentAccount::withdraw(double amount) {
        if (amount <= balance && balance<50000) {
            balance -= amount;
            recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(),"Withdrawal", amount, "Withdrawn amount");
        } else {
            throw InsufficientFundsException();
        }
    }

std::string CurrentAccount::getAccountInfo() const {
        return "Account Number: " + getAccountNumber() + "\nAccount Holder: " + getAccountHolderName() + "\nBalance: ₹" + std::to_string(balance);
    }

void CurrentAccount::recordTransaction(std::string transactionId, const std::string date ,const std::string& type, double amount, const std::string& description) {
        transactions.emplace_back(transactionId ,date ,type, amount, description);
    }

void CurrentAccount::displayTransactionHistory() const {
        for (const auto& transaction : transactions) {
            std::cout << transaction.getTransactionDetails() << std::endl;
        }
    }

CurrentAccount::~CurrentAccount() {}



int CurrentAccount::transactionCounter = 0;