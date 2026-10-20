// behaviour test for CAIStateGroup_LightHomeCandle::IsRunnable (0x00946B50): a copy of the logic over plain stubs
#include <cstdio>
struct Thing { unsigned char flags91; };
struct Material { long count; unsigned long flags; int destroyed; };
struct DefPtr {
    Material* obj;
    DefPtr(Material* m) : obj(m) { if (obj) obj->count++; }
    DefPtr(const DefPtr& o) : obj(o.obj) { if (obj) obj->count++; }
    ~DefPtr() { if (obj) { --obj->count; if (obj->count == 0) obj->destroyed++; } }
};
struct Group {
    Thing* building; bool workCandle; Material* material; Thing* found; Thing* candle; bool needs;
    int findCalls;
    DefPtr PeekPMaterial() { return DefPtr(material); }
    Thing* FindAndValidateTask(Thing*) { findCalls++; return found; }
    bool IsCandleInNeedOfChanging(Thing*) { return needs; }
    bool IsRunnable()
    {
        if (building && !(building->flags91 & 1)) {
            if (!workCandle) {
                DefPtr m = PeekPMaterial();
                if (m.obj && (m.obj->flags & 0x20))
                    return false;
            }
            candle = FindAndValidateTask(building);
        }
        return candle && !(candle->flags91 & 1) && IsCandleInNeedOfChanging(candle);
    }
};
int main()
{
    Thing home = { 0 }, c = { 0 }, dead = { 1 };
    Material m = { 1, 0x20, 0 };
    Group g = { &home, false, &m, &c, 0, true, 0 };
    bool ok = true;
    if (g.IsRunnable() || g.findCalls != 0 || m.count != 1) ok = false;      // flagged material: not runnable
    m.flags = 0;
    if (!g.IsRunnable() || g.candle != &c || g.findCalls != 1) ok = false;    // candle found, needs changing
    g.needs = false;
    if (g.IsRunnable()) ok = false;
    g.needs = true; g.found = &dead;
    if (g.IsRunnable()) ok = false;                                           // dead candle
    g.building = &dead; g.candle = &c;
    if (!g.IsRunnable() || g.findCalls != 3) ok = false;                      // dead home: keep old candle
    g.building = &home; g.workCandle = true; g.material = 0; g.found = &c;
    if (!g.IsRunnable()) ok = false;                                          // work candle skips the material
    if (!ok) { std::printf("00946b50_TEST FAIL\n"); return 1; }
    std::printf("00946b50_TEST PASS\n");
    return 0;
}