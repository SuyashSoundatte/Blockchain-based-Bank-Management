#include <iostream>
#include <chrono>
#include <random>
#include <unordered_set>
#include <sstream>

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

int main() {
    // Generate transaction IDs
    std::cout << "Transaction ID 1: " << TransactionIDGenerator::generateTransactionID() << std::endl;
    std::cout << "Transaction ID 4: " << TransactionIDGenerator::generateTransactionID() << std::endl;
    std::cout << "Transaction ID 2: " << TransactionIDGenerator::generateTransactionID() << std::endl;
    std::cout << "Transaction ID 3: " << TransactionIDGenerator::generateTransactionID() << std::endl;
    std::cout << "Transaction ID 4: " << TransactionIDGenerator::generateTransactionID() << std::endl;

    return 0;
}
