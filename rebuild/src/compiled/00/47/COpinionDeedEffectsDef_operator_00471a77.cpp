// COpinionDeedEffectsDef::operator= -- copy deed timing, reactions and eleven policy bits.
// Retail00471a77 length252; Copy00471a72 gives identity. Actual donor header
// included. Native parent39 vs3d, list4 vs12, vectors12 vs16 map donor90->7c.
// Callees431ed0 parent,471b73 list<OpinionTransientOffset> (24-byte elements),
// 471cc5 vector<OpinionDeedReactionDef> (native80-byte),432ee9 vector<CCharString>.
#pragma optimize("s",on)
#define COpinionDeedEffectsDef COpinionDeedEffectsDefDonor
#include "engine/_quarantine/COpinionDeedEffectsDef.h"
#undef COpinionDeedEffectsDef
#define CParentDefClassBase CParentDefClassBaseDonor
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
#ifdef _MSC_VER
typedef long S32;
#else
typedef int S32;
#endif
#pragma pack(push,1)
struct CParentDefClassBase {
    void* __vftable;
    unsigned char DefClassBase[0x21]; // donor named flattened base, actual bytes4..24
    S32 InstantiationName_TablePos;
    unsigned char SubDefInfoMap[0x10]; // native vector12 + comparison/flags4
    CParentDefClassBase& operator=(const CParentDefClassBase&); // 0x00431ed0
};
#pragma pack(pop)

struct COpinionTransientOffset;
struct OpinionOffsetNode;
struct OpinionOffsetList { OpinionOffsetNode* Head; OpinionOffsetList& operator=(const OpinionOffsetList&); }; // 00471b73
struct COpinionDeedReactionDef; struct CCharString;
struct ReactionVector { COpinionDeedReactionDef *Start,*Finish,*End; ReactionVector& operator=(const ReactionVector&); }; // 00471cc5
struct WarningVector { CCharString *Start,*Finish,*End; WarningVector& operator=(const WarningVector&); }; // 00432ee9
struct COpinionDeedEffectsDef : CParentDefClassBase {
    S32 KnowledgeLifeInSeconds,MinimumSecondsBetweenPostings,MinimumSecondsBetweenEffectsForIndividual;
    float PreReactionDelayIgnoreOnDelay; OpinionOffsetList Effects;
    S32 ReactionPriority; ReactionVector Reaction;
    S32 ReactionDialogueLayer; WarningVector WarningText;
    S32 CrimeSeverity,CrimeFineAmount,Tutorial;
    bool CanBeHeard:1;
    bool ReactionRestrictToTarget:1;
    bool AllowSimultaneousReactionDialogue:1;
    bool CanApologise:1;
    bool MustNotRunWhenWarned:1;
    bool OnlyGetFinedForOneInstance:1;
    bool IsFlirting:1;
    bool OnReactSetGreeted:1;
    bool AllowSpeechOnNonPureAISpeaker:1;
    bool AllowIndirectReactWhileCarrying:1;
    bool BlockWhileFollowingPlayer:1;
    COpinionDeedEffectsDef& operator=(const COpinionDeedEffectsDef&);
};
COpinionDeedEffectsDef& COpinionDeedEffectsDef::operator=(const COpinionDeedEffectsDef& o) {
    CParentDefClassBase::operator=(o);
    KnowledgeLifeInSeconds=o.KnowledgeLifeInSeconds;
    MinimumSecondsBetweenPostings=o.MinimumSecondsBetweenPostings;
    MinimumSecondsBetweenEffectsForIndividual=o.MinimumSecondsBetweenEffectsForIndividual;
    PreReactionDelayIgnoreOnDelay=o.PreReactionDelayIgnoreOnDelay;
    Effects=o.Effects;
    ReactionPriority=o.ReactionPriority;
    Reaction=o.Reaction;
    ReactionDialogueLayer=o.ReactionDialogueLayer;
    WarningText=o.WarningText;
    CrimeSeverity=o.CrimeSeverity;
    CrimeFineAmount=o.CrimeFineAmount;
    Tutorial=o.Tutorial;
    CanBeHeard=o.CanBeHeard;
    ReactionRestrictToTarget=o.ReactionRestrictToTarget;
    AllowSimultaneousReactionDialogue=o.AllowSimultaneousReactionDialogue;
    CanApologise=o.CanApologise;
    MustNotRunWhenWarned=o.MustNotRunWhenWarned;
    OnlyGetFinedForOneInstance=o.OnlyGetFinedForOneInstance;
    IsFlirting=o.IsFlirting;
    OnReactSetGreeted=o.OnReactSetGreeted;
    AllowSpeechOnNonPureAISpeaker=o.AllowSpeechOnNonPureAISpeaker;
    AllowIndirectReactWhileCarrying=o.AllowIndirectReactWhileCarrying;
    BlockWhileFollowingPlayer=o.BlockWhileFollowingPlayer;
    return *this;
}