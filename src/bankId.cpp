#include <iostream>
#include <string>
#include <ctime>

// Function to get last two digits of current year
int getLastTwoDigitsOfYear() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int year = 1900 + ltm->tm_year;
    return year % 100;
}

// Function to generate bank account IDs
std::string generateCurrentAccountID(const std::string& bankName, int sequenceNumber) {
    int lastTwoDigitsOfYear = getLastTwoDigitsOfYear();
    std::string id = std::to_string(lastTwoDigitsOfYear) + bankName + std::to_string(sequenceNumber);
    return id;
}

int main() {
    std::string bankName = "HDFC"; // Example bank name
    int numberOfAccounts = 5; // Example number of accounts to generate

    for (int i = 1; i <= numberOfAccounts; ++i) {
        std::string accountID = generateCurrentAccountID(bankName, i);
        std::cout << "Bank Account ID " << i << ": " << accountID << std::endl;
    }

    return 0;
}
