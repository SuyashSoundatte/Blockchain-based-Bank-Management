#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <exception>
#include <ctime>
#include <chrono>
#include <sstream>
#include <unordered_set>
#include <random>
#include <cpprest/http_client.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class TransactionIDGenerator {
private:
    static std::unordered_set<std::string> generatedIDs;

public:
    static std::string generateTransactionID() {
        std::string id;
        do {
            id = generateUniqueID();
        } while (!isUnique(id));
        generatedIDs.insert(id);
        return id;
    }

private:
    static std::string generateUniqueID() {
        // Get current time in milliseconds since epoch
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        // Generate a random number for uniqueness
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1000, 9999);
        int randomNumber = dis(gen);

        // Construct transaction ID in the format: timestamp-randomnumber
        std::ostringstream oss;
        oss << millis << "-" << randomNumber;
        return oss.str();
    }

    static bool isUnique(const std::string& id) {
        return generatedIDs.find(id) == generatedIDs.end();
    }
};

// Initialize static member variable
std::unordered_set<std::string> TransactionIDGenerator::generatedIDs;

// Get Current Date and time
class CurrentTime{
public:
    static std::string getCurrentDate(){
        time_t tt;

        // Declaring variable to store return value of 
        // localtime() 
        struct tm* ti;

        // Applying time() 
        time(&tt);

        // Using localtime() 
        ti = localtime(&tt);
        
        std::string time = asctime(ti);
        return time;
    }
};

// Custom exceptions
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

class DepositAmountNegative: public BankingException{
public:
    DepositAmountNegative(): BankingException("Deposit amount should not be negative!") {}
};

class WithdrawalLimitExceedException: public BankingException{
public:
    WithdrawalLimitExceedException(): BankingException("Withdrawl limit exceded, After this charges may applies") {}
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

// Transaction class
class Transaction {
private:
    std::string transactionId;
    std::string date;
    std::string typeOfTransaction;
    double amount;
    std::string description;

public:
    Transaction(const std::string transactionId, const std::string& date, const std::string& type, double amt, const std::string& desc)
        : transactionId(TransactionIDGenerator::generateTransactionID()), date(date), typeOfTransaction(type), amount(amt), description(desc) {}
    
    std::string getTransactionDetails() const {
        return "Transaction ID: " + transactionId + "\nDate: " + date + "\nType: " + typeOfTransaction + "\nAmount: ₹" + std::to_string(amount) + "\nDescription: " + description;
    }
};

class BankUser {
private:
    std::string accountNumber;
    std::string accountHolderName;
    std::string atmPin;

public:
    BankUser(std::string accNum, std::string accHolderName, std::string pin)
        : accountNumber(accNum), accountHolderName(accHolderName), atmPin(pin) {}

    std::string getAccountNumber() const {
        return accountNumber;
    }

    std::string getAccountHolderName() const {
        return accountHolderName;
    }

    void getAccountHolderDetails(std::string inputPin) {
        if (inputPin == atmPin) {
            std::cout << "Account Holder Name: " << accountHolderName << "\nAccount Number: " << accountNumber << std::endl;
        } else {
            std::cout << "Invalid PIN." << std::endl;
        }
    }
};

// Loan Class
class Loan {
protected:
    double amount;
    int durationMonths;

public:
    Loan(double amt, int months)
        : amount(amt), durationMonths(months) {}

    virtual double calculateEMI() const = 0;
    virtual void displayLoanDetails() const = 0;
    virtual double getTotalPayment() const {
        return calculateEMI() * durationMonths;
    }
};

class PersonalLoan : public Loan {
private:
    std::string purpose;
    double interestRate;

public:
    PersonalLoan(double amt, double rate, int months, std::string purpose)
        : Loan(amt, months), purpose(purpose), interestRate(rate) {}

    double calculateEMI() const override {
        // EMI calculation logic for personal loan
        // Example calculation:
        double monthlyRate = interestRate / 12.0 / 100.0;
        double emi = (amount * monthlyRate) / (1 - pow(1 + monthlyRate, -durationMonths));
        return emi;
    }

    void displayLoanDetails() const override {
        std::cout << "Personal Loan Details:" << std::endl;
        std::cout << "Amount: $" << amount << ", Interest Rate: " << interestRate << "%, Duration: " << durationMonths << " months" << std::endl;
        std::cout << "Purpose: " << purpose << std::endl;
    }
};

class CarLoan : public Loan {
private:
    std::string carModel;
    int year;
    double interestRate;

public:
    CarLoan(double amt, double rate, int months, std::string model, int year)
        : Loan(amt, months), carModel(model), year(year), interestRate(rate) {}

    double calculateEMI() const override {
        // EMI calculation logic for car loan
        // Example calculation:
        double monthlyRate = interestRate / 12.0 / 100.0;
        double emi = (amount * monthlyRate) / (1 - pow(1 + monthlyRate, -durationMonths));
        return emi;
    }

    void displayLoanDetails() const override {
        std::cout << "Car Loan Details:" << std::endl;
        std::cout << "Amount: $" << amount << ", Interest Rate: " << interestRate << "%, Duration: " << durationMonths << " months" << std::endl;
        std::cout << "Car Model: " << carModel << ", Year: " << year << std::endl;
    }
};

class HomeLoan : public Loan {
private:
    std::string propertyAddress;
    double interestRate;

public:
    HomeLoan(double amt, double rate, int months, std::string address)
        : Loan(amt, months), propertyAddress(address), interestRate(rate) {}

    double calculateEMI() const override {
        // EMI calculation logic for home loan
        // Example calculation:
        double monthlyRate = interestRate / 12.0 / 100.0;
        double emi = (amount * monthlyRate) / (1 - pow(1 + monthlyRate, -durationMonths));
        return emi;
    }

    void displayLoanDetails() const override {
        std::cout << "Home Loan Details:" << std::endl;
        std::cout << "Amount: $" << amount << ", Interest Rate: " << interestRate << "%, Duration: " << durationMonths << " months" << std::endl;
        std::cout << "Property Address: " << propertyAddress << std::endl;
    }
};

class StudentLoan : public Loan {
private:
    std::string universityName;
    double interestRate;

public:
    StudentLoan(double amt, double rate, int months, std::string university)
        : Loan(amt, months), universityName(university), interestRate(rate) {}

    double calculateEMI() const override {
        // EMI calculation logic for student loan
        // Example calculation:
        double monthlyRate = interestRate / 12.0 / 100.0;
        double emi = (amount * monthlyRate) / (1 - pow(1 + monthlyRate, -durationMonths));
        return emi;
    }

    void displayLoanDetails() const override {
        std::cout << "Student Loan Details:" << std::endl;
        std::cout << "Amount: $" << amount << ", Interest Rate: " << interestRate << "%, Duration: " << durationMonths << " months" << std::endl;
        std::cout << "University: " << universityName << std::endl;
    }
};

// BankAccount class
class BankAccount {
protected:
    std::string accountNumber;
    double balance;
    double interestRate;
    std::string accountType;

public:
    BankAccount(const std::string& accNumber, double initialDeposit, double rate, const std::string& type)
        : accountNumber(accNumber), balance(initialDeposit), interestRate(rate), accountType(type) {}

    virtual void deposit(double amount) {
        if (amount <= 0) {
            throw DepositAmountNegative();
        }
        balance += amount;
        std::cout << "Deposited: ₹" << amount << std::endl;
    }

    virtual void withdraw(double amount) {
        if (amount <= 0) {
            throw DepositAmountNegative();
        }
        if (amount > balance) {
            throw InsufficientFundsException();
        }
        if (amount >= 20000){
            throw WithdrawalLimitExceedException();
        }
        balance -= amount;
        std::cout << "Withdrawn: ₹" << amount << std::endl;
    }

    virtual void display() const {
        std::cout << "Account Number: " << accountNumber << "\nType: " << accountType << "\nBalance: ₹" << balance << std::endl;
    }

    double getBalance() const {
        return balance;
    }

    virtual ~BankAccount() = default;
};

// CurrentAccount class
class CurrentAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& accNumber, double initialDeposit, double rate, double limit)
        : BankAccount(accNumber, initialDeposit, rate, "Current Account"), overdraftLimit(limit) {}

    void withdraw(double amount) override {
        if (amount <= 0) {
            throw DepositAmountNegative();
        }
        if (amount > balance + overdraftLimit) {
            throw InsufficientFundsException();
        }
        if (amount >= 20000){
            throw WithdrawalLimitExceedException();
        }
        balance -= amount;
        std::cout << "Withdrawn: ₹" << amount << std::endl;
    }

    void display() const override {
        std::cout << "Account Number: " << accountNumber << "\nType: Current Account\nBalance: ₹" << balance << "\nOverdraft Limit: ₹" << overdraftLimit << std::endl;
    }
};

// SavingsAccount class
class SavingsAccount : public BankAccount {
private:
    int withdrawalsLeft;

public:
    SavingsAccount(const std::string& accNumber, double initialDeposit, double rate, int withdrawals)
        : BankAccount(accNumber, initialDeposit, rate, "Savings Account"), withdrawalsLeft(withdrawals) {}

    void withdraw(double amount) override {
        if (amount <= 0) {
            throw DepositAmountNegative();
        }
        if (amount > balance) {
            throw InsufficientFundsException();
        }
        if (withdrawalsLeft == 0) {
            throw InvalidTransactionException();
        }
        if (amount >= 20000){
            throw WithdrawalLimitExceedException();
        }
        balance -= amount;
        withdrawalsLeft--;
        std::cout << "Withdrawn: ₹" << amount << std::endl;
    }

    void display() const override {
        std::cout << "Account Number: " << accountNumber << "\nType: Savings Account\nBalance: ₹" << balance << "\nWithdrawals Left: " << withdrawalsLeft << std::endl;
    }
};

// FixedDeposit class
class FixedDeposit : public BankAccount {
private:
    int durationMonths;
    double penaltyPercentage;

public:
    FixedDeposit(const std::string& accNumber, double initialDeposit, double rate, int months, double penalty)
        : BankAccount(accNumber, initialDeposit, rate, "Fixed Deposit"), durationMonths(months), penaltyPercentage(penalty) {}

    void withdraw(double amount) override {
        if (amount <= 0) {
            throw DepositAmountNegative();
        }
        if (amount > balance) {
            throw InsufficientFundsException();
        }
        if (durationMonths < 6) {
            throw InvalidTransactionException();
        }
        if (amount >= 20000){
            throw WithdrawalLimitExceedException();
        }
        // Calculate penalty
        double penalty = (amount / 100) * penaltyPercentage;
        balance -= (amount + penalty);
        std::cout << "Withdrawn: ₹" << amount << " Penalty: ₹" << penalty << std::endl;
    }

    void display() const override {
        std::cout << "Account Number: " << accountNumber << "\nType: Fixed Deposit\nBalance: ₹" << balance << "\nDuration: " << durationMonths << " months" << std::endl;
    }
};

// NRI Account
class NRIAccount : public BankAccount {
public:
    NRIAccount(const std::string& accNumber, double initialDeposit, double rate)
        : BankAccount(accNumber, initialDeposit, rate, "NRI Account") {}

    void deposit(double amount) override {
        if (amount <= 0) {
            throw DepositAmountNegative();
        }
        balance += amount;
        std::cout << "Deposited: ₹" << amount << std::endl;
    }

    void withdraw(double amount) override {
        if (amount <= 0) {
            throw DepositAmountNegative();
        }
        if (amount > balance) {
            throw InsufficientFundsException();
        }
        if (amount >= 20000){
            throw WithdrawalLimitExceedException();
        }
        balance -= amount;
        std::cout << "Withdrawn: ₹" << amount << std::endl;
    }

    void display() const override {
        std::cout << "Account Number: " << accountNumber << "\nType: NRI Account\nBalance: ₹" << balance << std::endl;
    }
};

// API integration for fetching exchange rates
class CurrencyConverter {
public:
    static double fetchExchangeRate(const std::string& fromCurrency, const std::string& toCurrency) {
        // Example: Using external API to fetch exchange rates
        utility::string_t url = U("https://api.exchangerate-api.com/v4/latest/") + utility::conversions::to_string_t(fromCurrency);

        http_client client(url);
        http_response response = client.request(methods::GET).get();

        if (response.status_code() != status_codes::OK) {
            throw std::runtime_error("Failed to fetch exchange rates.");
        }

        json::value jsonResponse = response.extract_json().get();
        return jsonResponse[U("rates")].at(toCurrency).as_double();
    }
};

int main() {
    try {
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}

