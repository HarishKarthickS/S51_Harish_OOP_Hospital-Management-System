#include <iostream>
#include <string>
using namespace std;

class Patient {
private:
    string name;
    int id;
    string diagnosis;

public:
    Patient(string name, int id, string diagnosis)  {
        this->name = name;
        this->id = id;
        this->diagnosis = diagnosis;
    }
    void setDiagnosis(string d) { diagnosis = d; }
    void display() const { cout << "Patient ID: " << id << ", Name: " << name << ", Diagnosis: " << diagnosis << endl; }
    string getName() { return this->name; }
};

class Doctor {
private:
    string name;
    int id;
    string specialty;

public:
    Doctor(string name, int id, string specaility)  {
        this->name = name;
        this->id = id;
        this->specialty = specialty;
    }
    void diagnosePatient(Patient &p, string diagnosis) {
        p.setDiagnosis(diagnosis);
        cout << "Doctor " << this->name << " diagnosed Patient " << p.getName() << " with " << diagnosis << endl;
    }
    void display() const { cout << "Doctor ID: " << id << ", Name: " << name << ", Specialty: " << specialty << endl; }
};

class Room {
private:
    int roomNumber;
    bool isOccupied;
    Patient *currentPatient;

public:
    Room(int num) : roomNumber(num), isOccupied(false), currentPatient(nullptr) {}
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
    string patientName, doctorName, specialty, diagnosis;
    int patientId, doctorId, roomNumber, choice;

    cout << "Enter patient name: ";
    getline(cin, patientName);
    cout << "Enter patient ID: ";
    cin >> patientId;
    cin.ignore();
    cout << "Enter initial diagnosis: ";
    getline(cin, diagnosis);
    Patient patient1(patientName, patientId, diagnosis);

    cout << "\nEnter doctor name: ";
    getline(cin, doctorName);
    cout << "Enter doctor ID: ";
    cin >> doctorId;
    cin.ignore();
    cout << "Enter doctor's specialty: ";
    getline(cin, specialty);
    Doctor doctor1(doctorName, doctorId, specialty);

    cout << "\nEnter room number: ";
    cin >> roomNumber;
    cin.ignore();
    Room room1(roomNumber);

    do {
        cout << "\n1. Admit Patient\n2. Diagnose Patient\n3. Discharge Patient\n4. Display Patient Info\n5. Display Doctor Info\n6. Display Room Info\n7. Exit\nEnter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                room1.admitPatient(patient1);
                break;
            case 2:
                cout << "Enter new diagnosis: ";
                getline(cin, diagnosis);
                doctor1.diagnosePatient(patient1, diagnosis);
                break;
            case 3:
                room1.dischargePatient();
                break;
            case 4:
                patient1.display();
                break;
            case 5:
                doctor1.display();
                break;
            case 6:
                room1.display();
                break;
            case 7:
                cout << "Exiting the program." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 7);

    return 0;
}
