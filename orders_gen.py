import csv
import random
# Define the file name for order data
filename_orders = "orders.csv"

# Create the headers for order data
headers_orders = ["customer_id", "no_of_products", "product_id_1", "quantity_1", 
                  "product_id_2", "quantity_2", "product_id_3", "quantity_3"]

# Generate order data for some customers
orders = []
for i in range(1, 21):  # Generate orders for 20 customers
    customer_id = "cid"+str(i)
    no_of_products = 3  # Each customer orders between 1 to 3 products
    product_ids =  ["pid" + str(pid) for pid in random.sample(range(1, 1001), no_of_products)]# Random product ids from 1 to 1000
    quantities = [random.randint(1, 10) for _ in range(no_of_products)]  # Random quantities between 1 to 10
    
    # Fill in the order details (with placeholders for non-ordered products)
    order = [customer_id, no_of_products] + \
            [item for pair in zip(product_ids, quantities) for item in pair] + \
            [None] * (6 - 2 * no_of_products)  # Fill remaining columns with None if fewer than 3 products
    
    orders.append(order)

# Write to CSV
with open(filename_orders, 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(headers_orders)
    writer.writerows(orders)

filename_orders
