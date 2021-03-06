#include <iostream>

using namespace std;

struct Foo {
    Foo() { cout << "Foo construct" << endl; }
    ~Foo() { cout << "Foo destruct" << endl; }
    void greet() const { cout << "Foo greet" << endl; }
};

struct Demo1 {
    Demo1() { cout << "Demo1 construct" << endl; }
    ~Demo1() { cout << "Demo1 destruct" << endl; }

    void greet() const { foo_.greet(); }

    static Foo foo_;
};

// Foo Demo1::foo_;        // 无需引用，先于main构造Foo

template<typename T>
struct Demo {
    Demo() { cout << "Demo construct" << endl; }
    ~Demo() { cout << "Demo destruct" << endl; }

    // static bool init() { return true; }

    static Foo foo_;

    static T* getInstance() {
        static T obj;
        cout << "getInstance" << endl;
        // m_creater.do_nothing();
        return &obj;
    }
};

template<typename T>
Foo Demo<T>::foo_;      // 必须引用才能初始化, 也是先于main初始化

// static bool _init_dummy = Demo<int>::init(); // 必须指定具体类型

int main()
{
    cout << "main start" << endl;

    do {
        Demo<int> d; // 不会导致静态成员的构造
        // Demo<int>::getInstance();   // 不会导致静态成员的构造
        // Demo<int>::foo_.greet();    // 必须引用才会构建模板类的static成员
    } while (0);

    // do {
        // Demo1 d1;
        // d1.greet();
    // } while (0);

    return 0;
}

