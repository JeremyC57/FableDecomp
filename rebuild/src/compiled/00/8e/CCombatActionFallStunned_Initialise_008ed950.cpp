// CCombatAction_FallStunned::Initialise() -- push a fresh CModeDazed (not yet started) onto the creature's mode manager, then the base CCombatActionBase::Initialise
// Leaf: Initialise
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ED950, 192 bytes incl. padding (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8, twice), CVectorMap::LowerBound @ 0x0040F020
//          (inlined GetInterface 0x31 = CTCCreatureModeManager, bit 17 of +0x24; the result is not tested, so a missing
//          manager leaves the slot unwritten), operator new @ 0x00BFEA1A (0x84-byte CModeDazed, 12-byte count block),
//          CModeDazed::CModeDazed(CThing*) @ 0x00740160, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>)
//          @ 0x0073ACC0, CCombatActionBase::Initialise @ 0x00857520; count-block destroy fn 0x007180B0.
// Layout: retail CCombatActionBase model (PCreature +0x08); CModeDazed +0x6c a started flag.
// Shape: the by-value counted pointer is built in the argument area by the inline CCountedPointer(T*) ctor of the
//          landed 0x00718A00 (a user copy ctor makes the argument build in place); its null test folds away because the new mode was already dereferenced (`[eax+6Ch]`).
// Naming (NF144, 2026-11-20; q6408): the local empty `struct CModeBase { };` and the local CTCCreatureModeManager replaced by the shared
//   engine/CTCCreatureModeManager.h (ADDMODEPOINTER, nested CModeBase); 0x0073ACC0 now binds the Ego_r.pdb
//   ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z (was ...QAE_NV?$CCountedPointer@UCModeBase@@@@@Z) and
//   0x00718A00 the FableWin.pdb ??0?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@QAE@PAVCModeBase@CTCCreatureModeManager@@@Z (was
//   ??0?$CCountedPointer@UCModeBase@@@@QAE@PAUCModeBase@@@Z). The header-less mode class keeps its token and its constructor spelling, now on
//   the shared CModeBase (the landed 0x0096DBC0 shape), its pad resized so sizeof stays the retail allocation. The local CCountedPointer<T> /
//   SCountBlock models are replaced by the shared engine/CCountedPointer.h (its default inline (T*) ctor: `new CCPPointerInfo(1, DeleteData,
//   Data)`), so the count block's deleter 0x007180B0 binds its self spelling ?DeleteData@?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@SIXPAX@Z
//   (FableWin.pdb; was the free stand-in ?Destroy_007180b0@@YIXPAX@Z).
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
struct CModeDazed : CTCCreatureModeManager::CModeBase {
    unsigned char _pad[0x6c - 0x1c];
    bool          Started;                      // +0x6c
    unsigned char _pad6d[0x84 - 0x6d];
    CModeDazed(CThing* creature);               // 0x00740160
    virtual unsigned long GetType() const;
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
};

class CCombatAction_FallStunned : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCombatAction_FallStunned::Initialise()
{
    CTCCreatureModeManager* modeManager;
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
    CModeDazed* mode = new CModeDazed(PCreature.Get());
    mode->Started = false;
    modeManager->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(mode));
    CCombatActionBase::Initialise();
}