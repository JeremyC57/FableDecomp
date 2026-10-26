// behaviour test for 0x004D63FB CTCObstacle::CTCObstacle(CThing&): the base part is built from the thing first, the member starts at zero, and the object is usable through its base
#include <cstdio>
struct Thing { int id; };
static int gBaseBuilt = 0;
struct Base { Thing* parent; bool toBeRemoved; Base(Thing& t) : parent(&t), toBeRemoved(false) { ++gBaseBuilt; } virtual ~Base() { } virtual int Kind() const { return 0; } };
struct Derived : Base {
    int member;
    Derived(Thing& t) : Base(t), member(0) { }             // the function under test
    virtual int Kind() const { return 1; }
};
int main()
{
    int fail = 0;
    Thing t; t.id = 7;
    Derived d(t);
    if (gBaseBuilt != 1 || d.parent != &t || d.toBeRemoved) fail |= 1;
    if (d.member != 0) fail |= 2;
    const Base& b = d;
    if (b.Kind() != 1) fail |= 4;
    if (fail) { std::printf("004d63fb_TEST FAIL %d\n", fail); return 1; }
    std::printf("004d63fb_TEST PASS\n");
    return 0;
}