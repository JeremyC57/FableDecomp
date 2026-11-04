// Behaviour test for CEngineSubPrimitiveRenderer::AddSubPrimitive (0x00B84580): alpha sub-primitives (flag 1) are keyed by
// round((max - distance) * multiplier), opaque ones by round(distance * multiplier) (GFFloatToLongNear), clamped to [0, 0x3fffffff];
// a chain without a head queues the sub-primitive on list 1 (alpha) or 0 / 11 (flag 0x80) / 12 (flag 0x100) and becomes the head;
// a chain with a head links the new sub-primitive behind it. The flags, alpha and manager are stored on the sub-primitive.
#include <stdio.h>
#include <vector>
#include "engine/GFFloatToLongNear.h"
struct Sub { unsigned long Flags; unsigned char Alpha; Sub* Next; void* Manager; };
struct Chain { Sub* LastOpaque; Sub* LastAlpha; };
struct Elem { unsigned long Key; Sub* Prim; };
struct Renderer {
    float MaxSortDistance, Multiplier; std::vector<Elem> Lists[15];
    static long Clamp(long k) { return k < 0 ? 0 : (k > 0x3fffffff ? 0x3fffffff : k); }
    void Add(Sub* s, float distance, unsigned char alpha, unsigned long flags, void* manager, Chain& chain)
    {
        if (flags & 1) {
            distance = (MaxSortDistance - distance) * Multiplier;
            long key = Clamp(GFFloatToLongNear(distance));
            if (chain.LastAlpha) { chain.LastAlpha->Next = s; chain.LastAlpha = s; }
            else { Elem e = { (unsigned long)key, s }; Lists[1].push_back(e); chain.LastAlpha = s; }
        } else {
            long list = 0;
            if (flags & 0x80) list = 11;
            if (flags & 0x100) list = 12;
            distance = distance * Multiplier;
            long key = Clamp(GFFloatToLongNear(distance));
            if (chain.LastOpaque) chain.LastOpaque->Next = s;
            else { Elem e = { (unsigned long)key, s }; Lists[list].push_back(e); }
            chain.LastOpaque = s;
        }
        s->Flags = flags; s->Alpha = alpha; s->Manager = manager;
    }
};
int main()
{
    Renderer r; r.MaxSortDistance = 100.0f; r.Multiplier = 2.0f;
    Sub a = { 0, 0, 0, 0 }, b = { 0, 0, 0, 0 }, c = { 0, 0, 0, 0 };
    Chain ch = { 0, 0 };
    r.Add(&a, 10.25f, 7, 1, &r, ch);           // alpha: key round(89.75 * 2) = 180 (179.5 ties to even)
    r.Add(&b, 3.0f, 9, 0x80, &r, ch);          // opaque, list 11, key 6
    Chain ch2 = { &b, 0 };
    r.Add(&c, 1.0f, 1, 0, &r, ch2);            // linked behind b
    bool ok = r.Lists[1].size() == 1 && r.Lists[1][0].Key == 180 && r.Lists[1][0].Prim == &a && ch.LastAlpha == &a
        && r.Lists[11].size() == 1 && r.Lists[11][0].Key == 6 && ch.LastOpaque == &b && b.Next == &c && ch2.LastOpaque == &c
        && r.Lists[0].empty() && a.Flags == 1 && a.Alpha == 7 && c.Manager == &r;
    printf(ok ? "00b84580_TEST PASS\n" : "00b84580_TEST FAIL\n");
    return ok ? 0 : 1;
}