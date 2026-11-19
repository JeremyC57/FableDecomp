// Behaviour test for CEngineInternalPrimitive2DViewport::ClearChildPrimitive (0x00BE2120): a copy of the row's logic (HasChildPrimitive,
// the handle's Clear: last-handle notification, counted release, null; then the render-layer update) on stand-alone types.
#include <stdio.h>
struct Prim { long RefCount; int notified; int destroyed; };
struct Handle { Prim* Data; };
struct View {
    Handle Slots[4]; unsigned long Count; int layer_updates;
    bool HasChildPrimitive(unsigned long index) const { return index < Count && Slots[index].Data != 0; }
    void Clear(Handle& h)
    {
        if (h.Data != 0 && h.Data->RefCount == 1) h.Data->notified++;
        if (h.Data) { if (--h.Data->RefCount == 0) h.Data->destroyed++; h.Data = 0; }
    }
    void ClearChildPrimitive(unsigned long index)
    {
        if (HasChildPrimitive(index)) {
            Clear(Slots[index]);
            ++layer_updates;
        }
    }
};
int main()
{
    Prim a = { 1, 0, 0 }, b = { 2, 0, 0 };
    View v = { { { &a }, { 0 }, { &b }, { 0 } }, 3, 0 };
    int fails = 0;
    v.ClearChildPrimitive(0);           // unique: notified + destroyed
    if (v.Slots[0].Data || a.notified != 1 || a.destroyed != 1 || v.layer_updates != 1) ++fails;
    v.ClearChildPrimitive(1);           // empty slot: nothing
    if (v.layer_updates != 1) ++fails;
    v.ClearChildPrimitive(2);           // shared: no notification, one reference dropped
    if (v.Slots[2].Data || b.notified != 0 || b.destroyed != 0 || b.RefCount != 1 || v.layer_updates != 2) ++fails;
    v.ClearChildPrimitive(7);           // out of range: nothing
    if (v.layer_updates != 2) ++fails;
    printf(fails ? "00be2120_TEST FAIL\n" : "00be2120_TEST PASS\n");
    return fails ? 1 : 0;
}
