# 🏥 Super Awesome Hospital Management System 🏥

> "Where C++ meets healthcare, and nobody gets a segmentation fault!"

## 🚀 What's This All About?

Welcome to the most exciting hospital management system you've ever seen! (Okay, maybe the bar for "exciting hospital software" isn't very high, but we tried our best!)

This C++ application helps healthcare facilities manage their day-to-day operations without pulling their hair out. It's like having a tiny digital army of administrative assistants who never need coffee breaks!

## ✨ Features That Will Make You Say "Wow!"

### 👨‍⚕️ Doctor Management
* Add, update, and track doctors faster than they can say "that's not covered by insurance"
* Search by specialization (yes, we know what a "pulmonologist" is!)
* Track doctor availability without playing phone tag

### 🤒 Patient Management  
* Store patient details more securely than they guard the hospital cafeteria's cookie recipe
* Find patients by disease or age range
* Track medication histories that are actually readable (unlike doctor handwriting)

### 📅 Appointment Scheduling
* Book appointments with fancy time slots
* No more double-bookings (we promise!)
* Track appointment statuses - from "scheduled" to "completed" to "ran away screaming"

### 💊 Medication & Prescriptions
* Manage medications without the paper cuts
* Create and track prescriptions
* Everything is digital - no more deciphering if that's a "5mg" or "50mg"

### 💰 Billing System
* Generate bills that make sense (a revolutionary concept in healthcare!)
* Track payment statuses
* Calculate revenue without needing an accounting degree

### 👮 User Authentication
* Role-based access control (because not everyone should be able to diagnose patients from the accounting department)
* Secure login system (no, the password isn't "password123")

### 📝 Logging & Reporting
* Comprehensive logging system that catches everything except your coffee spills
* Financial reporting that will make your accountant smile

## 🔧 Installation

Getting started is easier than finding the hospital cafeteria!

```bash
# Clone the repository
git clone https://github.com/kalviumcommunity/S51_Harish_OOP_Hospital-Management-System.git

# Navigate to the directory
cd hospital-management-system

# Compile with your favorite C++ compiler
g++ -std=c++14 main.cpp -o hospital_system

# Run the application
./hospital_system
```

## 🎮 How to Use

1. Launch the application
2. Login with one of these test accounts:
   - Username: `admin`, Password: `admin123` (Admin access)
   - Username: `doctor`, Password: `doctor123` (Doctor access)
   - Username: `reception`, Password: `reception123` (Reception access)
3. Navigate the incredibly intuitive menu (we spent minutes designing it!)
4. Manage all your hospital needs without the papercuts

## 🧱 SOLID Principles - Because We're Fancy Like That

Our code follows SOLID principles so well, it could teach a masterclass:

### Single Responsibility Principle (SRP)
Each class has exactly one job, like that one friend who only brings dessert to potlucks but does it perfectly every time.
```cpp
// ILogger handles logging - that's it!
class ILogger {
public:
    virtual void logInfo(const std::string &message) = 0;
    virtual void logError(const std::string &message) = 0;
    // ...
};
```

### Open/Closed Principle (OCP)
Our code is open for extension but closed for modification - like a good pizza recipe.
```cpp
// Want a new repository? Just extend IRepository!
class CloudPatientRepository : public IPatientRepository {
    // Add cloud storage without changing existing code
};
```

### Liskov Substitution Principle (LSP)
Our subclasses can be used anywhere their parent classes are used. They're well-behaved that way.
```cpp
// Any IDisplayManager can be used where a display is needed
void showError(std::shared_ptr<IDisplayManager> display) {
    display->displayError("Oops!");  // Works with any implementation
}
```

### Interface Segregation Principle (ISP)
Our interfaces are so specific, they could win a laser-focus competition.
```cpp
// Different repository interfaces for different needs
class IPatientRepository : public IRepository<Patient> {
    virtual std::vector<Patient> findByDisease(const std::string &disease) const = 0;
    // Only patient-specific methods here!
};
```

### Dependency Inversion Principle (DIP)
High-level modules depend on abstractions, not details - like how you don't need to know how your coffee machine works internally.
```cpp
// Services depend on repository interfaces, not implementations
class PatientService {
private:
    std::shared_ptr<IPatientRepository> patientRepo;  // Interface, not concrete class
    // ...
};
```

## 🤝 Want to Contribute?

Found a bug? Want to add more features? Think our jokes aren't funny enough?

1. Fork the repository
2. Create your feature branch: `git checkout -b feature/AmazingFeature`
3. Commit your changes: `git commit -m 'Add some AmazingFeature'`
4. Push to the branch: `git push origin feature/AmazingFeature`
5. Open a Pull Request
6. Wait patiently while eating hospital cafeteria jello

## 📜 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🙏 Acknowledgements

* Coffee ☕ - The real hero behind this project
* Stack Overflow - For the collective wisdom of frustrated programmers everywhere
* You - For reading this far into the README. You deserve a cookie! 🍪

---

Made with ❤️ and excessive amounts of debugging
