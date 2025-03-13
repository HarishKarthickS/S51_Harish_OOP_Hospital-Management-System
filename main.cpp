#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <chrono>
#include <ctime>
#include <map>
#include <functional>
#include <sstream>
#include <iomanip>

// ------------------------------
// Interfaces for Cross-Cutting Concerns
// ------------------------------

// Interface for logging (SRP - separate logging responsibility)
class ILogger {
public:
    virtual ~ILogger() {}
    virtual void logInfo(const std::string &message) = 0;
    virtual void logError(const std::string &message) = 0;
    virtual void logWarning(const std::string &message) = 0;
};

// Concrete file logger implementation
class FileLogger : public ILogger {
private:
    std::string logFilePath;
    
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
public:
    FileLogger(const std::string &filePath = "hospital_log.txt") : logFilePath(filePath) {}
    
    void logInfo(const std::string &message) override {
        std::ofstream logFile(logFilePath, std::ios::app);
        if (logFile) {
            logFile << "[INFO] [" << getCurrentTimestamp() << "] " << message << std::endl;
        }
        logFile.close();
    }
    
    void logError(const std::string &message) override {
        std::ofstream logFile(logFilePath, std::ios::app);
        if (logFile) {
            logFile << "[ERROR] [" << getCurrentTimestamp() << "] " << message << std::endl;
        }
        logFile.close();
    }
    
    void logWarning(const std::string &message) override {
        std::ofstream logFile(logFilePath, std::ios::app);
        if (logFile) {
            logFile << "[WARNING] [" << getCurrentTimestamp() << "] " << message << std::endl;
        }
        logFile.close();
    }
};

// Display Interface for UI separation (SRP, ISP)
class IDisplayManager {
public:
    virtual ~IDisplayManager() {}
    virtual void displaySuccess(const std::string &message) = 0;
    virtual void displayError(const std::string &message) = 0;
    virtual void displayInfo(const std::string &message) = 0;
    virtual void displayWarning(const std::string &message) = 0;
};

// Console-based display implementation
class ConsoleDisplayManager : public IDisplayManager {
public:
    void displaySuccess(const std::string &message) override {
        std::cout << "✓ SUCCESS: " << message << std::endl;
    }
    
    void displayError(const std::string &message) override {
        std::cout << "✗ ERROR: " << message << std::endl;
    }
    
    void displayInfo(const std::string &message) override {
        std::cout << "ℹ INFO: " << message << std::endl;
    }
    
    void displayWarning(const std::string &message) override {
        std::cout << "⚠ WARNING: " << message << std::endl;
    }
};

// Validation interface (SRP for input validation)
class IValidator {
public:
    virtual ~IValidator() {}
    virtual bool validate() const = 0;
    virtual std::string getErrorMessage() const = 0;
};

// ------------------------------
// Entity Classes
// ------------------------------

class Patient {
private:
    int id;
    std::string name;
    int age;
    std::string disease;
    std::string contactNumber;
    std::string address;
    std::string bloodGroup;
    std::vector<int> medicationIds; // Store IDs of prescribed medications

public:
    Patient(int id, const std::string &name, int age, const std::string &disease,
            const std::string &contactNumber = "", const std::string &address = "", 
            const std::string &bloodGroup = "")
        : id(id), name(name), age(age), disease(disease), 
          contactNumber(contactNumber), address(address), bloodGroup(bloodGroup) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getDisease() const { return disease; }
    std::string getContactNumber() const { return contactNumber; }
    std::string getAddress() const { return address; }
    std::string getBloodGroup() const { return bloodGroup; }
    std::vector<int> getMedicationIds() const { return medicationIds; }

    void setName(const std::string &newName) { name = newName; }
    void setAge(int newAge) { age = newAge; }
    void setDisease(const std::string &newDisease) { disease = newDisease; }
    void setContactNumber(const std::string &newNumber) { contactNumber = newNumber; }
    void setAddress(const std::string &newAddress) { address = newAddress; }
    void setBloodGroup(const std::string &newBloodGroup) { bloodGroup = newBloodGroup; }
    
    void addMedicationId(int medicationId) {
        medicationIds.push_back(medicationId);
    }
    
    void removeMedicationId(int medicationId) {
        medicationIds.erase(
            std::remove(medicationIds.begin(), medicationIds.end(), medicationId),
            medicationIds.end()
        );
    }

    void display() const {
        std::cout << "Patient ID: " << id << "\nName: " << name 
                  << "\nAge: " << age << "\nDisease: " << disease;
        if (!contactNumber.empty()) std::cout << "\nContact: " << contactNumber;
        if (!address.empty()) std::cout << "\nAddress: " << address;
        if (!bloodGroup.empty()) std::cout << "\nBlood Group: " << bloodGroup;
        std::cout << "\n";
    }
};

class Doctor {
private:
    int id;
    std::string name;
    std::string specialization;
    std::string contactNumber;
    std::string email;
    double consultationFee;
    bool isAvailable;

public:
    Doctor(int id, const std::string &name, const std::string &specialization,
           const std::string &contactNumber = "", const std::string &email = "",
           double consultationFee = 0.0)
        : id(id), name(name), specialization(specialization), 
          contactNumber(contactNumber), email(email), 
          consultationFee(consultationFee), isAvailable(true) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getSpecialization() const { return specialization; }
    std::string getContactNumber() const { return contactNumber; }
    std::string getEmail() const { return email; }
    double getConsultationFee() const { return consultationFee; }
    bool getAvailability() const { return isAvailable; }

    void setName(const std::string &newName) { name = newName; }
    void setSpecialization(const std::string &newSpec) { specialization = newSpec; }
    void setContactNumber(const std::string &newContact) { contactNumber = newContact; }
    void setEmail(const std::string &newEmail) { email = newEmail; }
    void setConsultationFee(double newFee) { consultationFee = newFee; }
    void setAvailability(bool availability) { isAvailable = availability; }

    void display() const {
        std::cout << "Doctor ID: " << id << "\nName: " << name 
                  << "\nSpecialization: " << specialization;
        if (!contactNumber.empty()) std::cout << "\nContact: " << contactNumber;
        if (!email.empty()) std::cout << "\nEmail: " << email;
        std::cout << "\nConsultation Fee: $" << consultationFee;
        std::cout << "\nAvailability: " << (isAvailable ? "Available" : "Not Available");
        std::cout << "\n";
    }
};

// Enhanced appointment with time slots
class Appointment {
private:
    int appointmentId;
    int patientId;
    int doctorId;
    std::string date;  // Format: YYYY-MM-DD
    std::string timeSlot; // Format: HH:MM-HH:MM (24-hour format)
    std::string status; // Scheduled, Completed, Cancelled
    std::string notes;

public:
    Appointment(int appointmentId, int patientId, int doctorId, 
                const std::string &date, const std::string &timeSlot = "09:00-09:30",
                const std::string &status = "Scheduled", const std::string &notes = "")
        : appointmentId(appointmentId), patientId(patientId), doctorId(doctorId), 
          date(date), timeSlot(timeSlot), status(status), notes(notes) {}

    int getAppointmentId() const { return appointmentId; }
    int getPatientId() const { return patientId; }
    int getDoctorId() const { return doctorId; }
    std::string getDate() const { return date; }
    std::string getTimeSlot() const { return timeSlot; }
    std::string getStatus() const { return status; }
    std::string getNotes() const { return notes; }

    void setDate(const std::string &newDate) { date = newDate; }
    void setTimeSlot(const std::string &newTimeSlot) { timeSlot = newTimeSlot; }
    void setStatus(const std::string &newStatus) { status = newStatus; }
    void setNotes(const std::string &newNotes) { notes = newNotes; }

    void display() const {
        std::cout << "Appointment ID: " << appointmentId 
                  << "\nPatient ID: " << patientId 
                  << "\nDoctor ID: " << doctorId 
                  << "\nDate: " << date
                  << "\nTime Slot: " << timeSlot
                  << "\nStatus: " << status;
        if (!notes.empty()) std::cout << "\nNotes: " << notes;
        std::cout << "\n";
    }
};

// New class for medications
class Medication {
private:
    int medicationId;
    std::string name;
    std::string dosage;
    double price;
    std::string manufacturer;
    std::string description;

public:
    Medication(int medicationId, const std::string &name, const std::string &dosage,
               double price, const std::string &manufacturer = "", const std::string &description = "")
        : medicationId(medicationId), name(name), dosage(dosage), 
          price(price), manufacturer(manufacturer), description(description) {}
          
    int getMedicationId() const { return medicationId; }
    std::string getName() const { return name; }
    std::string getDosage() const { return dosage; }
    double getPrice() const { return price; }
    std::string getManufacturer() const { return manufacturer; }
    std::string getDescription() const { return description; }
    
    void setName(const std::string &newName) { name = newName; }
    void setDosage(const std::string &newDosage) { dosage = newDosage; }
    void setPrice(double newPrice) { price = newPrice; }
    void setManufacturer(const std::string &newManufacturer) { manufacturer = newManufacturer; }
    void setDescription(const std::string &newDescription) { description = newDescription; }
    
    void display() const {
        std::cout << "Medication ID: " << medicationId
                  << "\nName: " << name
                  << "\nDosage: " << dosage
                  << "\nPrice: $" << price;
        if (!manufacturer.empty()) std::cout << "\nManufacturer: " << manufacturer;
        if (!description.empty()) std::cout << "\nDescription: " << description;
        std::cout << "\n";
    }
};

// New class for prescription
class Prescription {
private:
    int prescriptionId;
    int patientId;
    int doctorId;
    std::string date;
    std::vector<int> medicationIds;
    std::string instructions;

public:
    Prescription(int prescriptionId, int patientId, int doctorId, const std::string &date,
                 const std::vector<int> &medicationIds = {}, const std::string &instructions = "")
        : prescriptionId(prescriptionId), patientId(patientId), doctorId(doctorId),
          date(date), medicationIds(medicationIds), instructions(instructions) {}
          
    int getPrescriptionId() const { return prescriptionId; }
    int getPatientId() const { return patientId; }
    int getDoctorId() const { return doctorId; }
    std::string getDate() const { return date; }
    std::vector<int> getMedicationIds() const { return medicationIds; }
    std::string getInstructions() const { return instructions; }
    
    void addMedicationId(int medicationId) {
        medicationIds.push_back(medicationId);
    }
    
    void removeMedicationId(int medicationId) {
        medicationIds.erase(
            std::remove(medicationIds.begin(), medicationIds.end(), medicationId),
            medicationIds.end()
        );
    }
    
    void setInstructions(const std::string &newInstructions) { instructions = newInstructions; }
    
    void display() const {
        std::cout << "Prescription ID: " << prescriptionId
                  << "\nPatient ID: " << patientId
                  << "\nDoctor ID: " << doctorId
                  << "\nDate: " << date
                  << "\nMedication IDs: ";
        if (medicationIds.empty()) {
            std::cout << "None";
        } else {
            for (size_t i = 0; i < medicationIds.size(); ++i) {
                std::cout << medicationIds[i];
                if (i < medicationIds.size() - 1) std::cout << ", ";
            }
        }
        if (!instructions.empty()) std::cout << "\nInstructions: " << instructions;
        std::cout << "\n";
    }
};

// New class for billing
class Bill {
private:
    int billId;
    int patientId;
    std::string date;
    double consultationFee;
    double medicationCharges;
    double otherCharges;
    std::string paymentStatus; // "Paid", "Pending", "Overdue"
    std::string paymentMethod; // "Cash", "Card", "Insurance"

public:
    Bill(int billId, int patientId, const std::string &date,
         double consultationFee = 0.0, double medicationCharges = 0.0,
         double otherCharges = 0.0, const std::string &paymentStatus = "Pending",
         const std::string &paymentMethod = "")
        : billId(billId), patientId(patientId), date(date),
          consultationFee(consultationFee), medicationCharges(medicationCharges),
          otherCharges(otherCharges), paymentStatus(paymentStatus),
          paymentMethod(paymentMethod) {}
          
    int getBillId() const { return billId; }
    int getPatientId() const { return patientId; }
    std::string getDate() const { return date; }
    double getConsultationFee() const { return consultationFee; }
    double getMedicationCharges() const { return medicationCharges; }
    double getOtherCharges() const { return otherCharges; }
    std::string getPaymentStatus() const { return paymentStatus; }
    std::string getPaymentMethod() const { return paymentMethod; }
    
    double getTotalAmount() const {
        return consultationFee + medicationCharges + otherCharges;
    }
    
    void setConsultationFee(double fee) { consultationFee = fee; }
    void setMedicationCharges(double charges) { medicationCharges = charges; }
    void setOtherCharges(double charges) { otherCharges = charges; }
    void setPaymentStatus(const std::string &status) { paymentStatus = status; }
    void setPaymentMethod(const std::string &method) { paymentMethod = method; }
    
    void display() const {
        std::cout << "Bill ID: " << billId
                  << "\nPatient ID: " << patientId
                  << "\nDate: " << date
                  << "\nConsultation Fee: $" << consultationFee
                  << "\nMedication Charges: $" << medicationCharges
                  << "\nOther Charges: $" << otherCharges
                  << "\nTotal Amount: $" << getTotalAmount()
                  << "\nPayment Status: " << paymentStatus;
        if (!paymentMethod.empty()) std::cout << "\nPayment Method: " << paymentMethod;
        std::cout << "\n";
    }
};

// New class for user authentication
class User {
private:
    int userId;
    std::string username;
    std::string passwordHash; // In a real system, you'd store a hashed password
    std::string role; // "Admin", "Doctor", "Receptionist", etc.
    bool isActive;

public:
    User(int userId, const std::string &username, const std::string &passwordHash,
         const std::string &role, bool isActive = true)
        : userId(userId), username(username), passwordHash(passwordHash),
          role(role), isActive(isActive) {}
          
    int getUserId() const { return userId; }
    std::string getUsername() const { return username; }
    std::string getRole() const { return role; }
    bool getIsActive() const { return isActive; }
    
    void setUsername(const std::string &newUsername) { username = newUsername; }
    void setPasswordHash(const std::string &newHash) { passwordHash = newHash; }
    void setRole(const std::string &newRole) { role = newRole; }
    void setIsActive(bool active) { isActive = active; }
    
    bool checkPassword(const std::string &passwordToCheck) const {
        // In a real system, you'd hash the input password and compare with stored hash
        return passwordToCheck == passwordHash;
    }
    
    void display() const {
        std::cout << "User ID: " << userId
                  << "\nUsername: " << username
                  << "\nRole: " << role
                  << "\nStatus: " << (isActive ? "Active" : "Inactive")
                  << "\n";
    }
};

// ------------------------------
// Repository Interfaces (Abstraction)
// ------------------------------

// Base repository interface with common operations (ISP)
template <typename T, typename IdType = int>
class IRepository {
public:
    virtual ~IRepository() {}
    virtual void add(const T &item) = 0;
    virtual bool remove(IdType id) = 0;
    virtual T* getById(IdType id) = 0;
    virtual std::vector<T> getAll() const = 0;
};

// Patient-specific repository interface (ISP)
class IPatientRepository : public IRepository<Patient> {
public:
    virtual std::vector<Patient> findByDisease(const std::string &disease) const = 0;
    virtual std::vector<Patient> findByAgeRange(int minAge, int maxAge) const = 0;
};

// Doctor-specific repository interface (ISP)
class IDoctorRepository : public IRepository<Doctor> {
public:
    virtual std::vector<Doctor> findBySpecialization(const std::string &specialization) const = 0;
    virtual std::vector<Doctor> findAvailableDoctors() const = 0;
};

// Appointment-specific repository interface (ISP)
class IAppointmentRepository : public IRepository<Appointment> {
public:
    virtual std::vector<Appointment> findByPatientId(int patientId) const = 0;
    virtual std::vector<Appointment> findByDoctorId(int doctorId) const = 0;
    virtual std::vector<Appointment> findByDate(const std::string &date) const = 0;
    virtual std::vector<Appointment> findByStatus(const std::string &status) const = 0;
};

// Medication repository interface (ISP)
class IMedicationRepository : public IRepository<Medication> {
public:
    virtual Medication* findByName(const std::string &name) = 0;
};

// Prescription repository interface (ISP)
class IPrescriptionRepository : public IRepository<Prescription> {
public:
    virtual std::vector<Prescription> findByPatientId(int patientId) const = 0;
    virtual std::vector<Prescription> findByDoctorId(int doctorId) const = 0;
};

// Bill repository interface (ISP)
class IBillRepository : public IRepository<Bill> {
public:
    virtual std::vector<Bill> findByPatientId(int patientId) const = 0;
    virtual std::vector<Bill> findByPaymentStatus(const std::string &status) const = 0;
    virtual double getTotalRevenue() const = 0;
};

// User repository interface (ISP)
class IUserRepository : public IRepository<User> {
public:
    virtual User* findByUsername(const std::string &username) = 0;
    virtual std::vector<User> findByRole(const std::string &role) const = 0;
};

// ------------------------------
// In-Memory Repository Implementations
// ------------------------------

class InMemoryPatientRepository : public IPatientRepository {
private:
    std::vector<Patient> patients;
public:
    void add(const Patient &patient) override {
        patients.push_back(patient);
    }

    bool remove(int id) override {
        auto it = std::remove_if(patients.begin(), patients.end(),
            [id](const Patient &p){ return p.getId() == id; });
        if (it != patients.end()) {
            patients.erase(it, patients.end());
            return true;
        }
        return false;
    }

    Patient* getById(int id) override {
        for (auto &p : patients)
            if (p.getId() == id)
                return &p;
        return nullptr;
    }

    std::vector<Patient> getAll() const override {
        return patients;
    }

    std::vector<Patient> findByDisease(const std::string &disease) const override {
        std::vector<Patient> result;
        for (const auto &p : patients) {
            if (p.getDisease() == disease) {
                result.push_back(p);
            }
        }
        return result;
    }

    std::vector<Patient> findByAgeRange(int minAge, int maxAge) const override {
        std::vector<Patient> result;
        for (const auto &p : patients) {
            if (p.getAge() >= minAge && p.getAge() <= maxAge) {
                result.push_back(p);
            }
        }
        return result;
    }
};

class InMemoryDoctorRepository : public IDoctorRepository {
private:
    std::vector<Doctor> doctors;
public:
    void add(const Doctor &doctor) override {
        doctors.push_back(doctor);
    }

    bool remove(int id) override {
        auto it = std::remove_if(doctors.begin(), doctors.end(),
            [id](const Doctor &d){ return d.getId() == id; });
        if (it != doctors.end()) {
            doctors.erase(it, doctors.end());
            return true;
        }
        return false;
    }

    Doctor* getById(int id) override {
        for (auto &d : doctors)
            if (d.getId() == id)
                return &d;
        return nullptr;
    }

    std::vector<Doctor> getAll() const override {
        return doctors;
    }

    std::vector<Doctor> findBySpecialization(const std::string &specialization) const override {
        std::vector<Doctor> result;
        for (const auto &d : doctors) {
            if (d.getSpecialization() == specialization) {
                result.push_back(d);
            }
        }
        return result;
    }

    std::vector<Doctor> findAvailableDoctors() const override {
        std::vector<Doctor> result;
        for (const auto &d : doctors) {
            if (d.getAvailability()) {
                result.push_back(d);
            }
        }
        return result;
    }
};

class InMemoryAppointmentRepository : public IAppointmentRepository {
private:
    std::vector<Appointment> appointments;
public:
    void add(const Appointment &appt) override {
        appointments.push_back(appt);
    }

    bool remove(int id) override {
        auto it = std::remove_if(appointments.begin(), appointments.end(),
            [id](const Appointment &a){ return a.getAppointmentId() == id; });
        if (it != appointments.end()) {
            appointments.erase(it, appointments.end());
            return true;
        }
        return false;
    }

    Appointment* getById(int id) override {
        for (auto &a : appointments)
            if (a.getAppointmentId() == id)
                return &a;
        return nullptr;
    }

    std::vector<Appointment> getAll() const override {
        return appointments;
    }

    std::vector<Appointment> findByPatientId(int patientId) const override {
        std::vector<Appointment> result;
        for (const auto &a : appointments) {
            if (a.getPatientId() == patientId) {
                result.push_back(a);
            }
        }
        return result;
    }

    std::vector<Appointment> findByDoctorId(int doctorId) const override {
        std::vector<Appointment> result;
        for (const auto &a : appointments) {
            if (a.getDoctorId() == doctorId) {
                result.push_back(a);
            }
        }
        return result;
    }

    std::vector<Appointment> findByDate(const std::string &date) const override {
        std::vector<Appointment> result;
        for (const auto &a : appointments) {
            if (a.getDate() == date) {
                result.push_back(a);
            }
        }
        return result;
    }

    std::vector<Appointment> findByStatus(const std::string &status) const override {
        std::vector<Appointment> result;
        for (const auto &a : appointments) {
            if (a.getStatus() == status) {
                result.push_back(a);
            }
        }
        return result;
    }
};

class InMemoryMedicationRepository : public IMedicationRepository {
private:
    std::vector<Medication> medications;
public:
    void add(const Medication &medication) override {
        medications.push_back(medication);
    }

    bool remove(int id) override {
        auto it = std::remove_if(medications.begin(), medications.end(),
            [id](const Medication &m){ return m.getMedicationId() == id; });
        if (it != medications.end()) {
            medications.erase(it, medications.end());
            return true;
        }
        return false;
    }

    Medication* getById(int id) override {
        for (auto &m : medications)
            if (m.getMedicationId() == id)
                return &m;
        return nullptr;
    }

    std::vector<Medication> getAll() const override {
        return medications;
    }

    Medication* findByName(const std::string &name) override {
        for (auto &m : medications)
            if (m.getName() == name)
                return &m;
        return nullptr;
    }
};

class InMemoryPrescriptionRepository : public IPrescriptionRepository {
private:
    std::vector<Prescription> prescriptions;
public:
    void add(const Prescription &prescription) override {
        prescriptions.push_back(prescription);
    }

    bool remove(int id) override {
        auto it = std::remove_if(prescriptions.begin(), prescriptions.end(),
            [id](const Prescription &p){ return p.getPrescriptionId() == id; });
        if (it != prescriptions.end()) {
            prescriptions.erase(it, prescriptions.end());
            return true;
        }
        return false;
    }

    Prescription* getById(int id) override {
        for (auto &p : prescriptions)
            if (p.getPrescriptionId() == id)
                return &p;
        return nullptr;
    }

    std::vector<Prescription> getAll() const override {
        return prescriptions;
    }

    std::vector<Prescription> findByPatientId(int patientId) const override {
        std::vector<Prescription> result;
        for (const auto &p : prescriptions) {
            if (p.getPatientId() == patientId) {
                result.push_back(p);
            }
        }
        return result;
    }

    std::vector<Prescription> findByDoctorId(int doctorId) const override {
        std::vector<Prescription> result;
        for (const auto &p : prescriptions) {
            if (p.getDoctorId() == doctorId) {
                result.push_back(p);
            }
        }
        return result;
    }
};

class InMemoryBillRepository : public IBillRepository {
private:
    std::vector<Bill> bills;
public:
    void add(const Bill &bill) override {
        bills.push_back(bill);
    }

    bool remove(int id) override {
        auto it = std::remove_if(bills.begin(), bills.end(),
            [id](const Bill &b){ return b.getBillId() == id; });
        if (it != bills.end()) {
            bills.erase(it, bills.end());
            return true;
        }
        return false;
    }

    Bill* getById(int id) override {
        for (auto &b : bills)
            if (b.getBillId() == id)
                return &b;
        return nullptr;
    }

    std::vector<Bill> getAll() const override {
        return bills;
    }

    std::vector<Bill> findByPatientId(int patientId) const override {
        std::vector<Bill> result;
        for (const auto &b : bills) {
            if (b.getPatientId() == patientId) {
                result.push_back(b);
            }
        }
        return result;
    }

    std::vector<Bill> findByPaymentStatus(const std::string &status) const override {
        std::vector<Bill> result;
        for (const auto &b : bills) {
            if (b.getPaymentStatus() == status) {
                result.push_back(b);
            }
        }
        return result;
    }

    double getTotalRevenue() const override {
        double total = 0.0;
        for (const auto &b : bills) {
            total += b.getTotalAmount();
        }
        return total;
    }
};

class InMemoryUserRepository : public IUserRepository {
private:
    std::vector<User> users;
public:
    void add(const User &user) override {
        users.push_back(user);
    }

    bool remove(int id) override {
        auto it = std::remove_if(users.begin(), users.end(),
            [id](const User &u){ return u.getUserId() == id; });
        if (it != users.end()) {
            users.erase(it, users.end());
            return true;
        }
        return false;
    }

    User* getById(int id) override {
        for (auto &u : users)
            if (u.getUserId() == id)
                return &u;
        return nullptr;
    }

    std::vector<User> getAll() const override {
        return users;
    }

    User* findByUsername(const std::string &username) override {
        for (auto &u : users)
            if (u.getUsername() == username)
                return &u;
        return nullptr;
    }

    std::vector<User> findByRole(const std::string &role) const override {
        std::vector<User> result;
        for (const auto &u : users) {
            if (u.getRole() == role) {
                result.push_back(u);
            }
        }
        return result;
    }
};

// ------------------------------
// Service Classes (Business Logic)
// ------------------------------

// Authentication service
class AuthenticationService {
private:
    std::shared_ptr<IUserRepository> userRepo;
    std::shared_ptr<ILogger> logger;
    int nextUserId = 1;
    User* currentUser = nullptr;

public:
    AuthenticationService(std::shared_ptr<IUserRepository> repo, std::shared_ptr<ILogger> log)
        : userRepo(repo), logger(log) {}
    
    bool login(const std::string &username, const std::string &password) {
        User* user = userRepo->findByUsername(username);
        if (user && user->checkPassword(password) && user->getIsActive()) {
            currentUser = user;
            logger->logInfo("User logged in: " + username);
            return true;
        }
        logger->logWarning("Failed login attempt for username: " + username);
        return false;
    }
    
    void logout() {
        if (currentUser) {
            logger->logInfo("User logged out: " + currentUser->getUsername());
            currentUser = nullptr;
        }
    }
    
    User* getCurrentUser() const {
        return currentUser;
    }
    
    bool isLoggedIn() const {
        return currentUser != nullptr;
    }
    
    bool hasRole(const std::string &role) const {
        return isLoggedIn() && currentUser->getRole() == role;
    }
    
    bool registerUser(const std::string &username, const std::string &password, const std::string &role) {
        if (userRepo->findByUsername(username)) {
            logger->logWarning("Failed to register: Username already exists: " + username);
            return false; // Username already exists
        }
        
        User user(nextUserId++, username, password, role);
        userRepo->add(user);
        logger->logInfo("New user registered: " + username + " with role: " + role);
        return true;
    }
    
    bool updateUserStatus(int userId, bool isActive) {
        User* user = userRepo->getById(userId);
        if (user) {
            user->setIsActive(isActive);
            logger->logInfo("User status updated: " + user->getUsername() + " is now " + 
                           (isActive ? "active" : "inactive"));
            return true;
        }
        return false;
    }
    
    std::vector<User> getAllUsers() const {
        return userRepo->getAll();
    }
};

class PatientService {
private:
    std::shared_ptr<IPatientRepository> patientRepo;
    std::shared_ptr<ILogger> logger;
    std::shared_ptr<IDisplayManager> display;
    int nextPatientId = 1;

public:
    PatientService(std::shared_ptr<IPatientRepository> repo, 
                  std::shared_ptr<ILogger> log,
                  std::shared_ptr<IDisplayManager> disp)
        : patientRepo(repo), logger(log), display(disp) {}

    void addPatient(const std::string &name, int age, const std::string &disease,
                   const std::string &contactNumber = "", const std::string &address = "",
                   const std::string &bloodGroup = "") {
        Patient p(nextPatientId++, name, age, disease, contactNumber, address, bloodGroup);
        patientRepo->add(p);
        logger->logInfo("Added patient: " + name + " (ID: " + std::to_string(p.getId()) + ")");
        display->displaySuccess("Patient added successfully with ID: " + std::to_string(p.getId()));
    }

    void updatePatient(int id, const std::string &name, int age, const std::string &disease,
                      const std::string &contactNumber = "", const std::string &address = "",
                      const std::string &bloodGroup = "") {
        Patient* p = patientRepo->getById(id);
        if (p) {
            p->setName(name);
            p->setAge(age);
            p->setDisease(disease);
            p->setContactNumber(contactNumber);
            p->setAddress(address);
            p->setBloodGroup(bloodGroup);
            logger->logInfo("Updated patient with ID: " + std::to_string(id));
            display->displaySuccess("Patient updated successfully.");
        } else {
            logger->logWarning("Failed to update: Patient not found with ID: " + std::to_string(id));
            display->displayError("Patient not found.");
        }
    }

    void removePatient(int id) {
        if (patientRepo->remove(id)) {
            logger->logInfo("Removed patient with ID: " + std::to_string(id));
            display->displaySuccess("Patient removed successfully.");
        } else {
            logger->logWarning("Failed to remove: Patient not found with ID: " + std::to_string(id));
            display->displayError("Patient not found.");
        }
    }

    void listPatients() const {
        auto patients = patientRepo->getAll();
        if (patients.empty()) {
            display->displayInfo("No patients registered.");
            return;
        }
        display->displayInfo("List of all patients:");
        for (const auto &p : patients) {
            p.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void findPatientsByDisease(const std::string &disease) const {
        auto patients = patientRepo->findByDisease(disease);
        if (patients.empty()) {
            display->displayInfo("No patients found with disease: " + disease);
            return;
        }
        display->displayInfo("Patients with disease '" + disease + "':");
        for (const auto &p : patients) {
            p.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void findPatientsByAgeRange(int minAge, int maxAge) const {
        auto patients = patientRepo->findByAgeRange(minAge, maxAge);
        if (patients.empty()) {
            display->displayInfo("No patients found in age range " + 
                               std::to_string(minAge) + " to " + std::to_string(maxAge));
            return;
        }
        display->displayInfo("Patients in age range " + std::to_string(minAge) + 
                           " to " + std::to_string(maxAge) + ":");
        for (const auto &p : patients) {
            p.display();
            std::cout << "-------------------------\n";
        }
    }

    Patient* getPatientById(int id) {
        return patientRepo->getById(id);
    }
};

class DoctorService {
private:
    std::shared_ptr<IDoctorRepository> doctorRepo;
    std::shared_ptr<ILogger> logger;
    std::shared_ptr<IDisplayManager> display;
    int nextDoctorId = 1;

public:
    DoctorService(std::shared_ptr<IDoctorRepository> repo,
                 std::shared_ptr<ILogger> log,
                 std::shared_ptr<IDisplayManager> disp)
        : doctorRepo(repo), logger(log), display(disp) {}

    void addDoctor(const std::string &name, const std::string &specialization,
                  const std::string &contactNumber = "", const std::string &email = "",
                  double consultationFee = 0.0) {
        Doctor d(nextDoctorId++, name, specialization, contactNumber, email, consultationFee);
        doctorRepo->add(d);
        logger->logInfo("Added doctor: " + name + " (ID: " + std::to_string(d.getId()) + ")");
        display->displaySuccess("Doctor added successfully with ID: " + std::to_string(d.getId()));
    }

    void updateDoctor(int id, const std::string &name, const std::string &specialization,
                     const std::string &contactNumber = "", const std::string &email = "",
                     double consultationFee = 0.0) {
        Doctor* d = doctorRepo->getById(id);
        if (d) {
            d->setName(name);
            d->setSpecialization(specialization);
            d->setContactNumber(contactNumber);
            d->setEmail(email);
            d->setConsultationFee(consultationFee);
            logger->logInfo("Updated doctor with ID: " + std::to_string(id));
            display->displaySuccess("Doctor updated successfully.");
        } else {
            logger->logWarning("Failed to update: Doctor not found with ID: " + std::to_string(id));
            display->displayError("Doctor not found.");
        }
    }

    void removeDoctor(int id) {
        if (doctorRepo->remove(id)) {
            logger->logInfo("Removed doctor with ID: " + std::to_string(id));
            display->displaySuccess("Doctor removed successfully.");
        } else {
            logger->logWarning("Failed to remove: Doctor not found with ID: " + std::to_string(id));
            display->displayError("Doctor not found.");
        }
    }

    void listDoctors() const {
        auto doctors = doctorRepo->getAll();
        if (doctors.empty()) {
            display->displayInfo("No doctors registered.");
            return;
        }
        display->displayInfo("List of all doctors:");
        for (const auto &d : doctors) {
            d.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void listAvailableDoctors() const {
        auto doctors = doctorRepo->findAvailableDoctors();
        if (doctors.empty()) {
            display->displayInfo("No available doctors found.");
            return;
        }
        display->displayInfo("List of available doctors:");
        for (const auto &d : doctors) {
            d.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void findDoctorsBySpecialization(const std::string &specialization) const {
        auto doctors = doctorRepo->findBySpecialization(specialization);
        if (doctors.empty()) {
            display->displayInfo("No doctors found with specialization: " + specialization);
            return;
        }
        display->displayInfo("Doctors with specialization '" + specialization + "':");
        for (const auto &d : doctors) {
            d.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void setDoctorAvailability(int id, bool isAvailable) {
        Doctor* d = doctorRepo->getById(id);
        if (d) {
            d->setAvailability(isAvailable);
            logger->logInfo("Updated doctor availability: Doctor ID " + std::to_string(id) + 
                          " is now " + (isAvailable ? "available" : "unavailable"));
            display->displaySuccess("Doctor availability updated successfully.");
        } else {
            logger->logWarning("Failed to update availability: Doctor not found with ID: " + std::to_string(id));
            display->displayError("Doctor not found.");
        }
    }

    Doctor* getDoctorById(int id) {
        return doctorRepo->getById(id);
    }
};

class AppointmentService {
private:
    std::shared_ptr<IAppointmentRepository> apptRepo;
    PatientService &patientService;
    DoctorService &doctorService;
    std::shared_ptr<ILogger> logger;
    std::shared_ptr<IDisplayManager> display;
    int nextAppointmentId = 1;

public:
    AppointmentService(std::shared_ptr<IAppointmentRepository> repo,
                       PatientService &ps, DoctorService &ds,
                       std::shared_ptr<ILogger> log,
                       std::shared_ptr<IDisplayManager> disp)
        : apptRepo(repo), patientService(ps), doctorService(ds), 
          logger(log), display(disp) {}

    void bookAppointment(int patientId, int doctorId, const std::string &date, 
                         const std::string &timeSlot = "09:00-09:30") {
        // Validate existence of patient and doctor
        Patient* patient = patientService.getPatientById(patientId);
        if (!patient) {
            logger->logWarning("Failed to book appointment: Invalid Patient ID: " + std::to_string(patientId));
            display->displayError("Invalid Patient ID.");
            return;
        }
        
        Doctor* doctor = doctorService.getDoctorById(doctorId);
        if (!doctor) {
            logger->logWarning("Failed to book appointment: Invalid Doctor ID: " + std::to_string(doctorId));
            display->displayError("Invalid Doctor ID.");
            return;
        }
        
        // Check if doctor is available
        if (!doctor->getAvailability()) {
            logger->logWarning("Failed to book appointment: Doctor is not available: " + std::to_string(doctorId));
            display->displayError("Doctor is not available for appointments.");
            return;
        }
        
        // Check for conflicts in the same time slot
        auto appointments = apptRepo->findByDate(date);
        for (const auto &a : appointments) {
            if (a.getDoctorId() == doctorId && a.getTimeSlot() == timeSlot) {
                logger->logWarning("Failed to book appointment: Time slot is already booked.");
                display->displayError("The selected time slot is already booked for this doctor.");
                return;
            }
        }
        
        Appointment a(nextAppointmentId++, patientId, doctorId, date, timeSlot);
        apptRepo->add(a);
        
        logger->logInfo("Booked appointment: Patient ID " + std::to_string(patientId) + 
                       " with Doctor ID " + std::to_string(doctorId) + 
                       " on " + date + " at " + timeSlot);
        display->displaySuccess("Appointment booked successfully with ID: " + 
                              std::to_string(a.getAppointmentId()));
    }

    void updateAppointmentDetails(int apptId, const std::string &newDate, 
                                 const std::string &newTimeSlot, 
                                 const std::string &newStatus,
                                 const std::string &notes) {
        Appointment* a = apptRepo->getById(apptId);
        if (a) {
            a->setDate(newDate);
            a->setTimeSlot(newTimeSlot);
            a->setStatus(newStatus);
            a->setNotes(notes);
            
            logger->logInfo("Updated appointment: ID " + std::to_string(apptId) + 
                           " to " + newDate + " at " + newTimeSlot + 
                           " (Status: " + newStatus + ")");
            display->displaySuccess("Appointment updated successfully.");
        } else {
            logger->logWarning("Failed to update: Appointment not found with ID: " + std::to_string(apptId));
            display->displayError("Appointment not found.");
        }
    }

    void updateAppointmentStatus(int apptId, const std::string &newStatus) {
        Appointment* a = apptRepo->getById(apptId);
        if (a) {
            a->setStatus(newStatus);
            logger->logInfo("Updated appointment status: ID " + std::to_string(apptId) + 
                           " to " + newStatus);
            display->displaySuccess("Appointment status updated successfully.");
        } else {
            logger->logWarning("Failed to update status: Appointment not found with ID: " + std::to_string(apptId));
            display->displayError("Appointment not found.");
        }
    }

    void cancelAppointment(int apptId) {
        Appointment* a = apptRepo->getById(apptId);
        if (a) {
            a->setStatus("Cancelled");
            logger->logInfo("Cancelled appointment: ID " + std::to_string(apptId));
            display->displaySuccess("Appointment marked as cancelled.");
        } else if (apptRepo->remove(apptId)) {
            logger->logInfo("Removed appointment: ID " + std::to_string(apptId));
            display->displaySuccess("Appointment removed completely.");
        } else {
            logger->logWarning("Failed to cancel: Appointment not found with ID: " + std::to_string(apptId));
            display->displayError("Appointment not found.");
        }
    }

    void listAllAppointments() const {
        auto appointments = apptRepo->getAll();
        if (appointments.empty()) {
            display->displayInfo("No appointments found.");
            return;
        }
        display->displayInfo("List of all appointments:");
        for (const auto &a : appointments) {
            a.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void listAppointmentsByPatient(int patientId) const {
        auto appointments = apptRepo->findByPatientId(patientId);
        if (appointments.empty()) {
            display->displayInfo("No appointments found for patient ID: " + std::to_string(patientId));
            return;
        }
        display->displayInfo("Appointments for patient ID " + std::to_string(patientId) + ":");
        for (const auto &a : appointments) {
            a.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void listAppointmentsByDoctor(int doctorId) const {
        auto appointments = apptRepo->findByDoctorId(doctorId);
        if (appointments.empty()) {
            display->displayInfo("No appointments found for doctor ID: " + std::to_string(doctorId));
            return;
        }
        display->displayInfo("Appointments for doctor ID " + std::to_string(doctorId) + ":");
        for (const auto &a : appointments) {
            a.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void listAppointmentsByDate(const std::string &date) const {
        auto appointments = apptRepo->findByDate(date);
        if (appointments.empty()) {
            display->displayInfo("No appointments found for date: " + date);
            return;
        }
        display->displayInfo("Appointments for date " + date + ":");
        for (const auto &a : appointments) {
            a.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void listAppointmentsByStatus(const std::string &status) const {
        auto appointments = apptRepo->findByStatus(status);
        if (appointments.empty()) {
            display->displayInfo("No appointments found with status: " + status);
            return;
        }
        display->displayInfo("Appointments with status '" + status + "':");
        for (const auto &a : appointments) {
            a.display();
            std::cout << "-------------------------\n";
        }
    }
};

class MedicationService {
private:
    std::shared_ptr<IMedicationRepository> medRepo;
    std::shared_ptr<ILogger> logger;
    std::shared_ptr<IDisplayManager> display;
    int nextMedicationId = 1;

public:
    MedicationService(std::shared_ptr<IMedicationRepository> repo,
                     std::shared_ptr<ILogger> log,
                     std::shared_ptr<IDisplayManager> disp)
        : medRepo(repo), logger(log), display(disp) {}
        
    void addMedication(const std::string &name, const std::string &dosage, double price,
                      const std::string &manufacturer = "", const std::string &description = "") {
        if (medRepo->findByName(name)) {
            logger->logWarning("Failed to add: Medication with name '" + name + "' already exists");
            display->displayError("Medication with this name already exists.");
            return;
        }
        
        Medication m(nextMedicationId++, name, dosage, price, manufacturer, description);
        medRepo->add(m);
        logger->logInfo("Added medication: " + name + " (ID: " + std::to_string(m.getMedicationId()) + ")");
        display->displaySuccess("Medication added successfully with ID: " + std::to_string(m.getMedicationId()));
    }
    
    void updateMedication(int id, const std::string &name, const std::string &dosage, double price,
                         const std::string &manufacturer = "", const std::string &description = "") {
        Medication* m = medRepo->getById(id);
        if (!m) {
            logger->logWarning("Failed to update: Medication not found with ID: " + std::to_string(id));
            display->displayError("Medication not found.");
            return;
        }
        
        // Check if the new name conflicts with an existing medication (only if name is changing)
        if (name != m->getName()) {
            Medication* existing = medRepo->findByName(name);
            if (existing && existing->getMedicationId() != id) {
                logger->logWarning("Failed to update: Medication name '" + name + "' already in use");
                display->displayError("A medication with this name already exists.");
                return;
            }
        }
        
        m->setName(name);
        m->setDosage(dosage);
        m->setPrice(price);
        m->setManufacturer(manufacturer);
        m->setDescription(description);
        
        logger->logInfo("Updated medication: ID " + std::to_string(id));
        display->displaySuccess("Medication updated successfully.");
    }
    
    void removeMedication(int id) {
        if (medRepo->remove(id)) {
            logger->logInfo("Removed medication with ID: " + std::to_string(id));
            display->displaySuccess("Medication removed successfully.");
        } else {
            logger->logWarning("Failed to remove: Medication not found with ID: " + std::to_string(id));
            display->displayError("Medication not found.");
        }
    }
    
    void listAllMedications() const {
        auto medications = medRepo->getAll();
        if (medications.empty()) {
            display->displayInfo("No medications available.");
            return;
        }
        display->displayInfo("List of all medications:");
        for (const auto &m : medications) {
            m.display();
            std::cout << "-------------------------\n";
        }
    }
    
    Medication* getMedicationById(int id) {
        return medRepo->getById(id);
    }
    
    Medication* getMedicationByName(const std::string &name) {
        return medRepo->findByName(name);
    }
};

class PrescriptionService {
private:
    std::shared_ptr<IPrescriptionRepository> prescRepo;
    PatientService &patientService;
    DoctorService &doctorService;
    MedicationService &medicationService;
    std::shared_ptr<ILogger> logger;
    std::shared_ptr<IDisplayManager> display;
    int nextPrescriptionId = 1;

public:
    PrescriptionService(std::shared_ptr<IPrescriptionRepository> repo,
                       PatientService &ps, DoctorService &ds, MedicationService &ms,
                       std::shared_ptr<ILogger> log,
                       std::shared_ptr<IDisplayManager> disp)
        : prescRepo(repo), patientService(ps), doctorService(ds), 
          medicationService(ms), logger(log), display(disp) {}
          
    void createPrescription(int patientId, int doctorId, const std::string &date,
                           const std::vector<int> &medicationIds, const std::string &instructions = "") {
        // Validate patient and doctor
        if (!patientService.getPatientById(patientId)) {
            logger->logWarning("Failed to create prescription: Invalid Patient ID: " + std::to_string(patientId));
            display->displayError("Invalid Patient ID.");
            return;
        }
        
        if (!doctorService.getDoctorById(doctorId)) {
            logger->logWarning("Failed to create prescription: Invalid Doctor ID: " + std::to_string(doctorId));
            display->displayError("Invalid Doctor ID.");
            return;
        }
        
        // Validate all medications
        for (int medId : medicationIds) {
            if (!medicationService.getMedicationById(medId)) {
                logger->logWarning("Failed to create prescription: Invalid Medication ID: " + std::to_string(medId));
                display->displayError("Invalid Medication ID: " + std::to_string(medId));
                return;
            }
        }
        
        Prescription p(nextPrescriptionId++, patientId, doctorId, date, medicationIds, instructions);
        prescRepo->add(p);
        
        // Update patient's medication list
        Patient* patient = patientService.getPatientById(patientId);
        for (int medId : medicationIds) {
            patient->addMedicationId(medId);
        }
        
        logger->logInfo("Created prescription for Patient ID " + std::to_string(patientId) + 
                       " by Doctor ID " + std::to_string(doctorId));
        display->displaySuccess("Prescription created successfully with ID: " + std::to_string(p.getPrescriptionId()));
    }
    
    void updatePrescription(int prescriptionId, const std::vector<int> &medicationIds, 
                           const std::string &instructions) {
        Prescription* p = prescRepo->getById(prescriptionId);
        if (!p) {
            logger->logWarning("Failed to update: Prescription not found with ID: " + std::to_string(prescriptionId));
            display->displayError("Prescription not found.");
            return;
        }
        
        // Validate all medications
        for (int medId : medicationIds) {
            if (!medicationService.getMedicationById(medId)) {
                logger->logWarning("Failed to update prescription: Invalid Medication ID: " + std::to_string(medId));
                display->displayError("Invalid Medication ID: " + std::to_string(medId));
                return;
            }
        }
        
        // Update the patient's medication list
        Patient* patient = patientService.getPatientById(p->getPatientId());
        if (patient) {
            // Remove old medications
            for (int medId : p->getMedicationIds()) {
                patient->removeMedicationId(medId);
            }
            
            // Add new medications
            for (int medId : medicationIds) {
                patient->addMedicationId(medId);
            }
        }
        
        // Update prescription with new medications and instructions
        p->getMedicationIds() = medicationIds;
        p->setInstructions(instructions);
        
        logger->logInfo("Updated prescription with ID: " + std::to_string(prescriptionId));
        display->displaySuccess("Prescription updated successfully.");
    }
    
    void removePrescription(int prescriptionId) {
        Prescription* p = prescRepo->getById(prescriptionId);
        if (!p) {
            logger->logWarning("Failed to remove: Prescription not found with ID: " + std::to_string(prescriptionId));
            display->displayError("Prescription not found.");
            return;
        }
        
        // Remove medications from patient's list
        Patient* patient = patientService.getPatientById(p->getPatientId());
        if (patient) {
            for (int medId : p->getMedicationIds()) {
                patient->removeMedicationId(medId);
            }
        }
        
        if (prescRepo->remove(prescriptionId)) {
            logger->logInfo("Removed prescription with ID: " + std::to_string(prescriptionId));
            display->displaySuccess("Prescription removed successfully.");
        }
    }
    
    void listAllPrescriptions() const {
        auto prescriptions = prescRepo->getAll();
        if (prescriptions.empty()) {
            display->displayInfo("No prescriptions found.");
            return;
        }
        display->displayInfo("List of all prescriptions:");
        for (const auto &p : prescriptions) {
            p.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void listPrescriptionsByPatient(int patientId) const {
        auto prescriptions = prescRepo->findByPatientId(patientId);
        if (prescriptions.empty()) {
            display->displayInfo("No prescriptions found for patient ID: " + std::to_string(patientId));
            return;
        }
        display->displayInfo("Prescriptions for patient ID " + std::to_string(patientId) + ":");
        for (const auto &p : prescriptions) {
            p.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void listPrescriptionsByDoctor(int doctorId) const {
        auto prescriptions = prescRepo->findByDoctorId(doctorId);
        if (prescriptions.empty()) {
            display->displayInfo("No prescriptions found for doctor ID: " + std::to_string(doctorId));
            return;
        }
        display->displayInfo("Prescriptions by doctor ID " + std::to_string(doctorId) + ":");
        for (const auto &p : prescriptions) {
            p.display();
            std::cout << "-------------------------\n";
        }
    }
    
    Prescription* getPrescriptionById(int id) {
        return prescRepo->getById(id);
    }
};

class BillingService {
private:
    std::shared_ptr<IBillRepository> billRepo;
    PatientService &patientService;
    DoctorService &doctorService;
    std::shared_ptr<ILogger> logger;
    std::shared_ptr<IDisplayManager> display;
    int nextBillId = 1;

public:
    BillingService(std::shared_ptr<IBillRepository> repo,
                  PatientService &ps, DoctorService &ds,
                  std::shared_ptr<ILogger> log,
                  std::shared_ptr<IDisplayManager> disp)
        : billRepo(repo), patientService(ps), doctorService(ds), 
          logger(log), display(disp) {}
          
    void generateBill(int patientId, const std::string &date, double consultationFee,
                     double medicationCharges = 0.0, double otherCharges = 0.0) {
        // Validate patient
        if (!patientService.getPatientById(patientId)) {
            logger->logWarning("Failed to generate bill: Invalid Patient ID: " + std::to_string(patientId));
            display->displayError("Invalid Patient ID.");
            return;
        }
        
        Bill bill(nextBillId++, patientId, date, consultationFee, medicationCharges, otherCharges);
        billRepo->add(bill);
        
        logger->logInfo("Generated bill for Patient ID " + std::to_string(patientId) + 
                       " with total amount: $" + std::to_string(bill.getTotalAmount()));
        display->displaySuccess("Bill generated successfully with ID: " + 
                              std::to_string(bill.getBillId()) + 
                              " (Total: $" + std::to_string(bill.getTotalAmount()) + ")");
    }
    
    void updateBillPaymentStatus(int billId, const std::string &status, const std::string &paymentMethod = "") {
        Bill* bill = billRepo->getById(billId);
        if (!bill) {
            logger->logWarning("Failed to update: Bill not found with ID: " + std::to_string(billId));
            display->displayError("Bill not found.");
            return;
        }
        
        bill->setPaymentStatus(status);
        if (!paymentMethod.empty()) {
            bill->setPaymentMethod(paymentMethod);
        }
        
        logger->logInfo("Updated bill payment status: ID " + std::to_string(billId) + 
                       " to " + status + 
                       (paymentMethod.empty() ? "" : " (Method: " + paymentMethod + ")"));
        display->displaySuccess("Bill payment status updated successfully.");
    }
    
    void listAllBills() const {
        auto bills = billRepo->getAll();
        if (bills.empty()) {
            display->displayInfo("No bills found.");
            return;
        }
        display->displayInfo("List of all bills:");
        for (const auto &b : bills) {
            b.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void listBillsByPatient(int patientId) const {
        auto bills = billRepo->findByPatientId(patientId);
        if (bills.empty()) {
            display->displayInfo("No bills found for patient ID: " + std::to_string(patientId));
            return;
        }
        display->displayInfo("Bills for patient ID " + std::to_string(patientId) + ":");
        for (const auto &b : bills) {
            b.display();
            std::cout << "-------------------------\n";
        }
    }
    
    void listBillsByPaymentStatus(const std::string &status) const {
        auto bills = billRepo->findByPaymentStatus(status);
        if (bills.empty()) {
            display->displayInfo("No bills found with payment status: " + status);
            return;
        }
        display->displayInfo("Bills with payment status '" + status + "':");
        for (const auto &b : bills) {
            b.display();
            std::cout << "-------------------------\n";
        }
    }
    
    double getTotalRevenue() const {
        double total = billRepo->getTotalRevenue();
        display->displayInfo("Total revenue: $" + std::to_string(total));
        return total;
    }
    
    Bill* getBillById(int id) {
        return billRepo->getById(id);
    }
};

// ------------------------------
// Application / User Interface
// ------------------------------

class HospitalManagementApp {
private:
    // Cross-cutting concerns
    std::shared_ptr<ILogger> logger;
    std::shared_ptr<IDisplayManager> display;
    
    // Repositories
    std::shared_ptr<IPatientRepository> patientRepo;
    std::shared_ptr<IDoctorRepository> doctorRepo;
    std::shared_ptr<IAppointmentRepository> appointmentRepo;
    std::shared_ptr<IMedicationRepository> medicationRepo;
    std::shared_ptr<IPrescriptionRepository> prescriptionRepo;
    std::shared_ptr<IBillRepository> billRepo;
    std::shared_ptr<IUserRepository> userRepo;
    
    // Services
    AuthenticationService authService;
    PatientService patientService;
    DoctorService doctorService;
    AppointmentService appointmentService;
    MedicationService medicationService;
    PrescriptionService prescriptionService;
    BillingService billingService;
    
    bool isLoggedIn = false;

    // Helper function to read a line of text
    std::string readLine() {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    // Helper function to read an integer
    int readInt() {
        int value;
        while (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            display->displayError("Invalid input. Please enter a number.");
            std::cout << "Enter a number: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
    
    // Helper function to read a double
    double readDouble() {
        double value;
        while (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            display->displayError("Invalid input. Please enter a number.");
            std::cout << "Enter a number: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
    
    // Helper function to get date input
    std::string getDateInput(const std::string &prompt = "Enter Date (YYYY-MM-DD): ") {
        std::cout << prompt;
        return readLine();
    }
    
    // Helper function to get time slot input
    std::string getTimeSlotInput() {
        std::cout << "Available time slots:\n";
        std::cout << "1. 09:00-09:30\n";
        std::cout << "2. 09:30-10:00\n";
        std::cout << "3. 10:00-10:30\n";
        std::cout << "4. 10:30-11:00\n";
        std::cout << "5. 11:00-11:30\n";
        std::cout << "6. 11:30-12:00\n";
        std::cout << "7. 14:00-14:30\n";
        std::cout << "8. 14:30-15:00\n";
        std::cout << "9. 15:00-15:30\n";
        std::cout << "10. 15:30-16:00\n";
        std::cout << "Enter your choice (1-10): ";
        
        int choice = readInt();
        std::string timeSlot;
        
        switch (choice) {
            case 1: timeSlot = "09:00-09:30"; break;
            case 2: timeSlot = "09:30-10:00"; break;
            case 3: timeSlot = "10:00-10:30"; break;
            case 4: timeSlot = "10:30-11:00"; break;
            case 5: timeSlot = "11:00-11:30"; break;
            case 6: timeSlot = "11:30-12:00"; break;
            case 7: timeSlot = "14:00-14:30"; break;
            case 8: timeSlot = "14:30-15:00"; break;
            case 9: timeSlot = "15:00-15:30"; break;
            case 10: timeSlot = "15:30-16:00"; break;
            default: timeSlot = "09:00-09:30"; break;
        }
        
        return timeSlot;
    }

    void displayLoginMenu() {
        std::cout << "\n----- Hospital Management System Login -----\n";
        std::cout << "1. Login\n";
        std::cout << "2. Exit\n";
        std::cout << "Enter your choice: ";
    }

    void displayMainMenu() {
        std::cout << "\n----- Hospital Management System Menu -----\n";
        
        // Only show admin options if user has admin role
        if (authService.hasRole("Admin")) {
            std::cout << "==== Admin Functions ====\n";
            std::cout << "1. User Management\n";
            std::cout << "2. View System Logs\n";
            std::cout << "3. Financial Reports\n";
        }
        
        std::cout << "==== Patient Management ====\n";
        std::cout << "4. Add Patient\n";
        std::cout << "5. Update Patient\n";
        std::cout << "6. Remove Patient\n";
        std::cout << "7. List All Patients\n";
        std::cout << "8. Find Patients by Disease\n";
        std::cout << "9. Find Patients by Age Range\n";
        
        std::cout << "==== Doctor Management ====\n";
        std::cout << "10. Add Doctor\n";
        std::cout << "11. Update Doctor\n";
        std::cout << "12. Remove Doctor\n";
        std::cout << "13. List All Doctors\n";
        std::cout << "14. List Available Doctors\n";
        std::cout << "15. Find Doctors by Specialization\n";
        std::cout << "16. Set Doctor Availability\n";
        
        std::cout << "==== Appointment Management ====\n";
        std::cout << "17. Book Appointment\n";
        std::cout << "18. Update Appointment\n";
        std::cout << "19. Cancel Appointment\n";
        std::cout << "20. List All Appointments\n";
        std::cout << "21. List Appointments by Patient\n";
        std::cout << "22. List Appointments by Doctor\n";
        std::cout << "23. List Appointments by Date\n";
        
        std::cout << "==== Medication Management ====\n";
        std::cout << "24. Add Medication\n";
        std::cout << "25. Update Medication\n";
        std::cout << "26. Remove Medication\n";
        std::cout << "27. List All Medications\n";
        
        std::cout << "==== Prescription Management ====\n";
        std::cout << "28. Create Prescription\n";
        std::cout << "29. Update Prescription\n";
        std::cout << "30. Remove Prescription\n";
        std::cout << "31. List Prescriptions by Patient\n";
        
        std::cout << "==== Billing Management ====\n";
        std::cout << "32. Generate Bill\n";
        std::cout << "33. Update Payment Status\n";
        std::cout << "34. List Bills by Patient\n";
        std::cout << "35. List Bills by Payment Status\n";
        
        std::cout << "==== System ====\n";
        std::cout << "36. Logout\n";
        std::cout << "37. Exit\n";
        
        std::cout << "Enter your choice: ";
    }

    bool login() {
        std::cout << "Enter username: ";
        std::string username = readLine();
        std::cout << "Enter password: ";
        std::string password = readLine();
        
        if (authService.login(username, password)) {
            display->displaySuccess("Login successful. Welcome, " + username + "!");
            isLoggedIn = true;
            return true;
        } else {
            display->displayError("Login failed. Invalid username or password.");
            return false;
        }
    }
    
    void logout() {
        authService.logout();
        isLoggedIn = false;
        display->displayInfo("You have been logged out.");
    }
    
    void setupTestData() {
        // Create an admin user
        authService.registerUser("admin", "admin123", "Admin");
        authService.registerUser("doctor", "doctor123", "Doctor");
        authService.registerUser("reception", "reception123", "Reception");
        
        // Add some sample doctors
        doctorService.addDoctor("Dr. John Smith", "Cardiology", "123-456-7890", "john@hospital.com", 100.0);
        doctorService.addDoctor("Dr. Jane Doe", "Neurology", "987-654-3210", "jane@hospital.com", 150.0);
        doctorService.addDoctor("Dr. Robert Johnson", "Pediatrics", "555-123-4567", "robert@hospital.com", 80.0);
        
        // Add some sample patients
        patientService.addPatient("Alice Brown", 35, "Hypertension", "111-222-3333", "123 Main St", "O+");
        patientService.addPatient("Bob Wilson", 42, "Diabetes", "444-555-6666", "456 Oak Ave", "A-");
        patientService.addPatient("Carol Martinez", 28, "Asthma", "777-888-9999", "789 Pine Blvd", "B+");
        
        // Add some medications
        medicationService.addMedication("Aspirin", "100mg", 5.99, "Bayer", "Pain reliever and anti-inflammatory");
        medicationService.addMedication("Amoxicillin", "500mg", 15.50, "Generic", "Antibiotic");
        medicationService.addMedication("Lisinopril", "10mg", 8.75, "Generic", "Blood pressure medication");
        
        logger->logInfo("Test data has been set up successfully.");
    }

public:
    HospitalManagementApp()
        : // Initialize cross-cutting concerns
          logger(std::make_shared<FileLogger>()),
          display(std::make_shared<ConsoleDisplayManager>()),
          
          // Initialize repositories
          patientRepo(std::make_shared<InMemoryPatientRepository>()),
          doctorRepo(std::make_shared<InMemoryDoctorRepository>()),
          appointmentRepo(std::make_shared<InMemoryAppointmentRepository>()),
          medicationRepo(std::make_shared<InMemoryMedicationRepository>()),
          prescriptionRepo(std::make_shared<InMemoryPrescriptionRepository>()),
          billRepo(std::make_shared<InMemoryBillRepository>()),
          userRepo(std::make_shared<InMemoryUserRepository>()),
          
          // Initialize services
          authService(userRepo, logger),
          patientService(patientRepo, logger, display),
          doctorService(doctorRepo, logger, display),
          appointmentService(appointmentRepo, patientService, doctorService, logger, display),
          medicationService(medicationRepo, logger, display),
          prescriptionService(prescriptionRepo, patientService, doctorService, medicationService, logger, display),
          billingService(billRepo, patientService, doctorService, logger, display) {
        
        // Setup test data
        setupTestData();
    }

    void run() {
        // First handle login
        bool exitProgram = false;
        while (!exitProgram && !isLoggedIn) {
            displayLoginMenu();
            int choice = readInt();
            
            switch (choice) {
                case 1:
                    if (login()) {
                        runMainApplication();
                    }
                    break;
                case 2:
                    exitProgram = true;
                    display->displayInfo("Exiting program. Goodbye!");
                    break;
                default:
                    display->displayError("Invalid choice. Please try again.");
            }
        }
    }

    void runMainApplication() {
        int choice = 0;
        while (isLoggedIn && choice != 37) {
            displayMainMenu();
            choice = readInt();
            
            // Process the menu choice
            processMenuChoice(choice);
        }
    }
    
    void processMenuChoice(int choice) {
        // Admin functions (1-3)
        if (choice >= 1 && choice <= 3 && !authService.hasRole("Admin")) {
            display->displayError("Access denied. Admin privileges required.");
            return;
        }
        
        switch (choice) {
            // Admin Functions
            case 1: manageUsers(); break;
            case 2: viewSystemLogs(); break;
            case 3: generateFinancialReports(); break;
            
            // Patient Management
            case 4: addPatient(); break;
            case 5: updatePatient(); break;
            case 6: removePatient(); break;
            case 7: listAllPatients(); break;
            case 8: findPatientsByDisease(); break;
            case 9: findPatientsByAgeRange(); break;
            
            // Doctor Management
            case 10: addDoctor(); break;
            case 11: updateDoctor(); break;
            case 12: removeDoctor(); break;
            case 13: listAllDoctors(); break;
            case 14: listAvailableDoctors(); break;
            case 15: findDoctorsBySpecialization(); break;
            case 16: setDoctorAvailability(); break;
            
            // Appointment Management
            case 17: bookAppointment(); break;
            case 18: updateAppointment(); break;
            case 19: cancelAppointment(); break;
            case 20: listAllAppointments(); break;
            case 21: listAppointmentsByPatient(); break;
            case 22: listAppointmentsByDoctor(); break;
            case 23: listAppointmentsByDate(); break;
            
            // Medication Management
            case 24: addMedication(); break;
            case 25: updateMedication(); break;
            case 26: removeMedication(); break;
            case 27: listAllMedications(); break;
            
            // Prescription Management
            case 28: createPrescription(); break;
            case 29: updatePrescription(); break;
            case 30: removePrescription(); break;
            case 31: listPrescriptionsByPatient(); break;
            
            // Billing Management
            case 32: generateBill(); break;
            case 33: updateBillPaymentStatus(); break;
            case 34: listBillsByPatient(); break;
            case 35: listBillsByPaymentStatus(); break;
            
            // System
            case 36: logout(); break;
            case 37: 
                logout();
                display->displayInfo("Exiting application. Goodbye!");
                break;
            default:
                display->displayError("Invalid choice. Please try again.");
        }
    }
    
    // Admin Functions
    void manageUsers() {
        std::cout << "\n----- User Management -----\n";
        std::cout << "1. Add User\n";
        std::cout << "2. List All Users\n";
        std::cout << "3. Enable/Disable User\n";
        std::cout << "4. Back to Main Menu\n";
        std::cout << "Enter your choice: ";
        
        int choice = readInt();
        switch (choice) {
            case 1: {
                std::cout << "Enter username: ";
                std::string username = readLine();
                std::cout << "Enter password: ";
                std::string password = readLine();
                std::cout << "Enter role (Admin, Doctor, Reception): ";
                std::string role = readLine();
                
                if (authService.registerUser(username, password, role)) {
                    display->displaySuccess("User added successfully.");
                } else {
                    display->displayError("Failed to add user. Username may already exist.");
                }
                break;
            }
            case 2: {
                auto users = authService.getAllUsers();
                if (users.empty()) {
                    display->displayInfo("No users registered.");
                    return;
                }
                display->displayInfo("List of all users:");
                for (const auto &u : users) {
                    u.display();
                    std::cout << "-------------------------\n";
                }
                break;
            }
            case 3: {
                std::cout << "Enter user ID: ";
                int userId = readInt();
                std::cout << "Enable user? (1: Yes, 0: No): ";
                int enable = readInt();
                
                if (authService.updateUserStatus(userId, enable == 1)) {
                    display->displaySuccess("User status updated successfully.");
                } else {
                    display->displayError("Failed to update user status. User not found.");
                }
                break;
            }
            case 4:
                return;
            default:
                display->displayError("Invalid choice. Please try again.");
        }
    }
    
    void viewSystemLogs() {
        std::cout << "System logs are stored in hospital_log.txt\n";
        display->displayInfo("Please check the log file for detailed system logs.");
    }
    
    void generateFinancialReports() {
        std::cout << "\n----- Financial Reports -----\n";
        std::cout << "1. Total Revenue\n";
        std::cout << "2. Pending Payments\n";
        std::cout << "3. Back to Main Menu\n";
        std::cout << "Enter your choice: ";
        
        int choice = readInt();
        switch (choice) {
            case 1:
                billingService.getTotalRevenue();
                break;
            case 2:
                billingService.listBillsByPaymentStatus("Pending");
                break;
            case 3:
                return;
            default:
                display->displayError("Invalid choice. Please try again.");
        }
    }
    
    // Patient Management
    void addPatient() {
        std::cout << "Enter Patient Name: ";
        std::string name = readLine();
        std::cout << "Enter Age: ";
        int age = readInt();
        std::cout << "Enter Disease: ";
        std::string disease = readLine();
        std::cout << "Enter Contact Number (optional): ";
        std::string contact = readLine();
        std::cout << "Enter Address (optional): ";
        std::string address = readLine();
        std::cout << "Enter Blood Group (optional): ";
        std::string bloodGroup = readLine();
        
        patientService.addPatient(name, age, disease, contact, address, bloodGroup);
    }
    
    void updatePatient() {
        std::cout << "Enter Patient ID to update: ";
        int id = readInt();
        std::cout << "Enter new Name: ";
        std::string name = readLine();
        std::cout << "Enter new Age: ";
        int age = readInt();
        std::cout << "Enter new Disease: ";
        std::string disease = readLine();
        std::cout << "Enter new Contact Number (optional): ";
        std::string contact = readLine();
        std::cout << "Enter new Address (optional): ";
        std::string address = readLine();
        std::cout << "Enter new Blood Group (optional): ";
        std::string bloodGroup = readLine();
        
        patientService.updatePatient(id, name, age, disease, contact, address, bloodGroup);
    }
    
    void removePatient() {
        std::cout << "Enter Patient ID to remove: ";
        int id = readInt();
        patientService.removePatient(id);
    }
    
    void listAllPatients() {
        patientService.listPatients();
    }
    
    void findPatientsByDisease() {
        std::cout << "Enter disease to search for: ";
        std::string disease = readLine();
        patientService.findPatientsByDisease(disease);
    }
    
    void findPatientsByAgeRange() {
        std::cout << "Enter minimum age: ";
        int minAge = readInt();
        std::cout << "Enter maximum age: ";
        int maxAge = readInt();
        patientService.findPatientsByAgeRange(minAge, maxAge);
    }
    
    // Doctor Management
    void addDoctor() {
        std::cout << "Enter Doctor Name: ";
        std::string name = readLine();
        std::cout << "Enter Specialization: ";
        std::string spec = readLine();
        std::cout << "Enter Contact Number (optional): ";
        std::string contact = readLine();
        std::cout << "Enter Email (optional): ";
        std::string email = readLine();
        std::cout << "Enter Consultation Fee: ";
        double fee = readDouble();
        
        doctorService.addDoctor(name, spec, contact, email, fee);
    }
    
    void updateDoctor() {
        std::cout << "Enter Doctor ID to update: ";
        int id = readInt();
        std::cout << "Enter new Name: ";
        std::string name = readLine();
        std::cout << "Enter new Specialization: ";
        std::string spec = readLine();
        std::cout << "Enter new Contact Number (optional): ";
        std::string contact = readLine();
        std::cout << "Enter new Email (optional): ";
        std::string email = readLine();
        std::cout << "Enter new Consultation Fee: ";
        double fee = readDouble();
        
        doctorService.updateDoctor(id, name, spec, contact, email, fee);
    }
    
    void removeDoctor() {
        std::cout << "Enter Doctor ID to remove: ";
        int id = readInt();
        doctorService.removeDoctor(id);
    }
    
    void listAllDoctors() {
        doctorService.listDoctors();
    }
    
    void listAvailableDoctors() {
        doctorService.listAvailableDoctors();
    }
    
    void findDoctorsBySpecialization() {
        std::cout << "Enter specialization to search for: ";
        std::string spec = readLine();
        doctorService.findDoctorsBySpecialization(spec);
    }
    
    void setDoctorAvailability() {
        std::cout << "Enter Doctor ID: ";
        int id = readInt();
        std::cout << "Set as available? (1: Yes, 0: No): ";
        int available = readInt();
        
        doctorService.setDoctorAvailability(id, available == 1);
    }
    
    // Appointment Management
    void bookAppointment() {
        std::cout << "Enter Patient ID: ";
        int patientId = readInt();
        std::cout << "Enter Doctor ID: ";
        int doctorId = readInt();
        std::string date = getDateInput();
        std::string timeSlot = getTimeSlotInput();
        
        appointmentService.bookAppointment(patientId, doctorId, date, timeSlot);
    }
    
    void updateAppointment() {
        std::cout << "Enter Appointment ID to update: ";
        int apptId = readInt();
        std::string date = getDateInput("Enter new Date (YYYY-MM-DD): ");
        std::string timeSlot = getTimeSlotInput();
        
        std::cout << "Enter new status (Scheduled, Completed, Cancelled): ";
        std::string status = readLine();
        
        std::cout << "Enter notes (optional): ";
        std::string notes = readLine();
        
        appointmentService.updateAppointmentDetails(apptId, date, timeSlot, status, notes);
    }
    
    void cancelAppointment() {
        std::cout << "Enter Appointment ID to cancel: ";
        int apptId = readInt();
        appointmentService.cancelAppointment(apptId);
    }
    
    void listAllAppointments() {
        appointmentService.listAllAppointments();
    }
    
    void listAppointmentsByPatient() {
        std::cout << "Enter Patient ID: ";
        int patientId = readInt();
        appointmentService.listAppointmentsByPatient(patientId);
    }
    
    void listAppointmentsByDoctor() {
        std::cout << "Enter Doctor ID: ";
        int doctorId = readInt();
        appointmentService.listAppointmentsByDoctor(doctorId);
    }
    
    void listAppointmentsByDate() {
        std::string date = getDateInput();
        appointmentService.listAppointmentsByDate(date);
    }
    
    // Medication Management
    void addMedication() {
        std::cout << "Enter Medication Name: ";
        std::string name = readLine();
        std::cout << "Enter Dosage: ";
        std::string dosage = readLine();
        std::cout << "Enter Price: ";
        double price = readDouble();
        std::cout << "Enter Manufacturer (optional): ";
        std::string manufacturer = readLine();
        std::cout << "Enter Description (optional): ";
        std::string description = readLine();
        
        medicationService.addMedication(name, dosage, price, manufacturer, description);
    }
    
    void updateMedication() {
        std::cout << "Enter Medication ID to update: ";
        int id = readInt();
        std::cout << "Enter new Name: ";
        std::string name = readLine();
        std::cout << "Enter new Dosage: ";
        std::string dosage = readLine();
        std::cout << "Enter new Price: ";
        double price = readDouble();
        std::cout << "Enter new Manufacturer (optional): ";
        std::string manufacturer = readLine();
        std::cout << "Enter new Description (optional): ";
        std::string description = readLine();
        
        medicationService.updateMedication(id, name, dosage, price, manufacturer, description);
    }
    
    void removeMedication() {
        std::cout << "Enter Medication ID to remove: ";
        int id = readInt();
        medicationService.removeMedication(id);
    }
    
    void listAllMedications() {
        medicationService.listAllMedications();
    }
    
    // Prescription Management
    void createPrescription() {
        std::cout << "Enter Patient ID: ";
        int patientId = readInt();
        std::cout << "Enter Doctor ID: ";
        int doctorId = readInt();
        std::string date = getDateInput();
        
        std::vector<int> medicationIds;
        bool addMore = true;
        while (addMore) {
            std::cout << "Enter Medication ID: ";
            int medId = readInt();
            medicationIds.push_back(medId);
            
            std::cout << "Add another medication? (1: Yes, 0: No): ";
            int more = readInt();
            addMore = (more == 1);
        }
        
        std::cout << "Enter Instructions (optional): ";
        std::string instructions = readLine();
        
        prescriptionService.createPrescription(patientId, doctorId, date, medicationIds, instructions);
    }
    
    void updatePrescription() {
        std::cout << "Enter Prescription ID to update: ";
        int prescId = readInt();
        
        std::vector<int> medicationIds;
        bool addMore = true;
        while (addMore) {
            std::cout << "Enter Medication ID: ";
            int medId = readInt();
            medicationIds.push_back(medId);
            
            std::cout << "Add another medication? (1: Yes, 0: No): ";
            int more = readInt();
            addMore = (more == 1);
        }
        
        std::cout << "Enter Instructions (optional): ";
        std::string instructions = readLine();
        
        prescriptionService.updatePrescription(prescId, medicationIds, instructions);
    }
    
    void removePrescription() {
        std::cout << "Enter Prescription ID to remove: ";
        int id = readInt();
        prescriptionService.removePrescription(id);
    }
    
    void listPrescriptionsByPatient() {
        std::cout << "Enter Patient ID: ";
        int patientId = readInt();
        prescriptionService.listPrescriptionsByPatient(patientId);
    }
    
    // Billing Management
    void generateBill() {
        std::cout << "Enter Patient ID: ";
        int patientId = readInt();
        std::string date = getDateInput();
        std::cout << "Enter Consultation Fee: ";
        double consultationFee = readDouble();
        std::cout << "Enter Medication Charges: ";
        double medicationCharges = readDouble();
        std::cout << "Enter Other Charges: ";
        double otherCharges = readDouble();
        
        billingService.generateBill(patientId, date, consultationFee, medicationCharges, otherCharges);
    }
    
    void updateBillPaymentStatus() {
        std::cout << "Enter Bill ID: ";
        int billId = readInt();
        std::cout << "Enter new Payment Status (Paid, Pending, Overdue): ";
        std::string status = readLine();
        std::cout << "Enter Payment Method (Cash, Card, Insurance) (optional): ";
        std::string method = readLine();
        
        billingService.updateBillPaymentStatus(billId, status, method);
    }
    
    void listBillsByPatient() {
        std::cout << "Enter Patient ID: ";
        int patientId = readInt();
        billingService.listBillsByPatient(patientId);
    }
    
    void listBillsByPaymentStatus() {
        std::cout << "Enter Payment Status (Paid, Pending, Overdue): ";
        std::string status = readLine();
        billingService.listBillsByPaymentStatus(status);
    }
};

// ------------------------------
// Main Function
// ------------------------------

int main() {
    try {
        HospitalManagementApp app;
        app.run();
    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }
    return 0;
}
