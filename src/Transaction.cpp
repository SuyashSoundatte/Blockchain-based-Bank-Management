#include "include/Transaction.h"

void TransactionHistory::addTransaction(const Transaction& transaction) {
        transactions.push_back(transaction);
    }

void TransactionHistory::displayTransactionHistory() const {
        for (const auto& transaction : transactions) {
            std::cout << transaction.getTransactionDetails() << std::endl;
        }
    }

Transaction::Transaction(const std::string transactionId, const std::string& date, const std::string& type, double amt, const std::string& desc)
        : transactionId(TransactionIDGenerator::generateTransactionID()), date(date), typeOfTransaction(type), amount(amt), description(desc) {}    

std::string Transaction::getTransactionDetails() const {
        return "Transaction ID: " + transactionId + "\nDate: " + date + "\nType: " + typeOfTransaction + "\nAmount: ₹" + std::to_string(amount) + "\nDescription: " + description;
    }   