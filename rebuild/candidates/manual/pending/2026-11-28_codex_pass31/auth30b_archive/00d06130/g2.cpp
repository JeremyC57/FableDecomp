// 00d06130 NScript::CQ_BanditCampBossBattleScript::CAssassin3::Main
// Leaf: ?Main@CAssassin3@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Flags: /O2 /Oy
#define FABLE_RETAIL_COUNTED_DESTRUCTOR_INLINE_PASS29
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
// Inline-budget: inline the movie destructor at both early exits, sharing their cleanup tail.
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BANDIT_CAMP_ASSASSIN_TYPED_CTORS_PASS29
#define FABLE_RETAIL_BANDIT_CAMP_ASSASSINS_PASS29
#define FABLE_RETAIL_BANDIT_CAMP_ASSASSIN_INCLASS_MAIN_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_BASE_OBJECT

#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "fable_compiler.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/EScriptAIPriority.h"
#include "engine/TRetVoid.h"
#include "engine/EHeroAbility.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CIsThingAlive.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#ifdef _MSC_VER
#pragma inline_depth(2)
#endif
FABLE_FORCEINLINE NScript::CIsThingAlive::~CIsThingAlive() { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline CScriptThing::~CScriptThing() {}
namespace NScript {
inline CQ_BanditCampBossBattleScript::CAssassin3::CAssassin3(
    CQ_BanditCampBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game, me), ParentClass(parent)
{
    PMasterData = static_cast<CQ_SunnyvaleMasterData*>(data);
}
}
namespace {
// Fidelity: scaffolding, not retail (probe g1): constructs the entity so this TU emits its vftable, and through it the inline Main,
// without a second strong Alloc / GetParentScript (landed rows 0x00D05FE0 / 0x00D06080 own them).
CEntityScriptBase* EmitAssassin3Table(NScript::CQ_BanditCampBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return new NScript::CQ_BanditCampBossBattleScript::CAssassin3(parent, data, me);
}
}
