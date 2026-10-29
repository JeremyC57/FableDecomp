// CTCTrapBase::Trigger() -- fire the trap: when its def names an explosion, create it at the trap with every live hit and immune thing excluded, and when the def also names a physical obstruction, create one of the explosion's physics radius (remembered by the trap, alive 100 seconds, ignoring the immune things); then run the trap-specific trigger and mark the trap triggered
// Leaf: Trigger
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00788229, length 351 bytes (`leave`, plain `ret`): frame-pointer size TU,
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)` as the landed siblings UpdateTriggerOnProximity 0x007887BE
//   and SetThingAsImmune 0x007898A3. Ego_r.pdb `?Trigger@CTCTrapBase@@IAEXXZ`.
// Callees: CThing::GetPos() const 0x004C73D0; CTCDExplosion::Create(long, const C3DVector&) 0x0080C460 and
//   CTCDPhysicalObstruction::Create(const C3DVector&, const C3DVector&, float) 0x007FD4B0 (static __fastcall; Ego_r.pdb
//   `?Create@CTCDExplosion@@SIPAVCThing@@JABVC3DVector@@@Z` / `?Create@CTCDPhysicalObstruction@@SIPAVCThing@@ABVC3DVector@@0M@Z`,
//   egor_signatures; catalog "GetFOVToBeInToBeTargetable" propagated); IS_THING_ALIVE(CThing* const&) 0x0040DCE0 and
//   IS_THING_ALIVE(const CIntelligentPointer<CThing>&) 0x004CC340 (__fastcall, as landed); CThing::GetTCDriver
//   <CTCDExplosion> 0x007512C0 and <CTCDPhysicalObstruction> 0x007890EA (folded bodies, as landed);
//   CTCDExplosion::SetThingAsImmune(CThing&) 0x0080CBF0; CBaseIntelligentPointer::GetPItem 0x00A01B50;
//   CIntelligentPointer<CThing>(CThing*) 0x004A96F0, operator= 0x004A9760, ~CBaseIntelligentPointer 0x00A01C10;
//   CTCDPhysicalObstruction::SetLifeTimeSeconds(float) 0x007FD3D0 and SetThingToIgnoreObstruction(CThing*) 0x007898A3
//   (bodies folded with landed names); the trap's virtual slot 30 (+0x78, the trap-specific TriggerTrap).
// Data: 100.0f 0x01230A08.
// Layout: retail CTCTrapBase (12-byte STLport vectors, as the landed constructor 0x0078918D): HitThings +0x0c (12-byte
//   pair<CIntelligentPointer<CThing>, long>), ImmuneThings +0x18, PObstruction +0x24, PTrapDef +0x54, Triggered +0x59;
//   retail CTrapDef ExplosionDefIndex +0x50, PhysicalObstructionDefIndex +0x54 (PDB - 4, the 12-byte STLport map);
//   CTCDExplosion PhysicsRadius +0x10 (PDB).
// Shape: the def pointer is a local (edi, re-read for the obstruction index); same-TU noinline stand-ins for
//   IS_THING_ALIVE and GetTCDriver let VC7.1 pack the things and drivers into one frame word (each driver comes through an
//   out-parameter in the word of the thing it came from) and the radius and the obstruction driver into a second; the
//   hit things are walked by iterator, the immune things by index against a size read once, addressed as
//   `*(begin() + i)` (`lea ecx,[eax+ebx*8]`, i in ebx; an `operator[]` strength-reduces i*8 and spills i), then by
//   iterator again for the obstruction; the radius is a named float local read before the position; the obstruction
//   pointer is assigned straight from the thing (the implicit CIntelligentPointer<CThing> temporary's address is re-taken).
#pragma optimize("y", off)
#pragma optimize("s", on)
class C3DVector { public: float X, Y, Z; };
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {
    void*   PData;                                      // +0x04
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
};
template <class T> class CIntelligentPointer;
template <> class CIntelligentPointer<CThing> : public CBaseIntelligentPointer { public:  // CIntelligentPointer<CThing>
    CIntelligentPointer(CThing* thing);                 // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};
struct CHitThing {                                      // pair<CIntelligentPointer<CThing>, long>
    CIntelligentPointer<CThing> first;
    long                second;
};
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
};
class CTCDExplosion {
public:
    unsigned char _pad00[0x10];
    float         PhysicsRadius;                        // +0x10
    static CThing* __fastcall Create(long defIndex, const C3DVector& position);   // 0x0080C460
    void SetThingAsImmune(CThing& thing);               // 0x0080CBF0
};
class CTCDPhysicalObstruction {
public:
    static CThing* __fastcall Create(const C3DVector& position, const C3DVector& target, float radius);   // 0x007FD4B0
    void SetLifeTimeSeconds(float seconds);             // 0x007FD3D0
    void SetThingToIgnoreObstruction(CThing* thing);    // 0x007898A3
};
class CThing {
public:
    unsigned char  _pad00[0x28];
    unsigned long  m_usingBits;                         // +0x28 (driver interface bits)
    unsigned char  _pad2c[0x5c - 0x2c];
    void*          PTCDriver;                           // +0x5c
    unsigned char  _pad60[0x91 - 0x60];
    unsigned char  m_flags;                             // +0x91 bit 0: dead
    const C3DVector& GetPos() const;               // 0x004C73D0
    bool GetTCDriver(CTCDExplosion** out);              // 0x007512C0
    bool GetTCDriver(CTCDPhysicalObstruction** out);    // 0x007890EA
};
#pragma auto_inline(off)
// Same-TU stand-ins for the out-of-line helpers (bodies as landed): VC7.1 then knows the frame words' addresses do not
// escape and packs the things, drivers and the radius into two words like retail.
__declspec(noinline) bool __fastcall IS_THING_ALIVE(CThing* const& thing)        // 0x0040DCE0
{
    const CThing* p = thing;
    if (p != 0 && (p->m_flags & 1) == 0)
        return true;
    return false;
}
__declspec(noinline) bool __fastcall IS_THING_ALIVE_004cc340(const CBaseIntelligentPointer& pointer)   // 0x004CC340
{
    if (((CThing*)pointer.GetPItem()) != 0) {
        if ((((CThing*)pointer.GetPItem())->m_flags & 1) == 0)
            return true;
    }
    return false;
}
__declspec(noinline) bool CThing::GetTCDriver(CTCDExplosion** out)                // 0x007512C0
{
    if (m_usingBits & 2) {
        *out = (CTCDExplosion*)PTCDriver;
        return true;
    }
    return false;
}
__declspec(noinline) bool CThing::GetTCDriver(CTCDPhysicalObstruction** out)      // 0x007890EA
{
    if (m_usingBits & 0x2000) {
        *out = (CTCDPhysicalObstruction*)PTCDriver;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CTrapDef { public:
    unsigned char _pad00[0x50];
    long          ExplosionDefIndex;                    // +0x50
    long          PhysicalObstructionDefIndex;          // +0x54
};
class CTCTrapBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void TriggerTrap();                         // slot 30 (+0x78)
    CThing*                             ParentThing;    // +0x04
    unsigned char                       _pad08[4];
    CRetailVector<CHitThing>            HitThings;      // +0x0c
    CRetailVector<CIntelligentPointer<CThing> >  ImmuneThings;   // +0x18
    CIntelligentPointer<CThing>                 PObstruction;   // +0x24
    unsigned char                       _pad2c[0x54 - 0x2c];
    const CTrapDef*                     PTrapDef;       // +0x54 CDefPointer<const CTrapDef>::Object
    bool                                Active;         // +0x58
    bool                                Triggered;      // +0x59
    void Trigger();
};

void CTCTrapBase::Trigger()
{
    const CTrapDef* def = PTrapDef;
    if (def->ExplosionDefIndex >= 0) {
        CThing* explosion = CTCDExplosion::Create(def->ExplosionDefIndex, ParentThing->GetPos());
        if (IS_THING_ALIVE(explosion)) {
            CTCDExplosion* explosionDriver;
            explosion->GetTCDriver(&explosionDriver);
            for (CHitThing* it = HitThings.begin(); it != HitThings.end(); ++it)
                if (IS_THING_ALIVE_004cc340(it->first))
                    explosionDriver->SetThingAsImmune(*((CThing*)it->first.GetPItem()));
            long count = ImmuneThings.size();
            for (long i = 0; i < count; ++i)
                if (IS_THING_ALIVE_004cc340(*(ImmuneThings.begin() + i)))
                    explosionDriver->SetThingAsImmune(*((CThing*)(ImmuneThings.begin() + i)->GetPItem()));
            if (PTrapDef->PhysicalObstructionDefIndex >= 0) {
                float radius = explosionDriver->PhysicsRadius;
                const C3DVector& position = ParentThing->GetPos();
                CThing* obstruction = CTCDPhysicalObstruction::Create(position, position, radius);
                if (IS_THING_ALIVE(obstruction)) {
                    PObstruction = obstruction;
                    CTCDPhysicalObstruction* obstructionDriver;
                    obstruction->GetTCDriver(&obstructionDriver);
                    obstructionDriver->SetLifeTimeSeconds(100.0f);
                    for (CIntelligentPointer<CThing>* it = ImmuneThings.begin(); it != ImmuneThings.end(); ++it) {
                        CThing* thing = ((CThing*)it->GetPItem());
                        if (IS_THING_ALIVE(thing))
                            obstructionDriver->SetThingToIgnoreObstruction(thing);
                    }
                }
            }
        }
    }
    TriggerTrap();
    Triggered = true;
}