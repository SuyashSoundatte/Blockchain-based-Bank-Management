
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <ctime>

// Assuming these classes and methods exist:
class TransactionIDGenerator {
public:
    static std::string generateTransactionID() {
        // Implementation of unique transaction ID generation
        return "TXN123456";  // Placeholder implementation
    }
};

class CurrentTime {
public:
    static std::string getCurrentDate() {
        // Implementation to get current date
        return "2024-07-11";  // Placeholder implementation
    }
};

class TransactionHistory {
public:
    void displayTransactionHistory() const {
        // Placeholder implementation for displaying transaction history
        std::cout << "Displaying transaction history..." << std::endl;
    }
};

// Base class
class BankUser {
protected:
    std::string accountNumber;
    std::string accountHolderName;
    std::string atmPin;

public:
    BankUser(std::string accNum, std::string accHolderName, std::string atmPin)
        : accountNumber(accNum), accountHolderName(accHolderName), atmPin(atmPin) {}

    virtual ~BankUser() {}

    std::string getAccountNumber() const { return accountNumber; }
    std::string getAccountHolderName() const { return accountHolderName; }
    virtual double checkBalance() const = 0;
};

// Derived class 1: OverdraftAccount
class OverdraftAccount : public BankUser {
private:
    double balance;
    double overdraftLimit;
    TransactionHistory transactionHistory;

public:
    OverdraftAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, double overdraftLimit)
        : BankUser(accNum, accHolderName, atmPin), balance(bal), overdraftLimit(overdraftLimit) {}

    double checkBalance() const override { return balance; }

    void withdraw(double amount) {
        if (balance + overdraftLimit >= amount) {
            balance -= amount;
            // Record the transaction (assuming a method exists for this)
            transactionHistory.displayTransactionHistory();
        } else {
            throw std::runtime_error("Insufficient funds, including overdraft limit.");
        }
    }

    void getOverdraftDetails() const {
        std::cout << "Overdraft Account Summary:";
        std::cout << "Account Holder Name: " << getAccountHolderName() << "Account Number: " << getAccountNumber() << "Balance: ₹" << balance << "Overdraft Limit: ₹" << overdraftLimit << std::endl;
        transactionHistory.displayTransactionHistory();
    }
};

// Derived class 2: JointAccount
class JointAccount : public BankUser {
private:
    double balance;
    std::vector<std::string> accountHolders;
    TransactionHistory transactionHistory;

public:
    JointAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, std::vector<std::string> accHolders)
        : BankUser(accNum, accHolderName, atmPin), balance(bal), accountHolders(accHolders) {}

    double checkBalance() const override { return balance; }

    void addAccountHolder(std::string name) {
        accountHolders.push_back(name);
    }

    void removeAccountHolder(std::string name) {
        accountHolders.erase(std::remove(accountHolders.begin(), accountHolders.end(), name), accountHolders.end());
    }

    void getJointAccountDetails() const {
        std::cout << "Joint Account Summary:
";
        std::cout << "Account Holders: ";
        for (const auto& holder : accountHolders) {
            std::cout << holder << " ";
        }
        std::cout << "
Account Number: " << getAccountNumber() << "
Balance: ₹" << balance << std::endl;
        transactionHistory.displayTransactionHistory();
    }
};

// Derived class 3: StudentAccount
class StudentAccount : public BankUser {
private:
    double balance;
    double scholarshipAmount;
    TransactionHistory transactionHistory;

public:
    StudentAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, double scholarship)
        : BankUser(accNum, accHolderName, atmPin), balance(bal), scholarshipAmount(scholarship) {}

    double checkBalance() const override { return balance; }

    void applyScholarship(double amount) {
        scholarshipAmount += amount;
        balance += amount;
    }

    void getStudentAccountDetails() const {
        std::cout << "Student Account Summary:
";
        std::cout << "Account Holder Name: " << getAccountHolderName() << "
Account Number: " << getAccountNumber() << "
Balance: ₹" << balance << "
Scholarship Amount: ₹" << scholarshipAmount << std::endl;
        transactionHistory.displayTransactionHistory();
    }
};

// Derived class 4: BusinessAccount
class BusinessAccount : public BankUser {
private:
    double balance;
    double transactionLimit;
    TransactionHistory transactionHistory;

public:
    BusinessAccount(std::string accNum, std::string accHolderName, std::string atmPin, double bal, double transLimit)
        : BankUser(accNum, accHolderName, atmPin), balance(bal), transactionLimit(transLimit) {}

    double checkBalance() const override { return balance; }

    void increaseTransactionLimit(double amount) {
        transactionLimit += amount;
    }

    void getBusinessAccountDetails() const {
        std::cout << "Business Account Summary:
";
        std::cout << "Account Holder Name: " << getAccountHolderName() << "
Account Number: " << getAccountNumber() << "
Balance: ₹" << balance << "
Transaction Limit: ₹" << transactionLimit << std::endl;
        transactionHistory.displayTransactionHistory();
    }
};

// Derived class 5: LoanAccount
class LoanAccount : public BankUser {
private:
    double loanAmount;
    double interestRate;
    TransactionHistory transactionHistory;

public:
    LoanAccount(std::string accNum, std::string accHolderName, std::string atmPin, double loanAmt, double interest)
        : BankUser(accNum, accHolderName, atmPin), loanAmount(loanAmt), interestRate(interest) {}

    double checkBalance() const override { return loanAmount; }

    void applyForLoan(double amount, double rate) {
        loanAmount += amount;
        interestRate = rate;
        // Record the transaction (assuming a method exists for this)
        transactionHistory.displayTransactionHistory();
    }

    void repayLoan(double amount) {
        if (loanAmount >= amount) {
            loanAmount -= amount;
            // Record the transaction (assuming a method exists for this)
            transactionHistory.displayTransactionHistory();
        } else {
            throw std::runtime_error("Repayment amount exceeds loan amount.");
        }
    }

    void getLoanAccountDetails() const {
        std::cout << "Loan Account Summary:
";
        std::cout << "Account Holder Name: " << getAccountHolderName() << "
Account Number: " << getAccountNumber() << "
Loan Amount: ₹" << loanAmount << "
Interest Rate: " << interestRate << "%" << std::endl;
        transactionHistory.displayTransactionHistory();
    }
};

int main() {
    try {
        OverdraftAccount overdraftAcc("OD123", "John Doe", "1234", 10000, 5000);
        overdraftAcc.withdraw(12000);
        overdraftAcc.getOverdraftDetails();

        std::vector<std::string> holders = {"John Doe", "Jane Doe"};
        JointAccount jointAcc("JA123", "John Doe", "1234", 20000, holders);
        jointAcc.addAccountHolder("Jake Doe");
        jointAcc.getJointAccountDetails();

        StudentAccount studentAcc("SA123", "John Doe", "1234", 5000, 1000);
        studentAcc.applyScholarship(500);
        studentAcc.getStudentAccountDetails();

        BusinessAccount businessAcc("BA123", "John Doe", "1234", 30000, 10000);
        businessAcc.increaseTransactionLimit(5000);
        businessAcc.getBusinessAccountDetails();

        LoanAccount loanAcc("LA123", "John Doe", "1234", 50000, 5);
        loanAcc.applyForLoan(20000, 6);
        loanAcc.repayLoan(10000);
        loanAcc.getLoanAccountDetails();
    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
