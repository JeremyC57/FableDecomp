// CCreatureAction_JackFlash::FinishAction() -- for a flagged target, push a new CModeJackOfBladesFlash (aimed at the target) on the creature's mode manager and suspend a player creature's animation; then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_JackFlash@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C0C00, 270 bytes (272 with padding), virtual FinishAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0xa8, PCreature +0x08), CVectorMap::LowerBound 0x0040F020
//   (inlined unchecked lookup of 0x31 = CTCCreatureModeManager, bit 17 of +0x24), operator new 0x00BFEA1A (0x98 bytes),
//   NCreatureMode::CModeJackOfBladesFlash(CThing*) 0x008D7E60, CCountedPointer<CModeJackOfBladesFlash>(T*) 0x008C3260
//   (out of line), the mode's SetTarget(CThing*) 0x008D7320 (manifest "AssignCreatureToTask" is propagated),
//   CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0, CThingAICreature::SuspendAI(bool,
//   long) 0x00833A10, operator delete 0x00BFE9BC, CCombatActionBase::FinishAction 0x00859A20.
// Layout: retail CCombatActionBase is 0x114 bytes: PTarget +0xa8; CThing +0x10 the type byte (1 here: a player), +0x6c
//   flag bit 3.
// Shape: the target flag is read without a null test; the target is a named local; the mode manager comes from the
//   unchecked out-parameter lookup; the counted pointer is a named local (ctor out of line) copied by value into AddMode
//   (inline copy: object, count, addref) and released inline at the block end (count block destroy + delete);
//   `this` is spilled to the frame because edi is reused for &PCreature; the player creature is a named local so it is
//   fetched before SuspendAI's argument pushes.
// Naming (NF147, 2026-11-20; q6408): the local SCountBlock / empty CModeBase / CCountedPointer<T> template (with its same-TU ctor
//   specialisation 0x008C3260 and the DestroyJackOfBladesFlash deleter stand-in) and the local CTCCreatureModeManager replaced by the shared
//   engine/CCountedPointer.h (VISIBLE_CALLED_CTOR: the landed 0x008C4990 shape; the called ctor 0x008C3260 keeps its HEAD spelling) and
//   engine/CTCCreatureModeManager.h (ADDMODEPOINTER, nested CModeBase); 0x0073ACC0 now binds the Ego_r.pdb
//   ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z. The header-less mode class keeps its
//   token and its constructor / SetTarget spellings, now on the shared CModeBase (the landed 0x0096DBC0 shape), its pad resized so sizeof
//   stays the retail allocation (0x98).
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

#include "engine/CTCCreatureModeManager.h"
struct CModeJackOfBladesFlash : CTCCreatureModeManager::CModeBase {
    unsigned char _pad[0x98 - 0x1c];
    CModeJackOfBladesFlash(CThing* owner);            // 0x008D7E60
    void SetTarget(CThing* target);                   // 0x008D7320
    virtual unsigned long GetType() const;
};
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:                      // slot 4: 0x00859A20
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;                  // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
};
class CCreatureAction_JackFlash : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_JackFlash::FinishAction()
{
    if (PTarget.Get()->GetTypeFlags() & 8) {
        CThing* target = PTarget.Get();
        CTCCreatureModeManager* modes;
        PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
        CCountedPointer<CModeJackOfBladesFlash> mode(new CModeJackOfBladesFlash(PCreature.Get()));
        mode->SetTarget(target);
        modes->AddMode(mode);
        if (PCreature.Get()->GetType() == 1) {
            CThing* creature = PCreature.Get();
            ((CThingAICreature*)creature)->SuspendAI(true, 0);
        }
    }
    CCombatActionBase::FinishAction();
}