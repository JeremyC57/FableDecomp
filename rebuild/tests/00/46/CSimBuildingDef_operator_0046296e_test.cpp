// behaviour test for 0x0046296E CSimBuildingDef::operator=: the base part is assigned exactly once through the base's operator= (with the source), the three own members are copied, the object's dynamic type is untouched and *this is returned
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
    long               Property;                     // +0x3c
    long               WorkSpaces;
    long               LiveSpaces;
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
    a.Property = -7;
    a.WorkSpaces = 123456;
    a.LiveSpaces = -7;
    b.Property = 123456;
    b.WorkSpaces = -7;
    b.LiveSpaces = 123456;
    Def& r = (b = a);
    if (&r != &b) fails |= 1;
    if (gBaseAssign != 1 || gBaseFrom != &a || b.Tag != 5) fails |= 2;
    if (!Same(b.Property, a.Property)) fails |= 4;
    if (!Same(b.WorkSpaces, a.WorkSpaces)) fails |= 8;
    if (!Same(b.LiveSpaces, a.LiveSpaces)) fails |= 32;
    Base* pb = &b;
    if (pb->Kind() != 2) fails |= 16;
    if (fails) { std::printf("0046296e_TEST FAIL %d\n", fails); return 1; }
    std::printf("0046296e_TEST PASS\n");
    return 0;
}