#ifndef CUSTOM_EXCEPTION_H
#define CUSTOM_EXCEPTION_H

#pragma once

#include <string>
#include <stdexcept>
#include <exception>

// Custom exception base class
class BankingException : public std::exception {
public:
    explicit BankingException(const std::string& message);
    virtual const char* what() const noexcept override;
private:
    std::string message_;
};

// Custom exception classes

class AccountNotFoundException : public BankingException {
public:
    AccountNotFoundException();
};

class InvalidTransactionException : public BankingException {
public:
    InvalidTransactionException();
};

class InsufficientFundsException : public BankingException {
public:
    InsufficientFundsException();
};

class DepositAmountNegative : public BankingException {
public:
    DepositAmountNegative();
};

class WithdrawalLimitExceedException : public BankingException {
public:
    WithdrawalLimitExceedException();
};

#endif // CUSTOM_EXCEPTION_H
