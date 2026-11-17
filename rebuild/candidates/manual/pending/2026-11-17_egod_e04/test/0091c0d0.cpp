// Behaviour test for CAIStateGroup_NymphFloatAround::IsRunnable (0x0091C0D0): when the creature lacks the nymph component
// (interface 245) the state adds "CTCNymph" (no params, not loading) and then reports whether the component is present now;
// a creature that already has it gets no AddTC.
#include <cstdio>
#include <cstring>
struct Thing {
    unsigned long UsingComponents[9];
    int adds; bool add_works; char last_name[32]; bool last_flag; const void* last_params;
    bool HasTC(unsigned t) const { return (UsingComponents[t / 32] & (1ul << (t % 32))) != 0; }
    void* AddTC(const char* name, bool flag, const void* params)
    {
        ++adds; std::strcpy(last_name, name); last_flag = flag; last_params = params;
        if (add_works && std::strcmp(name, "CTCNymph") == 0) UsingComponents[245 / 32] |= 1ul << (245 % 32);
        return 0;
    }
};
struct State {
    Thing* creature;
    bool IsRunnable()
    {
        if (!creature->HasTC(245))
            creature->AddTC("CTCNymph", false, 0);
        return creature->HasTC(245);
    }
};
static int fail(int n) { std::printf("0091c0d0_TEST FAIL %d\n", n); return 1; }
int main()
{
    Thing t; std::memset(&t, 0, sizeof(t)); t.add_works = true; t.last_flag = true; t.last_params = &t;
    State s = { &t };
    if (!s.IsRunnable()) return fail(1);                                       // missing: added, now present
    if (t.adds != 1 || std::strcmp(t.last_name, "CTCNymph") != 0 || t.last_flag || t.last_params != 0) return fail(2);
    if (!s.IsRunnable() || t.adds != 1) return fail(3);                       // present: no second AddTC
    if ((t.UsingComponents[7] & 0x200000ul) == 0) return fail(4);
    Thing u; std::memset(&u, 0, sizeof(u)); u.add_works = false; u.UsingComponents[7] = ~0x200000ul;
    State r = { &u };
    if (r.IsRunnable() || u.adds != 1) return fail(5);                         // AddTC fails: re-test says not runnable
    std::printf("0091c0d0_TEST PASS\n");
    return 0;
}
