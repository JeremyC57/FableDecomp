// Behaviour test for 0x00C57420 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct Base {
    unsigned int a, b; unsigned char result;
    virtual unsigned char CreateAndPlay(unsigned int x, unsigned int y) { a = x; b = y; return result; }
    virtual bool Create(unsigned int x) { return false; }
    virtual ~Base() {}
};
struct Derived : Base {
    virtual bool Create(unsigned int x)
    {
        if (!CreateAndPlay(1, x))
            return false;
        return true;
    }
};
int main()
{
    Derived d; d.result = 0;
    Base* p = &d;
    if (p->Create(77) || d.a != 1 || d.b != 77) { std::printf("00c57420_TEST FAIL\n"); return 1; }
    d.result = 5;
    if (!p->Create(3)) { std::printf("00c57420_TEST FAIL\n"); return 1; }
    std::printf("00c57420_TEST PASS\n");
    return 0;
}