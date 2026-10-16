
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#include <clocale>

using namespace std;

class Abonent
{
private:
    string pib;
    string homePhone;
    string workPhone;
    string mobilePhone;
    string info;

    static int countAbonent;

public:
    // Конструктор за замовчуванням
    Abonent() : Abonent("", "", "", "", "") {}

    // Конструктор з параметрами
    Abonent(string p, string h, string w, string m, string i)
        : pib(p), homePhone(h), workPhone(w),
        mobilePhone(m), info(i)
    {
        countAbonent++;
    }

    // Конструктор копіювання
    Abonent(const Abonent& a)
        : pib(a.pib),
        homePhone(a.homePhone),
        workPhone(a.workPhone),
        mobilePhone(a.mobilePhone),
        info(a.info)
    {
        countAbonent++;
    }

    // Оператор присвоєння
    Abonent& operator=(const Abonent& a)
    {
        if (this != &a)
        {
            pib = a.pib;
            homePhone = a.homePhone;
            workPhone = a.workPhone;
            mobilePhone = a.mobilePhone;
            info = a.info;
        }
        return *this;
    }

    // Деструктор
    ~Abonent()
    {
        countAbonent--;
    }

    void SetData()
    {
        cout << "Введіть ПІБ: ";
        getline(cin >> ws, pib);

        cout << "Домашній телефон: ";
        getline(cin, homePhone);

        cout << "Робочий телефон: ";
        getline(cin, workPhone);

        cout << "Мобільний телефон: ";
        getline(cin, mobilePhone);

        cout << "Додаткова інформація: ";
        getline(cin, info);
    }

    void Show() const
    {
        cout << "\nПІБ: " << pib << '\n';
        cout << "Домашній телефон: " << homePhone << '\n';
        cout << "Робочий телефон: " << workPhone << '\n';
        cout << "Мобільний телефон: " << mobilePhone << '\n';
        cout << "Додаткова інформація: " << info << '\n';
        cout << "-----------------------------\n";
    }

    bool SearchByName(string name) const
    {
        return pib.find(name) != string::npos;
    }

    bool HasPhone(string phone) const
    {
        return homePhone == phone ||
            workPhone == phone ||
            mobilePhone == phone;
    }

    void Save(ofstream& file) const
    {
        file << pib << '\n'
            << homePhone << '\n'
            << workPhone << '\n'
            << mobilePhone << '\n'
            << info << '\n';
    }

    bool Load(ifstream& file)
    {
        if (!getline(file, pib)) return false;
        if (!getline(file, homePhone)) return false;
        if (!getline(file, workPhone)) return false;
        if (!getline(file, mobilePhone)) return false;
        if (!getline(file, info)) return false;

        return true;
    }

    static int GetCount()
    {
        return countAbonent;
    }
};

int Abonent::countAbonent = 0;

class PhoneBook
{
private:
    vector<Abonent> abonents;

public:
    void Add(const Abonent& a)
    {
        abonents.push_back(a);
        cout << "Абонента додано!\n";
    }

    void Delete(string phone)
    {
        for (int i = 0; i < (int)abonents.size(); i++)
        {
            if (abonents[i].HasPhone(phone))
            {
                abonents.erase(abonents.begin() + i);
                cout << "Абонента видалено!\n";
                return;
            }
        }

        cout << "Абонента не знайдено!\n";
    }

    void SearchByName(string name) const
    {
        bool found = false;

        for (const Abonent& a : abonents)
        {
            if (a.SearchByName(name))
            {
                a.Show();
                found = true;
            }
        }

        if (!found)
            cout << "Абонента не знайдено!\n";
    }

    void SearchByPhone(string phone) const
    {
        bool found = false;

        for (const Abonent& a : abonents)
        {
            if (a.HasPhone(phone))
            {
                a.Show();
                found = true;
            }
        }

        if (!found)
            cout << "Абонента не знайдено!\n";
    }

    void ShowAll() const
    {
        if (abonents.empty())
        {
            cout << "Телефонна книга порожня!\n";
            return;
        }

        for (const Abonent& a : abonents)
            a.Show();
    }

    void SaveToFile() const
    {
        ofstream file("PhoneBook.txt");

        if (!file.is_open())
        {
            cout << "Помилка відкриття файлу!\n";
            return;
        }

        file << abonents.size() << '\n';

        for (const Abonent& a : abonents)
            a.Save(file);

        file.close();
        cout << "Дані збережено у файл!\n";
    }

    void LoadFromFile()
    {
        ifstream file("PhoneBook.txt");

        if (!file.is_open())
        {
            cout << "Файл PhoneBook.txt не знайдено!\n";
            return;
        }

        int n;

        if (!(file >> n) || n < 0)
        {
            cout << "Помилка читання файлу!\n";
            return;
        }

        file.ignore(10000, '\n');

        vector<Abonent> loaded;

        for (int i = 0; i < n; i++)
        {
            Abonent a;

            if (!a.Load(file))
            {
                cout << "Помилка читання даних!\n";
                return;
            }

            loaded.push_back(a);
        }

        abonents = loaded;
        file.close();

        cout << "Дані завантажено з файлу!\n";
    }
};

int main()
{
    // Налаштування кодування консолі UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    SetConsoleTitleW(L"Телефонна книга");

    PhoneBook book;
    int choice;

    do
    {
        cout << "\n===== ТЕЛЕФОННА КНИГА =====\n";
        cout << "1. Додати абонента\n";
        cout << "2. Видалити абонента за телефоном\n";
        cout << "3. Пошук за ПІБ\n";
        cout << "4. Пошук за телефоном\n";
        cout << "5. Показати всіх абонентів\n";
        cout << "6. Зберегти у файл\n";
        cout << "7. Завантажити з файлу\n";
        cout << "8. Кількість абонентів у книзі\n";
        cout << "0. Вихід\n";
        cout << "Ваш вибір: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Введіть число!\n";
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            Abonent a;
            a.SetData();
            book.Add(a);
            break;
        }

        case 2:
        {
            string phone;
            cout << "Введіть номер телефону: ";
            getline(cin >> ws, phone);
            book.Delete(phone);
            break;
        }

        case 3:
        {
            string name;
            cout << "Введіть ПІБ або його частину: ";
            getline(cin >> ws, name);
            book.SearchByName(name);
            break;
        }

        case 4:
        {
            string phone;
            cout << "Введіть номер телефону: ";
            getline(cin >> ws, phone);
            book.SearchByPhone(phone);
            break;
        }

        case 5:
            book.ShowAll();
            break;

        case 6:
            book.SaveToFile();
            break;

        case 7:
            book.LoadFromFile();
            break;

        case 8:
            cout << "Кількість абонентів у книзі: "
                << Abonent::GetCount() << '\n';
            break;

        case 0:
            cout << "Завершення програми.\n";
            break;

        default:
            cout << "Неправильний вибір!\n";
        }

    } while (choice != 0);

    return 0;
}

