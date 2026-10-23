// CTCDExplosion::UpdateExplosiveAdditionalImpulse() -- push the things the explosion has hit: for a ringed (dynamic) explosion the ones between the current ring's inner and outer radius, otherwise along the start/end line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080C590, 305 bytes (+ int3 padding), plain `ret`.
// Callees: CThing::GetPosition @ 0x004C73D0, the hit-thing transform into a local vector<CThing*> @ 0x0080EBE0, the ring
//   impulse for_each @ 0x0080E530 and the line impulse for_each @ 0x0080E5D0 (all three with first/last in ecx/edx and the
//   functor by value on the stack: modelled `__fastcall`), free @ 0x00BFEA14. Constant 0.0f @ 0x0122DEDC.
// Layout: Ego_r.pdb CTCDExplosion (retail 12-byte vectors): ParentThing +0x04, DamageRadius +0x0c, StartPos +0x20, EndPos +0x2c,
//   PExplosionDef +0x38, CurrentExplosionRing +0x50, HitThings +0x54; CExplosionDef ring vector (16-byte entries, radius first)
//   +0x58.
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CThing { const C3DVector& GetPosition() const; };    // 0x004C73D0
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
struct CExplosionDef {
    unsigned char _pad00[0x58];
    CRingVector Rings;                          // +0x58
};
struct CTCDExplosion {
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