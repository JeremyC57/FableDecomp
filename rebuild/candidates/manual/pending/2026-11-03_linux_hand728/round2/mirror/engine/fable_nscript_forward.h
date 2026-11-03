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
enum ECameraOp;
enum EClothingCoversArea;
enum ECreatureGroup;
enum ECrime;
enum ECutsceneBehaviour;
enum EDoorTriggerType;
enum EGameAction;
enum EHeroAbility;
enum EHeroTitle;
enum EHeroTrainableStatType;
enum EMiniGameType;
enum EMorality;
enum EMusicSetType;
enum EOpinion;
enum EOpinionAttitudeType;
enum EOpinionDeedType;
enum EOpinionPostDeedType;
enum EOpinionReactionType;
enum EScriptAIPriority;
enum EScriptVillageAttitude;
enum EScriptingStateGroups;
enum ESex;
enum ETargetingType;
enum ETextGroupSelectionMethod;
enum ETutorialCategory;
namespace NEntityEvents { enum EEventType; }
#else
typedef unsigned long long FableNScriptUInt64;
enum EAIStateGroupType { };
enum ECameraOp { };
enum EClothingCoversArea { };
enum ECreatureGroup { };
enum ECrime { };
enum ECutsceneBehaviour { };
enum EDoorTriggerType { };
enum EGameAction { };
enum EHeroAbility { };
enum EHeroTitle { };
enum EHeroTrainableStatType { };
enum EMiniGameType { };
enum EMorality { };
enum EMusicSetType { };
enum EOpinion { };
enum EOpinionAttitudeType { };
enum EOpinionDeedType { };
enum EOpinionPostDeedType { };
enum EOpinionReactionType { };
enum EScriptAIPriority { };
enum EScriptVillageAttitude { };
enum EScriptingStateGroups { };
enum ESex { };
enum ETargetingType { };
enum ETextGroupSelectionMethod { };
enum ETutorialCategory { };
namespace NEntityEvents { enum EEventType { }; }
#endif

#endif // FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
