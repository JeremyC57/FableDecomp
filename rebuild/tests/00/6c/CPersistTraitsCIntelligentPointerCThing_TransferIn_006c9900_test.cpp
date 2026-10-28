// Behaviour test for 0x006C9900 CPersistTraits<CIntelligentPointer<CThing> >::TransferIn: a zero UID leaves the value alone;
// a non-zero UID creates a placeholder thing, stores the UID in its serialisation component and points the value at it.
#include <cstdio>
typedef unsigned long long u64_t;
struct DummyTC { u64_t ThingUID; };
struct Thing { DummyTC Dummy; bool HasDummy; };
static Thing g_things[4]; static int g_created = 0;
Thing* CreateDummy() { Thing* t = &g_things[g_created++]; t->HasDummy = true; t->Dummy.ThingUID = 0; return t; }
struct Pointer { Thing* Item; };
void TransferIn(u64_t uid, Pointer& value)
{
    if (uid > 0) {
        Thing* thing = CreateDummy();
        DummyTC* dummy = 0;
        if (thing->HasDummy) dummy = &thing->Dummy;
        dummy->ThingUID = uid;
        value.Item = thing;
    }
}
int main()
{
    Pointer p = { 0 };
    TransferIn(0, p);
    bool ok = p.Item == 0 && g_created == 0;
    u64_t big = 0x12345678; big = (big << 32) | 0x9ABCDEF0u;
    TransferIn(big, p);
    ok = ok && p.Item == &g_things[0] && g_created == 1 && p.Item->Dummy.ThingUID == big;
    if (!ok) { std::printf("006c9900_TEST FAIL\n"); return 1; }
    std::printf("006c9900_TEST PASS\n");
    return 0;
}