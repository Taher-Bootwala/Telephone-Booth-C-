#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <iomanip> // Include this for std::setw and std::setfill

using namespace std;

struct Subscriber {
    int id;
    string name;
    int visitCount;
    int pin; // Added pin for subscriber authentication
};

struct CallHistory {
    int callerID;
    string callType;
};

// Function to save subscriber information to a file
void saveSubscriberToFile(const Subscriber& subscriber) {
    ofstream file("subscribers.txt", ios_base::app);
    if (file.is_open()) {
        file << subscriber.id << ' '
             << subscriber.name << ' '
             << subscriber.visitCount << ' '
             << setw(4) << setfill('0') << subscriber.pin << '\n';
        file.close();
    } else {
        cerr << "Error opening file for writing.\n";
    }
}

// Function to save PINs to a file
void savePinsToFile(const vector<Subscriber>& subscribers) {
    ofstream file("pins.txt", ios_base::trunc);
    if (file.is_open()) {
        for (const auto& subscriber : subscribers) {
            file << subscriber.id << ' '
                 << setw(4) << setfill('0') << subscriber.pin << '\n';
        }
        file.close();
    } else {
        cerr << "Error opening file for writing PINs.\n";
    }
}

// Function to load PINs from a file
void loadPinsFromFile(vector<Subscriber>& subscribers) {
    ifstream file("pins.txt");
    if (file.is_open()) {
        subscribers.clear();
        Subscriber tempSubscriber;
        while (file >> tempSubscriber.id >> tempSubscriber.pin) {
            tempSubscriber.name = "";
            tempSubscriber.visitCount = 0;
            subscribers.push_back(tempSubscriber);
        }
        file.close();
    } else {
        cerr << "Error opening file for reading PINs.\n";
    }
}

// Function to save call history to a file
void saveCallHistoryToFile(const CallHistory& callHistory) {
    ofstream file("call_history.txt", ios_base::app);
    if (file.is_open()) {
        file << callHistory.callerID << ' '
             << callHistory.callType << '\n';
        file.close();
    } else {
        cerr << "Error opening file for writing call history.\n";
    }
}

// Function to display call history from a file
void displayCallHistoryFromFile() {
    ifstream file("call_history.txt");
    if (file.is_open()) {
        cout << "Call History:\n";
        CallHistory callHistory;
        while (file >> callHistory.callerID >> callHistory.callType) {
            cout << "Customer " << callHistory.callerID << ": "
                      << callHistory.callType << '\n';
        }
        file.close();
    } else {
        cerr << "Error opening file for reading call history.\n";
    }
}

// Function to generate a random 4-digit PIN
int generateRandomPIN() {
    return rand() % 10000;
}

// Function to authenticate a subscriber based on name and PIN
int authenticateSubscriber(const vector<Subscriber>& subscribers, const string& name, int pin) {
    return find_if(subscribers.begin(), subscribers.end(),
                        [&name, pin](const Subscriber& subscriber) {
                            return subscriber.name == name && subscriber.pin == pin;
                        }) - subscribers.begin();
}

int main() {
    // Seed for random number generation
    srand((unsigned int)time(NULL));

    int max;
    cout << "Enter the maximum number of customers the telephone booth can serve: ";
    cin >> max;

    vector<int> queue(max);
    vector<int> customerIDs(max);
    vector<string> customerNames(max);
    int rear = -1, front = -1;
    int choice, customer, uniqueID = 1;
    int totalEarnings = 0;
    int totalServedCustomers = 0;

    vector<Subscriber> subscribers(max);
    int subscriberCount = 0;

    // Load PINs from file
    loadPinsFromFile(subscribers);

    while (true) {
        cout << "Telephone Booth Queue Management:\n"
                  << "1) Add customer\n"
                  << "2) Serve customer\n"
                  << "3) Display subscribers\n"
                  << "4) Total Served Customers\n"
                  << "5) Total Earnings\n"
                  << "6) Subscriber Login\n"
                  << "7) Display Call History\n"
                  << "8) exit\n";
        cin >> choice;

        switch (choice) {
            case 1:
                // Add customer (including subscriber creation)
                if ((rear + 1) % max == front) {
                    cout << "Queue is full. Cannot add more customers.\n";
                } else {
                    if (front == -1 && rear == -1) {
                        front = rear = 0;
                    } else {
                        rear = (rear + 1) % max;
                    }
                    cout << "Enter the customer number: ";
                    cin >> customer;

                    // Assigning customer ID
                    customerIDs[rear] = uniqueID;

                    cout << "Customer " << customer
                              << ", please pay 5 Rs before using the telephone.\n";
                    cout << "Did the customer pay? (1 for Yes, 0 for No): ";
                    int paid;
                    cin >> paid;

                    if (paid == 1) {
                        cout << "Customer " << customer
                                  << ", choose the type of call:\n";
                        cout << "1) Local Call\n"
                                  << "2) Abroad Call\n";
                        int callType;
                        cin >> callType;

                        CallHistory callHistory;
                        callHistory.callerID = customerIDs[rear];

                        if (callType == 1) {
                            cout << "Local call initiated for Customer " << customer << ".\n";
                            callHistory.callType = "Local Call";
                            saveCallHistoryToFile(callHistory);

                            if (customerIDs[rear] != 0) {
                                totalEarnings += 5; // Assuming a fixed charge for local calls
                                totalServedCustomers++;
                            }
                        } else if (callType == 2) {
                            cout << "Abroad call initiated for Customer " << customer << ".\n";
                            cout << "Enter the abroad number: ";
                            string abroadNumber;
                            cin.ignore();
                            getline(cin, abroadNumber);

                            callHistory.callType = "Abroad Call: " + abroadNumber;
                            saveCallHistoryToFile(callHistory);

                            if (customerIDs[rear] != 0) {
                                totalEarnings += 10; // Assuming a fixed charge for abroad calls
                                totalServedCustomers++;
                            }
                        } else {
                            cout << "Invalid choice. Call type not recognized.\n";
                        }

                        // Continue with the rest of the code for subscription and queue addition
                        cout << "Would you like to purchase a yearly subscription for 250 Rs? (1 for Yes, 0 for No): ";
                        int subscribe;
                        cin >> subscribe;

                        if (subscribe == 1) {
                            if (customerIDs[rear] != 0) {
                                totalEarnings += 250;

                                cout << "Enter the customer name: ";
                                cin.ignore();
                                getline(cin, customerNames[uniqueID - 1]);

                                Subscriber subscriber;
                                subscriber.id = uniqueID;
                                subscriber.name = customerNames[uniqueID - 1];
                                subscriber.visitCount = 1;
                                subscriber.pin = generateRandomPIN();

                                subscribers[subscriberCount] = subscriber;
                                subscriberCount++;

                                saveSubscriberToFile(subscriber);
                                // Save PINs to file after adding a new subscriber
                                savePinsToFile(subscribers);

                                cout << "Customer " << customer
                                          << ", your yearly subscription is confirmed. Welcome, "
                                          << customerNames[uniqueID - 1] << "! Your PIN is: "
                                          << setw(4) << setfill('0') << subscriber.pin << '\n';
                            }
                        } else {
                            customerIDs[rear] = 0;
                        }

                        cout << "Customer " << customer << " added to the queue.\n";
                        uniqueID++;
                    } else {
                        cout << "Customer " << customer
                                  << " did not pay and was not added to the queue.\n";
                        rear = (rear - 1 + max) % max;
                    }
                }
                break;

            case 2:
                // Serve customer
                if (front == -1 && rear == -1) {
                    cout << "Queue is empty. No customers to serve.\n";
                } else {
                    cout << "Serving customers...\n";

                    while (front != -1) {
                        cout << "Serving customer " << customerIDs[front] << '\n';

                        // Increment visit count for the subscriber
                        if (customerIDs[front] > 0 && customerIDs[front] <= max) {
                            subscribers[customerIDs[front] - 1].visitCount++;
                        }

                        if (front == rear) {
                            front = rear = -1;
                        } else {
                            front = (front + 1) % max;
                        }
                    }

                    cout << "Total customers served: " << totalServedCustomers << '\n';
                }
                break;

            case 3:
                // Display subscribers
                if (subscriberCount == 0) {
                    cout << "No subscribers to display.\n";
                } else {
                    cout << "Subscribers:\n";
                    for (int i = 0; i < subscriberCount; i++) {
                        cout << "ID: " << subscribers[i].id
                                  << ", Name: " << subscribers[i].name
                                  << ", Visit Count: " << subscribers[i].visitCount
                                  << ", PIN: " << setw(4) << setfill('0') << subscribers[i].pin
                                  << '\n';
                    }
                }
                break;

            case 4:
                // Display total served customers
                cout << "Total Served Customers: " << totalServedCustomers << '\n';
                break;

            case 5:
                // Display total earnings
                cout << "Total Earnings: " << totalEarnings << " Rs\n";
                break;

            case 6:
                // Subscriber login
                cout << "Subscriber Login:\n";
                cout << "Enter your name: ";
                string loginName;
                cin.ignore();
                getline(cin, loginName);
                cout << "Enter your 4-digit PIN: ";
                int loginPIN;
                cin >> loginPIN;

                int subscriberIndex = authenticateSubscriber(subscribers, loginName, loginPIN);
                if (subscriberIndex >= 0 && subscriberIndex < subscribers.size()) {
                    cout << "Welcome, " << subscribers[subscriberIndex].name << "!\n";

                    // Serving subscriber
                    cout << "Subscriber " << subscribers[subscriberIndex].name
                              << " is being served.\n";
                    subscribers[subscriberIndex].visitCount++;

                    cout << "Choose the type of call:\n";
                    cout << "1) Local Call\n"
                              << "2) Abroad Call\n";
                    int callChoice;
                    cin >> callChoice;

                    CallHistory callHistory;
                    callHistory.callerID = subscribers[subscriberIndex].id;

                    switch (callChoice) {
                        case 1:
                            // Local call
                            cout << "Local call initiated.\n";
                            cout << "Local call details: Enter local number, duration, etc.\n";
                            callHistory.callType = "Local Call";
                            saveCallHistoryToFile(callHistory);
                            break;

                        case 2:
                            // Abroad call
                            cout << "Abroad call initiated.\n";
                            cout << "Enter the abroad number: ";
                            string abroadNumber;
                            cin.ignore();
                            getline(cin, abroadNumber);

                            cout << "Abroad call details: Enter duration, etc.\n";
                            callHistory.callType = "Abroad Call: " + abroadNumber;
                            saveCallHistoryToFile(callHistory);
                            break;

                        default:
                            cout << "Invalid choice. Call type not recognized.\n";
                            break;
                    }
                } else {
                    cout << "Invalid name or PIN. Access denied.\n";
                }
                break;

            case 7:
                // Display call history
                displayCallHistoryFromFile();
                break;

            case 8:
                // Exit
                cout << "Exiting the program.\n";
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}
