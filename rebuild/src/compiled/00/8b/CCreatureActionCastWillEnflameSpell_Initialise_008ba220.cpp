// CCreatureAction_CastWillEnflameSpell::Initialise() -- base Initialise, then the owner's CTCWillEnflame component (looked up, or added by name when missing) is activated and aimed at the action's target with the Z-targetting flag; an AI-state owner gives it the cast level, and a live target thing is handed over too
// Leaf: ?Initialise@CCreatureAction_CastWillEnflameSpell@@UAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BA220, 323 bytes (336 with padding).
// Callees: CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, ctor 0x00A01B10,
//          SetPItem 0x00A01B90, dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C); CVectorMap::LowerBound
//          0x0040F020 (inlined GetInterface, type 0x95 = bit 21 of the bitset dword +0x30); CThing::AddTC(const
//          CCharString&, bool, CTCParamsBase*) 0x004C9D60 with the "CTCWillEnflame" literal 0x0123C444 (CCharString
//          ctor 0x0099EBF0 / dtor 0x0099EAE0); CTCWillEnflame::SetPTarget(CThing*, bool) 0x007A4350 (label from the
//          propagated CTCAssassinRush name).
//
// Shape: the landed CastWillGhostSwordSpell::Initialise model (0x008B9970) with the component taken from the
// interface map when present (the out-parameter GetInterface: dword bit test, inline LowerBound) and added by name
// otherwise (an implicit literal temporary); the component rides in edi; the Z-targetting byte is zero-extended for SetPTarget.
// Layout: local model -- retail CCombatActionBase is 0x114 bytes (PCreature +8, PItemRemoved +0x2c, the target
// pointer +0xa8); engine/CCreatureAction_CastWillEnflameSpell.h has Level +0x12c / IsZTargetting +0x130 (PDB), retail
// +0x114 / +0x118. The component's fields (active byte +0xc, level +0x14, target pointer +0x18) are descriptive.
// Frame: the same-TU CVectorMap::LowerBound stand-in (auto_inline(off), above the caller) is what lets the lookup key
// share its frame word with the name temporary and the by-value operator= parameter, as retail (LTCG) does.
class CTCParamsBase;
class CBaseObject;
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CThing;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CThing* PItem;                              // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject*a0);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); }
    CIntelligentPointerThing& operator=(CIntelligentPointerThing other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCWillEnflame { public:                         // interface type 0x95
    unsigned char _pad00[0xc];
    bool          Active;                       // +0x0c
    unsigned char _pad0d[7];
    long          Level;                        // +0x14
    CIntelligentPointerThing PTarget;           // +0x18
    void SetPTarget(CThing* target, bool zTargetting);   // 0x007A4350
};
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char m_state;                      // +0x10
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    CBaseObject* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances to the map once
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CCombatActionBase { public:
    virtual void v00();
    long _pad04;
    CBaseIntelligentPointer PCreature;          // +0x08 (the owning creature)
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointerThing PItemRemoved;      // +0x2c (CCreatureActionBase, Ego_r.pdb name; the spell's target thing)
    unsigned char _pad34[0xa8 - 0x34];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    virtual void Initialise();                  // 0x00857520
};

class CCreatureAction_CastWillEnflameSpell : public CCombatActionBase { public:
    long Level;                                 // +0x114 (PDB +0x12c)
    bool IsZTargetting;                         // +0x118 (PDB +0x130)
    virtual void Initialise();
};

void CCreatureAction_CastWillEnflameSpell::Initialise()
{
    CCombatActionBase::Initialise();
    CTCWillEnflame* tc;
    if (!((CThing*)PCreature.GetPItem())->GetInterface(&tc, 0x95))
        tc = (CTCWillEnflame*)((CThing*)PCreature.GetPItem())->AddTC("CTCWillEnflame", true, 0);
    tc->Active = true;
    CThing* aim = ((CThing*)PTarget.GetPItem());
    tc->SetPTarget(aim, IsZTargetting);
    if (((CThing*)PCreature.GetPItem())->m_state == 1)
        tc->Level = Level;
    CIntelligentPointerThing target = PItemRemoved;
    if (((CThing*)target.GetPItem()) && !(((CThing*)target.GetPItem())->m_flags91 & 1))
        tc->PTarget = target;
}