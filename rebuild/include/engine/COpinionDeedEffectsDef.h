#ifndef FABLE_ENGINE_COPINIONDEEDEFFECTSDEF_H
#define FABLE_ENGINE_COPINIONDEEDEFFECTSDEF_H
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
class CPersistContext;
class COpinionTransientOffset;
class COpinionDeedReactionDef;
#ifdef _MSC_VER
namespace NSpeechGainManager {enum EDialogueLayer;}
enum ECrimeSeverity;
enum ETutorialCategory;
#else
namespace NSpeechGainManager {enum EDialogueLayer{};}
enum ECrimeSeverity{};
enum ETutorialCategory{};
#endif
// Layout: both full original TPI records. Native parent39, first field3c;
// STLport list4/vector12 put CrimeFineAmount at74, whole owner128.
class COpinionDeedEffectsDef:public CParentDefClassBase {
public:
 virtual void Copy(const CDefClassBase*);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase* FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 virtual void Transfer(CPersistContext&);
 COpinionDeedEffectsDef();
 COpinionDeedEffectsDef(const COpinionDeedEffectsDef&);
 virtual ~COpinionDeedEffectsDef();
 COpinionDeedEffectsDef& operator=(const COpinionDeedEffectsDef&);
 long KnowledgeLifeInSeconds;
 long MinimumSecondsBetweenPostings;
 long MinimumSecondsBetweenEffectsForIndividual;
 float PreReactionDelayIgnoreOnDelay;
 std::list<COpinionTransientOffset> Effects;
 long ReactionPriority;
 std::vector<COpinionDeedReactionDef> Reaction;
 NSpeechGainManager::EDialogueLayer ReactionDialogueLayer;
 std::vector<CCharString> WarningText;
 ECrimeSeverity CrimeSeverity;
 long CrimeFineAmount;
 ETutorialCategory Tutorial;
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
};
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef,CrimeFineAmount)==0x74);
FABLE_STATIC_ASSERT(sizeof(COpinionDeedEffectsDef)==128);
#endif
