#include <iostream>
#include <string>
using namespace std;

// ============================================================
// BASE CLASS: Vehicle
// ============================================================
class Vehicle {
private:
    int vehicleID;
    string manufacturer;
    string model;
    int year;

protected:
    static int totalVehicles;

public:
    Vehicle() : vehicleID(0), manufacturer("Unknown"), model("Unknown"), year(0) {
        totalVehicles++;
    }
    Vehicle(int id, string mfr, string mdl, int yr)
        : vehicleID(id), manufacturer(mfr), model(mdl), year(yr) {
        totalVehicles++;
    }
    virtual ~Vehicle() { totalVehicles--; }

    int getVehicleID() const { return vehicleID; }
    string getManufacturer() const { return manufacturer; }
    string getModel() const { return model; }
    int getYear() const { return year; }
    static int getTotalVehicles() { return totalVehicles; }

    void setVehicleID(int id) { vehicleID = id; }
    void setManufacturer(string mfr) { manufacturer = mfr; }
    void setModel(string mdl) { model = mdl; }
    void setYear(int yr) { year = yr; }

    virtual void display() const {
        cout << "  Vehicle ID   : " << vehicleID << endl;
        cout << "  Manufacturer : " << manufacturer << endl;
        cout << "  Model        : " << model << endl;
        cout << "  Year         : " << year << endl;
    }
    virtual string getType() const { return "Vehicle"; }
};
int Vehicle::totalVehicles = 0;


// SINGLE INHERITANCE: Car -> Vehicle
class Car : public Vehicle {
private:
    string fuelType;
public:
    Car() : Vehicle(), fuelType("Petrol") {}
    Car(int id, string mfr, string mdl, int yr, string fuel)
        : Vehicle(id, mfr, mdl, yr), fuelType(fuel) {}
    ~Car() {}

    string getFuelType() const { return fuelType; }
    void setFuelType(string fuel) { fuelType = fuel; }

    void display() const override {
        Vehicle::display();
        cout << "  Fuel Type    : " << fuelType << endl;
    }
    string getType() const override { return "Car"; }
};


// MULTILEVEL INHERITANCE Level 2: ElectricCar -> Car -> Vehicle
class ElectricCar : public Car {
private:
    double batteryCapacity;
public:
    ElectricCar() : Car(), batteryCapacity(0.0) {}
    ElectricCar(int id, string mfr, string mdl, int yr, double battery)
        : Car(id, mfr, mdl, yr, "Electric"), batteryCapacity(battery) {}
    ~ElectricCar() {}

    double getBatteryCapacity() const { return batteryCapacity; }
    void setBatteryCapacity(double cap) { batteryCapacity = cap; }

    void display() const override {
        Car::display();
        cout << "  Battery Cap  : " << batteryCapacity << " kWh" << endl;
    }
    string getType() const override { return "Electric Car"; }
};


// MULTILEVEL INHERITANCE Level 3: SportsCar -> ElectricCar -> Car -> Vehicle
class SportsCar : public ElectricCar {
private:
    double topSpeed;
public:
    SportsCar() : ElectricCar(), topSpeed(0.0) {}
    SportsCar(int id, string mfr, string mdl, int yr, double battery, double speed)
        : ElectricCar(id, mfr, mdl, yr, battery), topSpeed(speed) {}
    ~SportsCar() {}

    double getTopSpeed() const { return topSpeed; }
    void setTopSpeed(double speed) { topSpeed = speed; }

    void display() const override {
        ElectricCar::display();
        cout << "  Top Speed    : " << topSpeed << " km/h" << endl;
    }
    string getType() const override { return "Sports Car"; }
};


// BASE CLASS FOR MULTIPLE INHERITANCE: Aircraft
class Aircraft {
private:
    double flightRange;
public:
    Aircraft() : flightRange(0.0) {}
    Aircraft(double range) : flightRange(range) {}
    virtual ~Aircraft() {}

    double getFlightRange() const { return flightRange; }
    void setFlightRange(double range) { flightRange = range; }

    virtual void displayAircraft() const {
        cout << "  Flight Range : " << flightRange << " km" << endl;
    }
};


// MULTIPLE INHERITANCE: FlyingCar -> Car + Aircraft
class FlyingCar : public Car, public Aircraft {
public:
    FlyingCar() : Car(), Aircraft() {}
    FlyingCar(int id, string mfr, string mdl, int yr, string fuel, double range)
        : Car(id, mfr, mdl, yr, fuel), Aircraft(range) {}
    ~FlyingCar() {}

    void display() const override {
        Car::display();
        Aircraft::displayAircraft();
    }
    string getType() const override { return "Flying Car"; }
};


// HIERARCHICAL INHERITANCE: Sedan -> Car
class Sedan : public Car {
public:
    Sedan() : Car() {}
    Sedan(int id, string mfr, string mdl, int yr, string fuel)
        : Car(id, mfr, mdl, yr, fuel) {}
    ~Sedan() {}

    void display() const override {
        Car::display();
        cout << "  Body Type    : Sedan" << endl;
    }
    string getType() const override { return "Sedan"; }
};


// HIERARCHICAL INHERITANCE: SUV -> Car
class SUV : public Car {
public:
    SUV() : Car() {}
    SUV(int id, string mfr, string mdl, int yr, string fuel)
        : Car(id, mfr, mdl, yr, fuel) {}
    ~SUV() {}

    void display() const override {
        Car::display();
        cout << "  Body Type    : SUV" << endl;
    }
    string getType() const override { return "SUV"; }
};


// VehicleRegistry CLASS
class VehicleRegistry {
private:
    Vehicle* registry[100];
    int count;
public:
    VehicleRegistry() : count(0) {}
    ~VehicleRegistry() {
        for (int i = 0; i < count; i++) delete registry[i];
    }

    void addVehicle(Vehicle* v) {
        if (count < 100) {
            registry[count++] = v;
            cout << "\n  [SUCCESS] Vehicle added successfully!" << endl;
        } else {
            cout << "\n  [ERROR] Registry is full!" << endl;
        }
    }

    void displayAll() const {
        if (count == 0) { cout << "\n  No vehicles registered yet." << endl; return; }
        cout << "\n  ===== All Registered Vehicles =====" << endl;
        for (int i = 0; i < count; i++) {
            cout << "\n  [" << i+1 << "] Type: " << registry[i]->getType() << endl;
            cout << "  -----------------------------------" << endl;
            registry[i]->display();
        }
        cout << "\n  Total Vehicles in Registry: " << Vehicle::getTotalVehicles() << endl;
    }

    void searchByID(int id) const {
        for (int i = 0; i < count; i++) {
            if (registry[i]->getVehicleID() == id) {
                cout << "\n  ===== Vehicle Found =====" << endl;
                cout << "  Type: " << registry[i]->getType() << endl;
                cout << "  -------------------------" << endl;
                registry[i]->display();
                return;
            }
        }
        cout << "\n  [NOT FOUND] No vehicle with ID " << id << " exists." << endl;
    }
};


int readInt(const string& prompt) {
    int val; cout << prompt; cin >> val; cin.ignore(); return val;
}
string readString(const string& prompt) {
    string val; cout << prompt; getline(cin, val); return val;
}
double readDouble(const string& prompt) {
    double val; cout << prompt; cin >> val; cin.ignore(); return val;
}


int main() {
    VehicleRegistry reg;
    int choice;

    cout << "============================================" << endl;
    cout << "       VEHICLE REGISTRY SYSTEM (C++)        " << endl;
    cout << "============================================" << endl;

    do {
        cout << "\n  ===== MAIN MENU =====" << endl;
        cout << "  1. Add a Vehicle" << endl;
        cout << "  2. View All Vehicles" << endl;
        cout << "  3. Search Vehicle by ID" << endl;
        cout << "  4. Exit" << endl;
        choice = readInt("  Enter your choice: ");

        switch (choice) {
        case 1: {
            cout << "\n  Select Vehicle Type:" << endl;
            cout << "  1. Car          (Single Inheritance)" << endl;
            cout << "  2. Electric Car (Multilevel Lvl 2)" << endl;
            cout << "  3. Sports Car   (Multilevel Lvl 3)" << endl;
            cout << "  4. Flying Car   (Multiple Inheritance)" << endl;
            cout << "  5. Sedan        (Hierarchical)" << endl;
            cout << "  6. SUV          (Hierarchical)" << endl;
            int t = readInt("  Enter type: ");
            int id     = readInt("  Enter Vehicle ID   : ");
            string mfr = readString("  Enter Manufacturer : ");
            string mdl = readString("  Enter Model        : ");
            int yr     = readInt("  Enter Year         : ");
            switch (t) {
                case 1: { 
                    string f = readString("  Enter Fuel Type: "); reg.addVehicle(new Car(id,mfr,mdl,yr,f)); break; 
                }
                case 2: { 
                    double b = readDouble("  Enter Battery (kWh): "); reg.addVehicle(new ElectricCar(id,mfr,mdl,yr,b)); break; 
                }
                case 3: { 
                    double b = readDouble("  Enter Battery (kWh): "); double s = readDouble("  Enter Top Speed (km/h): "); reg.addVehicle(new SportsCar(id,mfr,mdl,yr,b,s)); break; 
                }
                case 4: { 
                    string f = readString("  Enter Fuel Type: "); double r = readDouble("  Enter Flight Range (km): "); reg.addVehicle(new FlyingCar(id,mfr,mdl,yr,f,r)); break; 
                }
                case 5: { 
                    string f = readString("  Enter Fuel Type: "); reg.addVehicle(new Sedan(id,mfr,mdl,yr,f)); break; }
                case 6: { string f = readString("  Enter Fuel Type: "); reg.addVehicle(new SUV(id,mfr,mdl,yr,f)); break; }
                default: cout << "  [ERROR] Invalid type!" << endl;
            }
            break;
        }
        case 2: reg.displayAll(); break;
        case 3: { int sid = readInt("  Enter Vehicle ID to search: "); reg.searchByID(sid); break; }
        case 4: cout << "\n  Thank you! Goodbye!" << endl; break;
        default: cout << "\n  [ERROR] Invalid choice!" << endl;
        }
    } while (choice != 4);

    return 0;
}