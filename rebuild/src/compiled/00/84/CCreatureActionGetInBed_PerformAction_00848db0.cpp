// CCreatureAction_GetInBed::PerformAction() -- push a new NCreatureMode::CModeSleepBed(creature, AnimGroup, SleepAnimName) onto the creature's mode manager, then when the bed is alive and has a CTCBed component start its get-in animation (BedGetInAnim, BedSleepAnim) or, with no animation, just mark the bed as occupied
// Leaf: ?PerformAction@CCreatureAction_GetInBed@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00848DB0, length 343 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager, bit 17 of +0x24, untested; 0x35 = CTCBed, bit 21 of +0x24, tested), operator new @ 0x00BFEA1A (0x34-byte
// mode, 12-byte count block), CModeBase::CModeBase(CThing*) @ 0x0073EAC0, CAnimFilterOfGroup(const CCharString&) @ 0x0073E8F0,
// CCharString copy ctor @ 0x0099EC30, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0, CTCBed::
// StartGetInBedAnimation(long, long) @ 0x007469C0; count-block destroy fn 0x007180B0; CModeSleepBed vtable 0x01275760.
// Layout: retail CCreatureActionBase is 0xa8 bytes (the PDB header is 12 larger): PBed +0xa8, AnimGroup +0xb0, SleepAnimName +0xb4,
// BedGetInAnim +0xb8, BedSleepAnim +0xbc; CTCBed +0x0c Empty (the landed 0x007469C0 naming).
// Shape: GetTC written as the end/hit ternary leaves the bed component in ecx for the call; the mode ctor is inline (base ctor, vtable, filter, name copy, two zeroed longs); the counted pointer is built in the
// argument area with its null-checked count block (the mode was never dereferenced); IS_THING_ALIVE on PBed then HasTC/GetTC.
// Naming (NF147, 2026-11-20; q6408): the local SCountBlock / CCountedPointer<T> template (inline ctor storing the Destroy_007180b0
//   stand-in) / CModeBase(CThing*) / CTCCreatureModeManager models replaced by the shared engine/CCountedPointer.h (default inline (T*) ctor)
//   and engine/CTCCreatureModeManager.h (ADDMODEPOINTER; FABLE_RETAIL_CMODEBASE_CTOR, nested CModeBase); 0x0073ACC0 now binds the Ego_r.pdb
//   ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z, 0x0073EAC0 the PDB
//   ??0CModeBase@CTCCreatureModeManager@@QAE@AAVCThing@@@Z and the block's deleter 0x007180B0 its self spelling
//   ?DeleteData@?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@SIXPAX@Z. The header-less mode class keeps its token (vtable symbol
//   unchanged), now on the shared CModeBase (the landed 0x0096DBC0 shape), allocation 0x34 unchanged.
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CMODEBASE_CTOR
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CAnimFilterOfGroup { public:                     // 12 bytes
    unsigned char _body[0xc];
    CAnimFilterOfGroup(const CCharString& group);   // 0x0073E8F0
};
#include "engine/CTCCreatureModeManager.h"
struct CModeSleepBed : CTCCreatureModeManager::CModeBase {   // retail sizeof 0x34, vtable 0x01275760
    CAnimFilterOfGroup AnimFilter;              // +0x1c
    CCharString SleepAnimName;                  // +0x28
    long GetInAnim;                             // +0x2c
    long SleepAnim;                             // +0x30
    CModeSleepBed(CThing* creature, const CCharString& group, const CCharString& animName)
        : CTCCreatureModeManager::CModeBase(*creature), AnimFilter(group), SleepAnimName(animName), GetInAnim(0), SleepAnim(0) {}
    virtual unsigned long GetType() const;
};
class CTCBed { public:
    unsigned char _pad[0xc];
    bool Empty;                                 // +0x0c
    void StartGetInBedAnimation(long getInAnim, long sleepAnim);   // 0x007469C0
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
class CCreatureAction_GetInBed : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PBed;               // +0xa8
    CCharString AnimGroup;                      // +0xb0
    CCharString SleepAnimName;                  // +0xb4
    long BedGetInAnim;                          // +0xb8
    long BedSleepAnim;                          // +0xbc
    void PerformAction();
};

void CCreatureAction_GetInBed::PerformAction()
{
    CTCCreatureModeManager* modeManager;
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
    CModeSleepBed* mode = new CModeSleepBed(PCreature.Get(), AnimGroup, SleepAnimName);
    modeManager->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(mode));
    if (IS_THING_ALIVE(PBed)) {
        CThing* bedThing = PBed.Get();
        if (bedThing->HasTC(TCI_BED)) {
            CTCBed* bed = ((CTCBed*)&bedThing->GetTC(TCI_BED));
            if (BedGetInAnim)
                bed->StartGetInBedAnimation(BedGetInAnim, BedSleepAnim);
            else
                bed->Empty = false;
        }
    }
}