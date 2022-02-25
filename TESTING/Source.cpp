//  Написать программу обработки файла типа запись, содержащую следующие пункты меню : «Создание», «Просмотр»,
//  «Коррекция»(добавление новых данных или редактирование старых), «Решение индивидуального задания».
//  Каждая запись должна содержать следующую информацию о студентах :
//  – фамилия и инициалы;
//  – год рождения;
//  – номер группы;
//  – оценки за семестр: по физике, математике, информатике, химии;
//  – средний балл.
//  Организовать ввод исходных данных, средний балл рассчитать по введенным оценкам.
//  25.05.2021

// 9. Вычислить общий средний балл всех студентов и распечатать список студентов интересующей вас группы,
// имеющих средний балл выше общего среднего балла.

#define _CRT_SECURE_NO_WARNINGS
#define NAME_RESTRICTION 19
#define NUMBER_SUBJECTS 4
#define MAX_PATH 225

#include "nikish_func.h"
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <io.h>

struct Buffer {
    char full_name[NAME_RESTRICTION];
    int year_of_birth;
    unsigned int group_id;
    int mark_math, mark_phys, mark_it, mark_chem;
    float average_mark;
};

struct Files {
    char file_binary[40];
    char file_result[40];
};

int size_struct = sizeof(Buffer);
int size_files = sizeof(Files);

FILE* fp_data, *fp_result, *fp_files;

char standart_binary[] = "standart_binary.dat";
char standart_result[] = "standart_result.txt";
char fileLocation[] = "main.dat";

char* file_binary;
char* file_result;
char* file_list = fileLocation;

void init(void);  // Creates/refreshes file
void add(Buffer*);   // Add's new information about students
void solution(void); // Outputs solution for the problem
void peekWrite(Buffer*); // Outputs all the information in data file
void correct(Buffer*); // Deletes or corrects information about student
void structOutput(Buffer*); // Function to output structure in file and console
void fillStructure(Buffer*); // Fill's specific structure with information (Note: Structure should be "buffer" type)
void rename(void);          // Parental fucntion to rename the file
int get_filename(const char*, char *file); // gets the file name

using namespace std;

int main()
{
    int counter = 0;
    int userDes;
    Buffer clear{};
    Buffer* S;
    Files f;    // f contains info about file names, and then we use it to write file names in "main.dat"
    S = &clear; 
    
    if ((fp_files = fopen(file_list, "rb+")) == NULL) {
        cout << "Error 2: Cannot open file list";
        return 0;
    }
    
    fseek(fp_files, 0, SEEK_END);

    if (ftell(fp_files) == 0)
    {
        cout << "No new file names found. Using standart:\n1. Binary file: " << standart_binary << "\n2. Result file: " << standart_result;
        file_binary = standart_binary;
        file_result = standart_result;
        fclose(fp_files);
    }

    else {
        fseek(fp_files, 0, SEEK_SET);

        fread(&f, size_files, 1, fp_files);
    
        cout << "Files opened:\n1. File binary: " << f.file_binary << "\n2. Result file: " << f.file_result << endl;

        file_binary = f.file_binary;
        file_result = f.file_result;
        fclose(fp_files);
    }
    

    fp_result = fopen(file_result, "wb");
    cout << "\n\nFile I/O program with specific problem by Ignatov Nikita\nBSUIR November 2020\n";
    while (true) {
        cout << "\n1. Create or refresh the file\n2. Add new information\n"
            << "3. Correct information\n4. View data\n5. Solution\n6. Rename file\n0. Exit & Save information in textfile\nDes: ";
        userDes = custom::get_int();
        cout << endl;
        switch (userDes) {
        case 1:
            init();
            break;
        case 2:
            add(S);
            break;
        case 3:
            correct(S);
            break;
        case 4:
            peekWrite(S);
            break;
        case 5:
            solution();
            break;
        case 6:
            rename();

            fp_files = fopen(file_list, "wb");
            if (fp_files == NULL) {
                cout << "Error writing file names. Reset...";
                file_binary = standart_binary;
                file_result = standart_result;
                break;
            }
            else {
                custom::strcpy_p(f.file_binary, file_binary);
                custom::strcpy_p(f.file_result, file_result);

                fwrite(&f, size_files, 1, fp_files);

                cout << "\nSuccessfully saved file names.\n";
                fclose(fp_files);
                break;
            }

        case 0:
            fclose(fp_result);
            return 0;
        }
    }
}


void rename(void) {

    short file_to_rename;
    bool stop_point = true;
    cout << "\n1. Output file\n2. Data file\n0. Back\n\nChoose the file you want to rename: ";
    while ((file_to_rename = custom::get_int())  > 3) { cout << "\nThere is no file like that. Try again: "; }
    
    switch (file_to_rename) {
        case 1:
        {
            char filename[MAX_PATH];
        
            // Output file is opened all the time during compilation
            // We cannot renanme opened files, so we have to close it
            // and after all open it again as it was before
            fclose(fp_result);
            cout << "\n\nEnter file name: ";
            while (stop_point == true) {
                short execution_code = get_filename(".txt", filename);
    
                switch (execution_code) {
                case -1:
                    cout << "\nInvalid file format. Check the source code of the program.";
                    stop_point = false;
                    break;
                case -2:
                    cout << "\nA filename can't contain any of the following characters: \\ / : * ? < > |\n"
                        << "Try again: ";
                    break;
                default:
                    stop_point = false;
                    break;
                }
            }
            cout << endl << filename;

            if (_access(filename, 0) != -1) {
                cout << "\n\nFile '" << filename << "' already exists.\n";
                break;
            }
    
            if (rename(file_result, filename) == 0) {
                cout << "\nFile name changed from '" << file_result << "' to '" << filename << "' successfully.\n";
                custom::strcpy_p(file_result, filename);
            } else {
                perror("\nError renaming file");
                return;
            }
    
            if ((fp_result = fopen(file_result, "wb")) == NULL) {
                cout << "An ERROR occured with '" << file_result << "'." << endl;
                return;
            }
            break;
        }
        case 2:
        {
            char filename[MAX_PATH];
            cout << "\n\nEnter file name: ";
            while (stop_point == true) {
                short execution_code = get_filename(".dat", filename);
    
                switch (execution_code) {
                case -1:
                    cout << "\nInvalid file format. Check the source code of the program.";
                    stop_point = false;
                    break;
                case -2:
                    cout << "\nA filename can't contain any of the following characters: \\ / : * ? < > |\n"
                        << "Try again: ";
                    break;
                case -3:
                    cout << "\n\nDo not use the following reserved names for the name of a file: \nCON, PRN, AUX, NUL, COM1, COM2, COM3, COM4, \nCOM5, COM6, COM7, COM8, COM9, LPT1, LPT2, LPT3, LPT4, LPT5, LPT6, LPT7, LPT8, and LPT9.\nFor more information, see: https://docs.microsoft.com/en-us/windows/win32/fileio/naming-a-file?redirectedfrom=MSDN \n\nTry again: ";
                    break;
                default:
                    stop_point = false;
                    break;
                }
            }
    
    
            if (_access(filename, 0) != -1) {
                cout << "\n\nFile '" << filename << "' already exists.\n";
                break;
            }
    
            if (rename(file_binary, filename) == 0) {
                cout << "\nFile name changed from '" << file_binary << "' to '" << filename << "' successfully.\n";
                custom::strcpy_p(file_binary, filename); 
            } else {
                perror("\nError renaming file");
                return;
            }
            break;
        }
        case 0:
            break;
    }
    
}   


int get_filename(const char* file_format, char* filename)
{
    int str_length = custom::strlen_p(file_format);
    // file_format should be at least two characters length: .c
    if (file_format[0] != '.' || str_length < 2 || str_length > MAX_PATH) { return -1; }

    char c;
    int length = 0;

    while (length < MAX_PATH - str_length)
    {
        c = _getch();
        if (c == 13) break;
        if (c == '?' || c == '|' || c == '\\' || c == '/' || c == ':' || c == '*' || c == '<' || c == '>' || c == '"') { return -2; }
        if (c == '\b' && length != 0) { //deleting info.
            cout << "\b \b";    // moves the cursor back, prints space instead of character and then returns cursor back
            length--;
            continue;
        }
        filename[length] = c;
        cout << filename[length];
        length++;
    }
    // For comparsion below
    filename[length] = '\0';    // In the loop below it will be overwrited

    // add ff to the string
    for (int i = 0; i < str_length; i++, length++) { filename[length] = file_format[i]; }

    // form string

    filename[length] = '\0';

    cout << filename << endl;


    return 0;
}

// Opens and create/refresh data file
void init() {
    // Check if file exists
    if ((fp_data = fopen(file_binary, "rb")) == NULL) {
        cout << "File is about to create...";
        if ((fp_data = fopen(file_binary, "wb")) == NULL) {
            cout << "Error 1: Cannot open file.\n";
            return;
        }
        cout << "\nCreated.";
    }

    long size;
    char choice;
    do {

        cout << "\nWould you like delete all the information about students?(Y/N): ";
        cin >> choice;

        // Check if file is empty
        if (NULL != fp_data) {
            fseek(fp_data, 0, SEEK_END);
            size = ftell(fp_data);

            if (0 == size) {
                printf("File is already empty\n");
                fclose(fp_data);
                return;
            }
        }

        // Decision
        if (choice == 'Y' or choice == 'y') {
            if ((fp_data = fopen(file_binary, "wb")) == NULL) {
                cout << "Error 1: Cannot open file.\n";
                return;
            }
            cout << "Refreshed!" << endl;
            fclose(fp_data);
            return;
        }

        if (choice != 'Y' and choice != 'y' and choice != 'N' and choice != 'n') // if needed add input 
            cout << choice << " is not a valid option. Try agian" << endl; // validation

    } while (choice != 'N' && choice != 'n'); // the user input does not equal 'N'andr 'n'
    
    cout << "No changes saved!" << endl;
    fclose(fp_data);
}


// Add's information to data file
// Open's file, fill's global student's structure, append's it in file
void add(Buffer* S) {

    char choice;
    do {
        if ((fp_data = fopen(file_binary, "ab")) == NULL) {
            cout << "Error 1: Cannot open file.\n";
            return;
        }
        fillStructure(S);

        cout << "\nDo you want to continue adding some information? [y/n]\nDes: ";
        cin >> choice;
        cin.clear();
        cin.ignore(1000, '\n');

        if (choice == 'N' or choice == 'n') {
            fwrite(S, size_struct, 1, fp_data);
            fclose(fp_data);
            cout << "\nAdded successully" << endl;
            break;
        }
        else if (choice == 'Y' or choice == 'y')
        {
            fwrite(S, size_struct, 1, fp_data);
            fclose(fp_data);
        }

        while (choice != 'Y' and choice != 'y' and choice != 'N' and choice != 'n') // if needed add input 
        {
            cout << choice << " is not a valid option-> Try agian" << endl; // validation
            cout << "Des: ";
            cin >> choice;
            cin.clear();
            cin.ignore(1000, '\n');

            if (choice == 'N' or choice == 'n') {
                cout << "\nAdded successully" << endl;
                fwrite(S, size_struct, 1, fp_data);
                fclose(fp_data);
                break;
            }
            else if (choice == 'Y' || choice == 'y')
            {
                fwrite(S, size_struct, 1, fp_data);
                fclose(fp_data);
            }
        }

    } while (true);
}


// Fill's specific structure, main input
void fillStructure(Buffer* s)
{
    cout << "\nFull name: ";
    if (custom::getline(s->full_name, NAME_RESTRICTION) < 0) {
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cout << "Year of Birth: ";
    while ((s->year_of_birth = custom::get_int()) > 2020) {
        cout << "You cannot be born in future, try again: ";
    }

    cout << "Student's Group Id: ";
    while ((s->group_id = custom::get_int()) > 100) {
        cout << "Incorrect input (0-100): ";
    }
    cout << "Math semester mark: ";
    while ((s->mark_math = custom::get_int()) > 100) {
        cout << "You cannot be born in future, try again: ";
    }
    cout << "Physcis semester mark: ";
    while ((s->mark_phys = custom::get_int()) > 100) {
        cout << "You cannot be born in future, try again: ";
    }
    cout << "IT semester mark: ";
    while ((s->mark_it = custom::get_int()) > 100) {
        cout << "You cannot be born in future, try again: ";
    }
    cout << "Chemestry semseter mark: ";
    while ((s->mark_chem = custom::get_int()) > 100) {
        cout << "You cannot be born in future, try again: ";
    }

    s->average_mark = (float)(s->mark_math + s->mark_phys + s->mark_it + s->mark_chem) / 4;

}


// Outputs information about data file in console and result file
// Read's binary information from data file
void peekWrite(Buffer* S) {
    int counter = 0;
    long fileSize;
    int numStudents;
    if ((fp_data = fopen(file_binary, "rb")) == NULL) {
        cout << "Error 1: Cannot open file.\n";
        return;
    }

    /*Get file size*/
    fseek(fp_data, 0L, SEEK_END);
    fileSize = ftell(fp_data);
    fseek(fp_data, 0L, SEEK_SET);
    numStudents = fileSize / size_struct;
    /*---------------*/

    if (numStudents == 0) {
        cout << "No students found!" << endl;
        return;
    }

    cout << "Amount of students: " << numStudents << endl << endl;

    /*Table-like output*/
    cout << "ID\t";
    cout << left << setw(25) << setfill(' ') << "Name";
    cout << left << setw(15) << setfill(' ') << "Group ID";
    cout << left << setw(15) << setfill(' ') << "Birthyear";
    cout << left << setw(9) << setfill(' ') << "Math";
    cout << left << setw(11) << setfill(' ') << "Physics";
    cout << left << setw(8) << setfill(' ') << "IT";
    cout << left << setw(14) << setfill(' ') << "Chemestry";
    cout << left << setw(15) << setfill(' ') << "Average Mark" << endl << endl;
    /*---------------*/

    fprintf(fp_result, "%-25s%-15s%-15s%-9s%-11s%-8s%-14s%-15s\n", "Name", "Group ID", "Birthyear",
        "Math", "Physics", "IT", "Chemestry", "Average Mark");
    while (true) {
        if (fread(S, size_struct, 1, fp_data) == NULL) break;
        cout << counter++ << "\t";
        structOutput(S);    
    }
    counter = 0;
    fclose(fp_data);
}


// Takes all the students info from file and compares student's average mark with global one
void solution() {
    int studentsFound = 0, numStudents, selected_group, found = 0;
    double sum = 0, s_average;
    long fileSize;
    Buffer *students;
    fp_data = fopen(file_binary, "rb");

    /*Get's file size*/
    fseek(fp_data, 0L, SEEK_END);
    fileSize = ftell(fp_data);
    fseek(fp_data, 0L, SEEK_SET);
    numStudents = fileSize / size_struct;
    /*---------------*/

    if (numStudents == 0) {
        cout << "No students found. Unable to finish the solution!\n";
        return;
    }

    students = new Buffer[numStudents];
    
    for (int i = 0; i < numStudents; i++)
        fread(students+i, size_struct, 1, fp_data);
    fclose(fp_data);

    int found_s = 0;
    cout << "Found groups: ";
        
    cout << "\nEnter Interested Group ID: ";
    selected_group = custom::get_int();

    while (studentsFound == 0) {
        for (int i = 0; i < numStudents; ++i) {
            sum += students[i].average_mark;
            ++studentsFound;
        }
    }
    
    // Average mark of all students
    s_average = sum / studentsFound;
   
    cout << "\nAverage mark all of the students is " << s_average << "\n";
    fprintf(fp_result, "\nAverage mark all of the students is %lf", s_average);

    cout << "\n\t----- Students with highest average mark -----\n\t\t\tGroup " << selected_group << "\n\n";
    fprintf(fp_result, "\n\t----- Students with highest average mark -----\n\t\t\tGroup %d\n\n", selected_group);
    for (int i = 0; i < numStudents; ++i) {
        if (students[i].average_mark > s_average && students[i].group_id == selected_group) {
            structOutput(students+i);
            found++;
        }
    }
    if (!found) {
        cout << "\nNo students found! Please, select other group next time.\n";
        fprintf(fp_result, "\nNo students found!\n");
    }
    delete[]students;
}


// Function to delete student information from file
void correct(Buffer* S)
{
    int des;
    int id;
    int numStudents;
    long fileSize;
    Buffer* students;
    
    peekWrite(S);

    if ((fp_data = fopen(file_binary, "rb")) == NULL) {
        cout << "Error 1: Cannot open file.\n";
        return;
    }
    /*Get file size*/
    fseek(fp_data, 0L, SEEK_END);
    fileSize = ftell(fp_data);
    fseek(fp_data, 0L, SEEK_SET);
    numStudents = fileSize / size_struct;
    /*---------------*/

    if (numStudents == 0) {
        cout << "\nNo students found\n";
        return;
    }
    
    cout << "\n\nWhat would you like to do:\n1. Delete\n2. Correct\n\nDes: ";
    while ((des = custom::get_int()) != 1 && des != 2) {
        cout << "Try again: ";
    }

    cout << "\n";
    
    students = new Buffer[numStudents];

    cout << "Enter student id: ";
    while ((id = custom::get_int()) >= numStudents){
        cout << "No students found. Enter again: ";
    }
    cout << "\n\n";

    for (int i = 0; i < numStudents; i++) {
        if (i == id) {
            if (des == 1) {
                char choice;
                do {
                    cout << "Are you sure you want to delete all the information about student number " << id << " [y/n]: ";
                        
                    cin >> choice;
                    cin.clear();
                    cin.ignore(1000, '\n');

                    // Decision
                    if (choice == 'Y' or choice == 'y') {
                        --numStudents;
                        fseek(fp_data, size_struct, SEEK_CUR);
                        fread(students + i, size_struct, 1, fp_data);
                        cout << "Deleted!" << endl;
                        break;
                    }

                    if (choice == 'N' or choice == 'n')
                    {
                        fread(students + i, size_struct, 1, fp_data);
                        break;
                    }

                    if (choice != 'Y' and choice != 'y' and choice != 'N' and choice != 'n') // if needed add input 
                        cout << choice << " is not a valid option-> Try agian" << endl; // validation


                    break;
                } while (true); // the user input does not equal 'N'andr 'n'
            }
            else {
                fread(students + i, size_struct, 1, fp_data);
                int userDes1;
                char choice = 'y';

                do {
                    cout << "What you want to change:\n\n1.Full Name\n2.Group ID\n3.Year of birth\n4.Math Mark\n5.Physics mark\n6.IT mark\n7.Chemestry mark\nDes:";

                    while ((userDes1 = custom::get_int()) <= 0 && userDes1 > 7) {
                        cout << "Incorrect input, try again: ";
                    }

                    cout << "\n\n";

                    switch (userDes1)
                    {
                    case 1:
                        cout << "Enter students full name: ";
                        custom::getline(students[i].full_name, NAME_RESTRICTION);
                        break;

                    case 2:
                        cout << "Enter group ID: ";
                        students[i].group_id = custom::get_int();
                        break;

                    case 3:
                        cout << "Enter year of birth: ";
                        while ((students[i].year_of_birth = custom::get_int()) > 2020) {
                            cout << "Impossible year of birth. Try again: ";
                        }
                        break;

                    case 4:
                        cout << "Math mark: ";
                        while ((students[i].mark_math = custom::get_int()) > 100) {
                            cout << "Impossible mark (should be 0 - 100). Try again: ";
                        }
                        break;

                    case 5:
                        cout << "Physics mark: ";
                        while ((students[i].mark_phys = custom::get_int()) > 100) {
                            cout << "Impossible mark (should be 0 - 100). Try again: ";
                        }
                        break;

                    case 6:
                        cout << "IT mark: ";
                        while ((students[i].mark_it = custom::get_int()) > 100) {
                            cout << "Impossible mark (should be 0 - 100). Try again: ";
                        }
                        break;

                    case 7:
                        cout << "Chemestry mark: ";
                        while ((students[i].mark_chem = custom::get_int()) > 100) {
                            cout << "Impossible mark (should be 0 - 100). Try again: ";
                        }
                        break;
                    }

                    students[i].average_mark = (float)(students[i].mark_math + students[i].mark_phys + students[i].mark_it + students[i].mark_chem) / 4;

                    cout << "\nDo you want to continue correting some information? [y/n]\nDes: ";
                    cin >> choice;
                    cin.clear();
                    cin.ignore(1000, '\n');

                    if (choice == 'N' or choice == 'n') {
                        cout << "Corrected!" << endl;
                        break;
                    }

                    while (choice != 'Y' and choice != 'y' and choice != 'N' and choice != 'n') // if needed add input 
                    {
                        cout << choice << " is not a valid option-> Try agian" << endl; // validation
                        cout << "Des: ";
                        cin >> choice;
                        cin.clear();
                        cin.ignore(1000, '\n');

                        if (choice == 'N' or choice == 'n') {
                            cout << "Corrected!" << endl;
                            break;
                        }
                    }   

                } while (true);

            }
            cout << "Changes saved successfully!\n\n";
        } else {
            fread(students + i, size_struct, 1, fp_data);
        }
    }
    fclose(fp_data);

    /*--------Rewrite the file------*/
    if ((fp_data = fopen(file_binary, "wb")) == NULL) {
        cout << "Error 1: Cannot open file->\n";
        return;
    }
    for (int i = 0; i < numStudents; ++i) {
        fwrite(students + i, size_struct, 1, fp_data);
    }
    /*-----------------------------*/
    delete[]students;
    fclose(fp_data);
}


// Outputs information in textfile and console
void structOutput(Buffer* student) {
    const char separator = ' ';
    
    cout << left << setw(25) << setfill(separator) << student->full_name;
    cout << left << setw(15) << setfill(separator) << student->group_id;
    cout << left << setw(15) << setfill(separator) << student->year_of_birth;
    cout << left << setw(9) << setfill(separator) << student->mark_math;
    cout << left << setw(11) << setfill(separator) << student->mark_phys;
    cout << left << setw(8) << setfill(separator) << student->mark_it;
    cout << left << setw(14) << setfill(separator) << student->mark_chem;
    cout << left << setw(15) << setfill(separator)  << setprecision(5) << student->average_mark << endl;

    
    fprintf(fp_result, "%-25s%-15u%-15d%-9d%-11d%-8d%-14d%-15lf\n",
        student->full_name, student->group_id, student->year_of_birth, student->mark_math, student->mark_phys, 
        student->mark_it, student->mark_chem, student->average_mark);
}
