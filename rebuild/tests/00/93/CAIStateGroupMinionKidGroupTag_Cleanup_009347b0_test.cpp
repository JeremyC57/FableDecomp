// Behaviour test for 009347b0 (see src/009347b0.cpp); plain C++98, member access only.
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
// KidGroupTag::Cleanup: leave the tag game, drop mode 1 if added, FrameLastPlayedIn = now + random(2 s of frames)
static unsigned long ROR13(unsigned long v) { return (v >> 13) | (v << 19); }
struct TTag { TThing* removed; void RemoveMember(TThing* c) { removed = c; } };
struct TModes { long removedMode; void RemoveMode(long m) { removedMode = m; } };
static long fps = 30, frame = 1000;
struct TGroup { TThing* creature; TTag* PTCGroupTag; bool StandModeAddedToCreature; long FrameLastPlayedIn; unsigned long seed;
    unsigned long RandomFramesWithin(long seconds) {
        unsigned long range = fps * seconds; unsigned long next = seed * 0x24A1U + 0x24DFU; seed = next;
        unsigned long rot = ROR13(next); seed = rot; return range ? rot % range : 0; }
    void Cleanup() {
        PTCGroupTag->RemoveMember(creature);
        if (StandModeAddedToCreature) { TModes* m = 0; creature->GetInterface(&m, 0x31); m->RemoveMode(1); }
        FrameLastPlayedIn = frame + RandomFramesWithin(2); } };
int main() {
    TThing me; TTag tag; tag.removed = 0; TModes modes; modes.removedMode = 0; me.Add(0x31, &modes);
    TGroup g; g.creature = &me; g.PTCGroupTag = &tag; g.StandModeAddedToCreature = true; g.seed = 7;
    g.Cleanup();
    CHECK(tag.removed == &me && modes.removedMode == 1, "009347b0");
    CHECK(g.FrameLastPlayedIn >= 1000 && g.FrameLastPlayedIn < 1060 && g.seed != 7, "009347b0");
    std::printf("009347b0_TEST PASS\n"); return 0; }