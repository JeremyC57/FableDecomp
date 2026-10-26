#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
#include <stdio.h>
// CTCDExplosion::UpdateExplosiveAdditionalImpulse() -- push the things the explosion has hit: for a ringed (dynamic) explosion the ones between the current ring's inner and outer radius, otherwise along the start/end line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080C590, 305 bytes (+ int3 padding), plain `ret`.
// Callees: CThing::GetPosition @ 0x004C73D0, the hit-thing transform into a local vector<CThing*> @ 0x0080EBE0, the ring
//   impulse for_each @ 0x0080E530 and the line impulse for_each @ 0x0080E5D0 (all three with first/last in ecx/edx and the
//   functor by value on the stack: modelled `__fastcall`), free @ 0x00BFEA14. Constant 0.0f @ 0x0122DEDC.
// Layout: Ego_r.pdb CTCDExplosion (retail 12-byte vectors): ParentThing +0x04, DamageRadius +0x0c, StartPos +0x20, EndPos +0x2c,
//   PExplosionDef +0x38, CurrentExplosionRing +0x50, HitThings +0x54; CExplosionDef ring vector (16-byte entries, radius first)
//   +0x58.
#include <stddef.h>
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
extern "C" void* __cdecl malloc(size_t size);
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CThing { public: const C3DVector& GetPosition() const; };    // 0x004C73D0
struct CThingVector {
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
};
struct CBaseIntelligentPointer { void* __vftable; void* m_pItem; };
struct CBackInserter {
    CThingVector* container;
    explicit CBackInserter(CThingVector& v) : container(&v) { }
};
struct CGetPItem { };
CBackInserter __fastcall TransformHitThings(CBaseIntelligentPointer* first, CBaseIntelligentPointer* last,
                                            CBackInserter out, CGetPItem op);   // 0x0080EBE0
struct CRingImpulse {                           // 20 bytes
    C3DVector Centre;
    float     Inner;
    float     Outer;
    CRingImpulse(const C3DVector& centre, float inner, float outer) : Centre(centre), Inner(inner), Outer(outer) { }
};
struct CLineImpulse {                           // 28 bytes
    C3DVector Start;
    C3DVector End;
    float     Radius;
    CLineImpulse(const C3DVector& start, const C3DVector& end, float radius) : Start(start), End(end), Radius(radius) { }
};
struct SIterTag { SIterTag() { } };
void __fastcall ApplyRingImpulse(CThing** first, CThing** last, SIterTag tag, CRingImpulse f);   // 0x0080E530
void __fastcall ApplyLineImpulse(CThing** first, CThing** last, SIterTag tag, CLineImpulse f);   // 0x0080E5D0
struct SRing { float Radius; float _pad[3]; };
struct CRingVector {
    SRing* _M_start;
    SRing* _M_finish;
    SRing* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
};
class CExplosionDef { public:
    unsigned char _pad00[0x58];
    CRingVector Rings;                          // +0x58
};
class CTCDExplosion { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    float DamageRadius;                         // +0x0c
    unsigned char _pad10[0x20 - 0x10];
    C3DVector StartPos;                         // +0x20
    C3DVector EndPos;                           // +0x2c
    const CExplosionDef* PExplosionDef;         // +0x38
    unsigned char _pad3c[0x50 - 0x3c];
    long CurrentExplosionRing;                  // +0x50
    CBaseIntelligentPointer* HitThings_first;   // +0x54
    CBaseIntelligentPointer* HitThings_last;    // +0x58
    CBaseIntelligentPointer* HitThings_end;     // +0x5c
    void UpdateExplosiveAdditionalImpulse();
};

void CTCDExplosion::UpdateExplosiveAdditionalImpulse()
{
    float damageRadius = DamageRadius;
    const C3DVector& pos = ParentThing->GetPosition();
    CThingVector things;
    TransformHitThings(HitThings_first, HitThings_last, CBackInserter(things), CGetPItem());
    const CExplosionDef* def = PExplosionDef;
    if (!def->Rings.empty()) {
        long ring = CurrentExplosionRing;
        float inner = (ring >= 0 && (unsigned long)ring < def->Rings.size() && ring != 0) ? damageRadius + def->Rings._M_start[ring - 1].Radius : 0.0f;
        float outer = (ring >= 0 && (unsigned long)ring < def->Rings.size()) ? damageRadius + def->Rings._M_start[ring].Radius : 0.0f;
        ApplyRingImpulse(things._M_start, things._M_finish, SIterTag(), CRingImpulse(pos, inner, outer));
    } else {
        ApplyLineImpulse(things._M_start, things._M_finish, SIterTag(), CLineImpulse(StartPos, EndPos, damageRadius));
    }
}

static float g_posMem[3];
#define g_pos (*(C3DVector*)g_posMem)
const C3DVector& CThing::GetPosition() const { return g_pos; }
CBackInserter __fastcall TransformHitThings(CBaseIntelligentPointer* first, CBaseIntelligentPointer* last, CBackInserter out, CGetPItem)
{
    CThingVector& v = *out.container;
    if (!v._M_start) { v._M_start = v._M_finish = (CThing**)malloc(16 * sizeof(CThing*)); v._M_end_of_storage = v._M_start + 16; }
    for (; first != last; ++first) *v._M_finish++ = (CThing*)first->m_pItem;
    return out;
}
static int g_ringCalls, g_lineCalls, g_count;
static CThing* g_firstThing;
static float g_a, g_b, g_c, g_d, g_e, g_f, g_g;
void __fastcall ApplyRingImpulse(CThing** first, CThing** last, SIterTag, CRingImpulse f)
{
    ++g_ringCalls; g_count = (int)(last - first); g_firstThing = first != last ? *first : 0;
    g_a = f.Centre.X; g_b = f.Centre.Y; g_c = f.Centre.Z; g_d = f.Inner; g_e = f.Outer;
}
void __fastcall ApplyLineImpulse(CThing** first, CThing** last, SIterTag, CLineImpulse f)
{
    ++g_lineCalls; g_count = (int)(last - first); g_firstThing = first != last ? *first : 0;
    g_a = f.Start.X; g_b = f.Start.Z; g_c = f.End.X; g_d = f.End.Y; g_e = f.End.Z; g_f = f.Radius; g_g = f.Start.Y;
}
static unsigned char g_explosionMem[sizeof(CTCDExplosion)];
static unsigned char g_defMem[sizeof(CExplosionDef)];
int main()
{
    int ok = 1;
    CThing thingA, thingB, parent;
    CBaseIntelligentPointer hits[2];
    hits[0].__vftable = 0; hits[0].m_pItem = &thingA; hits[1].__vftable = 0; hits[1].m_pItem = &thingB;
    SRing rings[3]; rings[0].Radius = 1.0f; rings[1].Radius = 2.0f; rings[2].Radius = 3.0f;
    CTCDExplosion* e = (CTCDExplosion*)g_explosionMem;
    CExplosionDef* def = (CExplosionDef*)g_defMem;
    def->Rings._M_start = rings; def->Rings._M_finish = rings + 3; def->Rings._M_end_of_storage = rings + 3;
    e->ParentThing = &parent; e->DamageRadius = 10.0f; e->PExplosionDef = def;
    e->HitThings_first = hits; e->HitThings_last = hits + 2; e->HitThings_end = hits + 2;
    e->StartPos.X = 1.0f; e->StartPos.Y = 2.0f; e->StartPos.Z = 3.0f; e->EndPos.X = 4.0f; e->EndPos.Y = 5.0f; e->EndPos.Z = 6.0f;
    g_pos.X = 7.0f; g_pos.Y = 8.0f; g_pos.Z = 9.0f;
    // ring 1: inner 10+1, outer 10+2
    e->CurrentExplosionRing = 1;
    e->UpdateExplosiveAdditionalImpulse();
    ok &= g_ringCalls == 1 && g_lineCalls == 0 && g_count == 2 && g_firstThing == &thingA;
    ok &= g_a == 7.0f && g_b == 8.0f && g_c == 9.0f && g_d == 11.0f && g_e == 12.0f;
    // ring 0: no inner ring
    e->CurrentExplosionRing = 0;
    e->UpdateExplosiveAdditionalImpulse();
    ok &= g_ringCalls == 2 && g_d == 0.0f && g_e == 11.0f;
    // ring 3 (past the last ring): both radii 0
    e->CurrentExplosionRing = 3;
    e->UpdateExplosiveAdditionalImpulse();
    ok &= g_ringCalls == 3 && g_d == 0.0f && g_e == 0.0f;
    // ring -1: both 0
    e->CurrentExplosionRing = -1;
    e->UpdateExplosiveAdditionalImpulse();
    ok &= g_ringCalls == 4 && g_d == 0.0f && g_e == 0.0f;
    // no rings: line impulse from StartPos to EndPos with the damage radius
    def->Rings._M_finish = rings;
    e->UpdateExplosiveAdditionalImpulse();
    ok &= g_lineCalls == 1 && g_ringCalls == 4 && g_count == 2 && g_firstThing == &thingA;
    ok &= g_a == 1.0f && g_g == 2.0f && g_b == 3.0f && g_c == 4.0f && g_d == 5.0f && g_e == 6.0f && g_f == 10.0f;
    printf(ok ? "0080c590_TEST PASS\n" : "0080c590_TEST FAIL\n");
    return ok ? 0 : 1;
}