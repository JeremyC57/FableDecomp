// CCreatureAction_JackScream::Initialise() -- base Initialise, drop any running mode 0x31 from the creature's mode manager, create a fresh NCreatureMode::CModeJackOfBladesScream for the creature as this action's counted PMode, hand it the sword object carried in the primary slot and push it onto the mode manager
// Leaf: ?Initialise@CCreatureAction_JackScream@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C0D10, length 393 bytes (+ int3 padding).
// Callees: CCombatActionBase::Initialise @ 0x00857520, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface 0x31 = CTCCreatureModeManager, bit 17 of +0x24; 0x46 = CTCCarrying, bit 6 of +0x28; both untested),
// CTCCreatureModeManager::IsModeActive(long) @ 0x0073A8A0 and RemoveMode(long) @ 0x0073ABA0, operator new @ 0x00BFEA1A (0xbc-byte mode,
// 12-byte count block) / operator delete @ 0x00BFE9BC, NCreatureMode::CModeJackOfBladesScream(CThing*) @ 0x00740450,
// CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CModeJackOfBladesScream::SetSwordObject(CThing*) @ 0x008D7370,
// CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0; the count block's destroy fn for this mode type 0x008C2D60.
// Layout: retail CCombatActionBase is 0x114 bytes (the PDB header is 0x18 larger): PMode {object +0x114, count +0x118}.
// Shape: `PMode = CCountedPointer<T>(new T(creature))` -- the temporary's inline ctor (null-checked new, null-checked block), the
// inline operator= (release of the old block through the re-read member, adopt, add-ref) and the temporary's inline dtor; the
// AddMode argument is the converting copy built in the argument area. The mode manager is spilled to the lookup key slot.
// Naming (NF147, 2026-11-20; q6408): the local SCountBlock / CCountedPointer<T> template / empty CModeBase / CTCCreatureModeManager models
//   replaced by the shared engine/CCountedPointer.h (default inline (T*) ctor and ShareData operator=) and engine/CTCCreatureModeManager.h
//   (ADDMODEPOINTER + the FABLE_RETAIL_CTCCREATUREMODEMANAGER RemoveMode, nested CModeBase); 0x0073ACC0 now binds the Ego_r.pdb
//   ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z, and the block's deleter 0x008C2D60 the
//   CCountedPointer<T>::DeleteData of the row's T (was the free stand-in Destroy_008c2d60; the FableWin.pdb spelling
//   ?DeleteData@?$CCountedPointer@VCModeJackOfBladesScream@NCreatureMode@@@@SIXPAX@Z follows when the class moves into NCreatureMode).
//   The header-less mode class (only forward-declared in engine/CCreatureAction_JackScream.h) keeps HEAD's global `struct` token, re-based on
//   the shared CModeBase (the landed 0x0096DBC0 shape), pad resized (allocation 0xbc). Landing nf147: NF147's in-place respell into
//   NCreatureMode moved the 0x00740450 ctor binding off the global spelling 0x00742880 still binds (a new split), so its fallback applies:
//   0x00740450 / 0x008D7370 stay on the global spellings and 0x008C2D60 binds DeleteData of CCountedPointer<CModeJackOfBladesScream> (global).
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

#include "engine/CTCCreatureModeManager.h"
struct CModeJackOfBladesScream : public CTCCreatureModeManager::CModeBase {    // retail sizeof 0xbc
    unsigned char _body[0xbc - 0x1c];
    CModeJackOfBladesScream(CThing* owner);     // 0x00740450
    void SetSwordObject(CThing* sword);         // 0x008D7370
    virtual unsigned long GetType() const;
};
class CTCCarrying { public:
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char _pad10[0x114 - 0x10];
};
class CCreatureAction_JackScream : public CCombatActionBase { public:
    CCountedPointer<CModeJackOfBladesScream> PMode;   // +0x114
    virtual void Initialise();
};

void CCreatureAction_JackScream::Initialise()
{
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
    if (modeManager->IsModeActive(0x31))
        modeManager->RemoveMode(0x31);
    PMode = CCountedPointer<CModeJackOfBladesScream>(new CModeJackOfBladesScream(PCreature.Get()));
    CTCCarrying* carrying;
    PCreature.Get()->GetTC(TCI_CARRYING, &carrying);
    PMode.Data->SetSwordObject(carrying->GetThingInPrimarySlot());
    modeManager->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(PMode));
}