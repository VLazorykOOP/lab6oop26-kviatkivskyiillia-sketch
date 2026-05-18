#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
using namespace std;

namespace NonVirtual {
    struct A { int dataA = 1; };
    struct B : public A { int dataB = 2; };
    struct C : public A { int dataC = 3; };
    struct D : public B, public C { int dataD = 4; };
    struct E : public B { int dataE = 5; };
    struct F : public D, public E { int dataF = 6; };
}

namespace Virtual {
    struct A { int dataA = 1; };
    struct B : virtual public A { int dataB = 2; };
    struct C : virtual public A { int dataC = 3; };
    struct D : public B, public C { int dataD = 4; };
    struct E : virtual public B { int dataE = 5; };
    struct F : public D, public E { int dataF = 6; };
}

void runTask1() {
    cout << "\n=== WITHOUT VIRTUAL INHERITANCE ===" << endl;
    {
        using namespace NonVirtual;
        A a; B b; C c; D d; E e; F f;
        cout << "sizeof(A) = " << sizeof(A) << " bytes" << endl;
        cout << "sizeof(B) = " << sizeof(B) << " bytes" << endl;
        cout << "sizeof(C) = " << sizeof(C) << " bytes" << endl;
        cout << "sizeof(D) = " << sizeof(D) << " bytes  <- A is duplicated twice! (B::A + C::A)" << endl;
        cout << "sizeof(E) = " << sizeof(E) << " bytes" << endl;
        cout << "sizeof(F) = " << sizeof(F) << " bytes  <- D + E (with all A duplicates)" << endl;
        cout << "\n  d.B::dataA = " << d.B::dataA << "  (subobject A via B)" << endl;
        cout << "  d.C::dataA = " << d.C::dataA << "  (subobject A via C)" << endl;
    }

    cout << "\n=== WITH VIRTUAL INHERITANCE ===" << endl;
    {
        using namespace Virtual;
        A a; B b; C c; D d; E e; F f;
        cout << "sizeof(A) = " << sizeof(A) << " bytes" << endl;
        cout << "sizeof(B) = " << sizeof(B) << " bytes  <- +vptr for virtual" << endl;
        cout << "sizeof(C) = " << sizeof(C) << " bytes  <- +vptr for virtual" << endl;
        cout << "sizeof(D) = " << sizeof(D) << " bytes  <- A only ONCE!" << endl;
        cout << "sizeof(E) = " << sizeof(E) << " bytes" << endl;
        cout << "sizeof(F) = " << sizeof(F) << " bytes" << endl;
        cout << "\n  d.dataA = " << d.dataA << "  (single shared subobject A)" << endl;
        cout << "  f.dataA = " << f.dataA << endl;
        cout << "  f.dataF = " << f.dataF << endl;
    }

    cout << "\n=== SIZE COMPARISON ===" << endl;
    cout << "Class | Without virtual | With virtual" << endl;
    cout << "------|-----------------|-------------" << endl;
    cout << "  A   |        " << sizeof(NonVirtual::A) << "        |      " << sizeof(Virtual::A) << endl;
    cout << "  B   |        " << sizeof(NonVirtual::B) << "        |      " << sizeof(Virtual::B) << endl;
    cout << "  C   |        " << sizeof(NonVirtual::C) << "        |      " << sizeof(Virtual::C) << endl;
    cout << "  D   |        " << sizeof(NonVirtual::D) << "       |      " << sizeof(Virtual::D) << endl;
    cout << "  E   |        " << sizeof(NonVirtual::E) << "       |      " << sizeof(Virtual::E) << endl;
    cout << "  F   |        " << sizeof(NonVirtual::F) << "       |      " << sizeof(Virtual::F) << endl;
}


class Progression {
protected:
    double a0;
    double step;
public:
    Progression(double a0, double step) : a0(a0), step(step) {}
    virtual double sum(int n) const = 0;
    virtual void printTerms(int n) const = 0;
    virtual ~Progression() {}
};

class ArithmeticProgression : public Progression {
public:
    ArithmeticProgression(double a0, double d) : Progression(a0, d) {}

    double term(int j) const { return a0 + j * step; }

    double sum(int n) const override {
        double an = term(n);
        return (n + 1) * (a0 + an) / 2.0;
    }

    void printTerms(int n) const override {
        cout << "Terms of arithmetic progression (j=0.." << n << "):" << endl;
        for (int j = 0; j <= n; j++) {
            cout << "  a[" << j << "] = " << fixed << setprecision(4) << term(j);
            if (j < n) cout << ",";
            cout << endl;
        }
    }
};

class GeometricProgression : public Progression {
public:
    GeometricProgression(double a0, double r) : Progression(a0, r) {}

    double term(int j) const { return a0 * pow(step, j); }

    double sum(int n) const override {
        if (abs(step - 1.0) < 1e-12) return (n + 1) * a0;
        double an = term(n);
        return (a0 - an * step) / (1.0 - step);
    }

    void printTerms(int n) const override {
        cout << "Terms of geometric progression (j=0.." << n << "):" << endl;
        for (int j = 0; j <= n; j++) {
            cout << "  a[" << j << "] = " << fixed << setprecision(4) << term(j);
            if (j < n) cout << ",";
            cout << endl;
        }
    }
};

void printSummary(const Progression* prog, int n) {
    prog->printTerms(n);
    cout << fixed << setprecision(4);
    cout << "  Sum s[" << n << "] = " << prog->sum(n) << endl;
}

void runTask2() {
    cout << "\n=======================================" << endl;
    cout << "   ARITHMETIC PROGRESSION  a0=2, d=3, n=5" << endl;
    cout << "=======================================" << endl;
    ArithmeticProgression ap(2.0, 3.0);
    printSummary(&ap, 5);

    cout << "\n=======================================" << endl;
    cout << "   GEOMETRIC PROGRESSION  a0=1, r=2, n=5" << endl;
    cout << "=======================================" << endl;
    GeometricProgression gp(1.0, 2.0);
    printSummary(&gp, 5);

    cout << "\n=======================================" << endl;
    cout << "   GEOMETRIC PROGRESSION  a0=100, r=0.5, n=4" << endl;
    cout << "=======================================" << endl;
    GeometricProgression gp2(100.0, 0.5);
    printSummary(&gp2, 4);

    cout << "\n=======================================" << endl;
    cout << "   POLYMORPHISM via base pointer" << endl;
    cout << "=======================================" << endl;
    Progression* progs[] = {
        new ArithmeticProgression(10.0, 5.0),
        new GeometricProgression(3.0, 3.0)
    };
    for (int i = 0; i < 2; i++) {
        cout << "\nProgression #" << i + 1 << ", n=4:" << endl;
        printSummary(progs[i], 4);
        delete progs[i];
    }
}

class Employee {
protected:
    string name;
    string position;
    double salary;
public:
    Employee(const string& name = "-",
        const string& position = "-",
        double salary = 0.0)
        : name(name), position(position), salary(salary)
    {
        cout << "[Employee] Constructor: " << name << endl;
    }

    Employee(const Employee& other)
        : name(other.name), position(other.position), salary(other.salary)
    {
        cout << "[Employee] Copy constructor: " << name << endl;
    }

    virtual ~Employee()
    {
        cout << "[Employee] Destructor: " << name << endl;
    }

    string getName()     const { return name; }
    string getPosition() const { return position; }
    double getSalary()   const { return salary; }

    friend ostream& operator<<(ostream& os, const Employee& e) {
        os << "Employee:"
            << "\n  Name:     " << e.name
            << "\n  Position: " << e.position
            << "\n  Salary:   " << e.salary << " USD";
        return os;
    }

    friend istream& operator>>(istream& is, Employee& e) {
        cout << "  Enter name: ";     is >> e.name;
        cout << "  Enter position: "; is >> e.position;
        cout << "  Enter salary: ";   is >> e.salary;
        return is;
    }
};

class Woman {
protected:
    string womanName;
    int    age;
public:
    Woman(const string& womanName = "-", int age = 0)
        : womanName(womanName), age(age)
    {
        cout << "[Woman] Constructor: " << womanName << endl;
    }

    Woman(const Woman& other)
        : womanName(other.womanName), age(other.age)
    {
        cout << "[Woman] Copy constructor: " << womanName << endl;
    }

    virtual ~Woman()
    {
        cout << "[Woman] Destructor: " << womanName << endl;
    }

    string getWomanName() const { return womanName; }
    int    getAge()       const { return age; }

    friend ostream& operator<<(ostream& os, const Woman& w) {
        os << "Woman:"
            << "\n  Name: " << w.womanName
            << "\n  Age:  " << w.age;
        return os;
    }

    friend istream& operator>>(istream& is, Woman& w) {
        cout << "  Enter name: "; is >> w.womanName;
        cout << "  Enter age: ";  is >> w.age;
        return is;
    }
};

class EmployeeWoman : public Employee, public Woman {
private:
    int yearsExperience;
public:
    EmployeeWoman(const string& name,
        const string& position,
        double salary,
        int age,
        int yearsExperience)
        : Employee(name, position, salary),
        Woman(name, age),
        yearsExperience(yearsExperience)
    {
        cout << "[EmployeeWoman] Constructor: " << name << endl;
    }

    EmployeeWoman(const EmployeeWoman& other)
        : Employee(other), Woman(other),
        yearsExperience(other.yearsExperience)
    {
        cout << "[EmployeeWoman] Copy constructor: " << other.name << endl;
    }

    virtual ~EmployeeWoman()
    {
        cout << "[EmployeeWoman] Destructor: " << name << endl;
    }

    friend ostream& operator<<(ostream& os, const EmployeeWoman& ew) {
        os << "Employee-Woman:"
            << "\n  Name:       " << ew.name
            << "\n  Position:   " << ew.position
            << "\n  Salary:     " << ew.salary << " USD"
            << "\n  Age:        " << ew.age
            << "\n  Experience: " << ew.yearsExperience << " yr.";
        return os;
    }

    friend istream& operator>>(istream& is, EmployeeWoman& ew) {
        cout << "  Enter name: ";           is >> ew.name;
        ew.womanName = ew.name;
        cout << "  Enter position: ";       is >> ew.position;
        cout << "  Enter salary: ";         is >> ew.salary;
        cout << "  Enter age: ";            is >> ew.age;
        cout << "  Enter experience (yr.): "; is >> ew.yearsExperience;
        return is;
    }
};

void runTask3() {
    cout << "\n=======================================" << endl;
    cout << "   1. Creating objects" << endl;
    cout << "=======================================" << endl;
    Employee      e("John Smith", "Engineer", 32000.0);
    Woman         w("Mary Johnson", 28);
    EmployeeWoman ew("Anna Brown", "Manager", 45000.0, 34, 7);

    cout << "\n=======================================" << endl;
    cout << "   2. Output (operator<<)" << endl;
    cout << "=======================================" << endl;
    cout << "\n" << e << "\n" << endl;
    cout << w << "\n" << endl;
    cout << ew << "\n" << endl;

    cout << "=======================================" << endl;
    cout << "   3. Copy constructor" << endl;
    cout << "=======================================" << endl;
    Employee      e2(e);
    Woman         w2(w);
    EmployeeWoman ew2(ew);
    cout << "\nCopy of Employee:\n" << e2 << "\n" << endl;
    cout << "Copy of Woman:\n" << w2 << "\n" << endl;
    cout << "Copy of EmployeeWoman:\n" << ew2 << "\n" << endl;

    cout << "=======================================" << endl;
    cout << "   4. Polymorphism via Employee*" << endl;
    cout << "=======================================" << endl;
    Employee* ptr = &ew;
    cout << "\nVia Employee*: " << ptr->getName()
        << ", " << ptr->getPosition()
        << ", " << ptr->getSalary() << " USD" << endl;

    cout << "\n=======================================" << endl;
    cout << "   End of Task 3 - destructors:" << endl;
    cout << "=======================================" << endl;
}


void printMenu() {
    cout << "\n+----------------------------------+" << endl;
    cout << "|         SELECT TASK              |" << endl;
    cout << "+----------------------------------+" << endl;
    cout << "|  1 - Task 1: Virtual inherit.    |" << endl;
    cout << "|  2 - Task 2: Progressions        |" << endl;
    cout << "|  3 - Task 3: Employee / Woman    |" << endl;
    cout << "|  0 - Exit                        |" << endl;
    cout << "+----------------------------------+" << endl;
    cout << "Your choice: ";
}

int main() {
    int choice;
    do {
        printMenu();
        cin >> choice;

        switch (choice) {
        case 1: runTask1(); break;
        case 2: runTask2(); break;
        case 3: runTask3(); break;
        case 0: cout << "\nGoodbye!\n"; break;
        default: cout << "\nInvalid choice! Try again.\n"; break;
        }

    } while (choice != 0);

    return 0;
}