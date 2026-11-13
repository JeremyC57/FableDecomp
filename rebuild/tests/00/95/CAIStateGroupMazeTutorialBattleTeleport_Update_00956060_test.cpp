#include <cstdio>
#include <vector>
#include <algorithm>
// Behaviour model of CAIStateGroup_MazeTutorialBattleTeleport::Update (0x00956060): filter candidate positions (keep those further
// than 2*EnflameRange from the target and further than 7 from the creature), retry random picks until navigable (removing failures),
// fall back to LastTeleportPos, teleport in place when no candidate survives; stamp frame / last pos; terminate.
struct V { float x, y, z; };
static float d2(const V& a, const V& b) { float dx=a.x-b.x, dy=a.y-b.y, dz=a.z-b.z; return dx*dx+dy*dy+dz*dz; }
struct Near { V c; float r; bool operator()(const V& p) const { return !(d2(p, c) > r*r); } };
static unsigned long g_seed = 1;
static unsigned long GFRandom(unsigned long n) { g_seed = g_seed*0x24a1 + 0x24df; return n ? g_seed % n : 0; }
static float g_wallX = 100.0f;
static bool Navigable(const V& p) { return p.x < g_wallX; }
struct Teleporter {
    long FrameOfLastTeleport; V LastTeleportPos; int Terminates; V TeleportedTo;
    void Update(std::vector<V> v, const V& me, const V& target, float enflame, long frame) {
        V tp = me;
        Near n1 = { target, enflame * 2.0f };
        v.erase(std::remove_if(v.begin(), v.end(), n1), v.end());
        Near n2 = { me, 7.0f };
        v.erase(std::remove_if(v.begin(), v.end(), n2), v.end());
        if (v.size() == 0) { ++Terminates; }
        else for (;;) {
            unsigned long i = GFRandom((unsigned long)v.size());
            if (Navigable(v[i])) { tp = v[i]; break; }
            v.erase(v.begin() + i);
            if (v.size() == 0) { tp = LastTeleportPos; break; }
        }
        TeleportedTo = tp; FrameOfLastTeleport = frame; LastTeleportPos = tp; ++Terminates;
    }
};
int main()
{
    V me = {0,0,0}, target = {3,0,0};
    Teleporter t = { 0, {50,50,50}, 0, {0,0,0} };
    std::vector<V> v;
    V a = {1,0,0}, b = {-10,0,0}, c = {200,0,0};
    v.push_back(a); v.push_back(b); v.push_back(c);
    t.Update(v, me, target, 2.0f, 42);   // a: near target and me; c unreachable; b only choice
    if (t.TeleportedTo.x != -10.0f || t.FrameOfLastTeleport != 42 || t.LastTeleportPos.x != -10.0f || t.Terminates != 1) { std::printf("00956060_TEST FAIL\n"); return 1; }
    std::vector<V> w; w.push_back(c);
    t.Update(w, me, target, 2.0f, 43);   // only unreachable -> fall back to last pos
    if (t.TeleportedTo.x != -10.0f || t.FrameOfLastTeleport != 43 || t.Terminates != 2) { std::printf("00956060_TEST FAIL\n"); return 1; }
    std::vector<V> e; e.push_back(a);
    t.Update(e, me, target, 2.0f, 44);   // none survive -> terminate twice, teleport in place
    if (t.TeleportedTo.x != 0.0f || t.Terminates != 4 || t.LastTeleportPos.x != 0.0f) { std::printf("00956060_TEST FAIL\n"); return 1; }
    if (GFRandom(0) != 0) { std::printf("00956060_TEST FAIL\n"); return 1; }
    std::printf("00956060_TEST PASS\n");
    return 0;
}