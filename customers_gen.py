import random,csv
# Define the file name for customer data
filename_customers = "customers.csv"

# Create the headers for customer data
headers_customers = ["id", "name"]

# List of random names to choose from
names = ["Alice", "Bob", "Charlie", "David", "Emma", "Fiona", "George", "Hannah", "Ivy", "Jack",
         "Katherine", "Liam", "Mia", "Nathan", "Olivia", "Paul", "Quincy", "Rachel", "Sophia", "Tom",
         "Uma", "Vera", "Will", "Xander", "Yara", "Zoe"]

# Generate customer data for 100 customers
customers = []
for i in range(1, 101):
    customer_id = "cid"+str(i)
    customer_name = random.choice(names) + str(i)  # Random name with a unique number
    customers.append([customer_id, customer_name])

# Write to CSV
with open(filename_customers, 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(headers_customers)
    writer.writerows(customers)

filename_customers
