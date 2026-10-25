// behaviour test for 0x00d8ab10 CQ_HerosOldHouseScript::OnPersist(CPersistContext&): the flags Helping, Helped, BootyDugUp are transferred, in
// that order, under their own names with default false; no other member is touched
#include <cstdio>
#include <cstring>
struct Ctx {
    int n; const char* names[4]; bool* slots[4]; bool defaults[4];
    void Transfer(const char* name, bool& v, const bool& d) { names[n] = name; slots[n] = &v; defaults[n] = d; ++n; v = !v; }
};
struct Script {
    bool flags[8];
    void OnPersist(Ctx& c)
    {
        c.Transfer("Helping", flags[0], false);
        c.Transfer("Helped", flags[1], false);
        c.Transfer("BootyDugUp", flags[5], false);
    }
};
int main()
{
    int fails = 0;
    Script s; for (int i = 0; i < 8; ++i) s.flags[i] = false;
    Ctx c; c.n = 0;
    s.OnPersist(c);
    if (c.n != 3) fails |= 1;
    if (std::strcmp(c.names[0], "Helping") || std::strcmp(c.names[1], "Helped") || std::strcmp(c.names[2], "BootyDugUp")) fails |= 2;
    if (c.slots[0] != &s.flags[0] || c.slots[1] != &s.flags[1] || c.slots[2] != &s.flags[5]) fails |= 4;
    if (c.defaults[0] || c.defaults[1] || c.defaults[2]) fails |= 8;
    int touched = 0; for (int i = 0; i < 8; ++i) touched += s.flags[i];
    if (touched != 3) fails |= 16;
    if (fails) { std::printf("00d8ab10_TEST FAIL %d\n", fails); return 1; }
    std::printf("00d8ab10_TEST PASS\n");
    return 0;
}