#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Order {
    string productId;
    int quantity;
};

// Function to update the product quantity in the CSV file
bool updateProductQuantity(const string& productId, int orderQuantity) {
    ifstream file("products.csv");
    if (!file.is_open()) {
        cerr << "Failed to open product file!" << endl;
        return false;
    }

    vector<string> fileLines;
    string line;
    bool productFound = false;
    bool sufficientQuantity = true;

    // Read the CSV file line by line
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, quantity_str, price_str;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, quantity_str, ',');
        getline(ss, price_str, ',');

        if (id == productId) {
            productFound = true;
            int currentQuantity = stoi(quantity_str);

            if (currentQuantity < orderQuantity) {
                sufficientQuantity = false;
                break;
            }

            currentQuantity -= orderQuantity;
            line = id + "," + name + "," + to_string(currentQuantity) + "," + price_str;
            
        }

        fileLines.push_back(line);
    }

    file.close();

    if (!productFound) {
        cout << "Transaction failed: Product ID " << productId << " does not exist.\n";
        return false;
    }

    if (!sufficientQuantity) {
        cout << "Transaction failed: Product ID " << productId << " has insufficient quantity.\n";
        return false;
    }

    // Write the updated lines back to the CSV file
    ofstream outFile("products.csv");
    if (!outFile.is_open()) {
        cerr << "Failed to open product file for writing!" << endl;
        return false;
    }

    for (const auto& updatedLine : fileLines) {
        outFile << updatedLine << "\n";
    }

    outFile.close();
    return true;
}

// Function to process a single order
bool processOrder(const vector<Order>& orderList) {
    for (const auto& order : orderList) {
        if (!updateProductQuantity(order.productId, order.quantity)) {
            return false;
        }
    }
    return true;
}

// Function to process orders from the CSV file
void processOrders(const string& txn_id, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open order file!" << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header line

    bool found = false;
    while (getline(file, line)) {
        stringstream ss(line);
        string transactionId, no_of_products_str, product_id_1, quantity_1,
               product_id_2, quantity_2, product_id_3, quantity_3;

        getline(ss, transactionId, ',');
        getline(ss, no_of_products_str, ',');
        getline(ss, product_id_1, ',');
        getline(ss, quantity_1, ',');
        getline(ss, product_id_2, ',');
        getline(ss, quantity_2, ',');
        getline(ss, product_id_3, ',');
        getline(ss, quantity_3, ',');

        if (transactionId == txn_id) {
            found = true;

            vector<Order> orderList;
            if (!product_id_1.empty() && !quantity_1.empty())
                orderList.push_back({product_id_1, stoi(quantity_1)});
            if (!product_id_2.empty() && !quantity_2.empty())
                orderList.push_back({product_id_2, stoi(quantity_2)});
            if (!product_id_3.empty() && !quantity_3.empty())
                orderList.push_back({product_id_3, stoi(quantity_3)});

            if (processOrder(orderList)) {
                cout << "Transaction successful for Transaction ID " << txn_id << ".\n";
            } else {
                cout << "Transaction aborted for Transaction ID " << txn_id << ".\n";
            }

            break;  // Stop searching after finding the transaction ID
        }
    }

    if (!found) {
        cout << "Transaction ID " << txn_id << " not found.\n";
    }

    file.close();
}

int main() {
    //Store store;
    int txn_id;
    string filename = "orders.csv";
    
    // Input transaction ID
    cout << "Enter transaction ID: ";
    cin >> txn_id;

    // Convert integer txn_id to formatted string "txnX"
    string txn_id_str = "txn" + to_string(txn_id);

    // Process orders from CSV
    processOrders(txn_id_str, filename);

    return 0;
}



//For making atomic transacion and updating the product list in the csv file
// #include <iostream>
// #include <unordered_map>
// #include <vector>
// #include <fstream>
// #include <sstream>
// #include <string>

// using namespace std;

// struct Product {
//     string productId;
//     string productName;
//     int quantity;
//     double price;
// };

// class Store {
// private:
//     unordered_map<string, Product> productList;

// public:
//     // Load products from CSV file
//     void loadProducts(const string& filename) {
//         productList.clear(); // Clear the previous product list

//         ifstream file(filename);
//         if (!file.is_open()) {
//             cerr << "Failed to open product file!" << endl;
//             return;
//         }

//         string line;
//         getline(file, line); // Skip header line

//         while (getline(file, line)) {
//             stringstream ss(line);
//             string id, name, quantity_str, price_str;

//             getline(ss, id, ',');
//             getline(ss, name, ',');
//             getline(ss, quantity_str, ',');
//             getline(ss, price_str, ',');

//             int quantity = stoi(quantity_str);
//             double price = stod(price_str);

//             productList[id] = {id, name, quantity, price};
//         }
//            for (const auto& pair : productList) {
//             const Product& product = pair.second;
//             cout << product.productId << "," << product.productName << "," << product.quantity << "," << product.price << "\n";
//            }
//         file.close();
//     }

//     bool validateOrder(const vector<pair<string, int>>& orderList) {
//         // Check if all products are available and quantities are sufficient
//         for (const auto& order : orderList) {
//             const string& productId = order.first;
//             int orderQuantity = order.second;

//             if (productList.find(productId) == productList.end()) {
//                 cout << "Transaction failed: Product ID " << productId << " does not exist.\n";
//                 return false;
//             }

//             if (productList[productId].quantity < orderQuantity) {
//                 cout << "Transaction failed: Product ID " << productId << " has insufficient quantity.\n";
//                 return false;
//             }
//         }
//         return true;
//     }

//     void placeOrder(const string& txnId, const vector<pair<string, int>>& orderList) {
//         // Load products from CSV for each customer to ensure up-to-date quantities
//         loadProducts("products.csv");

//         // Validate the order before applying changes
//         if (!validateOrder(orderList)) {
//             cout << "Transaction aborted for Transaction ID " << txnId << ".\n";
//             return;  // Abort the transaction if validation fails
//         }

//         double totalCost = 0.0;

//         // Process the order if all products are validated
//         for (const auto& order : orderList) {
//             const string& productId = order.first;
//             int orderQuantity = order.second;

//             // Deduct the ordered quantity from the store
//             productList[productId].quantity -= orderQuantity;

//             // Calculate the total cost for the current product
//             totalCost += productList[productId].price * orderQuantity;
//         }

//         // Display successful transaction and total cost
//         cout << "Transaction successful for Transaction ID " << txnId << ".\n";
//         cout << "Total Cost: $" << totalCost << '\n';

//         // Save the updated product list back to the CSV file
//         saveUpdatedProducts("products.csv");
//     }

//     // Save only the updated product quantities back to the CSV file
//     void saveUpdatedProducts(const string& filename) {
//         ifstream infile(filename);
//         if (!infile.is_open()) {
//             cerr << "Failed to open product file for reading!" << endl;
//             return;
//         }

//         string tempFilename = "temp_products.csv";
//         ofstream outfile(tempFilename);
//         if (!outfile.is_open()) {
//             cerr << "Failed to open temp file for writing!" << endl;
//             return;
//         }

//         string line;
//         getline(infile, line); // Copy header
//         outfile << line << '\n';

//         while (getline(infile, line)) {
//             stringstream ss(line);
//             string id, name, quantity_str, price_str;

//             getline(ss, id, ',');
//             getline(ss, name, ',');
//             getline(ss, quantity_str, ',');
//             getline(ss, price_str, ',');

//             // If the product was updated, replace its quantity in the file
//             if (productList.find(id) != productList.end()) {
//                 Product& product = productList[id];
//                 outfile << product.productId << "," << product.productName << ","
//                         << product.quantity << "," << product.price << "\n";
//             } else {
//                 outfile << line << "\n"; // Keep the line unchanged
//             }
//         }

//         infile.close();
//         outfile.close();

//         // Replace the original file with the updated file
//         remove(filename.c_str());
//         rename(tempFilename.c_str(), filename.c_str());
//     }

//     void processOrders(const string& txn_id, const string& filename) {
//         ifstream file(filename);
//         if (!file.is_open()) {
//             cerr << "Failed to open order file!" << endl;
//             return;
//         }

//         string line;
//         getline(file, line); // Skip header line

//         bool found = false;
//         while (getline(file, line)) {
//             stringstream ss(line);
//             string transactionId, no_of_products_str, product_id_1, quantity_1,
//                    product_id_2, quantity_2, product_id_3, quantity_3;

//             getline(ss, transactionId, ',');
//             getline(ss, no_of_products_str, ',');
//             getline(ss, product_id_1, ',');
//             getline(ss, quantity_1, ',');
//             getline(ss, product_id_2, ',');
//             getline(ss, quantity_2, ',');
//             getline(ss, product_id_3, ',');
//             getline(ss, quantity_3, ',');

//             if (transactionId == txn_id) {
//                 found = true;

//                 vector<pair<string, int>> orderList;
//                 if (!product_id_1.empty() && !quantity_1.empty())
//                     orderList.push_back({product_id_1, stoi(quantity_1)});
//                 if (!product_id_2.empty() && !quantity_2.empty())
//                     orderList.push_back({product_id_2, stoi(quantity_2)});
//                 if (!product_id_3.empty() && !quantity_3.empty())
//                     orderList.push_back({product_id_3, stoi(quantity_3)});

//                 placeOrder(transactionId, orderList);
//                 break;  // Stop searching after finding the transaction ID
//             }
//         }

//         if (!found) {
//             cout << "Transaction ID " << txn_id << " not found.\n";
//         }

//         file.close();
//     }
// };

// int main() {
//     Store store;
//     int txn_id;
//     string filename = "orders.csv";
    
//     // Input transaction ID
//     cout << "Enter transaction ID: ";
//     cin >> txn_id;

//     // Convert integer txn_id to formatted string "txnX"
//     string txn_id_str = "txn" + to_string(txn_id);

//     // Process orders from CSV
//     store.processOrders(txn_id_str, filename);

//     return 0;
// }


// #include <iostream>
// #include <unordered_map>
// #include <vector>
// #include <fstream>
// #include <sstream>
// #include <string>

// using namespace std;

// struct Product {
//     string productId;
//     string productName;
//     int quantity;
//     double price;
// };

// class Store {
// private:
//     unordered_map<string, Product> productList;

// public:
//     // Load products from CSV file
//     void loadProducts(const string& filename) {
//         productList.clear(); // Clear the previous product list

//         ifstream file(filename);
//         if (!file.is_open()) {
//             cerr << "Failed to open product file!" << endl;
//             return;
//         }

//         string line;
//         getline(file, line); // Skip header line

//         while (getline(file, line)) {
//             stringstream ss(line);
//             string id, name, quantity_str, price_str;

//             getline(ss, id, ',');
//             getline(ss, name, ',');
//             getline(ss, quantity_str, ',');
//             getline(ss, price_str, ',');

//             int quantity = stoi(quantity_str);
//             double price = stod(price_str);

//             productList[id] = {id, name, quantity, price};
//         }

//         file.close();
//     }

//     bool validateOrder(const vector<pair<string, int>>& orderList) {
//         // Check if all products are available and quantities are sufficient
//         for (const auto& order : orderList) {
//             const string& productId = order.first;
//             int orderQuantity = order.second;

//             if (productList.find(productId) == productList.end()) {
//                 cout << "Transaction failed: Product ID " << productId << " does not exist.\n";
//                 return false;
//             }

//             if (productList[productId].quantity < orderQuantity) {
//                 cout << "Transaction failed: Product ID " << productId << " has insufficient quantity.\n";
//                 return false;
//             }
//         }
//         return true;
//     }

//     void placeOrder(const string& customerId, const vector<pair<string, int>>& orderList) {
//         // Load products from CSV for each customer to ensure up-to-date quantities
//         loadProducts("products.csv");

//         // Validate the order before applying changes
//         if (!validateOrder(orderList)) {
//             cout << "Transaction aborted for Customer ID " << customerId << ".\n";
//             return;  // Abort the transaction if validation fails
//         }

//         double totalCost = 0.0;

//         // Process the order if all products are validated
//         for (const auto& order : orderList) {
//             const string& productId = order.first;
//             int orderQuantity = order.second;

//             // Deduct the ordered quantity from the store
//             productList[productId].quantity -= orderQuantity;

//             // Calculate the total cost for the current product
//             totalCost += productList[productId].price * orderQuantity;
//         }

//         // Display successful transaction and total cost
//         cout << "Transaction successful for Customer ID " << customerId << ".\n";
//         cout << "Total Cost: $" << totalCost << '\n';

//         // Save the updated product list back to the CSV file
//         saveProducts("products.csv");
//     }

//     // Save the updated product list to CSV file
//     void saveProducts(const string& filename) {
//         ofstream file(filename);
//         if (!file.is_open()) {
//             cerr << "Failed to open product file for writing!" << endl;
//             return;
//         }

//         file << "Product ID,Product Name,Quantity,Price\n"; // Write header
//         for (const auto& pair : productList) {
//             const Product& product = pair.second;
//             file << product.productId << "," << product.productName << "," << product.quantity << "," << product.price << "\n";
//         }

//         file.close();
//     }

//     void processOrders(int txn_id,const string& filename) {
//         ifstream file(filename);
//         if (!file.is_open()) {
//             cerr << "Failed to open order file!" << endl;
//             return;
//         }

//         string line;
//         getline(file, line); // Skip header line

//         while (getline(file, line)) {
//             stringstream ss(line);
//             string customerId, no_of_products_str, product_id_1, quantity_1,
//                    product_id_2, quantity_2, product_id_3, quantity_3;

//             getline(ss, customerId, ',');
//             getline(ss, no_of_products_str, ',');
//             getline(ss, product_id_1, ',');
//             getline(ss, quantity_1, ',');
//             getline(ss, product_id_2, ',');
//             getline(ss, quantity_2, ',');
//             getline(ss, product_id_3, ',');
//             getline(ss, quantity_3, ',');

//             vector<pair<string, int>> orderList;
//             if (!product_id_1.empty() && !quantity_1.empty())
//                 orderList.push_back({product_id_1, stoi(quantity_1)});
//             if (!product_id_2.empty() && !quantity_2.empty())
//                 orderList.push_back({product_id_2, stoi(quantity_2)});
//             if (!product_id_3.empty() && !quantity_3.empty())
//                 orderList.push_back({product_id_3, stoi(quantity_3)});

//             placeOrder(customerId, orderList);
//         }

//         file.close();
//     }
// };

// int main() {
//     Store store;
//     int txn_id;
//     string filename="orders.csv";
//     cin>>txn_id;

//     // Process orders from CSV
//     store.processOrders(txn_id,filename);

//     return 0;
// }
