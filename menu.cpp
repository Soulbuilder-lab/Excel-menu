#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cctype>
using namespace std;

const int MAX_EMP = 100;
string empId[MAX_EMP], empName[MAX_EMP], jobTitle[MAX_EMP],
       department[MAX_EMP], gender[MAX_EMP], hireDate[MAX_EMP];
int age[MAX_EMP], salary[MAX_EMP];
int empCount = 0;

// ---------- function declarations ----------
void loadEmployees();
void saveEmployees();
void displayTable();
void showMenu();
int  getMenuChoice();
void addEmployee();
string trim(const string& s);
bool   isAllDigits(const string& s);
bool   idExists(const string& id);
string readText(const string& prompt);
string readName(const string& prompt);
string readId();
string readGender();
int    readInt(const string& prompt, int minVal, int maxVal);
string readDate();
bool   isValidDate(const string& d);
void   searchMenu();
int    searchEmployees(int results[]);
int    findEmployees(int field, const string& keyword, int results[]);
string toLowerStr(string s);
bool   containsText(const string& text, const string& keyLower);
void   printHeader();
void   printRow(int i);
void deleteEmployee();
void removeAt(int index);
int  findResultIndex(const string& id, const int results[], int count);
bool readYesNo(const string& prompt);
string trim(const string& s);
string toLowerStr(string s);
void updateEmployee();
int  pickEmployeeIndex(const int results[], int count, const string& action);
void showStatistics();
void writeStatistics(ostream& out);
double percent(int part, int total);
void sortBySalaryDescending(int order[]);
void writeEmployeeHeader(ostream& out);
void writeEmployeeLine(ostream& out, int i);
void printBar(ostream& out, const string& label, int count);



// ---------- main ----------
int main() {
    loadEmployees();
    int choice;
    do {
        showMenu();
        choice = getMenuChoice();
        switch (choice) {
            case 1: displayTable();                    break;
            case 2: addEmployee();                     break;
            case 3: searchMenu();                      break;
            case 4: deleteEmployee();                  break;
            case 5: updateEmployee();                  break;
            case 6: showStatistics();                  break;
            case 7: cout << "Goodbye! See you next time.\n";              break;
            default: cout<<" Invalid choice. Please try enter a choice from 1-7.\n"; break;
        }
    } while (choice != 7);
    return 0;
}

// ---------- load from file ----------
void loadEmployees() {
    ifstream file("employee.txt");
    if (!file) { 
        cout << "Cannot open employee.txt\n"; return; 
    }

    string line;
    getline(file, line);                       // skip header
    empCount = 0;

    while (getline(file, line) && empCount < MAX_EMP) {
        stringstream ss(line);
        string field[8], token;
        int n = 0;
        while (n < 8 && getline(ss, token, '\t'))
            if (!token.empty()) field[n++] = token;
        if (n < 8) continue;

        empId[empCount]      = field[0];
        empName[empCount]    = field[1];
        jobTitle[empCount]   = field[2];
        department[empCount] = field[3];
        gender[empCount]     = field[4];
        age[empCount]        = stoi(field[5]);
        hireDate[empCount]   = field[6];
        salary[empCount]     = stoi(field[7]);
        empCount++;
    }
}

// ---------- save back to file (call after add/delete/update) ----------
void saveEmployees() {
    ofstream file("employee.txt");
    file << "ID\tFull Name\tJob Title\tDepartment\tGender\tAge\tHire Date\tMonthly Salary\n";
    for (int i = 0; i < empCount; i++) {
        file << empId[i] << '\t' << empName[i] << '\t' << jobTitle[i] << '\t'
             << department[i] << '\t' << gender[i] << '\t' << age[i] << '\t'
             << hireDate[i] << '\t' << salary[i] << '\n';
    }
}

// ---------- display ----------
void displayTable() {
    printHeader();
    for (int i = 0; i < empCount; i++)
        printRow(i);
    cout << "Total employees: " << empCount << "\n";
}

// ---------- menu ----------
void showMenu() {
    cout << "\n================= MAIN MENU =================\n"
         << "1. Display Table\n"
         << "2. Add Employee\n"
         << "3. Search Employee\n"
         << "4. Delete Employee\n"
         << "5. Update Employee\n"
         << "6. Statistics\n"
         << "7. Exit\n";
}

// input validation: keeps asking until the choice is 1-7
int getMenuChoice() {
    string input;
    while (true) {
        cout << "Please enter your choice: ";
        getline(cin, input);
        if (input.length() == 1 && input[0] >= '1' && input[0] <= '7')
            return input[0] - '0';
        cout << "Invalid choice. Please try enter a number from 1-7.\n";
    }
}

// ---------- add function declarations ----------
void   addEmployee();
string trim(const string& s);
bool   isAllDigits(const string& s);
bool   idExists(const string& id);
string readText(const string& prompt);
string readName(const string& prompt);
string readId();
string readGender();
int    readInt(const string& prompt, int minVal, int maxVal);
string readDate();
bool   isValidDate(const string& d);

// ---------- Validated Input ----------
string trim(const string& s) {                 // remove spaces at both ends
    size_t start = s.find_first_not_of(" ");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" ");
    return s.substr(start, end - start + 1);
}

bool isAllDigits(const string& s) {            // true only if s is 1+ digits
    if (s.empty()) return false;
    for (size_t i = 0; i < s.length(); i++)
        if (!isdigit(s[i])) return false;
    return true;
}

bool idExists(const string& id) {              // used to block duplicate IDs
    for (int i = 0; i < empCount; i++)
        if (empId[i] == id) return true;
    return false;
}

// ---------- Validated Input ----------
string readText(const string& prompt) {       // non-empty, no TAB (TAB would break the file)
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (input.empty())
            cout << "Cannot be empty. Try again.\n";
        else if (input.find('\t') != string::npos)
            cout << "Tab characters are not allowed. Try again.\n";
        else
            return input;
    }
}

string readName(const string& prompt) {       // letters, spaces, ' - . only
    string input;
    while (true) {
        input = readText(prompt);
        bool ok = true;
        for (size_t i = 0; i < input.length(); i++)
            if (!isalpha(input[i]) && input[i] != ' ' && input[i] != '\'' &&
                input[i] != '-' && input[i] != '.')
                ok = false;
        if (ok) return input;
        cout << "Name can only contain letters, spaces, ' - and .\n";
    }
}

string readId() {                              // format EMP + 3 digits, must be unique
    string input;
    while (true) {
        cout << "Enter ID (e.g. EMP051, or 0 to cancel): ";
        getline(cin, input);
        input = trim(input);
        if (input == "0") return "0";  // allow cancel
        for (size_t i = 0; i < input.length(); i++)
            input[i] = toupper(input[i]);

        if (input.length() != 6 || input.substr(0, 3) != "EMP" ||
            !isAllDigits(input.substr(3)))
            cout << "Invalid format. Use EMP followed by 3 digits.\n";
        else if (idExists(input))
            cout << "This ID already exists. Try another.\n";
        else
            return input;
    }
}

string readGender() {                          // Male / Female, any letter case
    string input;
    while (true) {
        cout << "Enter gender (Male/Female): ";
        getline(cin, input);
        input = trim(input);
        for (size_t i = 0; i < input.length(); i++)
            input[i] = tolower(input[i]);
        if (input == "male")   return "Male";
        if (input == "female") return "Female";
        cout << "Invalid gender. Type Male or Female.\n";
    }
}

int readInt(const string& prompt, int minVal, int maxVal) {   // positive integer in range
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (!isAllDigits(input) || input.length() > 9)
            cout << "Enter a positive whole number.\n";
        else {
            int value = stoi(input);
            if (value >= minVal && value <= maxVal) return value;
            cout << "Value must be between " << minVal << " and " << maxVal << ".\n";
        }
    }
}

bool isValidDate(const string& d) {            // format YYYY-MM-DD, real calendar date
    if (d.length() != 10 || d[4] != '-' || d[7] != '-') return false;
    string ys = d.substr(0, 4), ms = d.substr(5, 2), ds = d.substr(8, 2);
    if (!isAllDigits(ys) || !isAllDigits(ms) || !isAllDigits(ds)) return false;

    int y = stoi(ys), m = stoi(ms), day = stoi(ds);
    if (y < 1980 || y > 2026 || m < 1 || m > 12 || day < 1) return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    if (leap) daysInMonth[1] = 29;
    return day <= daysInMonth[m - 1];
}

string readDate() {
    string input;
    while (true) {
        cout << "Enter hire date (YYYY-MM-DD): ";
        getline(cin, input);
        input = trim(input);
        if (isValidDate(input)) return input;
        cout << "Invalid date. Use YYYY-MM-DD, e.g. 2024-03-15.\n";
    }
}

// ---------- ADD ----------
void addEmployee() {
    if (empCount >= MAX_EMP) {
        cout << "Employee list is full (max " << MAX_EMP << ").\n";
        return;
    }

    cout << "\n--- Add New Employee ---\n";
    string newId = readId();
    if (newId == "0") {
        cout << "Add employee cancelled.\n";
        return;
    }
    
    empId[empCount]      = newId;
    empName[empCount]    = readName("Enter full name: ");
    jobTitle[empCount]   = readText("Enter job title: ");
    department[empCount] = readText("Enter department: ");
    gender[empCount]     = readGender();
    age[empCount]        = readInt("Enter age (18-65): ", 18, 65);
    hireDate[empCount]   = readDate();
    salary[empCount]     = readInt("Enter monthly salary: ", 1, 1000000);

    cout << "\nNew employee added:\n";
    printHeader();
    printRow(empCount);

    if (!readYesNo("Save this employee to employee.txt? (Y/N): ")) {
        cout << "Add employee cancelled. No data was saved.\n";
        return;
    }

    empCount++;
    saveEmployees();                           // write to employee.txt
    cout << "Employee added and saved to employee.txt.\n";
}

// ---------- Search Menu declarations ----------
void   searchMenu();
int    searchEmployees(int results[]);
int    findEmployees(int field, const string& keyword, int results[]);
string toLowerStr(string s);
bool   containsText(const string& text, const string& keyLower);
void   printHeader();
void   printRow(int i);

// ---------- Search Menu functions ----------
string toLowerStr(string s) {                  // lowercase copy, for case-insensitive compare
    for (size_t i = 0; i < s.length(); i++)
        s[i] = tolower(s[i]);
    return s;
}

bool containsText(const string& text, const string& keyLower) {   // partial match
    return toLowerStr(text).find(keyLower) != string::npos;
}

// ---------- table printing (shared by Display and Search) ----------
void printHeader() {
    cout << left
         << setw(8)  << "ID"        << setw(20) << "Name"
         << setw(28) << "Job Title" << setw(18) << "Department"
         << setw(8)  << "Gender"    << setw(5)  << "Age"
         << setw(12) << "Hire Date" << "Salary\n";
    cout << string(110, '-') << "\n";
}

void printRow(int i) {
    cout << left
         << setw(8)  << empId[i]      << setw(20) << empName[i]
         << setw(28) << jobTitle[i]   << setw(18) << department[i]
         << setw(8)  << gender[i]     << setw(5)  << age[i]
         << setw(12) << hireDate[i]   << salary[i] << "\n";
}

// ---------- core search: fills results[] with matching indexes, returns how many ----------
int findEmployees(int field, const string& keyword, int results[]) {
    int count = 0;
    string key = toLowerStr(keyword);

    for (int i = 0; i < empCount; i++) {
        bool match = false;
        switch (field) {
            case 1: match = containsText(empId[i], key);        break;
            case 2: match = containsText(empName[i], key);      break;
            case 3: match = containsText(jobTitle[i], key);     break;
            case 4: match = containsText(department[i], key);   break;
            case 5: match = (toLowerStr(gender[i]) == key);     break;  // exact, because "male" is inside "female"
            case 6: match = (age[i] == stoi(key));              break;
            case 7: match = containsText(hireDate[i], key);     break;
            case 8: match = (salary[i] == stoi(key));           break;
        }
        if (match) results[count++] = i;
    }
    return count;
}

// ---------- asks the user what to search, shows results, returns the count ----------
int searchEmployees(int results[]) {
    cout << "\n--- Search Employee ---\n"
         << "Search by:\n"
         << "1. ID\n2. Full Name\n3. Job Title\n4. Department\n"
         << "5. Gender\n6. Age\n7. Hire Date\n8. Monthly Salary\n"
         << "0. Back to Main Menu\n";

    int field = readInt("Choose attribute (0-8): ", 0, 8);
    if (field == 0) {
        cout << "Search cancelled.\n";
        return 0;
    }
    string keyword;

    switch (field) {
        case 1: keyword = readText("Enter ID (or part of it): ");                break;
        case 2: keyword = readText("Enter name (or part of it): ");              break;
        case 3: keyword = readText("Enter job title (or part of it): ");         break;
        case 4: keyword = readText("Enter department (or part of it): ");        break;
        case 5: keyword = readGender();                                          break;
        case 6: keyword = to_string(readInt("Enter age: ", 1, 150));             break;
        case 7: keyword = readText("Enter hire date (e.g. 2022 or 2022-04-10): "); break;
        case 8: keyword = to_string(readInt("Enter monthly salary: ", 1, 1000000)); break;
    }

    int count = findEmployees(field, keyword, results);

    if (count == 0) {
        cout << "No matching employee found.\n";
    } else {
        printHeader();
        for (int i = 0; i < count; i++)
            printRow(results[i]);
        cout << count << " employee(s) found.\n";
    }
    return count;
}

// ---------- menu option 3 ----------
void searchMenu() {
    int results[MAX_EMP];
    searchEmployees(results);
}

//---------- Delete Function Declarations ----------
void deleteEmployee();
void removeAt(int index);
int  findResultIndex(const string& id, const int results[], int count);
bool readYesNo(const string& prompt);
string trim(const string& s);
string toLowerStr(string s);

// ---------- Delete Function ----------
bool readYesNo(const string& prompt){
    string input;
    while (true){
        cout <<prompt;
        getline(cin, input);
        input = toLowerStr(trim(input));
        if (input == "y" || input == "yes" || input == "Y" || input == "YES") return true;
        if (input == "n" || input == "no" || input == "N" || input == "NO") return false;
        cout<< "Invalid input. Please enter Y/N or Yes/No.\n";
    }
}

//---------- is this ID inside the search results? returns array index, or -1 ----------
int findResultIndex(const string& id, const int results[], int count) {
    for (int i = 0; i < count; i++) {
        if (empId[results[i]] == id) {
            return i;
        }
    }
    return -1;
}

//---------- remove one employee by shifting everything after it up by one ----------
void removeAt(int index){
    for(int i = index; i< empCount-1; i++){
        empId[i]        = empId[i+1];
        empName[i]      = empName[i+1];
        jobTitle[i]     = jobTitle[i+1];
        department[i]   = department[i+1];
        gender[i]       = gender[i+1];
        age[i]          = age[i+1];
        hireDate[i]     = hireDate[i+1];
        salary[i]       = salary[i+1];
    }
    empCount--;
}

//---------- Delete Employee ----------
void deleteEmployee(){
    if(empCount == 0){
        cout << "No employees to delete. Please add employees first.\n";
        return;
    }

    int result[MAX_EMP];
    int count = searchEmployees(result);    // reuses search
    if(count == 0) return;                  //found nothing

    // ask for an exact ID, but only accept one that appeared in the search results
    int index = -1;
    string id;
    while(index == -1){
        cout<<"Enter the exact iD of the employee to delete (or 0 to cancel): ";
        getline(cin, id);
        id = trim(id);
        if (id == "0") {
            cout << "Delete cancelled.\n";
            return;
        }
        for (size_t i = 0; i < id.length(); i++)
            id[i] = toupper(id[i]);

        index = findResultIndex(id, result, count);
        if(index == -1){
            cout << "ID not found in the search results. Please try again.\n";
        }
    }

    cout<<"\nYou are about to delete the following employee:\n";
    printHeader();
    printRow(result[index]);

    if(readYesNo("Are you sure you want to delete this employee? (Y/N): ")){
        removeAt(result[index]);
        saveEmployees();
        cout << "Employee deleted and changes saved to employee.txt.\n";
    } else {
        cout << "Delete cancelled.\n";
    }
}

//---------- Update Employee ----------
void updateEmployee();
int  pickEmployeeIndex(const int results[], int count, const string& action);

//----------- Update Function ----------
// return the array index of the employee to update, or -1 if cancelled
int pickEmployeeIndex(const int results[], int count, const string& action) {
    string id;
    while (true){
        cout << "Enter the exact ID of the employee to " << action <<" (or 0 to cancel): ";
        getline(cin, id);
        id = trim(id);
        if (id == "0") return -1;
        for (size_t i = 0; i < id.length(); i++){
            id[i] = toupper(id[i]);
        }

        int index = findResultIndex(id, results, count);
        if (index != -1) {
            return results[index];  // return the actual array index
        } else {
            cout << "The ID is not found in the search results. Please try again.\n";
        }
    }
}

//---------- Update Employee Data ----------
void updateEmployee() {
    if (empCount == 0){
        cout << "No employees to update. Please add employees first or try again.\n";
        return;
    }

    int results[MAX_EMP];
    int count = searchEmployees(results);
    if (count == 0) return;  // found nothing
    
    int index = pickEmployeeIndex(results, count, "update");
    if (index == -1) {
        cout << "Update cancelled.\n";
        return;
    }

    // work on COPIES first; the real arrays change only after the user confirms
    string newTitle = jobTitle[index];
    string newDept = department[index];
    int newSalary = salary[index];

    cout <<"\nCurrent Record:\n";
    printHeader();
    printRow(index);
    cout << "\n(ID, Name, Gender cannot bbe changed.)\n";

    int choice;
    do{
        cout << "\nWhat do you want to change?\n"
             << "1. Job Title      (now: " << newTitle << ")\n"
             << "2. Department     (now: " << newDept << ")\n"
             << "3. Monthly Salary (now: " << newSalary << ")\n"
             << "4. Done   (review and update changes)\n"
             << "5. Cancel (discard changes)\n";
        choice = readInt("Enter your choice (1-5): ", 1, 5);

        switch(choice){
            case 1: newTitle = readText("Enter new job title: "); break;    
            case 2: newDept = readText("Enter new department: "); break;
            case 3: newSalary = readInt("Enter new monthly salary: ", 1, 1000000); break;
        }
    }while(choice != 4 && choice != 5);

    if ( choice == 5) {
        cout << "Update cancelled. No data changes was made.\n";
        return;
    }

    if(newTitle == jobTitle[index] && newDept == department[index] && newSalary == salary[index]){
        cout << "No changes were made. Update cancelled.\n";
        return;
    }

    cout <<"\nChanges for " << empId[index] << " (" << empName[index] << "):\n"
         <<"Job Title: " << jobTitle[index] << " -> " << newTitle << "\n "
         << "Department: " << department[index] << " -> " << newDept << "\n"
         << "Monthly Salary: " << salary[index] << " -> " << newSalary << "\n";

    if(readYesNo("Are you sure you want to save these changes? (Y/N): ")){
        jobTitle[index] = newTitle;
        department[index] = newDept;
        salary[index] = newSalary;
        saveEmployees();
        cout <<" Employee updated and changes saved to employee.txt.\n";
    } else {
        cout << "Update cancelled. No data changes was made.\n";
        return;
    }
}

//---------- Statistics Function Declarations ----------
void showStatistics();
void writeStatistics(ostream& out);
double percent(int part, int total);
void sortBySalaryDescending(int order[]);
void writeEmployeeHeader(ostream& out);
void writeEmployeeLine(ostream& out, int i);
void printBar(ostream& out, const string& label, int count);

//---------- Statistics Function ----------
//---------- percentage (when total is 0, return 0.0 to avoid division by zero) ----------
double percent(int part, int total){
    if (total == 0) return 0.0;
    return part*100.0 / total;
}

//---------- fill order[] with indexes of employees sorted by salary descending ----------
void sortBySalaryDescending(int order[]){
    for(int i = 0; i < empCount; i++){
        order[i] = i;
    }

    for(int pass = 0; pass < empCount - 1; pass++){
        for(int j = 0; j < empCount-1-pass; j++){
            if(salary[order[j]] < salary[order[j+1]]){
                int temp = order[j];
                order[j] = order[j+1];
                order[j+1] = temp;
            }
        }
    }
}

//---------- Print one employee header to the output file and screen ----------
void writeEmployeeHeader(ostream& out){
    out << left
        << setw(8)  << "ID"      << setw(20) << "Name"
        << setw(28) << "Job Title"  << setw(8)  << "Gender"
        << setw(8) << "Salary\n";
    out <<string(70, '-') << "\n";
}

//---------- Print the line for the employee table on the output file and screen----------
void writeEmployeeLine(ostream& out, int i){
    out << left
        << setw(8) << empId[i]      << setw(20) << empName[i]
        <<setw(28) << jobTitle[i]   <<setw(8) << gender[i]
        <<setw(8) << salary[i] << "\n";
}

//---------- Print histogram bar for the output file and screen ----------
void printBar(ostream& out, const string& label, int count){
    out << left 
        << setw(18) << label << " | "
        << string(count, '*') << " (" << count << ")\n";
}

//---------- Write all statistics and writes them to the statistics.txt file or screen ----------
void writeStatistics(ostream& out){
    out << fixed << setprecision(1);
    out << "\n================== Employee Statistics =================\n";
    out << "Total employees: " << empCount << "\n";

    int order[MAX_EMP];
    sortBySalaryDescending(order);

    // 1. gender percentage
    int malecount = 0, femalecount = 0;
    for (int i = 0; i < empCount; i++){
        if (gender[i] == "Male") malecount++;
        else if (gender[i] == "Female") femalecount++;
    }
    out <<"\n1. Gender Percentage:\n";
    out <<" Male:" << malecount <<" (" << percent(malecount, empCount) << "%)\n";
    out <<" Female:" << femalecount <<" (" << percent(femalecount, empCount) << "%)\n";

    // 2. HIghest and lowest monthlysalary (Print everyone who ties after search)
    int highest = salary[order[0]];
    int lowest = salary[order[empCount-1]];

    out <<"\n2a. Highest monthly salary :\n";
    writeEmployeeHeader(out); 
    for (int i = 0; i < empCount; i++){
        if (salary[i] == highest){
            writeEmployeeLine(out, i);
        }
    }

    out <<"\n2b. Lowest monthly salary :\n";
    writeEmployeeHeader(out);
    for (int i = 0; i < empCount; i++){
        if (salary[i] == lowest){
            writeEmployeeLine(out, i);
        }
    }

    // 3. Top 3 and bottom 3 monthly salaries of employees
    int n = empCount < 3 ? empCount : 3;

    out <<"\n3a. Top 3 salaries :\n";
    writeEmployeeHeader(out);
    for (int k = 0; k < n; k++){
        writeEmployeeLine(out, order[k]);
    }

    out <<"\n3b. Bottom 3 salaries :\n";
    writeEmployeeHeader(out);
    for (int k = 0; k < n; k++){
        writeEmployeeLine(out, order[empCount-1-k]);
    }

    // 4. Percentage of Employees in each department
    string deptName[MAX_EMP];
    int deptCount[MAX_EMP];
    int deptTotal = 0;

    for (int i = 0; i < empCount; i++){
        int found = -1;
        for (int d = 0; d < deptTotal; d++){
            if( toLowerStr(department[i]) == toLowerStr(deptName[d])){
                found = d;
                break;
            }
        }
        if (found == -1){
            deptName[deptTotal] = department[i];
            deptCount[deptTotal] = 1;
            deptTotal++;
        } else {
            deptCount[found]++;
        }
    }

    out <<"\n4. Number and Percentage of Employees in each Department:\n";
    out << left 
        << setw(20) << "Department" 
        << setw(10) << "Count" << "Percentage\n";
    out << string(40, '-') << "\n";
    for (int d = 0; d < deptTotal; d++){
        out << left
            << setw (20) << deptName[d]
            << setw (12) << deptCount[d]
            << percent(deptCount[d], empCount) << "%\n";
    }

    // 5. Number and percentage of employees who joined the company after 1st Jan 2022
    int joinedAfter = 0;
    for (int i = 0; i < empCount; i++){
        if(hireDate[i] > "2022-01-01") 
        joinedAfter++;
    }
    out << "\n5. Number and Percentage of Employees who joined after 1st Jan 2022:\n" << "\t" << joinedAfter 
        << " (" << percent(joinedAfter, empCount) << "%)\n";
    
    // 6. Histogram
    int below5000 = 0 , s1 = 0, s2 = 0, s3 = 0, above10000 = 0;
    int under20 = 0, a1 = 0, a2 = 0, above40 = 0;

    for (int i = 0; i < empCount; i++){
        if (salary[i] < 5000) below5000++;
        else if (salary[i] <= 7500) s1++;
        else if (salary[i] <= 9500) s2++;
        else if (salary[i] <= 10000) s3++;
        else above10000++;

        if (age[i] < 20) under20++;
        else if (age[i] <= 30) a1++;
        else if (age[i] <= 40) a2++;
        else above40++;
    }

    out <<"\n6. Histogram (each * = 1 employee)\n";

    out <<"\n6a. Monthly Salary :\n";
    printBar(out, "Below 5000", below5000);
    printBar(out, "5000 - 7500", s1);
    printBar(out, "7501 - 9500", s2);
    printBar(out, "9501 - 10000", s3);
    printBar(out, "Above 10000", above10000);

    out <<"\n6b. Gender\n";
    printBar(out, "Male", malecount);
    printBar(out, "Female", femalecount);

    out <<"\n6c. Age :\n";
    printBar(out, "Below 20", under20);
    printBar(out, "20 - 30", a1);
    printBar(out, "31 - 40", a2);
    printBar(out, "Above 40", above40);
}

//---------- menu option 6 ----------
void showStatistics(){
    if(empCount == 0) {
        cout << "No employees to show statistics. Please add employees first.\n";
        return;
    }

    writeStatistics(cout);  // print to screen

    ofstream file("statistics.txt"); 
    if (!file) {
        cout << "Cannot open statistics.txt for writing.\n";
        return;
    }
    writeStatistics(file);  // write to file
    cout << "Statistics written to statistics.txt\n";
}