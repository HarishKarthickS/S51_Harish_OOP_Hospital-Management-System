#include <iostream>
#include <string>
using namespace std;

const int MAX_PATIENTS = 10;
const int MAX_DOCTORS = 5;
const int MAX_ROOMS = 5;

// Interface for handling diagnosis - Demonstrates SRP and DIP
class IDiagnosis {
public:
    virtual void diagnose(string diagnosis) = 0;
    virtual ~IDiagnosis() = default;
};

// Separate Diagnosis class for managing patient diagnoses
// Implements SRP by separating diagnosis functionality
class Diagnosis : public IDiagnosis {
private:
    string diagnosis;
public:
    void diagnose(string d) override {
        diagnosis = d;
        cout << "Diagnosis set to: " << diagnosis << endl;
    }
    string getDiagnosis() const { return diagnosis; }
};

// Interface for displaying objects - Demonstrates ISP
class IDisplayable {
public:
    virtual void display() const = 0;
    virtual ~IDisplayable() = default;
};

// Patient class, implementing IDisplayable interface
// Follows SRP, as it only manages patient-related data and operations
class Patient : public IDisplayable {
private:
    string name;
    int id;
    Diagnosis diagnosis; // Dependency inversion with IDiagnosis interface
    static int totalPatients;

public:
    Patient(string n, int i) : name(n), id(i) { totalPatients++; }
    ~Patient() { totalPatients--; cout << "Destructor called for patient " << name << endl; }

    string getName() const { return name; }
    int getId() const { return id; }

    // Sets diagnosis for the patient
    // Demonstrates SRP by separating diagnosis details from Patient class
    void setDiagnosis(string d) { diagnosis.diagnose(d); }
    string getDiagnosis() const { return diagnosis.getDiagnosis(); }

    // Implements display from IDisplayable
    void display() const override {
        cout << "Patient ID: " << id << ", Name: " << name << ", Diagnosis: " << diagnosis.getDiagnosis() << endl;
    }

    // Static method to display total number of patients
    static void displayTotalPatients() { cout << "Total Patients: " << totalPatients << endl; }
};

int Patient::totalPatients = 0;

// Abstract Doctor class - Demonstrates DIP and ISP
// The class is only concerned with doctor-related data and actions, supporting SRP
class Doctor : public IDisplayable {
protected:
    string name;
    int id;
    static int totalDoctors;

public:
    Doctor(string n, int i) : name(n), id(i) { totalDoctors++; }
    virtual ~Doctor() { totalDoctors--; cout << "Destructor called for doctor " << name << endl; }

    // Abstract method to diagnose a patient, enabling specialization (OCP)
    virtual void diagnosePatient(Patient &p, string diagnosis) = 0;

    // Abstract method from IDisplayable, following ISP
    virtual void display() const override = 0;

    // Static method to display total number of doctors
    static void displayTotalDoctors() { cout << "Total Doctors: " << totalDoctors << endl; }
};

int Doctor::totalDoctors = 0;

// SpecialistDoctor class demonstrating inheritance and polymorphism
// Extends Doctor and follows LSP by implementing diagnosePatient and display
class SpecialistDoctor : public Doctor {
private:
    string specialty;
public:
    SpecialistDoctor(string n, int i, string s) : Doctor(n, i), specialty(s) {}

    // Overrides diagnosePatient, implementing specific behavior for SpecialistDoctor
    void diagnosePatient(Patient &p, string diagnosis) override {
        p.setDiagnosis(diagnosis);
        cout << "Specialist Doctor " << name << " diagnosed Patient " << p.getName() << " with " << diagnosis << endl;
    }

    // Implements display to show specific details for SpecialistDoctor
    void display() const override {
        cout << "Specialist Doctor ID: " << id << ", Name: " << name << ", Specialty: " << specialty << endl;
    }
};

// Room class demonstrating encapsulation of room properties and functionality
// Follows SRP by only managing room-related data and operations
class Room : public IDisplayable {
private:
    int roomNumber;
    bool isOccupied;
    Patient* currentPatient;

public:
    Room(int num) : roomNumber(num), isOccupied(false), currentPatient(nullptr) {}
    ~Room() { cout << "Destructor called for room " << roomNumber << endl; }

    // Method to admit a patient to the room
    void admitPatient(Patient& p) {
        if (!isOccupied) {
            currentPatient = &p;
            isOccupied = true;
            cout << "Patient " << p.getName() << " admitted to room " << roomNumber << endl;
        } else {
            cout << "Room " << roomNumber << " is already occupied." << endl;
        }
    }

    // Method to discharge a patient from the room
    void dischargePatient() {
        if (isOccupied) {
            cout << "Patient " << currentPatient->getName() << " discharged from room " << roomNumber << endl;
            currentPatient = nullptr;
            isOccupied = false;
        } else {
            cout << "Room " << roomNumber << " is already empty." << endl;
        }
    }

    // Implements display to show room status, demonstrating ISP
    void display() const override {
        cout << "Room Number: " << roomNumber << ", Occupied: " << (isOccupied ? "Yes" : "No") << endl;
        if (isOccupied && currentPatient) {
            cout << "Current Patient: " << currentPatient->getName() << endl;
        }
    }
};

int main() {
    // Create a Patient object
    Patient p1("John Doe", 101);
    Room r1(101); // Room object with room number
    SpecialistDoctor sd1("Dr. Specialist", 301, "Cardiology"); // SpecialistDoctor object

    // Admit the patient to the room and display room details
    r1.admitPatient(p1);
    r1.display();

    // Diagnose the patient and display doctor details
    sd1.diagnosePatient(p1, "Cold");
    sd1.display();

    // Display total counts for patients and doctors
    Patient::displayTotalPatients();
    Doctor::displayTotalDoctors();

    return 0;
}
