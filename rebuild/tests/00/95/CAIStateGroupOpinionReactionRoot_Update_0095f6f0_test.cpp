// Behaviour test for 0095f6f0 (see src/0095f6f0.cpp); plain C++98, member access only.
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
// OpinionReactionRoot::Update: run the current member-function update; mark handled; wait while talking; finish once
struct TOpinion { bool ReactionHandled; };
static int talking = 2, finished = 0, yields = 0; static bool valid = true, stopping = false;
struct TGroup { TThing* creature; bool (TGroup::*CurrentUpdateFunc)(); bool ok;
    bool Step() { return ok; }
    void Update() {
        if (!valid) { ++finished; return; }
        if (CurrentUpdateFunc != 0) {
            if (!(this->*CurrentUpdateFunc)()) { ++finished; return; }
            TOpinion* o = 0; creature->GetInterface(&o, 0x4f); o->ReactionHandled = true; }
        while (talking > 0) { if (stopping) break; --talking; ++yields; }
        ++finished; } };
int main() {
    TThing me; TOpinion op; op.ReactionHandled = false; me.Add(0x4f, &op);
    TGroup g; g.creature = &me; g.CurrentUpdateFunc = &TGroup::Step; g.ok = true;
    g.Update(); CHECK(op.ReactionHandled && yields == 2 && finished == 1, "0095f6f0");
    g.ok = false; op.ReactionHandled = false; g.Update(); CHECK(!op.ReactionHandled && finished == 2, "0095f6f0");
    valid = false; g.Update(); CHECK(finished == 3, "0095f6f0");
    std::printf("0095f6f0_TEST PASS\n"); return 0; }