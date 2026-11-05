// CCombatActionBase::Initialise() -- resets the combat state, plays the combat animation the creature picks from the candidates, runs the base Initialise and, for a strike action, advances the creature's combo chain
// Leaf: ?Initialise@CCombatActionBase@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 28 (family d4): 0x00666B50 is ?GetAppropriateCombatAnimationFromList@CThingCreatureBase@@QBEJABV?$vector@PBVCAnimationEntry@@..@Z
//   (Ego_r.pdb / FableWin.pdb const member, DECISIONS_W27 D4) over the STLport vector<const CAnimationEntry*>. Was CThing::..(const CAnimationEntryVector&) (QAE); PCreature is the Ego_r
//   CIntelligentPointer<CThingCreatureBase>.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00857520, 145 bytes of code (virtual slot 12).
// Callees: the combat-state reset @ 0x008561C0 (clears the strike-event flags/times; manifest "CV_ChickenKickingScript::Init"
//          is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice through one
//          `lea ebx,[esi+8]`), CThingCreatureBase::GetAppropriateCombatAnimationFromList(const vector&) @ 0x00666B50,
//          CCreatureActionBase::SetAnimation(const CAnimationEntry*, float, float) @ 0x00693030,
//          CCreatureActionBase::Initialise @ 0x00694D10 (direct base call), virtual IsStrikeAction (slot 15, +0x3c),
//          CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x30 = bit 16 of the bitset dword at +0x24),
//          CTCCombat::SetNextComboChainPos(CCombatActionBase*) @ 0x00734570.
// Shape: the candidate vector's address (edi) is pushed before the creature is fetched (an inline object expression);
//          the chosen entry is read the STLport way, `*(begin() + index)`; SetAnimation gets -1.0 / 1.0; the landed
//          out-parameter GetInterface template (0x008CB4A0 family) feeds the combat component.
// Members (retail layout, Ego_r.pdb names): CombatAnimations +0xf0 (PDB +0x104), so `this` is modelled locally.
class CTCBase;
class CAnimationEntry;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public:
    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& animations) const;   // 0x00666B50
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

class CCombatActionBase;
class CTCCombat { public:
    void SetNextComboChainPos(CCombatActionBase* action);   // 0x00734570
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12: 0x00694D10
    virtual void v13(); virtual void v14();
    virtual bool IsStrikeAction();          // slot 15 (+0x3c)
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    void SetAnimation(const CAnimationEntry* entry, float blendIn, float speed);   // 0x00693030
};

class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _pada8[0xf0 - 0xa8];
    std::vector<const CAnimationEntry*> CombatAnimations; // +0xf0
    unsigned char _padfc[0x114 - 0xfc];
    void ResetCombatState();                // 0x008561C0
    virtual void Initialise();
};

void CCombatActionBase::Initialise()
{
    ResetCombatState();
    long index = PCreature.GetPItem()->GetAppropriateCombatAnimationFromList(CombatAnimations);
    SetAnimation(CombatAnimations[index], -1.0f, 1.0f);
    CCreatureActionBase::Initialise();
    if (IsStrikeAction()) {
        CTCCombat* pCombat;
        if (PCreature.GetPItem()->GetTC(TCI_COMBAT, &pCombat))
            pCombat->SetNextComboChainPos(this);
    }
}