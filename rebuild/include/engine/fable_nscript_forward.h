#ifndef FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
#define FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
// Forward declarations shared by the FABLE_RETAIL_NSCRIPT headers (CScriptThing, CGameScriptInterfaceBase, CScriptBase,
// CEntityScriptBase, CTimer, the NScript predicates, CEntityScriptBinding<T>, CLevelScriptBase<T>). Class keys are the
// ones the FableWin.pdb / Ego_r.pdb mangled names carry (V = class, U = struct: CScreenFilterSThingByPass, std::pair,
// std::less). Enums: VC7.1 accepts an MS forward enum declaration (the row defines the enumerators it uses, before or
// after the include); C++98 has none, so under g++ (syntax checks only) each enum gets an empty placeholder definition.
// std templates: STLport 4.6 parameter lists without defaults (a row's own definition may add the defaults).

class C2DCoordI;
class C2DVector;
class C3DVector;
class CCharString;
class CCounter;
class CRGBColour;
class CRGBFloatColour;
class CRightHandedSet;
class CScriptGameResourceObjectMovieBase;
class CScriptGameResourceObjectScriptedThingBase;
class CScriptThing;
class CThing;
class CTimer;
class CWideString;
class CPersistContext;
class CScriptBase;
class CSpawnedFuncBase;
class CEntityScriptBindingBase;
class CScriptDataBase;
class CActiveEntityScriptBase;
struct CScreenFilterSThingByPass;
class CHeroLogBook;
template <class T> class CArray;
template <class T> class CCountedPointer;
namespace NScript { class CPredicateBase; }
namespace std {
template <class T> class allocator;
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T, class A> class vector;
template <class T, class A> class list;
template <class K, class C, class A> class set;
template <class K, class V, class C, class A> class map;
}
#if defined(_MSC_VER)
typedef unsigned __int64 FableNScriptUInt64;                           // the PDB's unsigned __int64 (thing unique IDs)
enum EAIStateGroupType;
#ifdef FABLE_RETAIL_CONVERSATION_CAMERA_PASS29
enum ECameraOp {
    CAM_OP_SPEAKEE_LEFT_ZOOM_ON_SPEAKER = 0,
    CAM_OP_SPEAKEE_RIGHT_ZOOM_ON_SPEAKER = 1,
    CAM_OP_SPEAKER_LEFT_ZOOM_ON_SPEAKEE = 2,
    CAM_OP_SPEAKER_RIGHT_ZOOM_ON_SPEAKEE = 3,
    CAM_OP_ZOOM_ON_SPEAKER_FACE = 4,
    CAM_OP_ZOOM_ON_SPEAKEE_FACE = 5
};
#else
enum ECameraOp;
#endif
enum EClothingCoversArea;
enum ECreatureGroup;
enum ECrime;
enum ECutsceneBehaviour;
enum EDoorTriggerType;
enum EGameAction;
#ifdef FABLE_RETAIL_MURDER_ASSASSIN_GUARD_PASS26
#include "engine/EHeroAbility.h"
#else
enum EHeroAbility;
#endif
enum EHeroTitle;
enum EHeroTrainableStatType;
enum EMiniGameType;
#ifdef FABLE_RETAIL_BE_EVIL_MORALITY_PASS29
#include "engine/EMorality.h"
#else
enum EMorality;
#endif
enum EMusicSetType;
enum EOpinion;
enum EOpinionAttitudeType;
enum EOpinionDeedType;
enum EOpinionPostDeedType;
enum EOpinionReactionType;
#ifdef FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "engine/EScriptAIPriority.h"
#else
enum EScriptAIPriority;
#endif
enum EScriptVillageAttitude;
enum EScriptingStateGroups;
enum ESex;
enum ETargetingType;
#ifdef FABLE_RETAIL_MURDER_ASSASSIN_GUARD_PASS26
#include "engine/ETextGroupSelectionMethod.h"
#else
enum ETextGroupSelectionMethod;
#endif
enum ETutorialCategory;
namespace NEntityEvents { enum EEventType; }
#else
typedef unsigned long long FableNScriptUInt64;
enum EAIStateGroupType { };
#ifdef FABLE_RETAIL_CONVERSATION_CAMERA_PASS29
enum ECameraOp {
    CAM_OP_SPEAKEE_LEFT_ZOOM_ON_SPEAKER = 0,
    CAM_OP_SPEAKEE_RIGHT_ZOOM_ON_SPEAKER = 1,
    CAM_OP_SPEAKER_LEFT_ZOOM_ON_SPEAKEE = 2,
    CAM_OP_SPEAKER_RIGHT_ZOOM_ON_SPEAKEE = 3,
    CAM_OP_ZOOM_ON_SPEAKER_FACE = 4,
    CAM_OP_ZOOM_ON_SPEAKEE_FACE = 5
};
#else
enum ECameraOp { };
#endif
enum EClothingCoversArea { };
enum ECreatureGroup { };
enum ECrime { };
enum ECutsceneBehaviour { };
enum EDoorTriggerType { };
enum EGameAction { };
#ifdef FABLE_RETAIL_MURDER_ASSASSIN_GUARD_PASS26
#include "engine/EHeroAbility.h"
#else
enum EHeroAbility { };
#endif
enum EHeroTitle { };
enum EHeroTrainableStatType { };
enum EMiniGameType { };
#ifdef FABLE_RETAIL_BE_EVIL_MORALITY_PASS29
#include "engine/EMorality.h"
#else
enum EMorality { };
#endif
enum EMusicSetType { };
enum EOpinion { };
enum EOpinionAttitudeType { };
enum EOpinionDeedType { };
enum EOpinionPostDeedType { };
enum EOpinionReactionType { };
#ifdef FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "engine/EScriptAIPriority.h"
#else
enum EScriptAIPriority { };
#endif
enum EScriptVillageAttitude { };
#ifdef FABLE_RETAIL_ESCORT_TRADER_INIT_PASS29
#include "engine/EScriptingStateGroups.h"
#else
enum EScriptingStateGroups { };
#endif
enum ESex { };
enum ETargetingType { };
#ifdef FABLE_RETAIL_MURDER_ASSASSIN_GUARD_PASS26
#include "engine/ETextGroupSelectionMethod.h"
#else
enum ETextGroupSelectionMethod { };
#endif
enum ETutorialCategory { };
namespace NEntityEvents { enum EEventType { }; }
#endif

#endif // FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
