// CTCCreatureGroupBoastingCrowd::EnterCrowdState(long state) -- leave the current crowd state (pop one pushed hero opinion
// source, remove the pending opinion deed from the hero's log) and enter the new one: push its opinion source on the hero
// and log its opinion deed (state 0 kills the crowd thing); nothing happens when already in the state or in state 10
// Leaf: EnterCrowdState
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
//
// Retail 0x00784260, 614 bytes (`ret 4`, jump table at 0x007844C8).
// Callees: CTCBase::GetWorld @ 0x00686A70, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, COpinionReactionManager::
// ClearOpinionSource @ 0x007008C0 / AddOpinionSource @ 0x00700780, CVectorMap::LowerBound @ 0x0040F020 (the hero's deed
// log 0x51), CTCHeroOpinionDeedLog::RemoveDeed @ 0x006C5920 / AddDeed @ 0x006C6990, CCharString(const char*, long)
// @ 0x0099EBF0 / operator=(const char*) @ 0x0099EFE0 / operator!=(const char*) @ 0x0099E960 / dtor @ 0x0099EAE0,
// CThing::Kill(bool) @ 0x004C9B80, CThing::GetPos @ 0x004C73D0.
// Layout: as the landed FrameUpdate (0x00784500): PHero +0x24, CrowdState +0x2c, CurrentOpinionDeedHandle +0x38,
// HeroOpinionSourcesPushed +0x3c; CWorld opinion reaction manager +0x74.
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct COpinionSourceParams {                                // 4 bytes, left uninitialised (retail homes it in the dead lookup-key slot)
    long Reserved;
    COpinionSourceParams() { }
};
class COpinionReactionManager { public:
    void ClearOpinionSource(CThing* source, bool immediately);   // 0x007008C0
    bool AddOpinionSource(CThing* source, const CCharString& name, const COpinionSourceParams& params);   // 0x00700780
};
class CWorld { public:
    unsigned char _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager;        // +0x74
};
#include "engine/EOpinionDeedType.h"
enum EOpinionPostDeedType { };                   // Ego_r.pdb enum (values used here by number)
class CFaction;
class CThingAICreature;
class CTCHeroOpinionDeedLog { public:
    void RemoveDeed(long handle);                            // 0x006C5920
    long PostDeed(EOpinionDeedType type, const C3DVector& position, const CThing* thing, CThingAICreature* creature,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long l1);   // 0x006C6990 Ego_r.pdb / FableWin.pdb (engine/CTCHeroOpinionDeedLog.h)
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCBase
{
public:
    virtual void v00();
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCCreatureGroup : public CTCBase
{
public:
};

class CTCCreatureGroupBoastingCrowd : public CTCCreatureGroup { public:
    CThing* ParentThing;                                     // +0x04 (CTCBase)
    unsigned char _pad08[0x24 - 0x08];
    CIntelligentPointer<CThing> PHero;                           // +0x24
    long CrowdState;                                         // +0x2c
    long CurrentEncouragementState;                          // +0x30
    bool HeroBoasted;                                        // +0x34
    bool HeroMadeGoodBoast;                                  // +0x35
    long CurrentOpinionDeedHandle;                           // +0x38
    long HeroOpinionSourcesPushed;                           // +0x3c
    long FrameToClearRemainingOpinionSources;                // +0x40
    long FrameToLeavePostBoastState;                         // +0x44
    void EnterCrowdState(long state);
};

void CTCCreatureGroupBoastingCrowd::EnterCrowdState(long state)
{
    if (CrowdState == state || CrowdState == 10)
        return;
    if (HeroOpinionSourcesPushed > 0) {
        COpinionReactionManager* manager = GetWorld().POpinionReactionManager;
        manager->ClearOpinionSource(PHero.Get(), true);
        --HeroOpinionSourcesPushed;
    }
    if (CurrentOpinionDeedHandle != -1) {
        CTCHeroOpinionDeedLog* log;
        if (PHero.Get() && !PHero.Get()->IsDead() && PHero.Get()->GetTC(TCI_HERO_OPINION_DEED_LOG, &log))
            log->RemoveDeed(CurrentOpinionDeedHandle);
        CurrentOpinionDeedHandle = -1;
    }
    CCharString sourceName("");
    long deedType = 0;
    switch (state) {
    case 0:
        ParentThing->Kill(true);
        break;
    case 1:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ANTICIPATION";
        deedType = 0x40;
        break;
    case 2:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ENCOURAGEMENT";
        deedType = 0x41;
        break;
    case 3:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ENCOURAGEMENT";
        deedType = 0x42;
        break;
    case 4:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ENCOURAGEMENT";
        deedType = 0x43;
        break;
    case 5:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_POST_BOAST_GOOD";
        break;
    case 6:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_POST_BOAST_EVIL";
        break;
    case 7:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_WELL_WISHES";
        deedType = 0x44;
        break;
    case 8:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ILL_WISHES";
        break;
    case 9:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ANNOYED_NO_BOASTING";
        deedType = 0x45;
        break;
    case 10:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_HERO_ATTACKED_CROWD";
        break;
    }
    if (sourceName != "") {
        COpinionReactionManager* manager = GetWorld().POpinionReactionManager;
        if (manager->AddOpinionSource(PHero.Get(), sourceName, COpinionSourceParams()))
            ++HeroOpinionSourcesPushed;
    }
    if (deedType) {
        CTCHeroOpinionDeedLog* log;
        if (PHero.Get() && !PHero.Get()->IsDead() && PHero.Get()->GetTC(TCI_HERO_OPINION_DEED_LOG, &log))
            CurrentOpinionDeedHandle = log->PostDeed((EOpinionDeedType)deedType, PHero.Get()->GetPos(), 0, 0, (EOpinionPostDeedType)2, false, true, 0, false, 0);
    }
    CrowdState = state;
}