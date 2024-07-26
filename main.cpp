#include <iostream>
#include <string>

using namespace std;

// Patient class definition
class Patient {
private:
    string name;
    int id;
    string diagnosis;

public:
    // Constructor
    Patient(string n, int i, string d) : name(n), id(i), diagnosis(d) {}

    // Method to set diagnosis
    void setDiagnosis(string d) {
        diagnosis = d;
    }

    // Method to get diagnosis
    string getDiagnosis() {
        return diagnosis;
    }

    // Method to display patient information
    void display() {
        cout << "Patient ID: " << id << ", Name: " << name << ", Diagnosis: " << diagnosis << endl;
    }
};

// Doctor class definition
class Doctor {
private:
    string name;
    int id;
    string specialty;

public:
    // Constructor
    Doctor(string n, int i, string s) : name(n), id(i), specialty(s) {}

    // Method to set specialty
    void setSpecialty(string s) {
        specialty = s;
    }

    // Method to get specialty
    string getSpecialty() {
        return specialty;
    }

    // Method to display doctor information
    void display() {
        cout << "Doctor ID: " << id << ", Name: " << name << ", Specialty: " << specialty << endl;
    }

    // Method to diagnose a patient
    void diagnosePatient(Patient &p, string diagnosis) {
        p.setDiagnosis(diagnosis);
        cout << "Doctor " << name << " diagnosed Patient " << p.getDiagnosis() << " with " << diagnosis << endl;
    }
};

// Main function to simulate hospital management system
int main() {
    string patientName, doctorName, specialty, diagnosis;
    int patientId, doctorId;

    // Get input for patient
    cout << "Enter patient name: ";
    getline(cin, patientName);
    cout << "Enter patient ID: ";
    cin >> patientId;
    cin.ignore(); // to ignore the newline character left by cin
    cout << "Enter initial diagnosis: ";
    getline(cin, diagnosis);

    // Create Patient object
    Patient patient1(patientName, patientId, diagnosis);

    // Get input for doctor
    cout << "\nEnter doctor name: ";
    getline(cin, doctorName);
    cout << "Enter doctor ID: ";
    cin >> doctorId;
    cin.ignore(); // to ignore the newline character left by cin
    cout << "Enter doctor's specialty: ";
    getline(cin, specialty);

    // Create Doctor object
    Doctor doctor1(doctorName, doctorId, specialty);

    // Display initial data
    cout << "\nInitial Data:" << endl;
    patient1.display();
    doctor1.display();

    // Get new diagnosis input
    cout << "\nEnter new diagnosis for the patient: ";
    getline(cin, diagnosis);

    // Simulating a diagnosis
    doctor1.diagnosePatient(patient1, diagnosis);

    // Display updated data
    cout << "\nUpdated Data:" << endl;
    patient1.display();
    doctor1.display();

    return 0;
}
