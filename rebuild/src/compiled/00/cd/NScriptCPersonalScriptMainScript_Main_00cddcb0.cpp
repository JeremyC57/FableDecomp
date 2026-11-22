// 00cddcb0 NScript::CPersonalScriptMainScript::Main
// Leaf: ?Main@CPersonalScriptMainScript@NScript@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PERSONALSCRIPT_MAIN_PASS26
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_DIRECT_DEREF_PASS26
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_GETASUPPERCASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_LIST_DEFAULT_CTOR_NOINLINE
#define FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST
#define FABLE_RETAIL_PERSONAL_SCRIPT_LIST_DTOR_PASS26
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/NScript_CPersonalScriptMainScript.h"
#include "engine/CRegionScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CScriptDefinitionManager.h"
#include "engine/TRetVoid.h"

template <class T> inline T* CLevelScriptBase<T>::GetMasterData() {return &MasterScriptData;}

namespace NScript {
void CPersonalScriptMainScript::Main()
{
    TRetVoid _Fail_Val;
    if (false) {
    } else for (;; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        {
            std::list<CCharString> loaded_level_names_list;
            if (Game.MsgOnLevelLoaded(loaded_level_names_list)) {
                if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
// Inline-budget: retail calls the list destructor at the first termination exit.
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                if (false) {} else for (std::list<CCharString>::iterator it = loaded_level_names_list.begin(); it != loaded_level_names_list.end(); ++it) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CCharString level_name = *it;
                    CCharString script_to_activate = "PersonalScript_" + level_name;
                    if (Game.IsQuestRegistered(script_to_activate) && !Game.IsQuestActive(script_to_activate)) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Game.ActivateQuest(script_to_activate);
                        CCharString msg = CCharString("") + "Loading personal script " + script_to_activate;
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (!Game.IsQuestRegistered(script_to_activate) || Game.IsQuestActive(script_to_activate)) {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                    }
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            CCharString UnloadedRegionName;
            if (Game.MsgOnRegionUnloaded(UnloadedRegionName)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                GetMasterData()->RandomEntityVillagerMax = 0;
                GetMasterData()->RandomEntityGuardMax = 0;
                GetMasterData()->RandomEntityBanditMax = 0;
                GetMasterData()->RandomEntityVillagerCount = 0;
                GetMasterData()->RandomEntityGuardCount = 0;
                GetMasterData()->RandomEntityBanditCount = 0;
            }
            CCharString region_name;
            if (Game.MsgOnRegionLoaded(region_name)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                // Inferred: This scope keeps the handle in its native stack slot and omits the normal-exit clear at 00CDE171.
                {
                    CDefPointer<const CRegionScriptDef> pdef(NULL_DEF);
                    CCharString RegionDefName = CCharString("REGION_") + CCharString(region_name).GetAsUpperCase();
                    if (CScriptDefinitionManager::Get().GetDef(RegionDefName, &pdef)) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        GetMasterData()->RandomEntityVillagerMax = pdef->RandomVillagerMax;
                        GetMasterData()->RandomEntityGuardMax = pdef->RandomGuardMax;
                        GetMasterData()->RandomEntityBanditMax = pdef->RandomBanditMax;
                        GetMasterData()->RandomEntityVillagerCount = 0;
                        GetMasterData()->RandomEntityGuardCount = 0;
                        GetMasterData()->RandomEntityBanditCount = 0;
                        GetMasterData()->RandomEntityVillagerPercentage = 1.0f;
                    }
                    CCharString script_to_activate = "RegionScript_" + region_name;
                    if (Game.IsQuestRegistered(script_to_activate) && !Game.IsQuestActive(script_to_activate)) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Game.ActivateQuest(script_to_activate);
                        CCharString msg = CCharString("") + "Loading region script " + script_to_activate;
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (!Game.IsQuestRegistered(script_to_activate)) {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            CCharString msg = CCharString("") + "Cannot load script " + script_to_activate + ": not registered";
                        } else if (Game.IsQuestActive(script_to_activate)) {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            CCharString msg = CCharString("") + "Script " + script_to_activate + " already active";
                        }
                    }
                }
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
}

}