// CCreatureAction_CastStrengthBerserk::Initialise() -- base Initialise, add a "CTCBerserk" component to the creature and start its effect, then if the creature has an interface-167 component set its byte at +0xc and store 8 in its word at +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6400, length 142 bytes (+ int3 padding).
// Callees: CCombatActionBase::Initialise @ 0x00857520 (direct base call, first), CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice), CThing::AddTC(const CCharString&, bool, CTCParamsBase*)
// @ 0x004C9D60 (landed naming of 0x008C4250), CCharString::~CCharString @ 0x0099EAE0, CTCBerserk::CreateStartingEffect @ 0x0078C4C0,
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 167 = bit 7 of the bitset dword at +0x34, the sign test
// `test cl,cl ; jns`).
// Shape: `this` is dead after the base call, so esi is advanced to the PCreature address in place (`add esi,8`); the name is a scoped
// named local re-addressed with `lea`; the berserk component rides in edi across the string destructor and is not null-checked; the
// two stores into the looked-up component are immediates. Retail CCombatActionBase is 0x114 bytes (the engine header is the PDB's
// 0x12c-byte layout), so `this` is modelled locally.
#include "fable_string.h"
class CTCParamsBase;
class CTCBerserk { public:
    void CreateStartingEffect();            // 0x0078C4C0
};
class CTCBase;
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
    CTCBerserk* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CTCStrengthBoost {                   // the interface-167 component: a flag byte at +0xc and a strength word at +0x10
    unsigned char _pad[0xc];
    unsigned char m_active;                 // +0x0c
    unsigned char _pad0d[3];
    long m_boostLevel;                      // +0x10
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;      // +8
    unsigned char _pad10[0x114 - 0x10];
};

class CCreatureAction_CastStrengthBerserk : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_CastStrengthBerserk::Initialise()
{
    CCombatActionBase::Initialise();
    CTCBerserk* pBerserk;
    {
        CCharString name("CTCBerserk", -1);
        pBerserk = PCreature.GetPItem()->AddTC(name, true, 0);
    }
    pBerserk->CreateStartingEffect();
    CTCStrengthBoost* pBoost;
    if (PCreature.GetPItem()->GetInterface(&pBoost, 167)) {
        pBoost->m_active = 1;
        pBoost->m_boostLevel = 8;
    }
}