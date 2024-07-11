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


#include "include/CurrentTime.h"
#include "include/CustomException.h"
#include "include/Transaction.h"
#include "include/TransactionIDGenerator.h"
#include "include/BankUser.h"
#include "include/LoanAccount.h"
#include "include/CreditCardAccount.h"
#include "include/CurrentAccount.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;



// SavingsAccount derived class
class SavingsAccount : public BankUser {
private:
    std::vector<Transaction> transactions;
    double interestRate;
    int withdrawalCount;
    int maxWithdrawalsPerMonth;
    double withdrawalFee;
    double minimumBalance;
    double balance;

public:
    SavingsAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, double interest, int maxWithdrawPerMonth, double fee)
        : BankUser(accNum, accHolderName, atmPin), balance(bal), interestRate(interest), withdrawalCount(0), maxWithdrawalsPerMonth(maxWithdrawPerMonth), withdrawalFee(fee), minimumBalance(1000) {}

    void addInterest() {
        double interestAmount = balance * (interestRate / 100);
        balance += interestAmount;
        recordTransaction(TransactionIDGenerator::generateTransactionID(), CurrentTime::getCurrentDate(), "Interest", interestAmount, "Interest added");
    }

    double checkBalance() const {
        return balance;
    }

    void deposit(double amount) {
        if (amount < 0) {
            throw DepositAmountNegative();
        }
        balance += amount;
        recordTransaction(TransactionIDGenerator::generateTransactionID(), CurrentTime::getCurrentDate(), "Deposit", amount, "Deposited amount");
    }

    void withdraw(double amount) {
        if (withdrawalCount >= maxWithdrawalsPerMonth) {
            balance -= withdrawalFee;
            recordTransaction(TransactionIDGenerator::generateTransactionID(), CurrentTime::getCurrentDate(), "Fee", withdrawalFee, "Withdrawal limit exceeded fee");
            std::cout << "Withdrawal limit exceeded for the month. Fee applied: ₹" << withdrawalFee << std::endl;
        }
        if (amount <= balance - minimumBalance) {
            balance -= amount;
            withdrawalCount++;
            recordTransaction(TransactionIDGenerator::generateTransactionID(), CurrentTime::getCurrentDate(), "Withdrawal", amount, "Withdrawn amount");
        } else {
            throw InsufficientFundsException();
        }
    }

    void recordTransaction(const std::string& transactionId,const std::string& date, const std::string& type, double amount, const std::string& description) {
        transactions.emplace_back(transactionId, date, type, amount, description);
    }

    void displayTransactionHistory() const {
        for (const auto& transaction : transactions) {
            std::cout << transaction.getTransactionDetails() << std::endl;
        }
    }
};

class ZeroBalanceSavingsAccount : public BankUser {
    private:
        double balance;
        int withdrawalCount;
        int maxWithdrawalsPerDay;
        double withdrawalFee;
        double minimumBalance;
        std::vector<Transaction> transactions;
    public:
        ZeroBalanceSavingsAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, double interest, int maxWithdrawPerMonth, double fee)
            : BankUser(accNum, accHolderName, atmPin), withdrawalCount(0), maxWithdrawalsPerDay(50000), withdrawalFee(100), minimumBalance(0) {
            // Override minimum balance requirement for zero balance account
        }

        // No additional methods needed as per description
        void deposit(double amount) {
            if (amount < 0) {
                throw DepositAmountNegative();
            }
            balance += amount;
            recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(),"Deposit", amount, "Deposited amount");
        }

        void recordTransaction(const std::string& transactionId,const std::string date, const std::string& type, double amount, const std::string& description) {
            transactions.emplace_back(transactionId,date ,type, amount, description);
        }

        void withdraw(double amount) {
            if (withdrawalCount >= maxWithdrawalsPerDay) {
                balance -= withdrawalFee;
                recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(),"Fee", withdrawalFee, "Withdrawal limit exceeded fee");
                std::cout << "Withdrawal limit exceeded for the month. Fee applied: ₹" << withdrawalFee << std::endl;
            }
            if (amount <= balance - minimumBalance) {
                balance -= amount;
                withdrawalCount++;
                recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(),"Withdrawal", amount, "Withdrawn amount");
            } else {
                throw InsufficientFundsException();
            }
        }

        void displayTransactionHistory() const {
            for (const auto& transaction : transactions) {
                std::cout << transaction.getTransactionDetails() << std::endl;
            }
        }
};

// Special savings accounts
class WomenSavingsAccount : public SavingsAccount {
public:
    WomenSavingsAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, double interest, int maxWithdrawPerMonth, double fee)
        : SavingsAccount(accNum, accHolderName, atmPin, bal, interest, maxWithdrawPerMonth, fee) {}

    void healthInsuranceBenefit() {
        std::cout << "Health insurance benefit applied.\n";
    }

    void loanInterestDiscount() {
        std::cout << "Loan interest discount applied.\n";
    }
};

class KidsSavingsAccount : public SavingsAccount {
public:
    KidsSavingsAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, double interest, int maxWithdrawPerMonth, double fee)
        : SavingsAccount(accNum, accHolderName, atmPin, bal, interest, maxWithdrawPerMonth, fee) {}

    void educationalBonus() {
        std::cout << "Educational bonus applied.\n";
    }

    void parentalControl() {
        std::cout << "Parental control activated.\n";
    }
};

class SeniorCitizensSavingsAccount : public SavingsAccount {
public:
    SeniorCitizensSavingsAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, double interest, int maxWithdrawPerMonth, double fee)
        : SavingsAccount(accNum, accHolderName, atmPin, bal, interest, maxWithdrawPerMonth, fee) {}

    void higherInterestRate() {
        std::cout << "Higher interest rate applied.\n";
    }

    void medicalBenefit() {
        std::cout << "Medical benefit applied.\n";
    }
};



// OverdraftAccount derived class
class OverdraftAccount : public CurrentAccount {
private:
    double overdraftLimit;
    double overdraftFee;

public:
    OverdraftAccount(double m_overdraftLimit, double m_overdraftFee, std::string accNum, std::string atmPin, std::string accHolderName, double bal)
        : CurrentAccount(accNum, atmPin , accHolderName, bal), overdraftLimit(m_overdraftLimit), overdraftFee(m_overdraftFee) {}

    void withdraw(double amount) override {
        if (amount <= balance) {
            balance -= amount;
            recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(),"Withdrawal", amount, "Withdrawn amount");
        } else if (amount <= balance + overdraftLimit) {
            double overdraftAmount = amount - balance;
            balance -= amount;
            balance -= overdraftFee; // Apply overdraft fee
            recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(), "Overdraft", overdraftAmount, "Overdraft used");
            recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(), "Overdraft Fee", overdraftFee, "Overdraft fee applied");
        } else {
            throw WithdrawalLimitExceedException();
        }
    }
};

// Template class for managing accounts
class FixedDepositAccount : public BankUser {
private:
    std::vector<Transaction> transactions;
    double interestRate;
    time_t maturityDate;
    bool isMatured;
    double balance;


public:
    FixedDepositAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, double interest, int durationInDays)
        : BankUser(accNum, accHolderName, atmPin), interestRate(interest), isMatured(false) {
        time_t now = time(0);
        maturityDate = now + durationInDays * 24 * 60 * 60;
    }

    void withdraw(double amount) {
        time_t now = time(0);
        if (now >= maturityDate) {
            isMatured = true;
        }
        if (isMatured) {
            double interestAmount = balance * (interestRate / 100);
            balance += interestAmount;
            recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(),"Interest", interestAmount, "Interest added on maturity");
        }
        if (amount <= balance) {
            balance -= amount;
            recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(),"Withdrawal", amount, "Withdrawn amount");
        } else {
            throw InsufficientFundsException();
        }
    }

    void recordTransaction(const std::string transactionId,std::string date ,const std::string& type, double amount, const std::string& description) {
        transactions.emplace_back(transactionId,date,type,amount, description);
    }

    void displayTransactionHistory() const {
        for (const auto& transaction : transactions) {
            std::cout << transaction.getTransactionDetails() << std::endl;
        }
    }
};

// NRIAccount class inheriting BankUser
class NRIAccount : public BankUser {
private:
    std::vector<Transaction> transactions;
    double balance;

public:
    NRIAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal)
        : BankUser(accNum, accHolderName, atmPin), balance(bal) {}

    double checkBalance() const {
        return balance;
    }

    void deposit(double amount) {
        if (amount < 0) {
            throw DepositAmountNegative();
        }
        balance += amount;
        recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(), "Deposit", amount, "Deposited amount");
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(), "Withdrawal", amount, "Withdrawn amount");
        } else {
            throw InsufficientFundsException();
        }
    }

    std::string getAccountInfo() const {
        return "Account Number: " + getAccountNumber() + "\nAccount Holder: " + getAccountHolderName() + "\nBalance: ₹" + std::to_string(balance);
    }

    void recordTransaction(const std::string& transactionId, std::string date,const std::string& type, double amount, const std::string& description) {
        transactions.emplace_back(transactionId,date,type, amount, description);
    }

    void displayTransactionHistory() const {
        for (const auto& transaction : transactions) {
            std::cout << transaction.getTransactionDetails() << std::endl;
        }
    }

    virtual ~NRIAccount() {}
};

// NROAccount class definition

class NROAccount : public NRIAccount {
private:
    double balance;
    double interestRate;
    double tdsRate; // TDS (Tax Deducted at Source) rate for NRO account

public:
    NROAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, double interest)
        : NRIAccount(accNum, accHolderName, atmPin, bal), balance(bal), interestRate(interest), tdsRate(0.309) {}

    double checkBalance() const {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
        std::cout << "Deposit of ₹" << amount << " successful.\n";
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            std::cout << "Withdrawal of ₹" << amount << " successful.\n";
        } else {
            throw InsufficientFundsException();
        }
    }

    void applyTDS() {
        double tdsAmount = balance * tdsRate;
        balance -= tdsAmount;
        std::cout << "TDS of ₹" << tdsAmount << " deducted as per regulations.\n";
    }

    void deductTDS() {
        double tdsAmount = checkBalance() * (tdsRate / 100);
        if (checkBalance() < tdsAmount) {
            throw InsufficientFundsException();
        }
        withdraw(tdsAmount);
        recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(), "TDS", tdsAmount, "TDS deducted");
    }

    // Method to fetch current exchange rates from an API
    double fetchExchangeRate(const std::string& fromCurrency, const std::string& toCurrency) {
        // Example: Using CPP Rest SDK for making HTTP request
        utility::string_t url = U("https://api.exchangeratesapi.io/latest?base=") + utility::conversions::to_string_t(fromCurrency);
        http_client client(url);

        return client.request(methods::GET).then([=](http_response response) {
            if (response.status_code() == status_codes::OK) {
                return response.extract_json();
            } else {
                throw std::runtime_error("Failed to fetch exchange rates.");
            }
        }).then([=](json::value json) {
            double rate = json[U("rates")][utility::conversions::to_string_t(toCurrency)].as_double();
            return rate;
        }).get();
    }

    // Currency converter for NRO account using current market rates
    void convertCurrency(double foreignAmount, const std::string& fromCurrency, const std::string& toCurrency) {
        double exchangeRate = fetchExchangeRate(fromCurrency, toCurrency);
        double inrAmount = foreignAmount * exchangeRate;
        deposit(inrAmount); // Deposit the equivalent INR amount after conversion
        std::cout << "Converted " << foreignAmount << " " << fromCurrency << " to INR at rate " << exchangeRate << ".\n";
    }

    void displayAccountSummary() {
        std::cout << getAccountInfo() << std::endl;
        std::cout << "Interest Rate: " << interestRate << "%" << std::endl;
        std::cout << "TDS Rate: " << tdsRate << "%" << std::endl;
        displayTransactionHistory();
    }

    void addInterest() {
        double interestAmount = checkBalance() * (interestRate / 100);
        deposit(interestAmount);
        recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(), "Interest", interestAmount, "Interest added");
    }
};

// NREAccount class definition

class NREAccount : public NRIAccount {
private:
    double interestRate;

public:
    NREAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, double interest)
        : NRIAccount(accNum, accHolderName, atmPin, bal), interestRate(interest) {}

    void addInterest() {
        double interestAmount = checkBalance() * (interestRate / 100);
        deposit(interestAmount);
        recordTransaction(TransactionIDGenerator::generateTransactionID(),CurrentTime::getCurrentDate(), "Interest", interestAmount, "Interest added");
    }

    void displayAccountSummary() {
        std::cout << getAccountInfo() << std::endl;
        std::cout << "Interest Rate: " << interestRate << "%" << std::endl;
        displayTransactionHistory();
    }
};

// AccountManager template class
template<typename T>
class AccountManager {
private:
    std::vector<T> accounts;

public:
    void addAccount(const T& account) {
        accounts.push_back(account);
    }

    bool removeAccount(const std::string& accNum) {
        for (auto it = accounts.begin(); it != accounts.end(); ++it) {
            if (it->getAccountNumber() == accNum) {
                accounts.erase(it);
                return true;
            }
        }
        return false;
    }

    void displayAllAccounts() const {
        for (const auto& account : accounts) {
            std::cout << account.getAccountInfo() << std::endl;
        }
    }

    T* findAccount(const std::string& accountNumber) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accountNumber) {
                return &account;
            }
        }
        return nullptr;
    }
};

// Main function
int main() {
    try {

    } 
    catch (const InsufficientFundsException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch (const WithdrawalLimitExceedException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
