// CTCCreatureGroupBoastingCrowd::OnHeroLeaveBoastingArea() -- the hero left the boasting area: after a boast the crowd
// goes to well/ill wishes (7/8), otherwise to "annoyed, no boasting" (9) while the quest manager still has boasts, or
// dies (0); the remaining opinion sources clear after 30..60 seconds
// Leaf: OnHeroLeaveBoastingArea
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
//
// Retail 0x007846B0, 624 bytes (plain `ret`); EnterCrowdState (0x00784260) is defined in this TU and retail inlines the
// constant-9 call.
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
    static long __fastcall GetFrame();   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
};
class CTCHeroOpinionDeedLog { public:
    void RemoveDeed(long handle);                            // 0x006C5920
    long AddDeed(long type, const C3DVector& position, long a, long b, long c, long d, long e, long f, long g, long h);   // 0x006C6990
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public:
    unsigned char _pad00[0x68];
    struct SNode { SNode* next; SNode* prev; };
    SNode* BoastsNode;                                       // +0x68 (list head node)
    bool BoastsEmpty() const { return BoastsNode->next == BoastsNode; }
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
class CTCBase
{
public:
    virtual void v00();
protected:
    unsigned long* GetPWorldSeed() const;   // 0x00686DA0
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
    void OnHeroLeaveBoastingArea();
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
            CurrentOpinionDeedHandle = log->AddDeed(deedType, PHero.Get()->GetPos(), 0, 0, 2, 0, 1, 0, 0, 0);
    }
    CrowdState = state;
}

void CTCCreatureGroupBoastingCrowd::OnHeroLeaveBoastingArea()
{
    if (HeroBoasted) {
        long fps = CMainGameComponent::GetConstantFPS();
        float delay = (GFFloatRandom(0.5f, GetPWorldSeed()) + 0.5f) * fps * 60.0f;
        FrameToClearRemainingOpinionSources = (long)(CWorld::GetFrame() + delay);
        EnterCrowdState(HeroMadeGoodBoast ? 7 : 8);
        return;
    }
    if (!CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().BoastsEmpty()) {
        long fps = CMainGameComponent::GetConstantFPS();
        float delay = (GFFloatRandom(0.5f, GetPWorldSeed()) + 0.5f) * fps * 60.0f;
        FrameToClearRemainingOpinionSources = (long)(CWorld::GetFrame() + delay);
        EnterCrowdState(9);
    } else
        EnterCrowdState(0);
}