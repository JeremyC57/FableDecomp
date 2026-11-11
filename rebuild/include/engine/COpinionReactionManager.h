#ifndef FABLE_ENGINE_COPINIONREACTIONMANAGER_H
#define FABLE_ENGINE_COPINIONREACTIONMANAGER_H
// COpinionReactionManager (new, chunk 765): no RTTI (no virtuals). Ego_r.pdb members (struct_layouts_egor.tsv, 0x738 bytes) at retail
// offsets: the two STLport lists are 4 bytes (PDB 12), so every member after Reactions sits 8 lower (PThingOpinionSource +0x714:
// CGameScriptInterface::OpinionSourceSetAsExclusive 0x00894E9B reads it there; SetThingOpinionSource 0x00700770 sets it); sizeof 0x728.
// The EOpinion* enum arrays are modelled as long arrays (the enums are not declared in the engine headers). Name set (Ego_r / FableWin):
// ?SetThingOpinionSource@COpinionReactionManager@@QAEXPAVCThing@@@Z (Ego_r 0x00550510, then AttachOpinionSource 0x00550640, as retail
// 0x00700770 then 0x00700780). Only the methods rows use are declared.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CTCWIFE_MARRY
enum EOpinionAttitudeType;enum EOpinionReactionType;class CTCOpinionSource;
#endif
class CThing; class CWorld; class CThingAICreature; class CReactionMatch; class COpinionReactionManagerDef;
class COpinionReactionManager {
public:
#ifdef FABLE_RETAIL_CTCWIFE_MARRY
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing&)const;
    EOpinionAttitudeType GetAttitudeType(const CThing& thing,const CTCOpinionSource* source)const {
        return source?(EOpinionAttitudeType)GetAttitudeOrReactionType(thing,true,AttitudeMask,source):GetThingAttitudeToPlayer(thing);
    }
private:
    EOpinionReactionType GetAttitudeOrReactionType(const CThing&,bool,const bool*,const CTCOpinionSource*)const;
public:
#endif
    void SetThingOpinionSource(CThing* thing);                           // 0x00700770
private:
    CDefPointer<const COpinionReactionManagerDef> PDef;                  // +0x000
    CWorld* World;                                                       // +0x004
    bool ReactionFlagsTemp[79];                                          // +0x008
    std::list<CReactionMatch*> Reactions;                                // +0x058
    long AttitudeCondition[79];                                          // +0x05c EOpinionAttitudeType
    long TargetingCondition[79];                                         // +0x198 EOpinionTargetingConditionType
    unsigned long PreReactionDelaySeconds[79];                           // +0x2d4
    bool BlockFurtherReactionsFlags[79];                                 // +0x410
    bool AllowSpeechOnNonPureAISpeaker[79];                              // +0x45f
    bool AllowWhileCarrying[79];                                         // +0x4ae
    bool AllowWhileFollowingPlayer[79];                                  // +0x4fd
    bool FurtherReactionsBlocked;                                        // +0x54c
    unsigned long ReactionReenableFrame;                                 // +0x550
    bool AttitudeMask[79];                                               // +0x554
    long DeedReactionSpeechPlaying[92];                                  // +0x5a4
    CIntelligentPointer<CThing> PThingOpinionSource;                     // +0x714
    std::list<CIntelligentPointer<const CThing> > OpinionSourcesList;    // +0x71c
    CIntelligentPointer<const CThingAICreature> DebugCreature;           // +0x720
};
FABLE_STATIC_ASSERT(sizeof(COpinionReactionManager) == 0x728);
#endif
