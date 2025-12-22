#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// --------------------- VALIDATION HELPERS ---------------------
bool isValidEmail(string email) {
    for (int i = 0; i < email.length(); i++) {
        if (email[i] == '@') {
            return true;
        }
    }
    return false;
}

bool isValidCNIC(const string& cnic) {
    if (cnic.length() != 13) return false;
    for (char c : cnic) if (!isdigit(c)) return false;
    return true;
}

bool isValidPhone(const string& phone) {
    if (phone.length() != 11) return false;
    for (char c : phone) if (!isdigit(c)) return false;
    return true;
}

// --------------------- BASE USER ---------------------
class User {
protected:
    string name;
    string email;

public:
    User(string n = "", string e = "") : name(n), email(e) {}
    virtual void showMenu() {
        cout << "User Menu\n";
    }
};

// --------------------- ROOM CLASS ---------------------
class Room {
private:
    int roomID;
    string type;
    bool occupied;

public:
    Room() {
        roomID = 0;
        type = "Single";
        occupied = false;
    }

    Room(int id, string t) {
        roomID = id;
        type = t;
        occupied = false;
    }

    int getID() { return roomID; }
    bool isOccupied() { return occupied; }
    string getType() { return type; }

    void allocate() { occupied = true; }

    void showInfo() {
        cout << "Room " << roomID << " (" << type << ") - "
             << (occupied ? "Occupied" : "Vacant") << endl;
    }
};

// --------------------- STUDENT CLASS ---------------------
class Student : public User {
private:
    string fatherName;
    string cnic;
    string phone;
    string department;
    string address;
    int roomID;
    int feeDue;
    bool messEnrolled;

public:
    Student() {
        roomID = -1;
        feeDue = 50000;
        messEnrolled = false;
    }

    Student(string n, string fn, string e, string c, string p, string d, string a)
        : User(n, e), fatherName(fn), cnic(c), phone(p), department(d), address(a) {
        roomID = -1;
        feeDue = 50000;
        messEnrolled = false;
    }

    string getCNIC() { return cnic; }
    int getRoom() { return roomID; }

    void assignRoom(int r) { roomID = r; }

    void showDetails() {
        cout << "\n===== Student Profile =====\n";
        cout << "Name: " << name << endl;
        cout << "Father Name: " << fatherName << endl;
        cout << "Email: " << email << endl;
        cout << "CNIC: " << cnic << endl;
        cout << "Phone: " << phone << endl;
        cout << "Department: " << department << endl;
        cout << "Address: " << address << endl;
        cout << "Room: " << (roomID == -1 ? "Not Assigned" : to_string(roomID)) << endl;
        cout << "Fee Due: Rs. " << feeDue << endl;
        cout << "Mess: " << (messEnrolled ? "Enrolled" : "Not Enrolled") << endl;
    }

    void viewFee() {
        cout << "Fee Due: Rs. " << feeDue << endl;
    }

    void payFee(int amount) {
        try {
            if (amount <= 0)
                throw amount;
            if (amount > feeDue)
                cout << "Amount exceeds fee due!\n";
            else {
                feeDue -= amount;
                cout << "Payment Successful. Remaining Fee: " << feeDue << endl;
            }
        } catch (...) {
            cout << "Invalid Amount!\n";
        }
    }

    void enrollMess() {
        messEnrolled = true;
        cout << "Mess Enrollment Successful.\n";
    }

    void showMenu() override {
        cout << "\n--- Student Menu ---\n";
        cout << "1. Request Room (Warden Approval)\n";
        cout << "2. View Profile\n";
        cout << "3. View Fee\n";
        cout << "4. Pay Fee\n";
        cout << "5. Enroll Mess\n";
        cout << "6. Accountant\n";
        cout << "7. Admin\n";
        cout << "0. Exit\n";
    }

    friend class Accountant;
};

// --------------------- WARDEN CLASS (ENHANCED) ---------------------
class Warden : public User {
public:
    void approveRoomRequest(Student& s, Room rooms[], int roomCount) {
        cout << "\n--- Available Rooms ---\n";
        for (int i = 0; i < roomCount; i++) {
            if (!rooms[i].isOccupied())
                rooms[i].showInfo();
        }
        int reqRoom;
        cout << "\nStudent select room ID from above list: ";
        cin >> reqRoom;
        char decision;
        cout << "Warden Approval (Y/N): ";
        cin >> decision;
        if (decision == 'Y' || decision == 'y') {
            for (int i = 0; i < roomCount; i++) {
                if (rooms[i].getID() == reqRoom && !rooms[i].isOccupied()) {
                    rooms[i].allocate();
                    s.assignRoom(reqRoom);
                    cout << "Room Approved & Allocated Successfully!\n";
                    return;
                }
            }
            cout << "Invalid Room Selection or Room Already Occupied.\n";
        } else {
            cout << "Warden Rejected the Room Request.\n";
        }
    }
    
    void checkRoom(Student& s) {
        cout << "Warden checking room of student.\n";
        cout << "Room No: " << s.getRoom() << endl;
    }
};

// --------------------- OTHER MODULES ---------------------
class MessManager : public User {
public:
    void showMessMenu() {
        cout << "Mess Menu: Rice, Chicken Karhi, Daal,  Roti\n";
    }
};

class Accountant {
public:
    void viewPayment(Student& s) {
        cout << "...Accountant View... " << endl;
        cout << " Fee Due: Rs. " << endl;
        s.viewFee();
    }
};

class Admin {
public:
    void showSummary(Student& s) {
        cout << "\n--- Admin Summary ---\n";
        s.showDetails();
    }
};

// --------------------- REGISTRATION ---------------------
Student registerStudent() {
    string name, father, email, cnic, phone, dept, address;

    cout << "\n--- Student Registration ---\n";

    do { cout << "Name: "; cin >> ws; 
        getline(cin, name); } while (name.empty());
    do { cout << "Father Name: "; 
        getline(cin, father); } while (father.empty());
    do { cout << "Email: ";
         getline(cin, email); } while (!isValidEmail(email));
    do { cout << "CNIC (13 digits): ";
         getline(cin, cnic); } while (!isValidCNIC(cnic));
    do { cout << "Phone (11 digits): ";
         getline(cin, phone); } while (!isValidPhone(phone));
    do { cout << "Department: "; 
        getline(cin, dept); } while (dept.empty());
    do { cout << "Address: ";
         getline(cin, address); } while (address.empty());

    cout << "Registration Successful!\n";
    return Student(name, father, email, cnic, phone, dept, address);
}

// --------------------- LOGIN ---------------------
Student* loginStudent(Student stuArr[], int& count) {
    string input;
    cout << "Enter CNIC: ";
    cin >> input;

    for (int i = 0; i < count; i++) {
        if (stuArr[i].getCNIC() == input) {
            cout << "Login Successful.\n";
            return &stuArr[i];
        }
    }

    cout << "Student Not Found. Registering...\n";
    stuArr[count] = registerStudent();
    count++;
    return &stuArr[count - 1];
}

// --------------------- MAIN ---------------------
int main() {
    Student students[5];
    int studentCount = 2;

    students[0] = Student("Alishba", "Azhar", "alih@uos.edu.pk",
                          "3840328018404", "03124353068", "CS", "Sargodha");

    students[1] = Student("Ayesha", "Ahmed", "ayesha@uos.edu.pk",
                          "9876543210987", "03111234567", "IT", "Lahore");

    Room rooms[5] = {
        Room(101, "Single"),
        Room(102, "Double"),
        Room(103, "Single"),
        Room(104, "Double"),
        Room(105, "Single")
    };

    Warden warden;
    Accountant accountant;
    Admin admin;

    cout << "===== HOSTEL MANAGEMENT SYSTEM =====\n";

    Student* stu = loginStudent(students, studentCount);

    int choice;
    do {
        stu->showMenu();
        cout << "Choose Option: ";
        cin >> choice;

        if (choice == 1) warden.approveRoomRequest(*stu, rooms, 5);
        else if (choice == 2) stu->showDetails();
        else if (choice == 3) stu->viewFee();
        else if (choice == 4) {
            int amt;
            cout << "Enter Amount: ";
            cin >> amt;
            stu->payFee(amt);
        }
        else if (choice == 5) stu->enrollMess();
        else if (choice == 6) accountant.viewPayment(*stu);
        else if (choice == 7) admin.showSummary(*stu);

    } while (choice != 0);

    cout << "\n...........Thank you 😊 for using the Hostel Management System!............\n";
    return 0;
}