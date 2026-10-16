// Behaviour test for 0090cb80 (see src/0090cb80.cpp); plain C++98, member access only.
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
// MinionGossip::IsStillValid: once relying on the gossipee, valid only while the partner is alive and targets us back
struct TGossip { TPtr PGossipTarget; };
struct TGroup { TThing* creature; bool NowRelyingOnGossipee; TGossip* PTCGossip; TGossip* PTCGossipOther;
    bool IsStillValid() {
        if (NowRelyingOnGossipee) {
            creature->GetInterface(&PTCGossip, 0xc1);
            TThing* other = PTCGossip->PGossipTarget.GetPItem();
            if (other != 0 && !(other->flags91 & 1)) {
                other->GetInterface(&PTCGossipOther, 0xc1);
                TThing* self = creature;
                if (PTCGossipOther->PGossipTarget.GetPItem() == self) return true;
            }
            return false;
        }
        return true; } };
int main() {
    TThing me, pal; TGossip mine, theirs; me.Add(0xc1, &mine); pal.Add(0xc1, &theirs);
    TGroup g; g.creature = &me; g.NowRelyingOnGossipee = false; g.PTCGossip = 0; g.PTCGossipOther = 0;
    CHECK(g.IsStillValid(), "0090cb80");
    g.NowRelyingOnGossipee = true; mine.PGossipTarget.SetPItem(&pal); theirs.PGossipTarget.SetPItem(&me);
    CHECK(g.IsStillValid() && g.PTCGossipOther == &theirs, "0090cb80");
    theirs.PGossipTarget.SetPItem(&pal); CHECK(!g.IsStillValid(), "0090cb80");
    theirs.PGossipTarget.SetPItem(&me); pal.flags91 = 1; CHECK(!g.IsStillValid(), "0090cb80");
    std::printf("0090cb80_TEST PASS\n"); return 0; }