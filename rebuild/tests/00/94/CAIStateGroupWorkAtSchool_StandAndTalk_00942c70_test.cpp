// Behaviour test for 00942c70 (see src/00942c70.cpp): WorkAtSchool::StandAndTalk: mode 0x1B while idling until the end frame, the group stops or the idle ends; plain C++98, member access only.
#include <cstdio>
#include <cstring>
// shared test model: a thing with an interface table (sorted type -> component), a dead bit, and an
// intelligent pointer; mirrors the HasTC / GetInterface / FindEntry shapes of the sources.
struct TThing {
    unsigned long bits[9];
    long types[8];
    void* comps[8];
    int count;
    unsigned char flags91;
    unsigned long flags6c;
    TThing() : count(0), flags91(0), flags6c(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
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
#define CHECK(c, addr) do { if (!(c)) { std::printf("%s_TEST FAIL\n", addr); return 1; } } while (0)

static long g_frame = 0; static long g_fps = 30;
long GFGetCurrentFrame() { return g_frame; }
struct TModes { int adds, removes; long last; bool AddMode(unsigned long m) { ++adds; last = (long)m; return true; } void RemoveMode(unsigned long m) { ++removes; last = (long)m; } };
struct TGroup { TThing* creature; int stopAfter; int calls; long result;
    long AIAction_DoNothing() { ++calls; ++g_frame; return calls >= stopAfter ? result : 4; }
    bool IsStopping() { return false; }
    bool StandAndTalk(float seconds) {
        long fps = g_fps;
        long endFrame = (long)(GFGetCurrentFrame() + fps * seconds);
        TModes* m; creature->GetInterface(&m, 0x31);
        m->AddMode(0x1b);
        long r = AIAction_DoNothing();
        while (r == 4 && GFGetCurrentFrame() <= endFrame && !IsStopping()) r = AIAction_DoNothing();
        m->RemoveMode(0x1b);
        return r == 2; } };
int main() {
    TThing me; TModes md; md.adds = md.removes = 0; md.last = 0; me.Add(0x31, &md);
    TGroup g; g.creature = &me; g.stopAfter = 3; g.calls = 0; g.result = 2;
    CHECK(g.StandAndTalk(10.0f) && g.calls == 3 && md.adds == 1 && md.removes == 1 && md.last == 0x1b, "00942c70");
    g.calls = 0; g.stopAfter = 1000; g_frame = 0; CHECK(!g.StandAndTalk(1.0f) && g.calls == 31, "00942c70");
    g.calls = 0; g.stopAfter = 1; g.result = 3; CHECK(!g.StandAndTalk(1.0f), "00942c70");
    std::printf("00942c70_TEST PASS\n"); return 0; }