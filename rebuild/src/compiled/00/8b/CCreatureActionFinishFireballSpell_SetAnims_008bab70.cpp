// CCreatureAction_FinishFireballSpell::SetAnims() -- if the caster has a fireball-spell component (interface 0x9d), use its release animation name as the action's animation
// Leaf: ?SetAnims@CCreatureAction_FinishFireballSpell@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BAB70, length 98 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020,
//          CTCFireballSpell::GetReleaseAnimName() (CCharString by value, hidden result pointer) @ 0x007C86F0,
//          CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0 (manifest label
//          "CTCInGameMenu::SetPDef" is propagated), CCharString::~CCharString @ 0x0099EAE0.
// Shape: the bool-returning out-parameter GetInterface template (landed AlertToPlayer 0x00849320 shape) in the `if`:
//   bit 0x9d is tested in memory (`test dword ptr [eax+30h],20000000h`); the returned string temporary lands in the
//   same `push ecx` frame word as the lookup key, is passed by reference and destroyed after the setter.
// Layout: CCombatActionBase (retail 0x114 bytes) with PCreature at +8; FinishFireballSpell adds nothing used here.
class CTCBase;

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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

class CTCFireballSpell { public:                   // interface type 0x9d component
    CCharString GetReleaseAnimName();       // 0x007C86F0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_FinishFireballSpell : public CCombatActionBase { public:
    void SetAnims();
};

void CCreatureAction_FinishFireballSpell::SetAnims()
{
    CTCFireballSpell* fireball;
    if (PCreature.GetPItem()->GetTC(TCI_FIREBALL_SPELL, &fireball))
        SetCombatAnimation(fireball->GetReleaseAnimName());
}