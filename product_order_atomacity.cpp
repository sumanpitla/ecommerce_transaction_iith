#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

struct Order {
    string productId;
    int quantity;
};

// Function to check if all products have sufficient quantity
bool checkSufficientQuantity(const vector<Order>& orderList) {
    ifstream file("products.csv");
    if (!file.is_open()) {
        cerr << "Failed to open product file!" << endl;
        return false;
    }
     
    string line;
   bool productFound = false;
    bool sufficientQuantity = true;
    int size=orderList.size();
    string notFoundProductId;
    map<string, bool> productMap;
    for(const auto& order : orderList){
        productMap[order.productId]=false;
    }


    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, quantity_str, price_str;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, quantity_str, ',');
        getline(ss, price_str, ',');
        for (const auto& order : orderList) {
            //cout<< id << " " << order.productId << endl;
            if (id == order.productId) {
                
                productMap[order.productId]=true;
                size-=1;
                //cout << "Product ID " << order.productId << " found.\n";
                int currentQuantity = stoi(quantity_str);

                if (currentQuantity < order.quantity) {
                    sufficientQuantity = false;
                    cout << "Transaction failed: Product ID " << order.productId << " has insufficient quantity.\n";
                    return false;
                }
                //orderList.erase(remove(orderList.begin(), orderList.end(), order), orderList.end());
                
            }

        }
 
        if(size==0){
            break;
        }
         
    }
    if(size>0){
        for(const auto& product:productMap){
            cout << product.first << " " << product.second << endl;
            if(product.second==false){
                notFoundProductId=product.first;
                break;
            }
        }
        cout << "Transaction failed: "<<notFoundProductId <<" not found.\n";
        return false;
    }

    file.close();
    return sufficientQuantity;
}

// Function to update the product quantities in the CSV file
bool updateProductQuantities(const vector<Order>& orderList) {
    ifstream file("products.csv");
    if (!file.is_open()) {
        cerr << "Failed to open product file!" << endl;
        return false;
    }

    vector<string> fileLines;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, quantity_str, price_str;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, quantity_str, ',');
        getline(ss, price_str, ',');

        for (const auto& order : orderList) {
            if (id == order.productId) {
                int currentQuantity = stoi(quantity_str);
                currentQuantity -= order.quantity;
                line = id + "," + name + "," + to_string(currentQuantity) + "," + price_str;
            }
        }

        fileLines.push_back(line);
    }

    file.close();

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
    if (!checkSufficientQuantity(orderList)) {
        return false;
    }
    return updateProductQuantities(orderList);
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
