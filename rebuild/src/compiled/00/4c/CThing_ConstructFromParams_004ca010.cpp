// CThing::ConstructFromParams(long defGlobalIndex, long playerNumber, long mapNumber, bool reregister) -- base thing construction: move the thing in the class factory's lists, store def index / player / map, reset the lists, fetch the base def, allocate the UID, apply the def's default script name/data and add the editor / mapwho components
// Leaf: ?ConstructFromParams@CThing@@IAE_NJJJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CA010, 437 bytes, `ret 10h`. Called by CThingPhysical::ConstructFromParams 0x006A5950.
//   Identity: Ego_r.pdb ?ConstructFromParams@CThing@@IAE_NJJJ_N@Z (protected).
// Callees: CClassFactory::MoveAllocatedClassInLists(CClass&, long, bool) 0x00A37300 and CThingManager::ThingTypeToTypeFlags 0x0051E580 on the
// thing manager 0x013B8A1C (a CClassFactory), CWorld::GetFrame 0x0049D870, CLevelLoader::GetGlobalSectionName (inline: the CCharString copy
// ctor 0x0099EC30 from 0x013BAB0C) / operator= 0x0099EFB0 / dtor 0x0099EAE0 / (const char*, long) ctor 0x0099EBF0,
// CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C (manager +0x20), CThing::CreateUID 0x004C7FF0, the thing's virtual slot 15
// CreateFromDef (+0x3C), CDefStringTable::GetString(long) 0x009D49B0 on GDefStringTable 0x013CA828 (inline CDefString::GetString), the game
// component's virtual slot 9 IsEditorActive (+0x24, manager +0x1C), CThing::AddTC(const CCharString&, bool, const CTCParamsBase*)
// 0x004C9D60 with "CTCEditor" 0x01239BF0 and "CTCMapwho" 0x01239BE4.
// Shape: every CCharString temporary is homed in the dead bool argument slot; the section name is assigned from an
// inline by-value copy of the global (bare temporary); the two GetString results and the two AddTC names are NAMED
// locals in their own blocks (retail re-derives `lea [esp+18h]` instead of reusing eax, and builds the AddTC name
// before pushing the two zero arguments); the player-number clamp keeps its redundant `== 5` test.
// NF62 (2026-11-17, round 7): ported onto the shared CThing / CThingManager / CClassFactory / CThingBaseDef / CDefString /
//   CDefinitionManager / CMainGameComponent / CWorld / CLevelLoader headers (the local CThingBaseDef / CDefStringTable / CDefinitionManager /
//   CGameDefinitionManager / CMainGameComponent / CThingManager / CWorld / CThing models are gone). Self QAE -> IAE (protected, Ego_r.pdb).
//   Overlay opt-ins: FABLE_RETAIL_CTHING_CONSTRUCTFROMPARAMS, FABLE_RETAIL_CCLASSFACTORY_MOVEALLOCATEDCLASS,
//   FABLE_RETAIL_CTHINGBASEDEF_DEFAULTSCRIPT.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_CONSTRUCTFROMPARAMS
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASSFACTORY_MOVEALLOCATEDCLASS
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGBASEDEF_DEFAULTSCRIPT
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_THINGTYPEFLAGS
#define FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CLEVELLOADER_GLOBALSECTION
#define FABLE_RETAIL_CLEVELLOADER_GETGLOBALSECTION
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CClassFactory.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefString.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CLevelLoader.h"

bool CThing::ConstructFromParams(long defGlobalIndex, long playerNumber, long mapNumber, bool reregister)
{
    if (reregister)
        CThing::GetPCurrentThingManager()->MoveAllocatedClassInLists(*this, Type, reregister);
    DefGlobalIndex = (unsigned short)defGlobalIndex;
    PlayerNumber = (signed char)playerNumber;
    CurrentMapNumber = (short)mapNumber;
    WorldFrameCreatedAt = CWorld::GetFrame();
    RenderListNext = 0;
    RenderListPrev = 0;
    UpdateListNext = 0;
    UpdateListPrev = 0;
    SerialisationSectionName = CLevelLoader::GetGlobalSectionName();
    LevelPersistent = false;
    if (DefGlobalIndex > 0) {
        const CDefinitionManager& definitions = CThing::GetPCurrentThingManager()->PeekDefinitionManager();
        definitions.GetDef(DefGlobalIndex, &PBaseDef);
    }
    CreateUID();
    ThingTypeFlags = CThing::GetPCurrentThingManager()->ThingTypeToTypeFlags(Type);
    if (PlayerNumber == 5 || PlayerNumber < 0 || PlayerNumber >= 4)
        PlayerNumber = 4;
    if (!PBaseDef.IsNull()) {
        CreateFromDef(*PBaseDef);
        {
            CCharString name = PBaseDef->DefaultScriptName.GetString();
            ScriptName = name;
        }
        {
            CCharString data = PBaseDef->DefaultScriptData.GetString();
            ScriptData = data;
        }
    }
    if (CThing::GetPCurrentThingManager()->PeekComponent().IsEditorActive()) {
        CCharString editor("CTCEditor", -1);
        AddTC(editor, false, 0);
    }
    {
        CCharString mapwho("CTCMapwho", -1);
        AddTC(mapwho, false, 0);
    }
    return true;
}
