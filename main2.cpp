#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <exception>
#include <cstdlib>
#include <math.h>

// Base exception class
class BankingException : public std::exception {
public:
    explicit BankingException(const std::string& message) : message_(message) {}
    virtual const char* what() const noexcept override {
        return message_.c_str();
    }
private:
    std::string message_;
};

// Custom exception classes
class AccountNotFoundException : public BankingException {
public:
    AccountNotFoundException() : BankingException("Account not found.") {}
};

class InvalidTransactionException : public BankingException {
public:
    InvalidTransactionException() : BankingException("Invalid transaction.") {}
};

class InsufficientFundsException : public BankingException {
public:
    InsufficientFundsException() : BankingException("Insufficient funds.") {}
};

// Transaction class
class Transaction {
private:
    std::string transactionId;
    std::string date;
    std::string type;
    double amount;
    std::string description;

public:
    Transaction(const std::string& id, const std::string& date, const std::string& type, double amt, const std::string& desc)
        : transactionId(id), date(date), type(type), amount(amt), description(desc) {}

    std::string getTransactionDetails() const {
        return "Transaction ID: " + transactionId + "\nDate: " + date + "\nType: " + type + "\nAmount: ₹" + std::to_string(amount) + "\nDescription: " + description;
    }
};

// TransactionHistory class
class TransactionHistory {
private:
    std::vector<Transaction> transactions;

public:
    void addTransaction(const Transaction& transaction) {
        transactions.push_back(transaction);
    }

    void displayTransactionHistory() const {
        for (const auto& transaction : transactions) {
            std::cout << transaction.getTransactionDetails() << std::endl;
        }
    }
};

// BankUser class
class BankUser {
private:
    std::string accountNumber;
    std::string accountHolderName;
    std::string atmPin;

public:
    BankUser(const std::string& accNum, const std::string& accHolderName, const std::string& pin)
        : accountNumber(accNum), accountHolderName(accHolderName), atmPin(pin) {}

    std::string getAccountNumber() const {
        return accountNumber;
    }

    std::string getAccountHolderName() const {
        return accountHolderName;
    }

    void getAccountHolderDetails(const std::string& inputPin) const {
        if (inputPin == atmPin) {
            std::cout << "Account Holder Name: " << accountHolderName << "\nAccount Number: " << accountNumber << std::endl;
        } else {
            throw InvalidTransactionException();
        }
    }
};

// Base Account class
class Account : public BankUser {
protected:
    double balance;
    TransactionHistory transactionHistory;

public:
    Account(const std::string& accNum, const std::string& accHolderName, const std::string& pin, double bal)
        : BankUser(accNum, accHolderName, pin), balance(bal) {}

    virtual void deposit(double amount) {
        balance += amount;
        transactionHistory.addTransaction(Transaction("TXN" + std::to_string(rand()), "2024-07-05", "Deposit", amount, "Deposit successful"));
    }

    virtual void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            transactionHistory.addTransaction(Transaction("TXN" + std::to_string(rand()), "2024-07-05", "Withdrawal", amount, "Withdrawal successful"));
        } else {
            throw InsufficientFundsException();
        }
    }

    virtual void displayAccountSummary() const = 0;
};

// CurrentAccount class
class CurrentAccount : public Account {
public:
    CurrentAccount(const std::string& accNum, const std::string& accHolderName, const std::string& pin, double bal)
        : Account(accNum, accHolderName, pin, bal) {}

    void displayAccountSummary() const override {
        std::cout << "Current Account Summary:\n";
        std::cout << "Account Holder Name: " << getAccountHolderName() << "\nAccount Number: " << getAccountNumber() << "\nBalance: ₹" << balance << std::endl;
        transactionHistory.displayTransactionHistory();
    }
};

// SavingsAccount class
class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& accNum, const std::string& accHolderName, const std::string& pin, double bal, double interest)
        : Account(accNum, accHolderName, pin, bal), interestRate(interest) {}

    void addInterest() {
        double interestAmount = balance * (interestRate / 100);
        deposit(interestAmount);
    }

    void displayAccountSummary() const override {
        std::cout << "Savings Account Summary:\n";
        std::cout << "Account Holder Name: " << getAccountHolderName() << "\nAccount Number: " << getAccountNumber() << "\nBalance: ₹" << balance << "\nInterest Rate: " << interestRate << "%" << std::endl;
        transactionHistory.displayTransactionHistory();
    }
};

// LoanAccount class
class LoanAccount : public BankUser {
private:
    double loanAmount;
    double interestRate;
    int tenure; // in months
    double emi;
    TransactionHistory transactionHistory;

public:
    LoanAccount(const std::string& accNum, const std::string& accHolderName, const std::string& pin, double loanAmt, double interest, int tenure)
        : BankUser(accNum, accHolderName, pin), loanAmount(loanAmt), interestRate(interest), tenure(tenure) {
        calculateEMI();
    }

    void calculateEMI() {
        double monthlyInterestRate = interestRate / 12 / 100;
        emi = (loanAmount * monthlyInterestRate) / (1 - std::pow(1 + monthlyInterestRate, -tenure));
    }

    void makePayment(double amount) {
        loanAmount -= amount;
        transactionHistory.addTransaction(Transaction("TXN" + std::to_string(rand()), "2024-07-05", "Payment", amount, "Loan payment"));
    }

    void getLoanDetails() const {
        std::cout << "Loan Account Summary:\n";
        std::cout << "Account Holder Name: " << getAccountHolderName() << "\nAccount Number: " << getAccountNumber() << "\nRemaining Loan Amount: ₹" << loanAmount << "\nEMI: ₹" << emi << std::endl;
        transactionHistory.displayTransactionHistory();
    }
};

// CreditCardAccount class
class CreditCardAccount : public BankUser {
private:
    double creditLimit;
    double currentSpending;
    std::string dueDate;
    double interestRate;
    TransactionHistory transactionHistory;

public:
    CreditCardAccount(const std::string& accNum, const std::string& accHolderName, const std::string& pin, double limit, double interest)
        : BankUser(accNum, accHolderName, pin), creditLimit(limit), currentSpending(0), dueDate("2024-08-05"), interestRate(interest) {}

    void makePurchase(double amount) {
        if (currentSpending + amount <= creditLimit) {
            currentSpending += amount;
            transactionHistory.addTransaction(Transaction("TXN" + std::to_string(rand()), "2024-07-05", "Purchase", amount, "Credit card purchase"));
        } else {
            throw InvalidTransactionException();
        }
    }

    void makePayment(double amount) {
        if (amount <= currentSpending) {
            currentSpending -= amount;
            transactionHistory.addTransaction(Transaction("TXN" + std::to_string(rand()), "2024-07-05", "Payment", amount, "Credit card payment"));
        } else {
            throw InvalidTransactionException();
        }
    }

    void calculateInterest() {
        double interest = currentSpending * (interestRate / 100);
        currentSpending += interest;
    }

    void getCreditCardDetails() const {
        std::cout << "Credit Card Account Summary:\n";
        std::cout << "Account Holder Name: " << getAccountHolderName() << "\nAccount Number: " << getAccountNumber() << "\nCurrent Spending: ₹" << currentSpending << "\nCredit Limit: ₹" << creditLimit << "\nDue Date: " << dueDate << std::endl;
        transactionHistory.displayTransactionHistory();
    }
};

// Main function
int main() {
    try {
        SavingsAccount savings("123456789", "John Doe", "1234", 5000, 3.5);
        savings.deposit(2000);
        savings.withdraw(1000);
        savings.addInterest();
        savings.displayAccountSummary();

        LoanAccount loan("987654321", "Jane Smith", "5678", 100000, 7.5, 60);
        loan.makePayment(5000);
        loan.getLoanDetails();

        CreditCardAccount creditCard("123987456", "Alice Brown", "9876", 50000, 1.5);
        creditCard.makePurchase(15000);
        creditCard.makePayment(5000);
        creditCard.calculateInterest();
        creditCard.getCreditCardDetails();
    } catch (const BankingException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
