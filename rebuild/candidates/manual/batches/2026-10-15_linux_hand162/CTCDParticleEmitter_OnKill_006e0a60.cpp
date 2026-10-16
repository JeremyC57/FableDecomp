// CTCDParticleEmitter::OnKill() -- kill the live emitter, and register this emitter's thing as a dependent of the (living) attached thing's interface-0xD8 component
// Leaf: OnKill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0A60, length 137 bytes (+ int3 padding). Vtable 0x0125FEEC slot 14 (catalog
// "vfunc_14"; slot 14 of the CTCBase vtables is OnKill).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x5), NParticleEngine::CParticleEmitter::Kill @ 0x00AEE8C0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for type 0xD8, word 6 bit 24 at +0x38),
// the component's AddDependentThing(CThing*) @ 0x0071A8F0 (pushes a CIntelligentPointer<CThing> into its list at +0xC; the
// manifest attributes the body to CCreatureActionBase, a byte-twin label).
// Layout: CTCDParticleEmitter as in the landed SetAlwaysUpdate 0x006E0590: PEmitter +0x14, PAttachedToThing +0x1C.
// Shape: inline IS_THING_ALIVE then a checked GetInterface; the parent is pushed before the component moves into ecx.
struct CTCBaseInterface;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCDependentThings {                     // interface 0xD8 (class not established)
    void AddDependentThing(CThing* thing);      // 0x0071A8F0
};
namespace NParticleEngine {
struct CParticleEmitter {
    void Kill();                                // 0x00AEE8C0
};
}
struct CBaseIntelligentPointer {
    virtual void v00();
    void* PData;                                // +0x04
    void* GetPItem() const;                     // 0x00A01B50
};
struct CEmitterPointer : CBaseIntelligentPointer {
    NParticleEngine::CParticleEmitter* GetPItem() const { return (NParticleEngine::CParticleEmitter*)CBaseIntelligentPointer::GetPItem(); }
};
struct CThingPointer : CBaseIntelligentPointer {
    CThing* GetPItem() const { return (CThing*)CBaseIntelligentPointer::GetPItem(); }
};
inline bool IS_THING_ALIVE(const CThingPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual void OnKill();                      // slot 14
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
struct CTCDParticleEmitter : CTCBase {
    void*           EmitterName;                // +0x0c CCharString
    long            ParticleTypeID;             // +0x10
    CEmitterPointer PEmitter;                   // +0x14
    CThingPointer   PAttachedToThing;           // +0x1c
    virtual void OnKill();
};

void CTCDParticleEmitter::OnKill()
{
    if (PEmitter.GetPItem())
        PEmitter.GetPItem()->Kill();
    if (IS_THING_ALIVE(PAttachedToThing)) {
        CTCDependentThings* dependents;
        if (PAttachedToThing.GetPItem()->GetInterface(&dependents, 0xd8))
            dependents->AddDependentThing(ParentThing);
    }
}
