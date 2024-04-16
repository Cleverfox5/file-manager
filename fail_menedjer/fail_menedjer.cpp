#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <filesystem>
#include <vector>
#include <string>

using namespace std;

HANDLE hConsole;

void new_int(char massiv_char[5]) {
    bool fl = true;
    char ch;
    int i = 0;
    while (fl == true) {
        ch = _getch();
        if ((ch == 13)) {
            break;
        }
        else if (ch == 8) {
            if (i > 0) {
                cout << "\b";
                cout << " ";
                cout << "\b";
                massiv_char[i - 1] = 555;
                i--;
                continue;
            }
        }
        else {
            if (((ch >= '0') && (ch <= '9'))) {
                cout << (char)ch;
                massiv_char[i] = (char)ch;
                i++;
            }
        }
        if ((i == 5)) {
            break;
        }
    }
}
void gotoxy(int x, int y, int color) {
    SetConsoleTextAttribute(hConsole, color);
    COORD p = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}
void len_dir(string puti, int& dlin, vector <bool>& is_file) {
    dlin = 0;
    for (filesystem::directory_entry element : filesystem::directory_iterator(puti))
    {
        if (element.is_directory()) {
            is_file.push_back(false);
        }
        else {
            is_file.push_back(true);
        }
        dlin++;
    }
}
void del_is_file(int& dlin, vector <bool>& is_file) {
    for (int i = 0; i < dlin; i++)
    {
        is_file.pop_back();
    }
}
void preobrazovanie_name(string name_f) {
    int last_shift_position = 0;
    int dlin_str = name_f.size();
    for (int i = 0; i < dlin_str; i++) {
        if ((name_f[i] == '/') || (name_f[i] == 92)) {
            last_shift_position = i;
        }
    }
    for (int i = 0; i < (dlin_str - last_shift_position); i++) {
        cout << name_f[(last_shift_position + 1) + i];
    }
}
void paint() {
    gotoxy(0, 25, 7);
    for (int i = 0; i < 120; i++)
        cout << "/";
    gotoxy(0, 26, 7);
    for (int i = 0; i < 120; i++)
        cout << "/";
    for (int i = 0; i < 25; i++) {
        gotoxy(115, i, 7);
        cout << "/////";
    }
}
void vivod_massiva_string(string massiv_element[], int max_count_vivedeno, int tek_paper, int dlin, int count_papper) {
    system("cls");
    const int start_s = tek_paper * max_count_vivedeno;
    if (tek_paper == (count_papper - 1)) {
        for (int i = start_s; i < start_s + (dlin % max_count_vivedeno); i++) {
            cout << " ";
            preobrazovanie_name(massiv_element[i]);
            cout << endl;
        }
    }
    else {
        for (int i = start_s; i < max_count_vivedeno + start_s; i++) {
            cout << " ";
            preobrazovanie_name(massiv_element[i]);
            cout << endl;
        }
    }
    paint();
}
bool kursor(string& puti, int dlin, string massiv_element[], vector <string>& history, bool& first, vector <bool>& is_file) {
    int y = 0;
    bool klik = true;
    char ch = 0;
    const int count_vivedeno = 25;
    int count_papper = ceil((float)dlin / (float)count_vivedeno);
    int tek_paper = 0;
    vivod_massiva_string(massiv_element, count_vivedeno, tek_paper, dlin, count_papper);
    gotoxy(0, y, 7);
    cout << ">";
    while (klik) {
        ch = _getch();
        if ((ch == 0x48)) { //вверх
            if (y > 0) {
                cout << "\b";
                cout << " ";
                y--;
                gotoxy(0, y, 7);
                cout << ">";
            }
            else if (tek_paper != 0) {
                tek_paper--;
                vivod_massiva_string(massiv_element, count_vivedeno, tek_paper, dlin, count_papper);
                y = 24;
                gotoxy(0, y, 7);
                cout << ">";
            }
        }
        if (ch == 0x50) { //вниз
            if (tek_paper == (count_papper - 1)) {
                if (y < ((dlin % count_vivedeno) - 1)) {
                    if (y < (count_vivedeno - 1)) {
                        cout << "\b";
                        cout << " ";
                        y++;
                        gotoxy(0, y, 7);
                        cout << ">";
                    }
                    else if (tek_paper < (count_papper - 1)) {
                        tek_paper++;
                        vivod_massiva_string(massiv_element, count_vivedeno, tek_paper, dlin, count_papper);
                        y = 0;
                        gotoxy(0, y, 7);
                        cout << ">";
                    }
                }
            }
            else if (y < count_vivedeno) {
                if (y < (count_vivedeno - 1)) {
                    cout << "\b";
                    cout << " ";
                    y++;
                    gotoxy(0, y, 7);
                    cout << ">";
                }
                else if (tek_paper < (count_papper - 1)) {
                    tek_paper++;
                    vivod_massiva_string(massiv_element, count_vivedeno, tek_paper, dlin, count_papper);
                    y = 0;
                    gotoxy(0, y, 7);
                    cout << ">";
                }
            }
        }
        if ((ch == 13) || (ch == 77)) {
            puti = massiv_element[y + tek_paper * count_vivedeno];
            history.push_back(puti);
            klik = false;
            if (is_file[y + tek_paper * count_vivedeno]) {
                del_is_file(dlin, is_file);
                return true;
            }
            else {
                del_is_file(dlin, is_file);
                return false;
            }
        }
        if ((ch == 75)) {
            if (history.size() == 1) {
                first = true;
                history.pop_back();
                del_is_file(dlin, is_file);
                klik = false;
                return false;
            }
            if (history.size() > 1) {
                puti = history[(history.size() - 1) - 1];
                del_is_file(dlin, is_file);
                history.pop_back();
                klik = false;
                return false;
            }
        }
    }
}
void perehod(string puti, string massiv_element[]) {
    int i = 0;
    for (filesystem::directory_entry element : filesystem::directory_iterator(puti))
    {
        massiv_element[i] = element.path().string();
        i++;
    }
}
int menu_vibor_lab(string& puti, vector <string>& history, bool& file) {
    system("cls");
    paint();
    gotoxy(0, 0, 7);
    cout << "<<<<<<<<<<Change_the_prorramm>>>>>>>>>>\n";
    cout << ">Convert wav file to histogram\n";
    cout << " Convrt to bmp file\n";
    cout << " Count of bites\n";
    int y = 1;
    char ch;
    gotoxy(0, y, 7);
    bool klick = true;
    while (klick) {
        ch = _getch();
        if ((ch == 0x48)) {
            if (y > 1) {
                cout << "\b";
                cout << " ";
                y--;
                gotoxy(0, y, 7);
                cout << ">";
            }
        }
        if (ch == 0x50) { //вниз
            if (y < 3) {
                cout << "\b";
                cout << " ";
                y++;
                gotoxy(0, y, 7);
                cout << ">";
            }
        }
        if ((ch == 13) || (ch == 77)) {
            klick = false;
        }
        if ((ch == 75)) {
            if (history.size() > 1) {
                puti = history[(history.size() - 1) - 1];
                history.pop_back();
                klick = false;
                file = false;
                y = 0;
            }
        }
    }


    return (y - 1);
}
int main()
{
    setlocale(0, "");
    bool fl = true;
    string puti = "D:/University/proga/labs/cursach";
    int dlin = 0;
    vector <string> history;
    vector <bool> is_file;
    bool first = true;
    bool file = false;
    int nomer_labi;

    while (fl) {
        if (!file) {
            if (first) {
                system("cls");
                paint();
                gotoxy(0, 0, 7);
                char ch;
                int y = 0;
                string* massiv_element = new string[2];
                massiv_element[0] = "C:/";
                massiv_element[1] = "D:/";
                cout << ">C:/\n";
                cout << " D:/\n";
                gotoxy(1, y, 7);
                bool klick = true;
                while (klick) {
                    ch = _getch();
                    if ((ch == 0x48)) {
                        if (y > 0) {
                            cout << "\b";
                            cout << " ";
                            y--;
                            gotoxy(0, y, 7);
                            cout << ">";
                        }
                    }
                    if (ch == 0x50) { //вниз
                        if (y < 1) {
                            cout << "\b";
                            cout << " ";
                            y++;
                            gotoxy(0, y, 7);
                            cout << ">";
                        }
                    }
                    if ((ch == 13) || (ch == 77)) {
                        puti = massiv_element[y];
                        history.push_back(puti);
                        first = false;
                        klick = false;
                    }
                }
                delete[] massiv_element;
            }
            else {
                system("cls");
                len_dir(puti, dlin, is_file);
                string* massiv_element = new string[dlin];
                perehod(puti, massiv_element);
                file = kursor(puti, dlin, massiv_element, history, first, is_file);
                delete[] massiv_element;
            }
        }
        if (file) {
            system("cls");
            paint();
            gotoxy(0, 0, 7);
            nomer_labi = menu_vibor_lab(puti, history, file);
            if (nomer_labi == (-1)) {
                continue;
            }
            string laba;
            string argument;
            bool one;
            switch (nomer_labi) {
            case 0:
                laba = "D:/University/proga/labs/laba_1/x64/Debug/laba_1.exe";
                one = true;
                break;
            case 1:
                laba = "D:/University/proga/labs/laba_2/x64/Debug/laba_2.exe";
                one = true;
                break;
            case 2:
                laba = "D:/University/proga/labs/laba_3/x64/Debug/laba_3.exe";
                char massiv_char[5] = {0,0,0,0,0};
                system("cls");
                paint();
                gotoxy(0, 0, 7);
                cout << "Enter a value( <= 5 char): ";
                new_int(massiv_char);
                argument = to_string(atoi(massiv_char));
                one = false;
                break;
            }
            if (one) {
                string command = laba + " " + puti;
                system(command.c_str());
            }
            else {
                string command = laba + " " + puti + " " + argument;
                system(command.c_str());
            }
            break;
        }
    }
    
    return 0;
}