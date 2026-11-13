// CThing::ConstructFromLevelScript(const char* script) -- build a thing from its level-script text: identity (unless only the hero state is being loaded: then the hero's UID), the map number, the two persistence flags, the ScriptName / ScriptData keys up to EndThing, the remapped UID, the def / editor / map-who components and the creatable components, then every live component's level state
// Leaf: ?ConstructFromLevelScript@CThing@@UAE_NPBD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CAA00, 1730 bytes (0x004CAA00..0x004CB0C1, last instruction `jmp` at 0x004CB0C0; `ret 4` at
//   0x004CABB8 / 0x004CAF57), public virtual. CThing vtable 0x01239ABC slot 4 (+0x10; rtti-vtables.tsv), between SerialiseGameState (3)
//   and ConstructAlreadyExistingThingFromLevelScript (5) as engine/CThing.h LOADSINGLE spells the Ego_r slot order; CThingManager::
//   LoadSingleThing calls slot +0x10 with the script text.
// Identity (two sources): Ego_r.pdb 0x004CD4E0 ?ConstructFromLevelScript@CThing@@UAE_NPBD@Z; the derived overrides call it by name
//   (CThingPhysical 0x006A5D90, engine/CThing.h LEVELSCRIPT note); its progress literals "CThing: Construct From Level Script 1/2/3"
//   (0x01239D00 / 0x01239C98 / 0x01239C58). Catalog name already right.
// Callees: NProgressDisplay::DisplayProgress 0x009E9F40, CCharString (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / () 0x0099E4B0 /
//   operator= 0x0099EFB0 / ~ 0x0099EAE0, CThingManager::ThingTypeToTypeFlags 0x0051E580 / IsSerialisingGameState 0x0051E2E0 /
//   GetLevelBeingLoaded 0x0051E2C0 / RemapUIDForSerialisation 0x0051E210, CWorld::GetFrame 0x0049D870, CStringParser(const CCharString&,
//   cb, const CCharString&, long) 0x00404720 / NextItemExists 0x009BA4F0 / ReadAsString 0x009BA6F0 / PeekNextItemType 0x009BA900 /
//   ReadAsSymbol 0x009BA790 / ReadAsStringUntilString 0x009BA330 / SkipPastString 0x009B9BA0 and the generated ~CStringParser (expanded:
//   ~list<CStringParserCommentBoundaries> 0x004046B0, two ~CCharString, ~CBaseClass 0x0099A2E0), CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetPControlledCreature 0x00487DC0, CThing::SerialiseThingInfo(bool, CCharString&, CSerialiseInfo&) 0x004C88E0 (called,
//   loading) / InitFromThingInfo 0x004C7F40 / CreateUID 0x004C7FF0 / AddTC 0x004C9D60 / CreateSerialiseCreatableTCs 0x004CA230,
//   CPersistContext(CStringParser&, ...) 0x009BADD0 / Transfer<short> 0x004CC3F0 / Transfer<bool> 0x004045C0, CBasicString<char>::
//   operator==(const char*) 0x004115A0 (the inline CCharString compare's non-null arm), CTCBase::SerialiseLevelState 0x00686E10;
//   virtuals CreateFromDef (slot 15), CMainGameComponent::IsEditorActive (slot 9).
// Shape: the text-side mirror of the matched ConstructFromBinarySave 0x004CA390 (same TU; same prologue up to the section name, then
//   `LevelPersistent = false` where the binary loader calls InitFromThingInfo). Levers: (1) the render / update links are cleared
//   Next before Prev (0x004CAA74..0x004CAA83); (2) the CBasicString<char>::Compare / operator==(const char*) bodies are visible in the
//   TU (the landed CThingTrackNode / CThingAICreature level-script precedent), so VC7.1 hoists the key's null test out of the
//   ScriptName / ScriptData / EndThing chain (one `repe cmpsb` ladder against "", one operator== ladder on esi); (3) the TU's
//   out-of-line CThing::PeekComponent (0x004C7A70) is defined here, so the editor test reads the manager's Component in place; (4) the
//   final component walk keeps a separate `next` iterator (`lea edi,[eax+8]` ... `cmp edi,[ebp+48h] ; mov eax,edi`, 0x004CAEB3 /
//   0x004CAEDC); with `++it` the iterator stays in one CPU location. The second CStringParser over the same text (0x004CACB9, destroyed
//   at 0x004CAEF8..0x004CAF19) is never read: retail builds it and nothing uses it.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
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
#define FABLE_RETAIL_CTHING_CONSTRUCTFROMBINARYSAVE
#define FABLE_RETAIL_CTHING_CONSTRUCTFROMLEVELSCRIPT
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_PEEKWORLD
#define FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#define FABLE_RETAIL_CTHINGMANAGER_THINGTYPEFLAGS
#define FABLE_RETAIL_CTHINGMANAGER_LEVELBEINGLOADED_OUTOFLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
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
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/NProgressDisplay.h"

// Row-local template bodies (the landed CThingTrackNode::ConstructFromLevelScript 0x00837E40 precedent): fable_string.h declares
// CBasicString<T>::Compare 0x00411570 and operator==(const T*) 0x004115A0; retail's TU holds their bodies (operator== is still called
// out of line, the key's PStringData kept in esi across the compare chain).
template<class T> inline long __fastcall CBasicString<T>::Compare(const T* a,const T* b)
{
    while(*a || *b) {
        if(*a<*b)return -1;
        if(*a>*b)return 1;
        ++a; ++b;
    }
    return 0;
}
template<class T> inline bool CBasicString<T>::operator==(const T* text) const
{ return Compare(PData,text)==0; }

// 0x013B8636: a global bool read only here (0x004CADC6) and in CMap::LoadFromFile (0x00821B73, where it resets the map's UID count
// to 1); nothing in Fable.exe writes it (no other reference, no data pointer). No PDB data symbol: descriptive name.
extern bool g_bResetUIDsOnLoad;                         // 0x013B8636

// The section name every thing starts with: a by-value copy of the global CCharString 0x013BAB0C ("NULL"; the descriptive spelling of
// the landed CThing::ConstructFromParams 0x004CA010 of this TU).
extern CCharString g_DefaultSerialisationSectionName;   // 0x013BAB0C
inline CCharString GetDefaultSerialisationSectionName() { return g_DefaultSerialisationSectionName; }

// Ego_r.pdb / FableWin.pdb ?IsLevelLess@CThing@@QBE_NXZ, inline in this TU (the landed CThing::Save 0x004C9120 body): the PBaseDef null
// test, then LevelLess (bit 3 of def +0x41), 0x004CAD87..0x004CAD98.
inline bool CThing::IsLevelLess() const
{
    bool level_less = false;
    if (!PBaseDef.IsNull() && PBaseDef->LevelLess)
        level_less = true;
    return level_less;
}

// 0x004C7A70 (landed), the same TU: expanded here (the manager's Component +0x1C read in place).
const CMainGameComponent& CThing::PeekComponent() const
{
    return PCurrentThingManager->PeekComponent();
}

bool CThing::ConstructFromLevelScript(const char* script)
{
    NProgressDisplay::DisplayProgress("CThing: Construct From Level Script 1", -1.0f, false, false);
    bool hero_state_only = false;
    if ((ThingTypeFlags & 2) && PCurrentThingManager->PeekWorld().IsSerialisingHeroState())
        hero_state_only = true;
    else {
        CreatedBySerialisation = true;
        RenderListNext = 0;
        RenderListPrev = 0;
        UpdateListNext = 0;
        UpdateListPrev = 0;
        SetSerialisationSectionName(GetDefaultSerialisationSectionName());
        ThingTypeFlags = PCurrentThingManager->ThingTypeToTypeFlags(Type);
        WorldFrameCreatedAt = CWorld::GetFrame();
        LevelPersistent = false;
    }

    CCharString script_string(script);
    CStringParser parser(script_string);
    unsigned __int64 uid = 0;
    if (hero_state_only) {
        CThing* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (hero && !hero->IsDead())
            uid = hero->GetUniqueID();
    } else {
        CSerialiseInfo info;
        SerialiseThingInfo(false, script_string, info);
        if (!InitFromThingInfo(info))
            return false;
        uid = info.UID;
    }

    CPersistContext context(parser, 0, true, 0, 0, 0);
    if (!hero_state_only) {
        if (PCurrentThingManager->IsSerialisingGameState())
            context.Transfer("ThingCurrentMapID", CurrentMapNumber, (short)0);
        else
            CurrentMapNumber = (short)PCurrentThingManager->GetLevelBeingLoaded();
    }
    bool game_state_persisted;
    context.Transfer("ThingGamePersistent", game_state_persisted, false);
    bool level_persistent;
    context.Transfer("ThingLevelPersistent", level_persistent, false);
    GameStatePersisted = game_state_persisted;
    LevelPersistent = level_persistent;

    CStringParser unused_parser(script_string);         // built and destroyed unread (0x004CAC8C..0x004CACB9)
    while (parser.NextItemExists()) {
        NProgressDisplay::DisplayProgress("CThing: Construct From Level Script 2", -1.0f, false, false);
        CCharString key = parser.ReadAsString();
        if (key == "ScriptName")
            ScriptName = parser.ReadAsString();
        else if (key == "ScriptData") {
            if (parser.PeekNextItemType() == PARSED_ITEM_SYMBOL && parser.ReadAsSymbol() == "\"")
                ScriptData = parser.ReadAsStringUntilString("\"");
        } else if (key == "EndThing")
            break;
        parser.SkipPastString(";");
    }

    if (!PCurrentThingManager->IsSerialisingGameState()) {
        UniqueID = PCurrentThingManager->RemapUIDForSerialisation(false, IsLevelLess(), CurrentMapNumber, uid);
        if (g_bResetUIDsOnLoad)
            UniqueID = 0;
        if (UniqueID == 0)
            CreateUID();
    }
    if (!hero_state_only) {
        if (!PBaseDef.IsNull())
            CreateFromDef(*PBaseDef);
        if (PeekComponent().IsEditorActive())
            AddTC("CTCEditor", false, 0);
        AddTC("CTCMapwho", false, 0);
        CreateSerialiseCreatableTCs(script_string);
    }

    NProgressDisplay::DisplayProgress("CThing: Construct From Level Script 3", -1.0f, false, false);
    AddingTCsPermitted = false;
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it, next;
    for (it = Components.begin(); it != Components.end(); it = next) {
        CTCBase* tc = it->second;
        next = it;
        ++next;
        if (!tc->IsToBeRemoved())
            tc->SerialiseLevelState(false, script_string, !CMainGameComponent::Get()->IsEditorActive());
    }
    AddingTCsPermitted = true;
    return true;
}