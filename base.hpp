#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>
using namespace std;

class object {
protected:
    string contain;
public:
    object() = default;
    object(string _contain_) : contain(_contain_) {}

    virtual void copy(object* o) = 0;
    virtual string send_status(int y, int m, int d) = 0;
    virtual string type() = 0;

    virtual void print() {
        cout << "[object] contain: " << contain << endl;
    }

    virtual ~object() {}
};

#endif
