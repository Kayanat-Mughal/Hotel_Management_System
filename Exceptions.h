#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <iostream>
#include <ctime>
#include <cstring>

using namespace std;

// Base Hotel Exception
class HotelException : public exception {
protected:
    string message;
    string errorCode;
    string timestamp;
    
public:
    HotelException(const string& msg, const string& code = "HOTEL-000")
        : message(msg), errorCode(code) {
        time_t now = time(nullptr);
        char buffer[80];
        struct tm timeinfo;
        localtime_s(&timeinfo, &now);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
        timestamp = buffer;
    }
    
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
    
    string getErrorCode() const { return errorCode; }
    string getTimestamp() const { return timestamp; }
    string getFullMessage() const {
        return "[" + errorCode + "] " + timestamp + ": " + message;
    }
    
    virtual ~HotelException() = default;
};

// File-related Exceptions
class FileException : public HotelException {
public:
    FileException(const string& msg)
        : HotelException("File Error: " + msg, "FILE-001") {}
};

class FileNotFoundException : public FileException {
public:
    FileNotFoundException(const string& filename)
        : FileException("File not found: " + filename) {}
};

class FileReadException : public FileException {
public:
    FileReadException(const string& filename, const string& details = "")
        : FileException("Read operation failed on '" + filename + "'" + 
                       (details.empty() ? "" : " (" + details + ")")) {}
};

class FileWriteException : public FileException {
public:
    FileWriteException(const string& filename, const string& details = "")
        : FileException("Write operation failed on '" + filename + "'" + 
                       (details.empty() ? "" : " (" + details + ")")) {}
};

class FileCorruptedException : public FileException {
public:
    FileCorruptedException(const string& filename, const string& details = "")
        : FileException("File corrupted: '" + filename + "'" + 
                       (details.empty() ? "" : " (" + details + ")")) {}
};

// Database Exceptions
class DatabaseException : public HotelException {
public:
    DatabaseException(const string& operation, const string& details = "")
        : HotelException("Database operation failed: " + operation + 
                        (details.empty() ? "" : " (" + details + ")"), "DB-001") {}
};

class RecordNotFoundException : public DatabaseException {
public:
    RecordNotFoundException(const string& recordType, int recordId)
        : DatabaseException("Record not found", 
                          recordType + " with ID " + to_string(recordId) + " not found") {}
};

class DuplicateRecordException : public DatabaseException {
public:
    DuplicateRecordException(const string& recordType, const string& identifier)
        : DatabaseException("Duplicate record", 
                          recordType + " with identifier '" + identifier + "' already exists") {}
};

// Validation Exceptions
class ValidationException : public HotelException {
public:
    ValidationException(const string& field, const string& rule, const string& value = "")
        : HotelException("Validation failed for field '" + field + "': " + rule + 
                        (value.empty() ? "" : " (value: '" + value + "')"), "VAL-001") {}
    
    // Overloaded constructor for simple message
    explicit ValidationException(const string& msg)
        : HotelException(msg, "VAL-001") {}
};

class InvalidDateException : public ValidationException {
public:
    InvalidDateException(const string& dateStr, const string& expectedFormat)
        : ValidationException("Date", "Invalid format or value", 
                            "'" + dateStr + "' (expected format: " + expectedFormat + ")") {}
};

class InvalidEmailException : public ValidationException {
public:
    InvalidEmailException(const string& email)
        : ValidationException("Email", "Invalid email format", email) {}
};

class InvalidPhoneException : public ValidationException {
public:
    InvalidPhoneException(const string& phone)
        : ValidationException("Phone", "Invalid phone number format", phone) {}
};

class InvalidPriceException : public ValidationException {
public:
    InvalidPriceException(double price, double min, double max)
        : ValidationException("Price", "Price must be between " + to_string(min) + 
                            " and " + to_string(max), to_string(price)) {}
};

class InvalidCapacityException : public ValidationException {
public:
    InvalidCapacityException(int capacity, int min, int max)
        : ValidationException("Capacity", "Capacity must be between " + to_string(min) + 
                            " and " + to_string(max), to_string(capacity)) {}
};

// Room-related Exceptions
class RoomException : public HotelException {
public:
    RoomException(const string& operation, const string& details = "")
        : HotelException("Room operation failed: " + operation + 
                        (details.empty() ? "" : " (" + details + ")"), "ROOM-001") {}
};

class RoomNotAvailableException : public RoomException {
public:
    RoomNotAvailableException(int roomNumber, const string& status)
        : RoomException("Room not available", 
                       "Room #" + to_string(roomNumber) + " is currently " + status) {}
};

class RoomCapacityExceededException : public RoomException {
public:
    RoomCapacityExceededException(int roomNumber, int capacity, int requested)
        : RoomException("Capacity exceeded", 
                       "Room #" + to_string(roomNumber) + " can accommodate " + 
                       to_string(capacity) + " guests (requested: " + 
                       to_string(requested) + ")") {}
};

class RoomNotFoundException : public RoomException {
public:
    RoomNotFoundException(int roomNumber)
        : RoomException("Room not found", 
                       "Room #" + to_string(roomNumber) + " does not exist") {}
};

// Reservation Exceptions
class ReservationException : public HotelException {
public:
    ReservationException(const string& operation, const string& details = "")
        : HotelException("Reservation operation failed: " + operation + 
                        (details.empty() ? "" : " (" + details + ")"), "RES-001") {}
};

class ReservationDateException : public ReservationException {
public:
    ReservationDateException(const string& checkIn, const string& checkOut)
        : ReservationException("Invalid dates", 
                             "Check-out (" + checkOut + ") must be after check-in (" + checkIn + ")") {}
};

class ReservationConflictException : public ReservationException {
public:
    ReservationConflictException(int roomNumber, const string& dates)
        : ReservationException("Reservation conflict", 
                             "Room #" + to_string(roomNumber) + 
                             " is already booked for: " + dates) {}
};

// Customer Exceptions
class CustomerException : public HotelException {
public:
    CustomerException(const string& operation, const string& details = "")
        : HotelException("Customer operation failed: " + operation + 
                        (details.empty() ? "" : " (" + details + ")"), "CUST-001") {}
};

class CustomerNotFoundException : public CustomerException {
public:
    CustomerNotFoundException(int customerId)
        : CustomerException("Customer not found", 
                          "Customer with ID " + to_string(customerId) + " not found") {}
};

// Employee Exceptions
class EmployeeException : public HotelException {
public:
    EmployeeException(const string& operation, const string& details = "")
        : HotelException("Employee operation failed: " + operation + 
                        (details.empty() ? "" : " (" + details + ")"), "EMP-001") {}
};

class AuthenticationException : public EmployeeException {
public:
    AuthenticationException(const string& username, const string& reason = "Invalid credentials")
        : EmployeeException("Authentication failed", 
                          "Login failed for '" + username + "': " + reason) {}
};

class AuthorizationException : public EmployeeException {
public:
    AuthorizationException(const string& operation, const string& requiredRole)
        : EmployeeException("Authorization denied", 
                          "Operation '" + operation + "' requires role: " + requiredRole) {}
};

class EmployeeNotFoundException : public EmployeeException {
public:
    EmployeeNotFoundException(int employeeId)
        : EmployeeException("Employee not found", 
                          "Employee with ID " + to_string(employeeId) + " not found") {}
};

// Billing Exceptions
class BillingException : public HotelException {
public:
    BillingException(const string& operation, const string& details = "")
        : HotelException("Billing operation failed: " + operation + 
                        (details.empty() ? "" : " (" + details + ")"), "BILL-001") {}
};

class PaymentException : public BillingException {
public:
    PaymentException(const string& method, double amount, const string& reason)
        : BillingException("Payment failed", 
                          method + " payment of " + to_string(amount) + 
                          " failed: " + reason) {}
};

class InsufficientPaymentException : public BillingException {
public:
    InsufficientPaymentException(double paid, double required)
        : BillingException("Insufficient payment", 
                          "Paid: " + to_string(paid) + 
                          ", Required: " + to_string(required)) {}
};

class BillAlreadyPaidException : public BillingException {
public:
    BillAlreadyPaidException(int billId)
        : BillingException("Bill already paid", 
                          "Bill #" + to_string(billId) + " is already paid") {}
};

// System Exceptions
class SystemException : public HotelException {
public:
    SystemException(const string& component, const string& details = "")
        : HotelException("System error in " + component + 
                        (details.empty() ? "" : ": " + details), "SYS-001") {}
};

class ConfigurationException : public SystemException {
public:
    ConfigurationException(const string& configItem, const string& details = "")
        : SystemException("Configuration", 
                         "Error in '" + configItem + "' configuration: " + details) {}
};

class BackupException : public SystemException {
public:
    BackupException(const string& operation, const string& details = "")
        : SystemException("Backup", 
                         operation + " failed: " + details) {}
};

class ReportGenerationException : public SystemException {
public:
    ReportGenerationException(const string& reportType, const string& details = "")
        : SystemException("Report Generation", 
                         "Failed to generate '" + reportType + "' report: " + details) {}
};

class LoggingException : public SystemException {
public:
    LoggingException(const string& operation, const string& details = "")
        : SystemException("Logging", 
                         operation + " failed: " + details) {}
};

// Utility function to handle exceptions
class ExceptionHandler {
public:
    static void handle(const HotelException& e, bool rethrow = false) {
        logException(e);
        displayErrorMessage(e);
        if (rethrow) {
            throw;
        }
    }
    
    static void logException(const HotelException& e) {
        cerr << "\n[EXCEPTION LOGGED] " << e.getFullMessage() << endl;
    }
    
    static void displayErrorMessage(const HotelException& e) {
        cout << "\nERROR: " << e.what() << endl;
        cout << "   Error Code: " << e.getErrorCode() << endl;
    }
    
    static void displayWarning(const string& warning) {
        cout << "\nWARNING: " << warning << endl;
    }
    
    static void displayInfo(const string& info) {
        cout << "\nINFO: " << info << endl;
    }
};

#endif // EXCEPTIONS_H