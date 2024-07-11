#include "include/TransactionIDGenerator.h"

std::string TransactionIDGenerator::generateTransactionID() {
        std::string id;
        do {
            id = generateUniqueID();
        } while (!isUnique(id));
        generatedIDs.insert(id);
        return id;
    }

std::string TransactionIDGenerator::generateUniqueID() {
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


bool TransactionIDGenerator::isUnique(const std::string& id) {
        return generatedIDs.find(id) == generatedIDs.end();
    }

// Initialize static member variable
std::unordered_set<std::string> TransactionIDGenerator::generatedIDs;