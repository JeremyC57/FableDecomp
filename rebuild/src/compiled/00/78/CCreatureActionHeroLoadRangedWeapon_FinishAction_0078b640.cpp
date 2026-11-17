// CCreatureAction_HeroLoadRangedWeapon::FinishAction() -- load the weapon if the action finished, base FinishAction, then drop this action's entry from the creature's type-0x8C component map and let it recalculate
// Leaf: ?FinishAction@CCreatureAction_HeroLoadRangedWeapon@@UAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078B640, length 111 bytes (+ int3 padding). Vtable 0x0126B9D4 slot 4.
// Callees: CCreatureAction_HeroLoadRangedWeapon::SetWeaponLoaded @ 0x0078B6B0 (thiscall, `this` still in ecx),
// CCreatureActionBase::FinishAction @ 0x00694430 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x8C = bit 12 of the presence word at +0x30, `test ch,10h`),
// the CTCHaste component's map erase @ 0x0058119B (a frame-pointer tree erase(const key&); the manifest label
// "CBankFile::CompareDataOffset::operator()" is propagated), CTCHaste::UpdateActionSpeedMultiplier @ 0x00800020 (manifest
// "CAnimation_RecalculateScale": it resets +0xc to 1.0f and multiplies in every map value).
// Shape: `Finished` is CCreatureActionBase +0x61 (PDB header, which agrees with retail below +0x84); the erase key is `this`
// bound to a `const&` parameter -- VC builds the temporary in the lookup-key frame word (`mov [esp+0Ch],edi` after the
// push), the component stays in esi for both calls. Retail CCreatureActionBase is 0xa8 bytes (the PDB vectors are 16 bytes,
// retail 12), so the class is modelled locally.
// NF64 (round 7, q10667 / q10668): the local CTCActionScaled / CActionKeyedMap stand-ins replaced by engine/CTCHaste.h (the type-0x8C
//   component is CTCHaste: Ego_r.pdb std::map<void*,float> Multipliers at +0x18; FABLE_RETAIL_CTCHASTE_UNSET_INLINE, NF61's opt-in:
//   ?UnsetActionSpeedMultiplier@CTCHaste@@QAEXPAX@Z with its body visible, which retail expands here). The erase now binds the STLport
//   _Rb_tree::erase(const key_type&) spelling at 0x0058119B and the update the private ?UpdateActionSpeedMultiplier@CTCHaste@@AAEXXZ at
//   0x00800020 (was CActionKeyedMap::Erase / CTCActionScaled::Recalculate).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_UNSET_INLINE
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHaste.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CCreatureActionBase;
class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00694430
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x61 - 0x10];      // +0x10
    bool Finished;                          // +0x61
    unsigned char _pad62[0xa8 - 0x62];      // +0x62
};

class CCreatureAction_HeroLoadRangedWeapon : public CCreatureActionBase { public:
    virtual void FinishAction();
    void SetWeaponLoaded();                 // 0x0078B6B0
};

void CCreatureAction_HeroLoadRangedWeapon::FinishAction()
{
    if (Finished)
        SetWeaponLoaded();
    CCreatureActionBase::FinishAction();
    CTCHaste* haste;
    if (PCreature.Get()->GetTC(TCI_HASTE, &haste))
        haste->UnsetActionSpeedMultiplier(this);
}