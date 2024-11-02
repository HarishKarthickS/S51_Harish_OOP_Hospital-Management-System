#include <iostream>
#include <string>
using namespace std;

// Constants for limits
const int MAX_PATIENTS = 10;
const int MAX_DOCTORS = 5;
const int MAX_ROOMS = 5;

// Patient class demonstrating encapsulation and polymorphism through function overloading
class Patient {
private:
    string name;
    int id;
    string diagnosis;
    static int totalPatients;

public:
    // Default constructor
    Patient() : name(""), id(0), diagnosis("") {}

    // Parameterized constructor
    Patient(string n, int i, string d) : name(n), id(i), diagnosis(d) {
        totalPatients++;
    }

    // Destructor
    ~Patient() {
        totalPatients--;
        cout << "Destructor called for patient " << name << endl;
    }

    // Accessors and mutators
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

    // Overloaded display method to show full patient details
    void display() const {
        cout << "Patient ID: " << id << ", Name: " << name << ", Diagnosis: " << diagnosis << endl;
    }

    // Overloaded display method to show specific info (e.g., only the name)
    void display(string info) const {
        if (info == "name") {
            cout << "Patient Name: " << name << endl;
        } else {
            display(); // Fallback to full display if unrecognized info type
        }
    }
};

// Initialize static member
int Patient::totalPatients = 0;

// Abstract Doctor class demonstrating inheritance
class Doctor {
protected:
    string name;
    int id;
    string specialty;
    static int totalDoctors;

public:
    Doctor() : name(""), id(0), specialty("") {}

    Doctor(string n, int i, string s) : name(n), id(i), specialty(s) {
        totalDoctors++;
    }

    virtual ~Doctor() {
        totalDoctors--;
        cout << "Destructor called for doctor " << name << endl;
    }

    // Accessors and mutators
    string getName() const { return name; }
    void setName(string n) { name = n; }

    int getId() const { return id; }
    void setId(int i) { id = i; }

    string getSpecialty() const { return specialty; }
    void setSpecialty(string s) { specialty = s; }

    // Pure virtual function to make this class abstract
    virtual void display() const = 0;

    // Static method to display total number of doctors
    static void displayTotalDoctors() {
        cout << "Total Doctors: " << totalDoctors << endl;
    }

    // Public method to diagnose a patient
    void diagnosePatient(Patient &p, string diagnosis) {
        p.setDiagnosis(diagnosis);
        cout << "Doctor " << this->name << " diagnosed Patient " << p.getName() << " with " << diagnosis << endl;
    }
};

// Initialize static member
int Doctor::totalDoctors = 0;

// SpecialistDoctor demonstrates single inheritance from Doctor
class SpecialistDoctor : public Doctor {
private:
    string subSpecialty;

public:
    SpecialistDoctor(string n, int i, string s, string ss) : Doctor(n, i, s), subSpecialty(ss) {}

    void display() const override {
        cout << "Specialist Doctor ID: " << id << ", Name: " << name << ", Specialty: " << specialty
             << ", Sub-Specialty: " << subSpecialty << endl;
    }
};

// SeniorDoctor demonstrates multilevel inheritance from SpecialistDoctor
class SeniorDoctor : public SpecialistDoctor {
private:
    int experienceYears;

public:
    SeniorDoctor(string n, int i, string s, string ss, int years) 
        : SpecialistDoctor(n, i, s, ss), experienceYears(years) {}

    void display() const override {
        cout << "Senior Doctor ID: " << getId() << ", Name: " << getName() << ", Specialty: " << getSpecialty()
             << ", Experience: " << experienceYears << " years" << endl;
    }
};

// Room class demonstrating encapsulation and constructor overloading
class Room {
private:
    int roomNumber;
    bool isOccupied;
    Patient *currentPatient;

public:
    // Default constructor
    Room() : roomNumber(0), isOccupied(false), currentPatient(nullptr) {}

    // Parameterized constructor
    Room(int num) : roomNumber(num), isOccupied(false), currentPatient(nullptr) {}

    ~Room() {
        cout << "Destructor called for room " << roomNumber << endl;
    }

    void admitPatient(Patient &p) {
        if (!isOccupied) {
            currentPatient = &p;
            isOccupied = true;
            cout << "Patient " << p.getName() << " admitted to room " << roomNumber << endl;
        } else {
            cout << "Room " << roomNumber << " is already occupied." << endl;
        }
    }

    void dischargePatient() {
        if (isOccupied) {
            cout << "Patient " << currentPatient->getName() << " discharged from room " << roomNumber << endl;
            currentPatient = nullptr;
            isOccupied = false;
        } else {
            cout << "Room " << roomNumber << " is already empty." << endl;
        }
    }

    void display() const {
        cout << "Room Number: " << roomNumber << ", Occupied: " << (isOccupied ? "Yes" : "No") << endl;
        if (isOccupied) {
            cout << "Current Patient: " << currentPatient->getName() << endl;
        }
    }
};

int main() {
    Patient p1("John Doe", 101, "Flu");

    // Demonstrating function overloading with display methods
    cout << "Full Patient Display:" << endl;
    p1.display(); // Calls display() to show full details

    cout << "\nPartial Patient Display (name only):" << endl;
    p1.display("name"); // Calls display(string info) to show only the name

    // Demonstrating constructor overloading with Room class
    Room r1(101);
    r1.admitPatient(p1);
    r1.display();

    SpecialistDoctor sd1("Dr. Specialist", 301, "Cardiology", "Pediatrics");
    sd1.diagnosePatient(p1, "Cold");
    sd1.display();

    SeniorDoctor sr1("Dr. Senior", 401, "Cardiology", "Pediatrics", 20);
    sr1.display();

    Patient::displayTotalPatients();
    Doctor::displayTotalDoctors();

    return 0;
}
