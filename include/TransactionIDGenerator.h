#ifndef TransactionIDGenerator_H
#define TransactionIDGenerator_H

#pragma once

#include <iostream>
#include <ctime>
#include <unordered_set>
#include <random>
#include <mutex>
#include <sstream>
#include <chrono>

class TransactionIDGenerator {
private:
    static std::unordered_set<std::string> generatedIDs;

public:
    static std::string generateTransactionID();

private:
    static std::string generateUniqueID();

    static bool isUnique(const std::string& id);
};


#endif