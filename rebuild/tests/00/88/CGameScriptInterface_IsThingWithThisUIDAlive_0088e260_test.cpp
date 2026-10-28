extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
// Behaviour test for 0x0088E260 CGameScriptInterface::IsThingWithThisUIDAlive: the thing manager's lookup by 64-bit UID must
// find a thing that is not dead.
class CThing { public: u64 uid; bool Dead; bool IsAlive() const { return !Dead; } };
class CThingManager { public: CThing* things; int n; CThing* GetPThingWithUID(u64 u) { for (int i = 0; i < n; ++i) if (things[i].uid == u) return &things[i]; return 0; } };
static bool IsThingWithThisUIDAlive(CThingManager& m, u64 uid) { CThing* t = m.GetPThingWithUID(uid); if (t && t->IsAlive()) return true; return false; }
int main()
{
    CThing v[2]; v[0].uid = (((u64)1 << 32) | 1); v[0].Dead = false; v[1].uid = 7; v[1].Dead = true;
    CThingManager m; m.things = v; m.n = 2;
    CHECK(IsThingWithThisUIDAlive(m, (((u64)1 << 32) | 1)));
    CHECK(!IsThingWithThisUIDAlive(m, 1));
    CHECK(!IsThingWithThisUIDAlive(m, 7));
    if (fails == 0) printf("0088e260_TEST PASS\n"); else printf("0088e260_TEST FAIL\n");
    return fails;
}