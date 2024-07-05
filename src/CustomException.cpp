#include<iostream>
#include<stdexcept>
#include<string>
#include<exception>

class InsufficientFundsException : public std::exception {
private:
    std::string message;

public:
    InsufficientFundsException(const std::string& msg) : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class WithdrawalLimitExceedException : public std::exception {
private:
    std::string message;

public:
    WithdrawalLimitExceedException(const std::string& msg) : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class DepositAmountNegative: public std::exception{
    private:
        std::string message;

    public:
        DepositAmountNegative(const std::string& msg) : message(msg) {}

        virtual const char* what() const noexcept override {
            return message.c_str();
        }
};