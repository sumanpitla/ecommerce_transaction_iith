#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Product {
    string productId;
    int quantity;
    double price;
};

class Store {
private:
    unordered_map<string, Product> productList;

public:
    // Load products from CSV file
    void loadProducts(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open product file!" << endl;
            return;
        }

        string line;
        getline(file, line); // Skip header line

        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, quantity_str, price_str;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, quantity_str, ',');
            getline(ss, price_str, ',');

            int quantity = stoi(quantity_str);
            double price = stod(price_str);

            productList[id] = {id, quantity, price};
        }

        file.close();
    }

    void displayProductList() {
        cout << "Available Products:\n";
        cout << "Product ID | Quantity | Price\n";
        cout << "-----------------------------\n";
        for (const auto& productPair : productList) {
            const Product& product = productPair.second;
            cout << product.productId 
                 << " | " << product.quantity 
                 << " | $" << product.price << '\n';
        }
        cout << "-----------------------------\n";
    }

    bool validateOrder(const vector<pair<string, int>>& orderList) {
        for (const auto& order : orderList) {
            string productId = order.first;
            int orderQuantity = order.second;

            if (productList.find(productId) == productList.end()) {
                cout << "Transaction failed: Product ID " << productId << " does not exist.\n";
                return false;
            }

            if (productList[productId].quantity < orderQuantity) {
                cout << "Transaction failed: Product ID " << productId << " has insufficient quantity.\n";
                return false;
            }
        }
        return true;
    }

    void placeOrder(const string& customerId, const vector<pair<string, int>>& orderList) {
        if (!validateOrder(orderList)) {
            cout << "Transaction aborted for Customer ID " << customerId << ".\n";
            return;  // Abort the transaction if validation fails
        }

        double totalCost = 0.0;

        for (const auto& order : orderList) {
            string productId = order.first;
            int orderQuantity = order.second;

            // Deduct the ordered quantity from the store
            productList[productId].quantity -= orderQuantity;

            // Calculate the total cost for the current product
            totalCost += productList[productId].price * orderQuantity;
        }

        // Display successful transaction and total cost
        cout << "Transaction successful for Customer ID " << customerId << ".\n";
        cout << "Total Cost: $" << totalCost << '\n';
    }

    void processOrders(const string& filename, const string& outputFilename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open order file!" << endl;
            return;
        }

        ofstream outputFile(outputFilename);
        if (!outputFile.is_open()) {
            cerr << "Failed to open output file!" << endl;
            return;
        }

        string line;
        getline(file, line); // Skip header line

        while (getline(file, line)) {
            stringstream ss(line);
            string customerId, no_of_products_str;
            vector<pair<string, int>> orderList;

            getline(ss, customerId, ',');
            getline(ss, no_of_products_str, ',');

            int no_of_products = stoi(no_of_products_str);

            for (int i = 0; i < no_of_products; ++i) {
                string productId, quantity_str;
                getline(ss, productId, ',');
                getline(ss, quantity_str, ',');

                int quantity = stoi(quantity_str);
                orderList.push_back({productId, quantity});
            }

            if (validateOrder(orderList)) {
                placeOrder(customerId, orderList);
                outputFile << customerId << ", Transaction Successful\n";
            } else {
                outputFile << customerId << ", Transaction Failed\n";
            }
        }

        file.close();
        outputFile.close();
    }
};

int main() {
    Store store;

    // Load products from CSV
    store.loadProducts("products.csv");

    // Display predefined product list
    store.displayProductList();

    // Process orders from CSV and store transactions in output file
    store.processOrders("orders.csv", "customer_transactions.csv");

    // Display updated product list after transactions
    cout << "\nUpdated Product List:\n";
    store.displayProductList();

    return 0;
}
