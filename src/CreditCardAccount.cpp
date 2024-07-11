#include "include/CreditCardAccount.h"

CreditCardAccount::CreditCardAccount(const std::string& accNum, const std::string& accHolderName, const std::string& pin, double limit, double interest)
        : BankUser(accNum, accHolderName, pin), creditLimit(limit), currentSpending(0), dueDate("2024-08-05"), interestRate(interest) {}


void CreditCardAccount::makePurchase(double amount) {
        if (currentSpending + amount <= creditLimit) {
            currentSpending += amount;
            transactionHistory.addTransaction(Transaction(TransactionIDGenerator::generateTransactionID(), CurrentTime::getCurrentDate(), "Credit Card Account", amount, "Purchase happens using Credit Card"));
        } else {
            throw InvalidTransactionException();
        }
    }

void CreditCardAccount::makePayment(double amount) {
        if (amount <= currentSpending) {
            currentSpending -= amount;
            transactionHistory.addTransaction((Transaction(TransactionIDGenerator::generateTransactionID(), CurrentTime::getCurrentDate(), "Credit Card Account", amount, "Credit Card Payment Done")));
        } else {
            throw InvalidTransactionException();
        }
    }                    

void CreditCardAccount::calculateInterest() {
        double interest = currentSpending * (interestRate / 100);
        currentSpending += interest;
    }    

void CreditCardAccount::getCreditCardDetails() const {
        std::cout << "Credit Card Account Summary:\n";
        std::cout << "Account Holder Name: " << getAccountHolderName() << "\nAccount Number: " << getAccountNumber() << "\nCurrent Spending: ₹" << currentSpending << "\nCredit Limit: ₹" << creditLimit << "\nDue Date: " << dueDate << std::endl;
        transactionHistory.displayTransactionHistory();
    }    
    