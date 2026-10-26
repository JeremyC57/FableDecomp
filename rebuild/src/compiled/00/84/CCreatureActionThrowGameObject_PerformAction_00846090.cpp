// CCreatureAction_ThrowGameObject::PerformAction() -- take the thing in the creature's primary carry slot (interface type 0x46), release it and accelerate it towards the stored throw vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00846090, length 107 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for
// type 0x46 = bit 6 of the bitset byte at +0x28), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCCarrying::RemoveThing(CThing*) @ 0x006A19B0,
// CTCPhysicsBase::ApplyAcceleration(const C3DVector&) @ 0x006B0210 (the thrown thing's physics at +0x60).
// Shape: the carrying component is an UNINITIALISED out-parameter used unconditionally; the two paths join with it in edi (the miss path reads the
// never-written frame word). The thrown thing rides in esi; `this` in ebx becomes &Target (`add ebx,0A8h`) for the last call.
// No engine header layout used (the PDB flattens the base); modelled locally (retail CCreatureActionBase is 0xa8 bytes, Target at +0xa8).
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CTCPhysicsBase { public:
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThing { public:
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
class CTCCarrying { public:                        // interface type 0x46 component
    CThing* GetThingInPrimarySlot();        // 0x006A1440
    void RemoveThing(CThing* thing);        // 0x006A19B0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_ThrowGameObject : public CCreatureActionBase { public:
    C3DVector Target;                       // +0xa8
    virtual void PerformAction();
};

void CCreatureAction_ThrowGameObject::PerformAction()
{
    CTCCarrying* pCarrying;
    PCreature.GetPItem()->GetInterface(&pCarrying, 0x46);
    CThing* thrown = pCarrying->GetThingInPrimarySlot();
    if (thrown) {
        pCarrying->RemoveThing(thrown);
        thrown->m_pPhysics->ApplyAcceleration(Target);
    }
}