#include "include/LoanAccount.h"

LoanAccount::LoanAccount(const std::string& accNum, const std::string& accHolderName, const std::string& pin, double loanAmt, double interest, int tenure)
        : BankUser(accNum, accHolderName, pin), loanAmount(loanAmt), interestRate(interest), tenure(tenure) {
        calculateEMI();
    }

void LoanAccount::calculateEMI() {
        double monthlyInterestRate = interestRate / 12 / 100;
        emi = (loanAmount * monthlyInterestRate) / (1 - std::pow(1 + monthlyInterestRate, -tenure));
    }    


void LoanAccount::makePayment(double amount) {
        loanAmount -= amount;
        transactionHistory.addTransaction(Transaction(TransactionIDGenerator::generateTransactionID(), CurrentTime::getCurrentDate(), "Loan payment", amount, "Loan payment"));
    }    


void LoanAccount::getLoanDetails() const {
        std::cout << "Loan Account Summary:\n";
        std::cout << "Account Holder Name: " << getAccountHolderName() << "\nAccount Number: " << getAccountNumber() << "\nRemaining Loan Amount: ₹" << loanAmount << "\nEMI: ₹" << emi << std::endl;
        transactionHistory.displayTransactionHistory();
    }    