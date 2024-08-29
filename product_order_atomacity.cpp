#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

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
            string id_str, name, quantity_str, price_str;

            getline(ss, id_str, ',');
            getline(ss, name, ',');
            getline(ss, quantity_str, ',');
            getline(ss, price_str, ',');

            int id = stoi(id_str);
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
            cout << "   " << product.productId 
                 << "      |    " << product.quantity 
                 << "     |  $" << product.price << '\n';
        }
        cout << "-----------------------------\n";
    }

    bool validateOrder(const vector<pair<int, int>>& orderList) {
        // Check if all products are available and quantities are sufficient
        for (const auto& order : orderList) {
            int productId = order.first;
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

    void placeOrder(int customerId, const vector<pair<int, int>>& orderList) {
        // Validate the order before applying changes
        if (!validateOrder(orderList)) {
            cout << "Transaction aborted for Customer ID " << customerId << ".\n";
            return;  // Abort the transaction if validation fails
        }

        double totalCost = 0.0;

        // Process the order if all products are validated
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

            // Calculate the total cost for the current product
            totalCost += productList[productId].price * orderQuantity;
        }
        transactions.push_back(customerTransaction);

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

        int customerCount = 0;
        while (getline(file, line) && customerCount < 10) {
            stringstream ss(line);
            string customerId_str, no_of_products_str, product_id_1_str, quantity_1_str,
                   product_id_2_str, quantity_2_str, product_id_3_str, quantity_3_str;

            getline(ss, customerId_str, ',');
            getline(ss, no_of_products_str, ',');
            getline(ss, product_id_1_str, ',');
            getline(ss, quantity_1_str, ',');
            getline(ss, product_id_2_str, ',');
            getline(ss, quantity_2_str, ',');
            getline(ss, product_id_3_str, ',');
            getline(ss, quantity_3_str, ',');

            int customerId = stoi(customerId_str);
            int no_of_products = stoi(no_of_products_str);

            vector<pair<int, int>> orderList;
            if (!product_id_1_str.empty() && !quantity_1_str.empty())
                orderList.push_back({stoi(product_id_1_str), stoi(quantity_1_str)});
            if (!product_id_2_str.empty() && !quantity_2_str.empty())
                orderList.push_back({stoi(product_id_2_str), stoi(quantity_2_str)});
            if (!product_id_3_str.empty() && !quantity_3_str.empty())
                orderList.push_back({stoi(product_id_3_str), stoi(quantity_3_str)});

            placeOrder(customerId, orderList);

            // Save transaction to the output file if successful
            if (validateOrder(orderList)) {
                outputFile << customerId << ", ";
                for (const auto& order : orderList) {
                    outputFile << "Product ID: " << order.first << " Quantity: " << order.second << ", ";
                }
                outputFile << endl;

                customerCount++;
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
