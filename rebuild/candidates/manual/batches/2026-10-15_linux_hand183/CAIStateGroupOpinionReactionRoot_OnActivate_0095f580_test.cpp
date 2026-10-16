// Behaviour test for 0095f580 (see src/0095f580.cpp); plain C++98, member access only.
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
// OpinionReactionRoot::OnActivate: finish a long non-looping action, log the reaction, take its speech flag, cache CTCTalk
struct TAction { long LoopCount; long remaining; };
static int finishedAction = 0, logged = 0; static float maxSeconds = 2.0f; static long fps = 30;
struct TManager { bool allow[4]; };
struct TGroup { TThing* creature; TAction* action; TManager* mgr; void* PTCTalk; long CurrentReactionType; TPtr ThingReactingTo;
    bool AllowSpeech;
    void OnActivate() {
        if (action != 0) { long f = fps; if (action->remaining > f * maxSeconds && action->LoopCount <= 0) ++finishedAction; }
        AllowSpeech = false;
        if (CurrentReactionType != 0 && ThingReactingTo.GetPItem() != 0) { ++logged; AllowSpeech = mgr->allow[CurrentReactionType]; }
        creature->GetInterface(&PTCTalk, 0xf); } };
int main() {
    TThing me, hero; int talk = 0; me.Add(0xf, &talk); TAction a; a.LoopCount = 0; a.remaining = 100;
    TManager m; m.allow[0] = false; m.allow[1] = false; m.allow[2] = true; m.allow[3] = false;
    TGroup g; g.creature = &me; g.action = &a; g.mgr = &m; g.PTCTalk = 0; g.CurrentReactionType = 2; g.ThingReactingTo.SetPItem(&hero);
    g.OnActivate(); CHECK(finishedAction == 1 && logged == 1 && g.AllowSpeech && g.PTCTalk == &talk, "0095f580");
    a.remaining = 10; g.CurrentReactionType = 0; g.OnActivate(); CHECK(finishedAction == 1 && logged == 1 && !g.AllowSpeech, "0095f580");
    std::printf("0095f580_TEST PASS\n"); return 0; }
