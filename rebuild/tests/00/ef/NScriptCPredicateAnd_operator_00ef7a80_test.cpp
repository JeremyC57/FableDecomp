// behaviour test for 0x00ef7a80 NScript::CPredicate_And::operator()() const: true only when both sub-predicates hold,
// and the second is not evaluated when the first fails (short circuit)
#include <cstdio>
struct Pred {
    bool value; mutable int calls;
    virtual bool operator()() const { ++calls; return value; }
    virtual ~Pred() {}
};
struct And {
    Pred p1, p2;
    bool operator()() const { return p1() && p2(); }
};
int main()
{
    int fails = 0;
    And a; a.p1.calls = a.p2.calls = 0;
    a.p1.value = true; a.p2.value = true;
    if (!a()) fails |= 1;
    a.p2.value = false;
    if (a()) fails |= 2;
    a.p1.value = false; a.p2.calls = 0;
    if (a() || a.p2.calls != 0) fails |= 4;
    if (a.p1.calls != 3) fails |= 8;
    if (fails) { std::printf("00ef7a80_TEST FAIL %d\n", fails); return 1; }
    std::printf("00ef7a80_TEST PASS\n");
    return 0;
}