// CCreatureAction_SpiritHitResponse::Initialise() -- base Initialise, set the action length from a global frame count, double the knock-back vector and apply it to the creature's physics, then tell the co-op spirit component (interface type 0xc7) it was hit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00854810, length 158 bytes (+ int3 padding).
// Callees: CCreatureActionBase::Initialise @ 0x00694D10 (direct base call), __ftol2 @ 0x00BFEA70, CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCreature at +8, twice), CTCPhysicsBase::ApplyAcceleration(const C3DVector&) @ 0x006B0210 (the creature's physics at +0x60),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0xc7 = bit 7 of the bitset byte at +0x38: `test cl,cl ; jns`),
// CTCCoopSpirit::OnHit @ 0x006707A0. Global 0x01375550: a long (15) converted through float (`fild ; call __ftol2`).
// Shape: TotalTime (+0x10) = (long)(float)global; the vector at +0xa8 is scaled by 2 through its inline `operator*=` (x87 `fadd st(0),st(0)`
// per component, addressed through esi, which stays live for the argument push); the creature is fetched BEFORE that push, so it is
// a named local; the spirit pointer is an UNINITIALISED out-parameter used unconditionally, so the OnHit call and the
// epilogue are duplicated into the miss path (which reads the never-written frame word). `this` becomes &PCreature (`add edi,8`) once dead.
// The PDB header (CCreatureAction_SpiritHitResponse.h) flattens the base and sits 0x0c high; modelled locally.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector& operator*=(float scale) { X *= scale; Y *= scale; Z *= scale; return *this; }
};
struct CTCPhysicsBase {
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* m_pPhysics;             // +0x60
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CTCCoopSpirit {                      // interface type 0xc7 component
    void OnHit();                           // 0x006707A0
};
extern long g_SpiritHitResponseFrames;      // 0x01375550 (15)

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00694D10
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    long TotalTime;                         // +0x10
    unsigned char _pad14[0xa8 - 0x14];      // +0x14
};
struct CCreatureAction_SpiritHitResponse : CCreatureActionBase {
    C3DVector Knockback;                    // +0xa8
    virtual void Initialise();
};

void CCreatureAction_SpiritHitResponse::Initialise()
{
    CCreatureActionBase::Initialise();
    TotalTime = (long)(float)g_SpiritHitResponseFrames;
    Knockback *= 2.0f;
    CThing* creature = PCreature.GetPItem();   // fetched before the argument push: a named local
    creature->m_pPhysics->ApplyAcceleration(Knockback);
    CTCCoopSpirit* pSpirit;
    PCreature.GetPItem()->GetInterface(&pSpirit, 0xc7);
    pSpirit->OnHit();
}