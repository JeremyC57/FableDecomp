// CTCDExplosion::FrameUpdate() -- run the dynamic explosion (or trigger the static one once), update the additional impulse, drop the emitters whose thing has gone, then kill the explosion thing when the dynamic explosion has finished / the emitters are gone / the life timer runs out
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080D750, 243 bytes, plain `ret`.
// Callees: UpdateDynamicExplosion @ 0x0080D300, CreateStaticExplosion @ 0x0080D0B0, UpdateHitThings(const C3DVector&, const
//   C3DVector&) @ 0x0080CD40, UpdateExplosiveAdditionalImpulse @ 0x0080C590, CBaseIntelligentPointer ctor @ 0x00A01B10 /
//   GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C),
//   remove_if over the emitter vector @ 0x0080E2F0 (first/last in ecx/edx, the 8-byte predicate by value: modelled
//   `__fastcall`; manifest "ExplosionFilter_RemoveMatchingElement"), vector::erase(first, last) @ 0x0080E4E0 (manifest
//   "std_vector_CopyAssign" is propagated), IsDynamicExplosionFinished @ 0x0080C350, CThing::Kill(bool) @ 0x004C9B80.
// Layout: Ego_r.pdb CTCDExplosion: ParentThing +0x04, ExplosionLifeTimer +0x1c, StartPos +0x20, EndPos +0x2c,
//   PExplosionDef +0x38, CreatedEmitter +0x3c, NeedToTriggerExplosion +0x3d, Emitters (vector<pair<CIntelligentPointer<
//   CThing>, long> >, retail 12 bytes) +0x40; CExplosionDef's dynamic ring vector at +0x58/+0x5c (descriptive).
// Shape: `Emitters.erase(remove_if(begin, end, CEmitterThingIs(CIntelligentPointer<CThing>(0))), end)` in one statement:
//   the outer end() read first, the intelligent-pointer temporary built in the frame and copied into the by-value predicate
//   argument, destroyed after the erase; the first ring test reads begin/end directly, the second goes through the def
//   vector's inline empty() (`add eax,58h` forms its `this`); the kill tail is early returns (a ternary on CreatedEmitter
//   materialises sete/movzx), the emitters' emptiness through Emitters.empty() (addressed off the erase's edi).
struct C3DVector { float X, Y, Z; };
struct CThing {
    void Kill(bool immediately);                // 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct SEmitter {                               // pair<CIntelligentPointer<CThing>, long> (12 bytes)
    CIntelligentPointer first;
    long                second;
};
struct CEmitterThingIs {                        // remove_if predicate: the emitter's thing equals the held pointer
    CIntelligentPointer Thing;
    CEmitterThingIs(const CIntelligentPointer& thing) : Thing(thing) { }
};
SEmitter* __fastcall remove_if(SEmitter* first, SEmitter* last, CEmitterThingIs pred);   // 0x0080E2F0
struct CEmitterVector {                         // STLport vector (12 bytes)
    SEmitter* _M_start;
    SEmitter* _M_finish;
    SEmitter* _M_end_of_storage;
    SEmitter* begin() const { return _M_start; }
    SEmitter* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    SEmitter* erase(SEmitter* first, SEmitter* last);   // 0x0080E4E0
};
struct CRingVector {                            // STLport vector (12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
};
struct CExplosionDef {
    unsigned char _pad00[0x58];
    CRingVector DynamicRings;                   // +0x58
    bool IsDynamic() const { return DynamicRings._M_start != DynamicRings._M_finish; }
};
struct CTCDExplosion {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    float DamageRadius;                         // +0x0c
    float PhysicsRadius;                        // +0x10
    float MaxDamage;                            // +0x14
    long  FireDamage;                           // +0x18
    long  ExplosionLifeTimer;                   // +0x1c
    C3DVector StartPos;                         // +0x20
    C3DVector EndPos;                           // +0x2c
    const CExplosionDef* PExplosionDef;         // +0x38 CDefPointer<CExplosionDef const>
    bool  CreatedEmitter;                       // +0x3c
    bool  NeedToTriggerExplosion;               // +0x3d
    CEmitterVector Emitters;                    // +0x40
    void UpdateDynamicExplosion();              // 0x0080D300
    void CreateStaticExplosion();               // 0x0080D0B0
    void UpdateHitThings(const C3DVector& start, const C3DVector& end);   // 0x0080CD40
    void UpdateExplosiveAdditionalImpulse();    // 0x0080C590
    bool IsDynamicExplosionFinished();          // 0x0080C350
    void FrameUpdate();
};

void CTCDExplosion::FrameUpdate()
{
    if (PExplosionDef->IsDynamic())
        UpdateDynamicExplosion();
    else if (NeedToTriggerExplosion) {
        CreateStaticExplosion();
        UpdateHitThings(StartPos, EndPos);
        NeedToTriggerExplosion = false;
    }
    UpdateExplosiveAdditionalImpulse();
    Emitters.erase(remove_if(Emitters.begin(), Emitters.end(), CEmitterThingIs(CIntelligentPointer(0))), Emitters.end());
    if (!PExplosionDef->DynamicRings.empty()) {
        if (IsDynamicExplosionFinished())
            ParentThing->Kill(true);
        return;
    }
    if (CreatedEmitter) {
        if (!Emitters.empty())
            return;
    } else if (--ExplosionLifeTimer != 0)
        return;
    ParentThing->Kill(true);
}
