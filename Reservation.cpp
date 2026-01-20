#include "Reservation.h"

// ==================== CONSTRUCTORS ====================
Reservation::Reservation() : reservationId(0), customerId(0), roomNumber(0),
                            checkInDate(0), checkOutDate(0), numberOfGuests(1),
                            roomRate(0.0), totalAmount(0.0), paidAmount(0.0),
                            status(CONFIRMED), paymentStatus(PENDING),
                            bookingDate(time(nullptr)) {}

Reservation::Reservation(int resId, int custId, int roomNo, time_t checkIn, 
                        time_t checkOut, int guests, double rate)
    : reservationId(resId), customerId(custId), roomNumber(roomNo),
      checkInDate(checkIn), checkOutDate(checkOut), numberOfGuests(guests),
      roomRate(rate), status(CONFIRMED), paymentStatus(PENDING),
      bookingDate(time(nullptr)) {
    
    if (resId <= 0) throw ValidationException("Reservation ID must be positive");
    if (custId <= 0) throw ValidationException("Customer ID must be positive");
    if (roomNo <= 0) throw ValidationException("Room number must be positive");
    if (checkIn >= checkOut) throw ValidationException("Check-out must be after check-in");
    if (guests <= 0) throw ValidationException("Number of guests must be positive");
    if (rate <= 0) throw ValidationException("Room rate must be positive");
    
    totalAmount = calculateTotal();
    paidAmount = 0.0;
}

// ==================== GETTERS ====================
int Reservation::getReservationId() const { return reservationId; }
int Reservation::getCustomerId() const { return customerId; }
int Reservation::getRoomNumber() const { return roomNumber; }

string Reservation::getCheckInDate() const {
    return Utils::formatDate(checkInDate);
}

string Reservation::getCheckOutDate() const {
    return Utils::formatDate(checkOutDate);
}

time_t Reservation::getRawCheckIn() const { return checkInDate; }
time_t Reservation::getRawCheckOut() const { return checkOutDate; }
int Reservation::getNumberOfGuests() const { return numberOfGuests; }
double Reservation::getRoomRate() const { return roomRate; }
double Reservation::getTotalAmount() const { return totalAmount; }
double Reservation::getPaidAmount() const { return paidAmount; }

double Reservation::getDueAmount() const {
    return totalAmount - paidAmount;
}

ReservationStatus Reservation::getStatus() const { return status; }
PaymentStatus Reservation::getPaymentStatus() const { return paymentStatus; }
string Reservation::getSpecialRequests() const { return specialRequests; }

string Reservation::getBookingDate() const {
    return Utils::formatDate(bookingDate);
}

// ==================== STATUS STRINGS ====================
string Reservation::getStatusString() const {
    switch(status) {
        case CONFIRMED: return "Confirmed";
        case CHECKED_IN: return "Checked In";
        case CHECKED_OUT: return "Checked Out";
        case CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

string Reservation::getPaymentStatusString() const {
    switch(paymentStatus) {
        case PENDING: return "Pending";
        case PAID: return "Paid";
        case PARTIAL: return "Partial";
        default: return "Unknown";
    }
}

// ==================== SETTERS ====================
void Reservation::setDates(time_t newCheckIn, time_t newCheckOut) {
    if (newCheckIn >= newCheckOut) throw ValidationException("Check-out must be after check-in");
    checkInDate = newCheckIn;
    checkOutDate = newCheckOut;
    totalAmount = calculateTotal();
}

void Reservation::setGuests(int guests) {
    if (guests <= 0) throw ValidationException("Number of guests must be positive");
    numberOfGuests = guests;
}

void Reservation::setRoomRate(double rate) {
    if (rate <= 0) throw ValidationException("Room rate must be positive");
    roomRate = rate;
    totalAmount = calculateTotal();
}

void Reservation::setSpecialRequests(const string& requests) {
    specialRequests = requests;
}

// ==================== OPERATIONS ====================
bool Reservation::checkIn() {
    if (status != CONFIRMED) return false;
    status = CHECKED_IN;
    return true;
}

bool Reservation::checkOut() {
    if (status != CHECKED_IN) return false;
    status = CHECKED_OUT;
    return true;
}

bool Reservation::cancel() {
    if (status == CHECKED_IN || status == CHECKED_OUT) return false;
    status = CANCELLED;
    return true;
}

bool Reservation::makePayment(double amount) {
    if (amount <= 0) throw ValidationException("Payment amount must be positive");
    if (amount > getDueAmount()) throw ValidationException("Payment exceeds due amount");
    
    paidAmount += amount;
    
    if (paidAmount >= totalAmount) {
        paymentStatus = PAID;
    } else if (paidAmount > 0) {
        paymentStatus = PARTIAL;
    }
    
    return true;
}

bool Reservation::isActive() const {
    time_t now = time(nullptr);
    return (status == CONFIRMED || status == CHECKED_IN) &&
           now >= checkInDate && now <= checkOutDate;
}

bool Reservation::isPast() const {
    time_t now = time(nullptr);
    return now > checkOutDate;
}

int Reservation::calculateNights() const {
    return Utils::daysBetween(checkInDate, checkOutDate);
}

double Reservation::calculateTotal() const {
    return roomRate * calculateNights();
}

// ==================== DISPLAY ====================
void Reservation::display() const {
    cout << "\n";
    cout << "┌─────────────────────────────────────────────┐\n";
    cout << "│           RESERVATION DETAILS               │\n";
    cout << "├─────────────────────────────────────────────┤\n";
    cout << "│ Reservation ID: " << setw(25) << reservationId << " │\n";
    cout << "│ Customer ID:    " << setw(25) << customerId << " │\n";
    cout << "│ Room Number:    " << setw(25) << roomNumber << " │\n";
    cout << "│ Check-in:       " << setw(25) << getCheckInDate() << " │\n";
    cout << "│ Check-out:      " << setw(25) << getCheckOutDate() << " │\n";
    cout << "│ Nights:         " << setw(25) << calculateNights() << " │\n";
    cout << "│ Guests:         " << setw(25) << numberOfGuests << " │\n";
    cout << "│ Room Rate:      " << setw(25) << Utils::formatCurrency(roomRate) << " │\n";
    cout << "│ Total Amount:   " << setw(25) << Utils::formatCurrency(totalAmount) << " │\n";
    cout << "│ Paid Amount:    " << setw(25) << Utils::formatCurrency(paidAmount) << " │\n";
    cout << "│ Due Amount:     " << setw(25) << Utils::formatCurrency(getDueAmount()) << " │\n";
    cout << "│ Status:         " << setw(25) << getStatusString() << " │\n";
    cout << "│ Payment Status: " << setw(25) << getPaymentStatusString() << " │\n";
    if (!specialRequests.empty()) {
        cout << "│ Special Requests:" << setw(25) << specialRequests << " │\n";
    }
    cout << "│ Booked on:      " << setw(25) << getBookingDate() << " │\n";
    cout << "└─────────────────────────────────────────────┘\n";
}

void Reservation::displayBrief() const {
    cout << "Res ID: " << setw(6) << reservationId
         << " | Cust: " << setw(6) << customerId
         << " | Room: " << setw(4) << roomNumber
         << " | " << getCheckInDate() << " to " << getCheckOutDate()
         << " | Status: " << setw(10) << left << getStatusString()
         << " | Amount: " << Utils::formatCurrency(totalAmount) << "\n";
}

// ==================== FILE OPERATIONS ====================
void Reservation::saveToFile(ofstream& file) const {
    if (!file.is_open()) throw FileException("File is not open for writing");
    
    file << reservationId << " "
         << customerId << " "
         << roomNumber << " "
         << checkInDate << " "
         << checkOutDate << " "
         << numberOfGuests << " "
         << roomRate << " "
         << totalAmount << " "
         << paidAmount << " "
         << static_cast<int>(status) << " "
         << static_cast<int>(paymentStatus) << " "
         << specialRequests << " "
         << bookingDate << "\n";
}

void Reservation::loadFromFile(ifstream& file) {
    if (!file.is_open()) throw FileException("File is not open for reading");
    
    int statusInt, paymentStatusInt;
    file >> reservationId >> customerId >> roomNumber 
         >> checkInDate >> checkOutDate >> numberOfGuests
         >> roomRate >> totalAmount >> paidAmount
         >> statusInt >> paymentStatusInt;
    
    file.ignore();
    getline(file, specialRequests, '|');
    file >> bookingDate;
    
    status = static_cast<ReservationStatus>(statusInt);
    paymentStatus = static_cast<PaymentStatus>(paymentStatusInt);
}

// ==================== VALIDATION ====================
bool Reservation::isValid() const {
    return reservationId > 0 && customerId > 0 && roomNumber > 0 &&
           checkInDate < checkOutDate && numberOfGuests > 0 &&
           roomRate > 0 && totalAmount > 0;
}