#include "ProductOrderAtomacity.h"
#include <iostream>
#include <string>

int main() {
    std::string filename = "orders_temp.csv";
    int txn_id;

    // Input transaction ID
    std::cout << "Enter transaction ID: ";
    std::cin >> txn_id;

    // Convert transaction ID to formatted string


    // Call the function from the other file
    processOrders(txn_id, filename);
    

    return 0;
}
