#include <stdio.h>
#include <string.h>

    
int main() {
    int max;
    printf("Enter the maximum number of customers the telephone booth can serve: ");
    scanf("%d", &max);
    int queue[max];
    int customerIDs[max]; // Array to store customer IDs
    char customerPasswords[max][10]; // Array to store customer passwords
    int rear = -1, front = -1;
    int choice, customer, uniqueID = 1;
    int totalEarnings = 0;

    while (1) {
        printf("Telephone Booth Queue Management:\n");
        printf("1) Add customer\n2) Serve customer\n3) Display queue\n4) Total Earnings\n5) Unique ID\n6) Display Subscribers\n7) Exit\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:                
                if (((rear + 1) % max) == front) {
                    printf("Queue is full. Cannot add more customers.\n");
                } else {
                    if (front == -1 && rear == -1) {
                        front = rear = 0;
                    } else {
                        rear = (rear + 1) % max;
                    }
                    printf("Enter the customer number: ");
                    scanf("%d", &customer);
                    printf("Customer %d, please pay 5 Rs before using the telephone.\n", customer);
                    printf("Did the customer pay? (1 for Yes, 0 for No): ");
                    int paid;
                    scanf("%d", &paid);
                    if (paid == 1) {
                        printf("Would you like to purchase a lifetime subscription for 250 Rs? (1 for Yes, 0 for No): ");
                        int subscribe;
                        scanf("%d", &subscribe);
                        if (subscribe == 1) {
                            totalEarnings += 250;
                            printf("Customer %d purchased a lifetime subscription with ID %d.\n", customer, uniqueID);
                            customerIDs[uniqueID - 1] = uniqueID; // Assign a unique ID
                            sprintf(customerPasswords[uniqueID - 1], "Pass%d", uniqueID); // Generate a password
                            uniqueID++;
                        } else {
                            customerIDs[rear] = 0; // Mark as a non-subscriber
                        }
                        printf("Customer %d added to the queue.\n", customer);
                        queue[rear] = customer;
                    } else {
                        printf("Customer %d did not pay and was not added to the queue.\n", customer);
                    }
                }
                break;
            case 2:
                
                if (front == -1 && rear == -1) {
                    printf("Queue is empty. No customers to serve.\n");
                } else {
                    customer = queue[front];
                    if (front == rear) {
                        front = rear = -1;
                    } else {
                        front = (front + 1) % max;
                    }
                    printf("Customer %d served and removed from the queue.\n", customer);
                }
                break;
            case 3:
                if (front == -1 && rear == -1) {
                    printf("Queue is empty.\n");
                } else {
                    int i;
                    printf("Queue elements: ");
                    for (i = front; i != rear; i = (i + 1) % max) {
                        printf("%d ", queue[i]);
                    }
                    printf("%d\n", queue[i]);
                }
                break;
            case 4:
                printf("Total earnings: %d Rs\n", totalEarnings);
                break;
            case 5:
                if (uniqueID == 1) {
                    printf("No customers have purchased a lifetime subscription yet.\n");
                } else {
                    int id, enteredID;
                    char password[10];
                    printf("Enter your unique ID: ");
                    scanf("%d", &enteredID);
                    if (enteredID >= 1 && enteredID <= uniqueID) {
                        printf("Enter your password: ");
                        scanf("%s", password);
                        int index = enteredID - 1;
                        if (customerIDs[index] == enteredID && strcmp(customerPasswords[index], password) == 0) {
                            printf("Access granted! You are a lifetime subscriber with ID %d.\n", enteredID);
                        } else {
                            printf("Access denied. Invalid ID or password.\n");
                        }
                    } else {
                        printf("Invalid unique ID. Please try again.\n");
                    }
                }
                break;
            case 6:
                if (uniqueID == 1) {
                    printf("No customers have purchased a lifetime subscription yet.\n");
                } else {
                    printf("List of Subscribers:\n");
                    for (int i = 0; i < uniqueID; i++) {
                        if (customerIDs[i] != 0) {
                            printf("Customer ID: %d\n", customerIDs[i]);
                        }
                    }
                }
                break;
            case 7:
                printf("Exiting the telephone booth program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}