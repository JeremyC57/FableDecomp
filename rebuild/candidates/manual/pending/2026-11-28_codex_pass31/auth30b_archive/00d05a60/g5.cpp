// 00d05a60 NScript::CQ_BanditCampBossBattleScript::CAssassin2::Main
// Leaf: ?Main@CAssassin2@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Flags:
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
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
template <> inline CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
class CScriptGameResourceObjectScriptedThingBase;
// Inline-budget: the native counted destructor expands ReleaseData.
template <> FABLE_FORCEINLINE void CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::ReleaseData()
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data = 0;
    Info = 0;
}
// Inline-budget: native scripted-thing resource cleanup expands the counted destructor.
template <> FABLE_FORCEINLINE CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::~CCountedPointer() { ReleaseData(); }

// Inline-budget: the native counted destructor expands ReleaseData.
template <> FABLE_FORCEINLINE void CCountedPointer<CScriptGameResourceObjectMovieBase>::ReleaseData()
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data = 0;
    Info = 0;
}
template <> inline CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer() { ReleaseData(); }

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
inline NScript::CIsThingAlive::~CIsThingAlive() { }
inline CScriptThing::~CScriptThing() {}

namespace NScript {
inline CQ_BanditCampBossBattleScript::CAssassin2::CAssassin2(
    CQ_BanditCampBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game, me), ParentClass(parent)
{
    PMasterData = static_cast<CQ_SunnyvaleMasterData*>(data);
}
}
namespace {
// Fidelity: scaffolding, not retail (probe g4): constructs the entity so this TU emits its vftable, and through it the inline Main,
// without a second strong Alloc (landed row 0x00D05910 owns ?Alloc@CAssassin2@..).
CEntityScriptBase* EmitAssassin2Table(NScript::CQ_BanditCampBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return new NScript::CQ_BanditCampBossBattleScript::CAssassin2(parent, data, me);
}
}
