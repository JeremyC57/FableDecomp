// CThing::Save(CCharString& data) -- write the thing to a level script: its identity record, the CreateTC lines, the script name / data, the two persistence flags and every serialisable component's level state
// Leaf: ?Save@CThing@@UAEXAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9120, 1083 bytes, thiscall, `ret 4`; CClass vtable slot 2 (cells 0x01239AC4 / 0x01272C2C /
//   0x01272EEC), also called directly by the derived Save at 0x006A4BAC.
// Identity (two sources): Ego_r.pdb 0x004CAF70 ?Save@CThing@@UAEXAAVCCharString@@@Z; slot 2 of the CThing vtable = CClass::Save
//   (engine/CClass.h).
// Callees: CMainGameComponent::IsEditorActive (slot 9), CThingManager::IsSerialisingGameState 0x0051E2E0 (result dropped),
//   RemapUIDForSerialisation 0x0051E210, CDefinitionManager::GetDefNameFromGlobalIndex 0x009ACCC0, CDefStringTable::GetString 0x009D49B0,
//   the inlined SerialiseThingInfo(bool, CCharString&, CSerialiseInfo&) 0x004C88E0 (CBaseClass 0x0099A2D0 / 0x0099A2E0, malloc 0x00BFEA0E,
//   CPersistContext::Init(CCharString&, const CDefinitionManager*) 0x009BAA10, SerialiseThingInfo(CPersistContext&, CSerialiseInfo&)
//   0x004C81F0, ~list<CStringParserCommentBoundaries> 0x004046B0), SaveSerialiseCreatableTCs 0x004C72D0, CPersistContext(CCharString&)
//   0x009BAE10, CBasicString<char>::operator==(const char*) 0x004115A0, operator+ 0x0099F570 / 0x0099F600 / 0x0099F690,
//   CCharString::operator+= 0x0099F0A0, GFBoolToCharString 0x0099F9B0, IsLevelPersistent 0x004C82A0, CTCBase slot 20
//   IsSerialisableForHeroState, CTCBase::SerialiseLevelState 0x00686E10.
// Headers (overlay, FABLE_HDR_OVERLAY=$A/hdr): FABLE_RETAIL_CTHING_SAVE, FABLE_RETAIL_CSTRINGPARSER_INLINE_DEFAULTCTOR,
//   FABLE_RETAIL_CPERSISTCONTEXT_STRINGSAVE, FABLE_RETAIL_CTCBASE_SERIALISE.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_SERIALISE
#define FABLE_RETAIL_CTCBASE_SERIALISELEVELSTATE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_SAVE
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_PEEKWORLD
#define FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_INLINE_DEFAULTCTOR
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_STRINGSAVE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDefString.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"

CCharString __fastcall GFBoolToCharString(bool value);                         // 0x0099F9B0

// Ego_r.pdb / FableWin.pdb ?IsLevelLess@CThing@@QBE_NXZ, inline in this TU (the PBaseDef null test, then LevelLess: bit 3 of def +0x41).
inline bool CThing::IsLevelLess() const
{
    bool level_less = false;
    if (!PBaseDef.IsNull() && PBaseDef->LevelLess)
        level_less = true;
    return level_less;
}
// 0x004C8280 (landed), the same TU: Save expands it.
inline bool CThing::IsGameStatePersisted() const
{
    if (HasTC(TCI_COOP_SPIRIT))
        return false;
    return GameStatePersisted;
}
// 0x004C88E0 (landed), the same TU: Save expands its saving arm (the parser is built and destroyed unused).
inline void __fastcall CThing::SerialiseThingInfo(bool saving, CCharString& data, CSerialiseInfo& info)
{
    CStringParser parser;
    CPersistContext context;
    if (saving)
        context.Init(data, 0);
    else {
        parser.Init(data, 0, CCharString(""), 0);
        context.Init(parser, 0, true, 0, 0, 0);
    }
    SerialiseThingInfo(context, info);
    if (!saving) {
        if (parser.SkipPastWholeString("DefinitionSubType"))
            info.DefinitionType = parser.ReadAsString();
    }
}

void CThing::Save(CCharString& data)
{
    if (CMainGameComponent::Get()->IsEditorActive())
        PCurrentThingManager->IsSerialisingGameState();
    CSerialiseInfo info;
    unsigned __int64 uid = UniqueID;
    info.UID = PCurrentThingManager->RemapUIDForSerialisation(true, IsLevelLess(), CurrentMapNumber, uid);
    info.Player = PlayerNumber;
    info.DefinitionType = "";
    if (DefGlobalIndex > 0)
        info.DefinitionType = PCurrentThingManager->PeekDefinitionManager().GetDefNameFromGlobalIndex(DefGlobalIndex);
    SerialiseThingInfo(true, data, info);
    SaveSerialiseCreatableTCs(data);
    CPersistContext context(data);
    data += CCharString("ScriptName ") + (ScriptName == "" ? CCharString("NULL") : ScriptName) + ";\r\n";
    data += CCharString("ScriptData \"") + (ScriptData == "" ? CCharString("NULL") : ScriptData) + "\";\r\n";
    data += "ThingGamePersistent " + GFBoolToCharString(IsGameStatePersisted()) + ";\r\n";
    bool level_persistent = IsLevelPersistent();     // called before the line's ";\r\n" is pushed (0x004C9466)
    data += "ThingLevelPersistent " + GFBoolToCharString(level_persistent) + ";\r\n";

    bool hero_state_only = false;
    if ((ThingTypeFlags & 2) && PCurrentThingManager->PeekWorld().IsSerialisingHeroState())
        hero_state_only = true;
    AddingTCsPermitted = false;
    for (CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = Components.begin();
         it != Components.end(); ++it) {
        CTCBase* tc = it->second;
        if (hero_state_only && !tc->IsSerialisableForHeroState())
            continue;
        if (tc->IsToBeRemoved())
            continue;
        tc->SerialiseLevelState(true, data, !CMainGameComponent::Get()->IsEditorActive());
    }
    AddingTCsPermitted = true;
}