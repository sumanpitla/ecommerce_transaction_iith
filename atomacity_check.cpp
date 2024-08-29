#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Product {
    int productId;
    int quantity;
    double price;
};

class Store {
private:
    unordered_map<int, Product> productList;
    vector<vector<int>> transactions; // 2D vector to store transactions: [customerId, productId, quantity]

public:
    Store() {
        // Initialize product list with predefined products
        productList[1] = {1, 50, 5.0};
        productList[2] = {2, 40, 10.0};
        productList[3] = {3, 60, 7.5};
        productList[4] = {4, 30, 15.0};
        productList[5] = {5, 20, 20.0};
        productList[6] = {6, 25, 12.0};
        productList[7] = {7, 35, 8.0};
        productList[8] = {8, 45, 11.0};
        productList[9] = {9, 15, 25.0};
        productList[10] = {10, 55, 6.0};
    }

    void displayProductList() {
        cout << "Available Products:\n";
        cout << "Product ID | Quantity | Price\n";
        cout << "-----------------------------\n";
        for (const auto& productPair : productList) {
            const Product& product = productPair.second;
            cout << "   " << product.productId 
                 << "      |    " << product.quantity 
                 << "     |  $" << product.price << '\n';
        }
        cout << "-----------------------------\n";
    }

    void placeOrder(int customerId, const vector<pair<int, int>>& orderList) {
        double totalCost = 0.0;

        // Check if all products are available and quantities are sufficient
        for (const auto& order : orderList) {
            int productId = order.first;
            int orderQuantity = order.second;

            if (productList.find(productId) == productList.end()) {
                cout << "Transaction failed: Product ID " << productId << " does not exist.\n";
                return;
            }

            if (productList[productId].quantity < orderQuantity) {
                cout << "Transaction failed: Product ID " << productId << " has insufficient quantity.\n";
                return;
            }

            // Calculate the total cost for the current product
            totalCost += productList[productId].price * orderQuantity;
        }

        // All products are available and sufficient, process the order
        vector<int> customerTransaction;
        customerTransaction.push_back(customerId);
        for (const auto& order : orderList) {
            int productId = order.first;
            int orderQuantity = order.second;

            // Deduct the ordered quantity from the store
            productList[productId].quantity -= orderQuantity;

            // Record the transaction
            customerTransaction.push_back(productId);
            customerTransaction.push_back(orderQuantity);
        }
        transactions.push_back(customerTransaction);

        // Display successful transaction and total cost
        cout << "Transaction successful for Customer ID " << customerId << ".\n";
        cout << "Total Cost: $" << totalCost << '\n';
    }
};

int main() {
    Store store;

    // Display predefined product list
    store.displayProductList();

    int customerId;
    int numProducts;

    cout << "Enter Customer ID: ";
    cin >> customerId;
    cout << "Enter number of products you want to order: ";
    cin >> numProducts;

    vector<pair<int, int>> orderList;
    for (int i = 0; i < numProducts; ++i) {
        int productId, quantity;
        cout << "Enter Product ID and Quantity: ";
        cin >> productId >> quantity;
        orderList.push_back({productId, quantity});
    }

    store.placeOrder(customerId, orderList);

    // Display updated product list after transaction
    cout << "\nUpdated Product List:\n";
    store.displayProductList();

    return 0;
}

