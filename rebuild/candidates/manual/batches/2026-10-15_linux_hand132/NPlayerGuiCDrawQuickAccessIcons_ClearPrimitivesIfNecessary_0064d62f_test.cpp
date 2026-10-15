#include <cstdio>
// Behaviour test for 0064d62f.

// CDrawQuickAccessIcons::ClearPrimitivesIfNecessary: when not drawing, the sound is stopped, the charge-up handle cleared
// (notifying the primitive when it held the last reference, releasing it) and every special / cross handle cleared;
// the base clear always runs.
struct CPrimitive { long refCount; int notified; int destroyed; };
struct CHandle {
    CPrimitive* p; int clears;
    void Clear()
    {
        ++clears;
        if ((p ? p->refCount : 0) == 1) ++p->notified;
        CPrimitive* object = p;
        if (object) { if (--object->refCount == 0) ++object->destroyed; p = 0; }
        p = 0;
    }
};
struct CDrawQuickAccessIcons {
    bool Drawing; int soundCleared; int baseCleared; CHandle ChargeUp; CHandle Special[2]; CHandle Cross[3];
    void ClearSoundEffects() { ++soundCleared; }
    void BaseClear() { ++baseCleared; }
    void ClearPrimitivesIfNecessary()
    {
        if (!Drawing) {
            ClearSoundEffects();
            ChargeUp.Clear();
            for (int i = 0; i < 2; ++i) Special[i].Clear();
            for (int i = 0; i < 3; ++i) Cross[i].Clear();
        }
        BaseClear();
    }
};
int main()
{
    CPrimitive prim; prim.refCount = 1; prim.notified = 0; prim.destroyed = 0;
    CDrawQuickAccessIcons d; d.Drawing = true; d.soundCleared = d.baseCleared = 0; d.ChargeUp.p = &prim; d.ChargeUp.clears = 0;
    for (int i = 0; i < 2; ++i) { d.Special[i].p = 0; d.Special[i].clears = 0; }
    for (int i = 0; i < 3; ++i) { d.Cross[i].p = 0; d.Cross[i].clears = 0; }
    d.ClearPrimitivesIfNecessary();
    int ok = d.baseCleared == 1 && d.soundCleared == 0 && d.ChargeUp.p == &prim;
    d.Drawing = false; d.ClearPrimitivesIfNecessary();
    ok = ok && d.baseCleared == 2 && d.soundCleared == 1 && d.ChargeUp.p == 0 && prim.notified == 1 && prim.destroyed == 1
            && d.Special[1].clears == 1 && d.Cross[2].clears == 1;
    if (!ok) { std::printf("0064d62f_TEST FAIL\n"); return 1; }
    std::printf("0064d62f_TEST PASS\n");
    return 0;
}
