#ifndef ProductOrderAtomacity_H
#define ProductOrderAtomacity_H

#include <vector>
#include <string>

struct Order {
    std::string productId;
    int quantity;
};

// Function declarations
void processOrders(const int txn_id, const std::string& filename);

#endif
