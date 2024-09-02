import csv
import random

# Define the file name
filename = "products.csv"

# Create the headers
headers = ["id", "name", "quantity", "price"]

# Generate product data
products = []
for i in range(1, 1001):
    product_id = "pid"+str(i)  # Format the product ID to have 3 digits
    product_name = f"product{i}"
    quantity = random.randint(1, 500)  # Random quantity between 1 and 500
    price = round(random.uniform(10.0, 1000.0), 2)  # Random price between 10 and 1000, with 2 decimal places
    products.append([product_id, product_name, quantity, price])

# Write to CSV
with open(filename, 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(headers)
    writer.writerows(products)

filename
