// CCreatureAction_ScorpionTailPlunge::FinishAction() -- when the target's flag bit 3 (+0x6c) is set, put the scorpion king into a new CModeScorpionKingTailPlunge aimed at the action's target through the owner's mode manager; then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_ScorpionTailPlunge@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C48A0, length 239 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager), operator new @ 0x00BFEA1A (0xbc bytes), CModeScorpionKingTailPlunge::CModeScorpionKingTailPlunge(CThing*)
// @ 0x008D81F0 (vtable ctor labelled by its vtable store), CCountedPointer<CModeScorpionKingTailPlunge>::CCountedPointer(T*)
// @ 0x008C58F0 (out of line, per-type deleter 0x008C57E0), CModeScorpionKingTailPlunge::SetTarget(CThing*) @ 0x008D7430,
// CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0, operator delete @ 0x00BFE9BC (the count block),
// CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: the twin of 0x008C4990 testing the TARGET's flag; the target is fetched again (ebx), then the UNCHECKED out-parameter GetInterface (the absent path reads the never
// written out pointer from the lookup key's frame word, which the counted-pointer local reuses afterwards); the counted
// pointer is converted into a CCountedPointer<CModeBase> built in the argument area (inline converting copy, add-ref) and
// released inline afterwards (block {refs, __fastcall destroy, object}, freed with operator delete). `this` is spilled to the
// frame across the block (register pressure).
// Layout: the owner (CIntelligentPointer<CThingCreatureBase>) at +8, the target at +0xa8 (retail CCreatureActionBase is 0xa8).
#include "fable_compiler.h"
// Naming (NF147, 2026-11-20; q6408): the local SCountBlock / empty CModeBase / CCountedPointer<T> template (with its same-TU ctor
//   specialisation and the DestroyModeScorpionKingTailPlunge deleter stand-in) and the local CTCCreatureModeManager replaced by the shared
//   engine/CCountedPointer.h (VISIBLE_CALLED_CTOR: the landed twin 0x008C4990's shape; the called ctor 0x008C58F0 keeps its HEAD spelling) and
//   engine/CTCCreatureModeManager.h (ADDMODEPOINTER, nested CModeBase); 0x0073ACC0 now binds the Ego_r.pdb
//   ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z. The header-less mode class keeps its
//   token and its constructor / SetTarget spellings, now on the shared CModeBase (the landed 0x0096DBC0 shape), its pad resized so sizeof
//   stays the retail allocation (0xbc).
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


#include "engine/CTCCreatureModeManager.h"

struct CModeScorpionKingTailPlunge : CTCCreatureModeManager::CModeBase {
    unsigned char _pad[0xbc - 0x1c];
    CModeScorpionKingTailPlunge(CThing* owner); // 0x008D81F0
    void SetTarget(CThing* target);             // 0x008D7430
    virtual unsigned long GetType() const;
};

class CCombatActionBase {
public:
    protected: virtual void FinishAction(); public:                // 0x00859A20
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    FABLE_FORCEINLINE CTCCreatureModeManager* GetModeManager() const
    {
        CTCCreatureModeManager* modeManager;
        PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
        return modeManager;
    }
};

class CCreatureAction_ScorpionTailPlunge : public CCombatActionBase {
public:
    virtual void FinishAction();
};

void CCreatureAction_ScorpionTailPlunge::FinishAction()
{
    if (PTarget.Get()->GetTypeFlags() & 8) {
        CThing* target = PTarget.Get();
        CTCCreatureModeManager* modeManager = GetModeManager();
        CCountedPointer<CModeScorpionKingTailPlunge> mode(new CModeScorpionKingTailPlunge(PCreature.Get()));
        mode->SetTarget(target);
        modeManager->AddMode(mode);
    }
    CCombatActionBase::FinishAction();
}