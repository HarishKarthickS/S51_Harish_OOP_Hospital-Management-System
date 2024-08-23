#include <iostream>
#include <string>
using namespace std;

const int MAX_PATIENTS = 10;
const int MAX_DOCTORS = 5;
const int MAX_ROOMS = 5;

class Patient {
private:
    string name;
    int id;
    string diagnosis;
    static int totalPatients;
public:
    Patient() : name(""), id(0), diagnosis("") {}
    Patient(string n, int i, string d) : name(n), id(i), diagnosis(d) {
        totalPatients++;
    }

    void setDiagnosis(string d) { diagnosis = d; }
    void display() const {
        cout << "Patient ID: " << id << ", Name: " << name << ", Diagnosis: " << diagnosis << endl;
    }
    string getName() { return this->name; }
};

class Doctor {
private:
    string name;
    int id;
    string specialty;
    static int totalDoctors;
public:
    Doctor() : name(""), id(0), specialty("") {}
    Doctor(string n, int i, string s) : name(n), id(i), specialty(s) {
        totalDoctors++;
    }

    void diagnosePatient(Patient &p, string diagnosis) {
        p.setDiagnosis(diagnosis);
        cout << "Doctor " << this->name << " diagnosed Patient " << p.getName() << " with " << diagnosis << endl;
    }
    void display() const {
        cout << "Doctor ID: " << id << ", Name: " << name << ", Specialty: " << specialty << endl;
    }
};

class Room {
private:
    int roomNumber;
    bool isOccupied;
    Patient *currentPatient;

public:
    Room() : roomNumber(0), isOccupied(false), currentPatient(nullptr) {}
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
    Patient* patients[MAX_PATIENTS];
    Doctor* doctors[MAX_DOCTORS];
    Room* rooms[MAX_ROOMS];

    int patientCount = 0, doctorCount = 0, roomCount = 0;
    int choice;

    do {
        cout << "\n1. Add Patient\n2. Add Doctor\n3. Add Room\n4. Admit Patient to Room\n5. Diagnose Patient\n6. Discharge Patient from Room\n7. Display Patient Info\n8. Display Doctor Info\n9. Display Room Info\n10. Exit\nEnter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                if (patientCount < MAX_PATIENTS) {
                    string name, diagnosis;
                    int id;
                    cout << "Enter patient name: ";
                    getline(cin, name);
                    cout << "Enter patient ID: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter initial diagnosis: ";
                    getline(cin, diagnosis);
                    patients[patientCount++] = new Patient(name, id, diagnosis);
                } else {
                    cout << "Maximum number of patients reached." << endl;
                }
                break;
            }
            case 2: {
                if (doctorCount < MAX_DOCTORS) {
                    string name, specialty;
                    int id;
                    cout << "Enter doctor name: ";
                    getline(cin, name);
                    cout << "Enter doctor ID: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter doctor's specialty: ";
                    getline(cin, specialty);
                    doctors[doctorCount++] = new Doctor(name, id, specialty);
                } else {
                    cout << "Maximum number of doctors reached." << endl;
                }
                break;
            }
            case 3: {
                if (roomCount < MAX_ROOMS) {
                    int number;
                    cout << "Enter room number: ";
                    cin >> number;
                    cin.ignore();
                    rooms[roomCount++] = new Room(number);
                } else {
                    cout << "Maximum number of rooms reached." << endl;
                }
                break;
            }
            case 4: {
                int patientIndex, roomIndex;
                cout << "Enter patient index (0 to " << patientCount - 1 << "): ";
                cin >> patientIndex;
                cout << "Enter room index (0 to " << roomCount - 1 << "): ";
                cin >> roomIndex;
                if (patientIndex >= 0 && patientIndex < patientCount && roomIndex >= 0 && roomIndex < roomCount) {
                    rooms[roomIndex]->admitPatient(*patients[patientIndex]);
                } else {
                    cout << "Invalid indices." << endl;
                }
                break;
            }
            case 5: {
                int doctorIndex, patientIndex;
                string diagnosis;
                cout << "Enter doctor index (0 to " << doctorCount - 1 << "): ";
                cin >> doctorIndex;
                cout << "Enter patient index (0 to " << patientCount - 1 << "): ";
                cin >> patientIndex;
                cin.ignore();
                if (doctorIndex >= 0 && doctorIndex < doctorCount && patientIndex >= 0 && patientIndex < patientCount) {
                    cout << "Enter new diagnosis: ";
                    getline(cin, diagnosis);
                    doctors[doctorIndex]->diagnosePatient(*patients[patientIndex], diagnosis);
                } else {
                    cout << "Invalid indices." << endl;
                }
                break;
            }
            case 6: {
                int roomIndex;
                cout << "Enter room index (0 to " << roomCount - 1 << "): ";
                cin >> roomIndex;
                if (roomIndex >= 0 && roomIndex < roomCount) {
                    rooms[roomIndex]->dischargePatient();
                } else {
                    cout << "Invalid index." << endl;
                }
                break;
            }
            case 7: {
                int patientIndex;
                cout << "Enter patient index (0 to " << patientCount - 1 << "): ";
                cin >> patientIndex;
                if (patientIndex >= 0 && patientIndex < patientCount) {
                    patients[patientIndex]->display();
                } else {
                    cout << "Invalid index." << endl;
                }
                break;
            }
            case 8: {
                int doctorIndex;
                cout << "Enter doctor index (0 to " << doctorCount - 1 << "): ";
                cin >> doctorIndex;
                if (doctorIndex >= 0 && doctorIndex < doctorCount) {
                    doctors[doctorIndex]->display();
                } else {
                    cout << "Invalid index." << endl;
                }
                break;
            }
            case 9: {
                int roomIndex;
                cout << "Enter room index (0 to " << roomCount - 1 << "): ";
                cin >> roomIndex;
                if (roomIndex >= 0 && roomIndex < roomCount) {
                    rooms[roomIndex]->display();
                } else {
                    cout << "Invalid index." << endl;
                }
                break;
            }
            case 10:
                cout << "Exiting the program." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 10);

    for (int i = 0; i < patientCount; ++i) {
        delete patients[i];
    }
    for (int i = 0; i < doctorCount; ++i) {
        delete doctors[i];
    }
    for (int i = 0; i < roomCount; ++i) {
        delete rooms[i];
    }

    return 0;
}
