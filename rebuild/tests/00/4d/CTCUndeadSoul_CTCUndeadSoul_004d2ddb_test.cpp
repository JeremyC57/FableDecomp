// Behaviour model of CTCUndeadSoul::CTCUndeadSoul(CThing&) 0x004D2DDB: the CTCBase part is built for the owner thing (once), then the
// object is a CTCUndeadSoul (its own vtable: the virtual calls reach the derived overrides).
#include <cstdio>
struct Thing { int id; };
static Thing* g_owner = 0; static int g_base_ctors = 0;
struct TCBase {
    Thing* Owner;
    TCBase(Thing& thing) : Owner(&thing) { g_owner = &thing; ++g_base_ctors; }
    virtual ~TCBase() { }
    virtual int GetInterfaceType() const = 0;
};
struct TCUndeadSoul : TCBase {
    TCUndeadSoul(Thing& thing) : TCBase(thing) { }
    virtual int GetInterfaceType() const { return 77; }
};
int main()
{
    Thing t = { 5 };
    TCUndeadSoul soul(t);
    const TCBase& b = soul;
    if (g_owner != &t || g_base_ctors != 1 || soul.Owner != &t) { std::printf("004d2ddb_TEST FAIL base\n"); return 1; }
    if (b.GetInterfaceType() != 77) { std::printf("004d2ddb_TEST FAIL vtable\n"); return 1; }
    std::printf("004d2ddb_TEST PASS\n");
    return 0;
}
