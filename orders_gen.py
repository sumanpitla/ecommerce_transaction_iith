import csv
import random

# Define the file name for order data
filename_orders = "orders.csv"

# Generate order data for some customers
orders = []
max_products_per_order = 0

for i in range(1, 21):  # Generate orders for 20 customers
    customer_id = "txn" + str(i)
    no_of_products = random.randint(1, 9)  # Each customer orders between 1 to 9 products randomly
    product_ids = ["pid" + str(pid) for pid in random.sample(range(1, 1001), no_of_products)]  # Random product ids from 1 to 1000
    quantities = [random.randint(1, 10) for _ in range(no_of_products)]  # Random quantities between 1 to 10

    # Track the maximum number of products in any order
    max_products_per_order = max(max_products_per_order, no_of_products)

    # Fill in the order details
    order = [customer_id, no_of_products] + \
            [item for pair in zip(product_ids, quantities) for item in pair]
    
    orders.append(order)

# Create the headers dynamically based on the max number of products ordered in any transaction
headers_orders = ["transaction_id", "no_of_products"]
for i in range(1, max_products_per_order + 1):
    headers_orders.append(f"product_id_{i}")
    headers_orders.append(f"quantity_{i}")

# Write to CSV
with open(filename_orders, 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(headers_orders)
    
    # Write the orders, filling with None where there are fewer products in the order
    for order in orders:
        writer.writerow(order + [None] * (2 * max_products_per_order - len(order) + 2))

print(f"Orders written to {filename_orders}")



