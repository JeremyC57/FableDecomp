// behaviour test for 0x00953A80 CCreatureAction_DragonRearUpFull::Clone() const: a new object of the same dynamic type whose base part and PShot member are copies of the source's (base first, then the member), the source untouched
#include <cstdio>
static int gOrder = 0, gBaseOrder = 0, gMemOrder = 0;
struct Member {
    int Value;
    Member() : Value(0) { }
    Member(const Member& o) : Value(o.Value) { gMemOrder = ++gOrder; }
};
struct Base {
    int State;
    Base() : State(0) { }
    Base(const Base& o) : State(o.State) { gBaseOrder = ++gOrder; }
    virtual ~Base() { }
    virtual int Kind() const { return 0; }
    virtual Base* Clone() const = 0;
};
struct Action : Base {
    Member M;
    Action() { }
    virtual int Kind() const { return 6; }
    virtual Base* Clone() const { return new Action(*this); }   // the function under test
};
int main()
{
    int fail = 0;
    Action src; src.State = 7; src.M.Value = 42;
    gOrder = 0;
    Base* c = src.Clone();
    if (c == 0 || c == &src) fail |= 1;
    if (c && c->Kind() != 6) fail |= 2;
    if (c && (c->State != 7 || static_cast<Action*>(c)->M.Value != 42)) fail |= 4;
    if (gBaseOrder != 1 || gMemOrder != 2) fail |= 8;
    if (src.State != 7 || src.M.Value != 42) fail |= 16;
    delete c;
    if (fail) { std::printf("00953a80_TEST FAIL %d\n", fail); return 1; }
    std::printf("00953a80_TEST PASS\n");
    return 0;
}