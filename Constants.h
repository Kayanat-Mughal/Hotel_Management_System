#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace HotelConstants {
    // Hotel Information
    const std::string HOTEL_NAME = "Grand Luxury Hotel & Resorts";
    const std::string HOTEL_ADDRESS = "123 Luxury Street, Manhattan, New York, NY 10001";
    const std::string HOTEL_CONTACT = "+1 (555) 123-4567";
    const std::string HOTEL_EMAIL = "reservations@grandluxury.com";
    const std::string HOTEL_WEBSITE = "www.grandluxuryhotel.com";
    
    // Room Constants
    const double STANDARD_MIN_PRICE = 100.0;
    const double STANDARD_MAX_PRICE = 200.0;
    const double DELUXE_MIN_PRICE = 200.0;
    const double DELUXE_MAX_PRICE = 350.0;
    const double SUITE_MIN_PRICE = 350.0;
    const double SUITE_MAX_PRICE = 600.0;
    const double PRESIDENTIAL_MIN_PRICE = 800.0;
    const double PRESIDENTIAL_MAX_PRICE = 2000.0;
    
    const int STANDARD_CAPACITY = 2;
    const int DELUXE_CAPACITY = 3;
    const int SUITE_CAPACITY = 4;
    const int PRESIDENTIAL_CAPACITY = 6;
    
    // Pricing Constants
    const double TAX_RATE = 0.10;          // 10% tax
    const double SERVICE_CHARGE = 0.05;    // 5% service charge
    const double EARLY_CHECKIN_FEE = 50.0;
    const double LATE_CHECKOUT_FEE = 75.0;
    const double EXTRA_BED_CHARGE = 30.0;
    const double ROOM_SERVICE_FEE = 15.0;
    
    // Time Constants
    const std::string DATE_FORMAT = "%Y-%m-%d";
    const std::string TIME_FORMAT = "%H:%M:%S";
    const std::string DATETIME_FORMAT = "%Y-%m-%d %H:%M:%S";
    
    const int CHECKIN_TIME = 14;    // 2:00 PM
    const int CHECKOUT_TIME = 12;   // 12:00 PM
    const int MAX_STAY_DAYS = 30;
    const int MIN_ADVANCE_BOOKING = 0;     // 0 days (same day allowed)
    const int MAX_ADVANCE_BOOKING = 365;   // 1 year
    
    // Business Rules
    const int MIN_AGE_FOR_BOOKING = 18;
    const int MAX_GUESTS_PER_ROOM = 6;
    const int CANCELLATION_HOURS = 24;     // Free cancellation before 24 hours
    const double CANCELLATION_FEE_RATE = 0.20;  // 20% cancellation fee
    
    // Discount Rates
    const double WEEKLY_DISCOUNT = 0.10;   // 10% off for 7+ days
    const double MONTHLY_DISCOUNT = 0.20;  // 20% off for 30+ days
    const double CORPORATE_DISCOUNT = 0.15; // 15% corporate discount
    const double LOYALTY_DISCOUNT = 0.05;  // 5% loyalty discount
    
    // Payment Constants
    const std::string CURRENCY = "USD";
    const std::string CURRENCY_SYMBOL = "$";
    const double MIN_DEPOSIT_RATE = 0.25;  // 25% deposit required
    const std::string PAYMENT_METHODS[] = {
        "Cash", "Credit Card", "Debit Card", 
        "Bank Transfer", "Online Payment", "Hotel Credit"
    };
    
    // File Paths
    const std::string DATA_DIR = "data/";
    const std::string BACKUP_DIR = "backup/";
    const std::string REPORT_DIR = "reports/";
    const std::string LOG_DIR = "logs/";
    const std::string CONFIG_FILE = "config/hotel.cfg";
    
    // File Names
    const std::string ROOMS_FILE = "rooms.dat";
    const std::string CUSTOMERS_FILE = "customers.dat";
    const std::string RESERVATIONS_FILE = "reservations.dat";
    const std::string EMPLOYEES_FILE = "employees.dat";
    const std::string BILLS_FILE = "bills.dat";
    const std::string LOG_FILE = "hotel.log";
    const std::string AUDIT_FILE = "audit.log";
    
    // Log Levels
    const std::string LOG_INFO = "INFO";
    const std::string LOG_WARNING = "WARNING";
    const std::string LOG_ERROR = "ERROR";
    const std::string LOG_DEBUG = "DEBUG";
    const std::string LOG_AUDIT = "AUDIT";
    
    // User Roles
    enum class UserRole {
        ADMIN = 1,
        MANAGER = 2,
        RECEPTIONIST = 3,
        HOUSEKEEPING = 4,
        ACCOUNTS = 5,
        CUSTOMER = 6
    };
    
    // Room Amenities
    const std::string AMENITIES[] = {
        "WiFi", "TV", "AC", "Mini-fridge", "Mini-bar", 
        "Coffee Maker", "Safe", "Iron", "Hair Dryer",
        "Jacuzzi", "Balcony", "Sea View", "City View",
        "Kitchenette", "Living Room", "Dining Area",
        "Work Desk", "Sofa", "Bathtub", "Shower"
    };
    
    // Food & Beverage Items
    const std::string FOOD_ITEMS[] = {
        "Break Buffet:25.00",
        "Lunch Buffet:35.00", 
        "Dinner Buffet:45.00",
        "Room Service Burger:18.00",
        "Room Service Pizza:22.00",
        "Room Service Pasta:20.00",
        "Soft Drink:3.00",
        "Mineral Water:2.00",
        "Coffee:4.00",
        "Tea:3.00"
    };
    
    // Additional Services
    const std::string SERVICES[] = {
        "Laundry:25.00",
        "Dry Cleaning:35.00", 
        "Airport Pickup:60.00",
        "Car Rental:80.00",
        "Spa Treatment:100.00",
        "Gym Access:20.00",
        "Swimming Pool:15.00",
        "Business Center:10.00",
        "Conference Room:150.00",
        "Parking:20.00"
    };
    
    // Validation Constants
    const int MIN_PASSWORD_LENGTH = 6;
    const int MAX_NAME_LENGTH = 50;
    const int MAX_ADDRESS_LENGTH = 200;
    const int MAX_EMAIL_LENGTH = 100;
    const int MAX_PHONE_LENGTH = 20;
    
    // System Constants
    const int AUTO_BACKUP_HOURS = 24;
    const int AUTO_SAVE_MINUTES = 5;
    const int SESSION_TIMEOUT_MINUTES = 30;
    const int MAX_LOGIN_ATTEMPTS = 3;
    
    // Report Constants
    const std::string REPORT_TYPES[] = {
        "Daily Revenue",
        "Monthly Occupancy", 
        "Customer Statistics",
        "Employee Performance",
        "Room Utilization",
        "Financial Summary",
        "Tax Report",
        "Inventory Report"
    };
}

// Conversion Functions
namespace Convert {
    inline std::string roleToString(HotelConstants::UserRole role) {
        switch(role) {
            case HotelConstants::UserRole::ADMIN: return "Administrator";
            case HotelConstants::UserRole::MANAGER: return "Manager";
            case HotelConstants::UserRole::RECEPTIONIST: return "Receptionist";
            case HotelConstants::UserRole::HOUSEKEEPING: return "Housekeeping";
            case HotelConstants::UserRole::ACCOUNTS: return "Accounts";
            case HotelConstants::UserRole::CUSTOMER: return "Customer";
            default: return "Unknown";
        }
    }
    
    inline HotelConstants::UserRole stringToRole(const std::string& roleStr) {
        if (roleStr == "Administrator") return HotelConstants::UserRole::ADMIN;
        if (roleStr == "Manager") return HotelConstants::UserRole::MANAGER;
        if (roleStr == "Receptionist") return HotelConstants::UserRole::RECEPTIONIST;
        if (roleStr == "Housekeeping") return HotelConstants::UserRole::HOUSEKEEPING;
        if (roleStr == "Accounts") return HotelConstants::UserRole::ACCOUNTS;
        return HotelConstants::UserRole::CUSTOMER;
    }
}

#endif // CONSTANTS_H