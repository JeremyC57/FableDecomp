// Behaviour test for 00933c10 (see src/00933c10.cpp); plain C++98, member access only.
#include <cstdio>
// shared test model: a thing with an interface table (sorted type -> component), a dead bit, and an
// intelligent pointer; mirrors the HasTC / GetInterface / FindEntry shapes of the sources.
struct TThing {
    unsigned long bits[9];
    long types[8];
    void* comps[8];
    int count;
    unsigned char flags91;
    TThing() : count(0), flags91(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
    void Add(long type, void* comp) { types[count] = type; comps[count] = comp; ++count; bits[type >> 5] |= 1UL << (type & 31); }
    unsigned long HasTC(long type) const { return bits[type >> 5] & (1UL << (type & 31)); }
    void* Find(long type) const { for (int i = 0; i < count; ++i) if (types[i] == type) return comps[i]; return 0; }
    template<class T> bool GetInterface(T** out, long type) const { if (!HasTC(type)) return false; *out = (T*)Find(type); return true; }
};
struct TPtr {
    TThing* item;
    TPtr() : item(0) { }
    TThing* GetPItem() const { return item; }
    void SetPItem(TThing* t) { item = t; }
};
inline bool Alive(const TPtr& p) { return p.GetPItem() != 0 && !(p.GetPItem()->flags91 & 1); }
inline bool AliveT(const TThing* t) { return t != 0 && !(t->flags91 & 1); }
#define CHECK(c, addr) do { if (!(c)) { std::printf("%s_TEST FAIL\n", addr); return 1; } } while (0)
// MotherProtectFamily::IsRunnable: family component (8) names a threatened member the mother can see
struct TFamily { TThing* threatened; TThing* GetThreatenedFamilyMember() { return threatened; } };
static bool canSee = true;
static bool CanSee(TThing*, TThing*, bool) { return canSee; }
struct TGroup { TThing* creature; TPtr PFamilyMember;
    bool IsRunnable() {
        if (creature->HasTC(8)) {
            TThing* m = ((TFamily*)creature->Find(8))->GetThreatenedFamilyMember();
            if (m != 0 && CanSee(creature, m, false)) { PFamilyMember.SetPItem(m); return true; }
        }
        return false; } };
int main() {
    TThing mom, kid; TFamily fam; fam.threatened = &kid; TGroup g; g.creature = &mom;
    CHECK(!g.IsRunnable(), "00933c10");
    mom.Add(8, &fam); CHECK(g.IsRunnable() && g.PFamilyMember.GetPItem() == &kid, "00933c10");
    canSee = false; g.PFamilyMember.SetPItem(0); CHECK(!g.IsRunnable() && g.PFamilyMember.GetPItem() == 0, "00933c10");
    canSee = true; fam.threatened = 0; CHECK(!g.IsRunnable(), "00933c10");
    std::printf("00933c10_TEST PASS\n"); return 0; }