// CCreatureAction_BalverineDropInFromSky::FinishAction() -- sets the byte at +0x10 of the creature's interface-9 component, restores the graphic appearance's alpha to 255, marks the creature willable again, then runs the base FinishAction
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CCB00, length 116 bytes (+ int3 padding). FinishAction is vtable slot 4 (protected `MAE`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), CVectorMap::LowerBound @ 0x0040F020 (the inlined
// GetInterface lookup for type 9 = bit 9 of the bitset dword at +0x20, `test ch,2`), CTCGraphicAppearance::SetAlpha(unsigned char)
// @ 0x004BFAC0 (on the component at +0x64 of the creature), SetThingAsNotBloodyWillableAtAll(CThing*, bool) @ 0x007E4040 (`__fastcall`:
// ecx thing, dl flag, `xor dl,dl`), CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, last).
// Shape: `this` rides in ebx and the PCreature address in edi; esi is pushed only inside the lookup block. Retail CCombatActionBase is
// 0x114 bytes (the engine header is the PDB's 0x12c-byte layout), so `this` is modelled locally.
// Naming wave 17: CTCGraphicAppearance is NGraphicAppearance::CTCGraphicAppearance (Ego_r ?SetAlpha@CTCGraphicAppearance@NGraphicAppearance@@QAEXE@Z; a class) -- the local model moved into the namespace (name-only).
#include "fable_compiler.h"
namespace NGraphicAppearance { class CTCGraphicAppearance; }
using NGraphicAppearance::CTCGraphicAppearance;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad54[0xc];
    CTCPhysicsBase* m_pPhysics;             // +0x60
    CTCGraphicAppearance* m_pGraphicAppearance;   // +0x64
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }                           // inline accessors: the member is
    CTCGraphicAppearance* GetGraphicAppearance() const { return m_pGraphicAppearance; }   //   loaded straight into ecx
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
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
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    void SetAlpha(unsigned char alpha);     // 0x004BFAC0
};
} // namespace NGraphicAppearance

struct CTCCreatureFlags9 {                  // the interface-9 component: one byte at +0x10 toggled by this action
    unsigned char _pad[0x10];
    unsigned char m_canBeTargeted;          // +0x10
};
void __fastcall SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable);   // 0x007E4040

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_BalverineDropInFromSky : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_BalverineDropInFromSky::FinishAction()
{
    CTCCreatureFlags9* pFlags;
    if (PCreature.GetPItem()->GetInterface(&pFlags, 9))
        pFlags->m_canBeTargeted = 1;
    PCreature.GetPItem()->GetGraphicAppearance()->SetAlpha(255);
    SetThingAsNotBloodyWillableAtAll(PCreature.GetPItem(), false);
    CCombatActionBase::FinishAction();
}