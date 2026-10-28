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
// Behaviour test for 0x0088B980 CAIBrain::IsStateGroupPresentAndEnabled: the first state group whose name matches decides
// (its Enabled flag); false when no group has the name.
int main()
{
    CAIStateGroupBase g1 = { "Combat", false }, g2 = { "Idle", true }, g3 = { "Idle", false };
    SPair v[3] = { { 9, &g1 }, { 5, &g2 }, { 1, &g3 } };
    CAIBrain b; b.first = v; b.last = v + 3;
    CHECK(b.IsStateGroupPresentAndEnabled("Idle"));
    CHECK(!b.IsStateGroupPresentAndEnabled("Combat"));
    CHECK(!b.IsStateGroupPresentAndEnabled("Flee"));
    b.last = v; CHECK(!b.IsStateGroupPresentAndEnabled("Idle"));
    if (fails == 0) printf("0088b980_TEST PASS\n"); else printf("0088b980_TEST FAIL\n");
    return fails;
}