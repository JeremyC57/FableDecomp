// CCreatureAction_SnapDragonOutOf::Initialise() -- set the combat animation SNAP_ATTACK_OUTOF, run the CCombatActionBase initialisation, then remove creature mode 0x4F through the owner's mode manager when it has one
// Leaf: ?Initialise@CCreatureAction_SnapDragonOutOf@@UAEXXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F3A50, 117 bytes (+ int3 padding to 128), thiscall, plain `ret` (virtual).
// Identity (proven): CCreatureAction_SnapDragonOutOf::Initialise() (Ego_r.pdb / FableWin.pdb ?Initialise@CCreatureAction_SnapDragonOutOf@@UAEXXZ); evidence: slot 12 of the CCreatureAction_SnapDragonOutOf vtable 0x01286D94 (RTTI; bases CCombatActionBase <- CCreatureActionBase <- CBaseObject <- CBaseClass); string 0x01288474 "SNAP_ATTACK_OUTOF"
// Label: the manifest's "CCreatureAction_SnapDragonOutOf::Initialise" (doubled class) names this row's own class; family J019: six "out of" combat
//   actions that differ in the animation string, the mode removed and (BriarRose) the base whose Initialise runs.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (??0CCharString@@QAE@PBDJ@Z) and ~CCharString 0x0099EAE0,
//   CCombatActionBase::SetCombatAnimation(const CCharString&) 0x008574D0 (Ego_r.pdb; the catalog also lists SetPDef / SetName /
//   SetAnimName stand-ins there), CCombatActionBase::Initialise() 0x00857520 (catalog-bound, Ego_r.pdb ?Initialise@CCombatActionBase@@UAEXXZ),
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (Ego_r.pdb), CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound
//   0x0040F020 (as J018), CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0 (Ego_r.pdb spelling, global class).
// Shape: the animation name is a block-scoped local (ctor / call / dtor before the base call); the qualified base Initialise; CThing::GetTC
//   (interface 0x31 = CTCCreatureModeManager: bit 17 of UsingComponents word 1, `test [eax+24h], 20000h`) with its result
//   tested. Layout: CCreatureActionBase PCreature +0x08 (Ego_r.pdb, CIntelligentPointer<CThingCreatureBase>); retail
//   CCombatActionBase is 0x114 bytes. Local model: engine/CCreatureActionBase.h carries the Ego_r layout (0xB4) and a
//   data-member vptr.
#include <stddef.h>
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;

class CCharStringData;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                              // 8 bytes (Ego_r.pdb)
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
class CThingCreatureBase : public CThing { };
class CTCCreatureModeManager {
public:
    void RemoveMode(unsigned long mode);                     // 0x0073ABA0
};
class CCreatureActionBase {                                  // 0xA8 bytes in retail (vptr first)
public:
    virtual ~CCreatureActionBase();
    virtual void Initialise();                               // retail slot 12
    unsigned char _pad04[4];                                 // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    unsigned char _body10[0xA8 - 0x10];
};
class CCombatActionBase : public CCreatureActionBase {      // 0x114 bytes in retail
public:
    virtual void Initialise();                               // 0x00857520
    void SetCombatAnimation(const CCharString& name);        // 0x008574D0
    unsigned char _bodyA8[0x114 - 0xA8];
};

class CCreatureAction_SnapDragonOutOf : public CCombatActionBase {                           // RTTI: CCreatureAction_SnapDragonOutOf <- CCombatActionBase <- CCreatureActionBase <- CBaseObject <- CBaseClass
public:
    virtual void Initialise();
};

void CCreatureAction_SnapDragonOutOf::Initialise()
{
    {
        CCharString name("SNAP_ATTACK_OUTOF");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager))
        modeManager->RemoveMode(0x4F);
}