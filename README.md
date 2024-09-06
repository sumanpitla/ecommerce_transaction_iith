# E-commerce Transaction System

This project simulates a transaction system for an e-commerce platform. The system reads product information from a CSV file and processes orders from another CSV file to check product availability and update stock quantities accordingly.

## Project Information

### File Descriptions:

1. **`products.csv`**:
   - Contains details about each product in the following format:
     ```
     product_id,product_name,quantity,price
     ```

2. **`orders.csv`**:
   - Contains transaction details where each transaction includes a unique `txn_id`, the number of products in the order, and the corresponding product IDs and quantities ordered.
   - Format:
     ```
     txn_id,no_of_products,product_id_1,quantity_1,...,product_id_n,quantity_n
     ```

### How the Program Works:
- The program processes an order by reading from `orders.csv` based on the provided `txn_id` and verifies if there is enough stock in `products.csv` for each product.
- If the stock is sufficient, the program deducts the ordered quantity from the product stock and updates the `products.csv` file. If not, it prints an appropriate error message.

---

## Installation and Execution

### Clone the Repository:

```bash
git clone https://github.com/sumanpitla/ecommerce_transaction_iith.git
cd ecommerce_transaction_iith
```

## Commands to Execute (Windows):
### compile the program
```bash
g++ -o MainProgram MainProgram.cpp ProductOrderAtomacity.cpp

```
### Run the program
```bash
./MainProgram
```

## Input Example:
- The program will prompt you to enter a transaction ID.
- Example:
```bash
Enter transaction ID: 1
```
## Requirenments
- g++ compiler (GCC) for compilation.
- The project reads from and writes to two CSV files: products.csv and orders.csv.
