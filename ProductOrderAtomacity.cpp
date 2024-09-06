# include "ProductOrderAtomacity.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

// struct Order {
//     string productId;
//     int quantity;
// };

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
void processOrders(const int txn_id, const string& filename) {
    // Convert integer txn_id to formatted string "txnX"
    string txn_id_str = "txn" + to_string(txn_id);
    
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open order file!" << endl;
        return;
    }

    string line;
    //getline(file, line); // Skip header line
     
    bool found = false;
    while (getline(file, line)) {
        //Assuming that format of the CSV file is as follows:
        //transaction_id,no_of_products,{i=1 to no_of_products} product_id_i, quantity_i)
        //for example:if no_of_products=2 then=> product_id_1=1, quantity_1=2, product_id_2=3, quantity_2=4


        stringstream ss(line);
        string transactionId, no_of_products_str;

        getline(ss, transactionId, ',');
        getline(ss, no_of_products_str, ',');

        if (transactionId == txn_id_str) {
            found = true;

            int no_of_products = stoi(no_of_products_str);
            vector<Order> orderList;

            for (int i = 0; i < no_of_products; ++i) {
                string product_id, quantity_str;
                getline(ss, product_id, ',');
                getline(ss, quantity_str, ',');

                if (!product_id.empty() && !quantity_str.empty()) {
                    orderList.push_back({product_id, stoi(quantity_str)});
                }
            }


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

// int main() {
//     int txn_id;
//     string filename = "orders_temp.csv";
    
//     // Input transaction ID
//     cout << "Enter transaction ID: ";
//     cin >> txn_id;

    
    

//     // Process orders from CSV
//     processOrders(txn_id, filename);

//     return 0;
// }
