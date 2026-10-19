// NEntityEvents::CEventHitBase::CEventHitBase(const CHitParameters& params) -- a hit event: own a copy of the hit
// Grade: RELOCATION_MATCH -- the two early exits are `break`s out of a `do { ... } while (0)` block: VC7.1 then cross-jumps them into the one epilogue after the inner `pop esi` like retail (the early-return and nested-if forms both emitted a second pop edi / mov eax,ebp / ret 4 tail)
// parameters and, for a damage-attribute-4 hit by a live thing carrying TC 0x77 with a live weapon, record that weapon
// Leaf: ??0CEventHitBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00811760, length 199 bytes (padded to 208), `ret 4`.
// Callees: operator new @ 0x00BFEA1A, CHitParameters::CHitParameters(const CHitParameters&) @ 0x00812170 (0x60 bytes),
// CCountedPointer<CHitParameters>::Reset(CHitParameters*) @ 0x00812280 (labelled for another instantiation),
// CHitParameters damage-map test for attribute 4 @ 0x008688C0 (labelled CVertexGroupBlock::HasGroup), CBaseIntelligent
// Pointer::GetPItem @ 0x00A01B50, the TC map lower bound copy @ 0x004365B0, TC 0x77's weapon getter @ 0x007BEFF0 (its
// intelligent pointer at +0x30; labelled _Cons_val), CHitParameters::SetWeapon(CThing*) @ 0x00868CE0.
// Vtable 0x01272344 (NEntityEvents::CEventHitBase). Layout (PDB, retail agrees): CEventBase WorldFrame +4, PHitParams
// +8 (CCountedPointer {Data, Info}); CHitParameters PThingHitting +0x58; CThing TC bitset +0x20 (TC 0x77 = +0x2C bit
// 23), TC map +0x44, dead bit +0x91.
// Shape: the inline CEventBase ctor zeroes its member before the vtable store; the counted pointer is default
// constructed (zeros after the vtable) and then reset with the new copy; GetTC(k) keeps the component in esi for both
// weapon reads.

extern void* operator new(unsigned int size);       // 0x00BFEA1A

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];       // +0x20
    struct CInterfaceMap {
        struct SEntry { long type; void* component; };
        SEntry* first;
        SEntry* last;
        SEntry* LowerBound(const long* type);       // 0x004365B0
    } m_interfaces;                       // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_dead : 1;             // +0x91 bit 0
    unsigned char m_91_1 : 7;
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

struct CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CTCWeaponHolder_77 {                         // TC 0x77
    CThing* GetPWeapon();                           // 0x007BEFF0
};

class CHitParameters {
public:
    unsigned char _pad00[0x58];
    CIntelligentPointer<CThing> PThingHitting;      // +0x58
    CHitParameters(const CHitParameters& other);    // 0x00812170
    bool HasDamageAttribute4() const;               // 0x008688C0
    void SetWeapon(CThing* weapon);                 // 0x00868CE0
};

template<class T> struct CCountedPointer {
    T* Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Reset(T* data);                            // 0x00812280
    T* operator->() const { return Data; }
};

namespace NEntityEvents {
class __declspec(novtable) CEventBase {
public:
    virtual void v00();
    long WorldFrame;                                // +0x04
    CEventBase() : WorldFrame(0) {}
};
class CEventHitBase : public CEventBase {
public:
    CCountedPointer<CHitParameters> PHitParams;     // +0x08
    virtual void v00();
    CEventHitBase(const CHitParameters& params);
};
}

NEntityEvents::CEventHitBase::CEventHitBase(const CHitParameters& params)
{
    PHitParams.Reset(new CHitParameters(params));
    do {
        if (!PHitParams->HasDamageAttribute4())
            break;
        CThing* hitter = PHitParams->PThingHitting.GetPItem();
        if (!hitter || hitter->m_dead || !hitter->HasTC(0x77))
            break;
        CTCWeaponHolder_77* holder = hitter->GetTC<CTCWeaponHolder_77>(0x77);
        CThing* weapon = holder->GetPWeapon();
        if (weapon && !weapon->m_dead)
            PHitParams->SetWeapon(holder->GetPWeapon());
    } while (0);
}