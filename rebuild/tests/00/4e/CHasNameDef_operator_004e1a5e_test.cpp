// behaviour test for 0x004E1A5E CHasNameDef::operator=: the base part is assigned exactly once through the base's operator= (with the source), the three own members are copied, the object's dynamic type is untouched and *this is returned
#include <cstdio>
static int gBaseAssign = 0;
static const void* gBaseFrom = 0;
class Base {
public:
    Base() : Tag(1) { }
    virtual ~Base() { }
    virtual int Kind() const { return 1; }
    Base& operator=(const Base& other) { ++gBaseAssign; gBaseFrom = &other; Tag = other.Tag; return *this; }
    int Tag;
};
class Def : public Base {
public:
    virtual int Kind() const { return 2; }
    unsigned long      DefaultNameTag;                     // +0x28
    unsigned long      Home;
    unsigned long      Occupation;
};
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
static bool Same(float a, float b) { return Near(a, b); }
static bool Same(long a, long b) { return a == b; }
static bool Same(unsigned long a, unsigned long b) { return a == b; }
static bool Same(bool a, bool b) { return a == b; }
int main()
{
    int fails = 0;
    Def a, b;
    a.Tag = 5;
    a.DefaultNameTag = 0xFFFFFFF0;
    a.Home = 42;
    a.Occupation = 0xFFFFFFF0;
    b.DefaultNameTag = 42;
    b.Home = 0xFFFFFFF0;
    b.Occupation = 42;
    Def& r = (b = a);
    if (&r != &b) fails |= 1;
    if (gBaseAssign != 1 || gBaseFrom != &a || b.Tag != 5) fails |= 2;
    if (!Same(b.DefaultNameTag, a.DefaultNameTag)) fails |= 4;
    if (!Same(b.Home, a.Home)) fails |= 8;
    if (!Same(b.Occupation, a.Occupation)) fails |= 32;
    Base* pb = &b;
    if (pb->Kind() != 2) fails |= 16;
    if (fails) { std::printf("004e1a5e_TEST FAIL %d\n", fails); return 1; }
    std::printf("004e1a5e_TEST PASS\n");
    return 0;
}