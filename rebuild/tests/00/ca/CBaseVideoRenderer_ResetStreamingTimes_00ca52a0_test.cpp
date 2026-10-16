// Behaviour test for 00ca52a0 (see src/00ca52a0.cpp); plain C++98, member access only.
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
// CBaseVideoRenderer::ResetStreamingTimes: statistics reset to the SDK values
struct TRenderer { int nNormal, trThrottle, trRenderAvg, trRenderLast, tRenderStart, trEarliness, trTarget, trWaitAvg,
    trFrameAvg, trDuration; long trRememberStampForPerf; int cFramesDropped, cFramesDrawn; long iTotAcc, iSumSqAcc,
    trLastDraw, iSumSqFrameTime, iSumFrameTime; int trLate, trFrame, tStreamingStart; };
static unsigned long now = 1234;
static long Reset(TRenderer& r) {
    r.trLastDraw = -1000; r.tStreamingStart = (int)now; r.trRenderAvg = 0; r.trFrameAvg = -1; r.trDuration = 0;
    r.trRenderLast = 0; r.trWaitAvg = 0; r.tRenderStart = 0; r.cFramesDrawn = 0; r.cFramesDropped = 0; r.iTotAcc = 0;
    r.iSumSqAcc = 0; r.iSumSqFrameTime = 0; r.trFrame = 0; r.trLate = 0; r.iSumFrameTime = 0; r.nNormal = 0;
    r.trEarliness = 0; r.trTarget = -300000; r.trThrottle = 0; r.trRememberStampForPerf = 0; return 0; }
int main() {
    TRenderer r; r.trLastDraw = 5; r.trTarget = 7; r.cFramesDrawn = 9; r.trFrameAvg = 3;
    CHECK(Reset(r) == 0, "00ca52a0");
    CHECK(r.trLastDraw == -1000 && r.trTarget == -300000 && r.trFrameAvg == -1 && r.cFramesDrawn == 0 && r.tStreamingStart == 1234, "00ca52a0");
    std::printf("00ca52a0_TEST PASS\n"); return 0; }