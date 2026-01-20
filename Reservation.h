#ifndef RESERVATION_H
#define RESERVATION_H

#include "Utils.h"
#include "Room.h"
#include "Customer.h"
#include <vector>

class Reservation {
private:
    int reservationId;
    int customerId;
    int roomNumber;
    time_t checkInDate;
    time_t checkOutDate;
    int numberOfGuests;
    double roomRate;
    double totalAmount;
    double paidAmount;
    ReservationStatus status;
    PaymentStatus paymentStatus;
    string specialRequests;
    time_t bookingDate;
    
public:
    // Constructors
    Reservation();
    Reservation(int resId, int custId, int roomNo, time_t checkIn, 
                time_t checkOut, int guests, double rate);
    
    // Getters
    int getReservationId() const;
    int getCustomerId() const;
    int getRoomNumber() const;
    string getCheckInDate() const;
    string getCheckOutDate() const;
    time_t getRawCheckIn() const;
    time_t getRawCheckOut() const;
    int getNumberOfGuests() const;
    double getRoomRate() const;
    double getTotalAmount() const;
    double getPaidAmount() const;
    double getDueAmount() const;
    ReservationStatus getStatus() const;
    PaymentStatus getPaymentStatus() const;
    string getSpecialRequests() const;
    string getBookingDate() const;
    
    // Status strings
    string getStatusString() const;
    string getPaymentStatusString() const;
    
    // Setters
    void setDates(time_t newCheckIn, time_t newCheckOut);
    void setGuests(int guests);
    void setRoomRate(double rate);
    void setSpecialRequests(const string& requests);
    
    // Operations
    bool checkIn();
    bool checkOut();
    bool cancel();
    bool makePayment(double amount);
    bool isActive() const;
    bool isPast() const;
    int calculateNights() const;
    double calculateTotal() const;
    
    // Display
    void display() const;
    void displayBrief() const;
    
    // File operations
    void saveToFile(ofstream& file) const;
    void loadFromFile(ifstream& file);
    
    // Validation
    bool isValid() const;
};

#endif // RESERVATION_H