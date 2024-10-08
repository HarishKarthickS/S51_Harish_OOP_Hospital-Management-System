#include <iostream>
#include <string>
using namespace std;

// Constants for limits
const int MAX_PATIENTS = 10;
const int MAX_DOCTORS = 5;
const int MAX_ROOMS = 5;

// Abstracted Patient class
class Patient {
private:
    string name;  // Private data member (encapsulation)
    int id;       // Private data member
    string diagnosis; // Private data member
    static int totalPatients;

public:
    // Default constructor
    Patient() : name(""), id(0), diagnosis("") {}

    // Parameterized constructor
    Patient(string n, int i, string d) : name(n), id(i), diagnosis(d) {
        totalPatients++;
    }

    // Copy constructor
    Patient(const Patient &p) : name(p.name), id(p.id), diagnosis(p.diagnosis) {
        totalPatients++;
        cout << "Copy constructor called for patient " << name << endl;
    }

    // Destructor
    ~Patient() {
        totalPatients--;
        cout << "Destructor called for patient " << name << endl;
    }

    // Public interface (accessors and mutators) to access the private members
    string getName() const { return name; }
    void setName(string n) { name = n; }

    int getId() const { return id; }
    void setId(int i) { id = i; }

    string getDiagnosis() const { return diagnosis; }
    void setDiagnosis(string d) { diagnosis = d; }

    // Static method to display total number of patients
    static void displayTotalPatients() {
        cout << "Total Patients: " << totalPatients << endl;
    }

    // Public method to display patient information
    void display() const {
        cout << "Patient ID: " << id << ", Name: " << name << ", Diagnosis: " << diagnosis << endl;
    }
};

// Initialize static member
int Patient::totalPatients = 0;

// Abstracted Doctor class
class Doctor {
private:
    string name;    // Private data member (encapsulation)
    int id;         // Private data member
    string specialty;  // Private data member
    static int totalDoctors;

public:
    // Default constructor
    Doctor() : name(""), id(0), specialty("") {}

    // Parameterized constructor
    Doctor(string n, int i, string s) : name(n), id(i), specialty(s) {
        totalDoctors++;
    }

    // Destructor
    ~Doctor() {
        totalDoctors--;
        cout << "Destructor called for doctor " << name << endl;
    }

    // Public interface (accessors and mutators)
    string getName() const { return name; }
    void setName(string n) { name = n; }

    int getId() const { return id; }
    void setId(int i) { id = i; }

    string getSpecialty() const { return specialty; }
    void setSpecialty(string s) { specialty = s; }

    // Static method to display total number of doctors
    static void displayTotalDoctors() {
        cout << "Total Doctors: " << totalDoctors << endl;
    }

    // Public method to diagnose a patient
    void diagnosePatient(Patient &p, string diagnosis) {
        p.setDiagnosis(diagnosis);  // Using the mutator to set diagnosis
        cout << "Doctor " << this->name << " diagnosed Patient " << p.getName() << " with " << diagnosis << endl;
    }

    // Public method to display doctor information
    void display() const {
        cout << "Doctor ID: " << id << ", Name: " << name << ", Specialty: " << specialty << endl;
    }
};

// Initialize static member
int Doctor::totalDoctors = 0;

// Room class to demonstrate encapsulation as well
class Room {
private:
    int roomNumber;    // Private data member
    bool isOccupied;   // Private data member
    Patient *currentPatient; // Pointer to current patient in room

public:
    // Default constructor
    Room() : roomNumber(0), isOccupied(false), currentPatient(nullptr) {}

    // Parameterized constructor
    Room(int num) : roomNumber(num), isOccupied(false), currentPatient(nullptr) {}

    // Destructor
    ~Room() {
        cout << "Destructor called for room " << roomNumber << endl;
    }

    // Public method to admit a patient
    void admitPatient(Patient &p) {
        if (!isOccupied) {
            currentPatient = &p;
            isOccupied = true;
            cout << "Patient " << p.getName() << " admitted to room " << roomNumber << endl;
        } else {
            cout << "Room " << roomNumber << " is already occupied." << endl;
        }
    }

    // Public method to discharge a patient
    void dischargePatient() {
        if (isOccupied) {
            cout << "Patient " << currentPatient->getName() << " discharged from room " << roomNumber << endl;
            currentPatient = nullptr;
            isOccupied = false;
        } else {
            cout << "Room " << roomNumber << " is already empty." << endl;
        }
    }

    // Public method to display room info
    void display() const {
        cout << "Room Number: " << roomNumber << ", Occupied: " << (isOccupied ? "Yes" : "No") << endl;
        if (isOccupied) {
            cout << "Current Patient: " << currentPatient->getName() << endl;
        }
    }
};

int main() {
    // Demonstrating the use of constructors and destructors
    Patient p1("John Doe", 101, "Flu");
    p1.display();

    Patient p2 = p1;  // Copy constructor
    p2.display();

    Room r1(101);
    r1.admitPatient(p1);
    r1.display();

    r1.dischargePatient();
    r1.display();

    Doctor d1("Dr. Smith", 201, "Cardiology");
    d1.diagnosePatient(p1, "Cold");
    d1.display();

    Patient::displayTotalPatients();
    Doctor::displayTotalDoctors();

    return 0;
}
