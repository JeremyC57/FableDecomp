extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
static int cmp(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a - *b; }
class CAIStateGroupBase { public: const char* Name; bool Enabled; };
struct SPair { long first; CAIStateGroupBase* second; };
class CAIBrain { public:
    SPair* first; SPair* last;
    bool IsStateGroupPresentAndEnabled(const char* name) const
    {
        for (SPair* it = first; it != last; ++it) if (cmp(it->second->Name, name) == 0) return it->second->Enabled;
        return false;
    }
    void SetStateGroupEnabled(const char* name, bool enabled)
    {
        for (SPair* it = first; it != last; ++it) if (cmp(it->second->Name, name) == 0) { it->second->Enabled = enabled; return; }
    }
};
// Behaviour test for 0x0088B9E0 CAIBrain::SetStateGroupEnabled: only the first state group with the name is changed.
int main()
{
    CAIStateGroupBase g1 = { "Combat", false }, g2 = { "Idle", true }, g3 = { "Idle", true };
    SPair v[3] = { { 9, &g1 }, { 5, &g2 }, { 1, &g3 } };
    CAIBrain b; b.first = v; b.last = v + 3;
    b.SetStateGroupEnabled("Idle", false); CHECK(!g2.Enabled && g3.Enabled);
    b.SetStateGroupEnabled("Combat", true); CHECK(g1.Enabled);
    b.SetStateGroupEnabled("Flee", false); CHECK(g1.Enabled && g3.Enabled);
    if (fails == 0) printf("0088b9e0_TEST PASS\n"); else printf("0088b9e0_TEST FAIL\n");
    return fails;
}