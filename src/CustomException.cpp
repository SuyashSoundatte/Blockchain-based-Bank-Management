#include "CustomException.h"

BankingException::BankingException(const std::string& message) : message_(message) {}

const char* BankingException::what() const noexcept {
    return message_.c_str();
}

// Implementing AccountNotFoundException
AccountNotFoundException::AccountNotFoundException() : BankingException("Account not found.") {}

// Implementing InvalidTransactionException
InvalidTransactionException::InvalidTransactionException() : BankingException("Invalid transaction.") {}

// Implementing InsufficientFundsException
InsufficientFundsException::InsufficientFundsException() : BankingException("Insufficient funds.") {}

// Implementing DepositAmountNegative
DepositAmountNegative::DepositAmountNegative() : BankingException("Deposit amount should not be negative!") {}

// Implementing WithdrawalLimitExceedException
WithdrawalLimitExceedException::WithdrawalLimitExceedException() : BankingException("Withdrawl limit exceeded, charges may apply.") {}
