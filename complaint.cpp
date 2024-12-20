#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

// Function to handle SQLite errors
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << " = " << argv[i] << endl;
    }
    return 0;
}

// Function to insert a complaint into the database
void registerComplaint(sqlite3 *db) {
    string name, email, complaint;
    cout << "Enter your name: ";
    getline(cin, name);
    cout << "Enter your email: ";
    getline(cin, email);
    cout << "Enter your complaint: ";
    getline(cin, complaint);

    string sql = "INSERT INTO complaints (name, email, complaint, status) VALUES ('" + name + "', '" + email + "', '" + complaint + "', 'Open');";
    char *errMsg = 0;

    // Execute SQL query
    if (sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg) != SQLITE_OK) {
        cout << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Complaint registered successfully!" << endl;
    }
}

// Function to view all complaints
void viewComplaints(sqlite3 *db) {
    string sql = "SELECT * FROM complaints;";
    char *errMsg = 0;

    // Execute SQL query
    if (sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg) != SQLITE_OK) {
        cout << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

// Function to update complaint status
void updateComplaintStatus(sqlite3 *db) {
    int id;
    string status;
    cout << "Enter complaint ID: ";
    cin >> id;
    cin.ignore(); // Ignore leftover newline
    cout << "Enter new status (e.g., Resolved, In Progress): ";
    getline(cin, status);

    string sql = "UPDATE complaints SET status = '" + status + "' WHERE id = " + to_string(id) + ";";
    char *errMsg = 0;

    // Execute SQL query
    if (sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg) != SQLITE_OK) {
        cout << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Complaint status updated!" << endl;
    }
}

int main() {
    sqlite3 *db;
    char *errMsg = 0;

    // Open the SQLite database
    if (sqlite3_open("complaints.db", &db)) {
        cout << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return 1;
    }

    int choice;
    do {
        cout << "\n1. Register a Complaint\n2. View All Complaints\n3. Update Complaint Status\n4. Exit\nEnter your choice: ";
        cin >> choice;
        cin.ignore(); // Ignore leftover newline

        switch (choice) {
            case 1:
                registerComplaint(db);
                break;
            case 2:
                viewComplaints(db);
                break;
            case 3:
                updateComplaintStatus(db);
                break;
            case 4:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
                break;
        }
    } while (choice != 4);

    sqlite3_close(db);  // Close the database connection
    return 0;
}