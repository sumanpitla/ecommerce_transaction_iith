#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Product {
    string productId;
    string productName;
    int quantity;
    double price;
};

class Store {
private:
    unordered_map<string, Product> productList;

public:
    // Load products from CSV file
    void loadProducts(const string& filename) {
        productList.clear(); // Clear the previous product list

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

            productList[id] = {id, name, quantity, price};
        }

        file.close();
    }

    bool validateOrder(const vector<pair<string, int>>& orderList) {
        // Check if all products are available and quantities are sufficient
        for (const auto& order : orderList) {
            const string& productId = order.first;
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
        // Load products from CSV for each customer to ensure up-to-date quantities
        loadProducts("products.csv");

        // Validate the order before applying changes
        if (!validateOrder(orderList)) {
            cout << "Transaction aborted for Customer ID " << customerId << ".\n";
            return;  // Abort the transaction if validation fails
        }

        double totalCost = 0.0;

        // Process the order if all products are validated
        for (const auto& order : orderList) {
            const string& productId = order.first;
            int orderQuantity = order.second;

            // Deduct the ordered quantity from the store
            productList[productId].quantity -= orderQuantity;

            // Calculate the total cost for the current product
            totalCost += productList[productId].price * orderQuantity;
        }

        // Display successful transaction and total cost
        cout << "Transaction successful for Customer ID " << customerId << ".\n";
        cout << "Total Cost: $" << totalCost << '\n';

        // Save the updated product list back to the CSV file
        saveProducts("products.csv");
    }

    // Save the updated product list to CSV file
    void saveProducts(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open product file for writing!" << endl;
            return;
        }

        file << "Product ID,Product Name,Quantity,Price\n"; // Write header
        for (const auto& pair : productList) {
            const Product& product = pair.second;
            file << product.productId << "," << product.productName << "," << product.quantity << "," << product.price << "\n";
        }

        file.close();
    }

    void processOrders(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open order file!" << endl;
            return;
        }

        string line;
        getline(file, line); // Skip header line

        while (getline(file, line)) {
            stringstream ss(line);
            string customerId, no_of_products_str, product_id_1, quantity_1,
                   product_id_2, quantity_2, product_id_3, quantity_3;

            getline(ss, customerId, ',');
            getline(ss, no_of_products_str, ',');
            getline(ss, product_id_1, ',');
            getline(ss, quantity_1, ',');
            getline(ss, product_id_2, ',');
            getline(ss, quantity_2, ',');
            getline(ss, product_id_3, ',');
            getline(ss, quantity_3, ',');

            vector<pair<string, int>> orderList;
            if (!product_id_1.empty() && !quantity_1.empty())
                orderList.push_back({product_id_1, stoi(quantity_1)});
            if (!product_id_2.empty() && !quantity_2.empty())
                orderList.push_back({product_id_2, stoi(quantity_2)});
            if (!product_id_3.empty() && !quantity_3.empty())
                orderList.push_back({product_id_3, stoi(quantity_3)});

            placeOrder(customerId, orderList);
        }

        file.close();
    }
};

int main() {
    Store store;

    // Process orders from CSV
    store.processOrders("orders.csv");

    return 0;
}
