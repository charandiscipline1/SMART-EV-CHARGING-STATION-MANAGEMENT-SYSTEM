//program 
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

const int TOTAL_STATIONS = 3;
const float RATE = 10.0;

struct Vehicle {
    string id;
    string owner;
    float battery;
    float capacity;
    string status;
};

struct Station {
    int stationId;
    string vehicleId;
    bool available;
};

struct Bill {
    string vehicleId;
    int stationId;
    float energy;
    float amount;
};

vector<Vehicle> vehicles;
vector<string> waitingQueue;
vector<Bill> history;

Station stations[TOTAL_STATIONS] = {
    {1, "", true},
    {2, "", true},
    {3, "", true}
};

int findVehicle(string id) {
    for (int i = 0; i < vehicles.size(); i++) {
        if (vehicles[i].id == id)
            return i;
    }
    return -1;
}

// Allocate available charging stations
void allocateStations() {

    sort(waitingQueue.begin(), waitingQueue.end(),
        [](string a, string b) {
            return vehicles[findVehicle(a)].battery <
                   vehicles[findVehicle(b)].battery;
        });

    for (int i = 0; i < TOTAL_STATIONS; i++) {

        if (stations[i].available && !waitingQueue.empty()) {

            string id = waitingQueue.front();
            waitingQueue.erase(waitingQueue.begin());

            stations[i].vehicleId = id;
            stations[i].available = false;

            int index = findVehicle(id);
            vehicles[index].status = "Charging";

            cout << "\nVehicle " << id
                 << " assigned to Station "
                 << stations[i].stationId << endl;
        }
    }
}

// Register a vehicle
void registerVehicle() {

    Vehicle v;

    cout << "\nEnter Vehicle ID: ";
    cin >> v.id;

    if (findVehicle(v.id) != -1) {
        cout << "Vehicle ID already exists!\n";
        return;
    }

    cout << "Enter Owner Name: ";
    cin >> v.owner;

    cout << "Enter Battery Level (0-100): ";
    cin >> v.battery;

    cout << "Enter Battery Capacity (kWh): ";
    cin >> v.capacity;

    if (cin.fail() || v.battery < 0 ||
        v.battery > 100 || v.capacity <= 0) {
        cout << "Invalid vehicle details!\n";
        return;
    }

    v.status = "Waiting";
    vehicles.push_back(v);
    waitingQueue.push_back(v.id);

    cout << "\nVehicle Registered Successfully!\n";

    allocateStations();
}

// Display charging stations
void displayStations() {

    cout << "\n--- CHARGING STATIONS ---\n";

    for (int i = 0; i < TOTAL_STATIONS; i++) {

        cout << "Station " << stations[i].stationId
             << " : ";

        if (stations[i].available)
            cout << "Available\n";
        else
            cout << "Occupied by "
                 << stations[i].vehicleId << endl;
    }
}

// Display registered vehicles
void displayVehicles() {

    cout << "\n--- REGISTERED VEHICLES ---\n";

    if (vehicles.empty()) {
        cout << "No vehicles registered.\n";
        return;
    }

    cout << left << setw(12) << "Vehicle ID"
         << setw(15) << "Owner"
         << setw(12) << "Battery"
         << "Status\n";

    for (auto v : vehicles) {
        cout << left << setw(12) << v.id
             << setw(15) << v.owner
             << setw(12) << v.battery
             << v.status << endl;
    }
}

// Display waiting queue
void displayQueue() {

    cout << "\n--- WAITING QUEUE ---\n";

    if (waitingQueue.empty()) {
        cout << "No waiting vehicles.\n";
        return;
    }

    for (int i = 0; i < waitingQueue.size(); i++) {
        int index = findVehicle(waitingQueue[i]);

        cout << i + 1 << ". "
             << vehicles[index].id
             << " - Battery: "
             << vehicles[index].battery
             << "%\n";
    }
}

// Complete charging and generate bill
void completeCharging() {

    int stationId;

    cout << "\nEnter Station ID: ";
    cin >> stationId;

    if (stationId < 1 || stationId > TOTAL_STATIONS) {
        cout << "Invalid Station ID!\n";
        return;
    }

    Station &s = stations[stationId - 1];

    if (s.available) {
        cout << "Station is already available!\n";
        return;
    }

    int index = findVehicle(s.vehicleId);

    float energy =
        vehicles[index].capacity *
        (100 - vehicles[index].battery) / 100;

    float amount = energy * RATE;

    cout << "\n--- CHARGING COMPLETED ---\n";
    cout << "Vehicle ID: " << vehicles[index].id << endl;
    cout << "Energy Charged: " << energy << " kWh\n";
    cout << "Total Bill: Rs. "
         << fixed << setprecision(2) << amount << endl;

    history.push_back({
        vehicles[index].id,
        stationId,
        energy,
        amount
    });

    vehicles[index].battery = 100;
    vehicles[index].status = "Completed";

    s.available = true;
    s.vehicleId = "";

    cout << "\nStation is now available.\n";

    allocateStations();
}

// Display charging and billing history
void displayHistory() {

    cout << "\n--- CHARGING & BILLING HISTORY ---\n";

    if (history.empty()) {
        cout << "No billing records available.\n";
        return;
    }

    for (auto b : history) {

        cout << "\nVehicle ID: " << b.vehicleId;
        cout << "\nStation ID: " << b.stationId;
        cout << "\nEnergy: " << b.energy << " kWh";
        cout << "\nBill: Rs. " << b.amount << endl;
    }
}

// Main function
int main() {

    int choice;

    do {
        cout << "\n====================================\n";
        cout << " SMART EV CHARGING STATION SYSTEM\n";
        cout << "====================================\n";

        cout << "\n1. Register Vehicle & Request Charging";
        cout << "\n2. View Charging Stations";
        cout << "\n3. View Registered Vehicles";
        cout << "\n4. View Waiting Queue";
        cout << "\n5. Complete Charging & Generate Bill";
        cout << "\n6. View Charging & Billing History";
        cout << "\n7. Exit";

        cout << "\n\nEnter your choice: ";
        cin >> choice;

        switch (choice) {

            case 1:
                registerVehicle();
                break;

            case 2:
                displayStations();
                break;

            case 3:
                displayVehicles();
                break;

            case 4:
                displayQueue();
                break;

            case 5:
                completeCharging();
                break;

            case 6:
                displayHistory();
                break;

            case 7:
                cout << "\nThank you for using EV Charge Manager!\n";
                break;

            default:
                cout << "\nInvalid Choice! Try Again.\n";
        }

    } while (choice != 7);

    return 0;
}
