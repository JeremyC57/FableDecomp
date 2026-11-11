// NThingManager::CEntityRuntimePersistence::LoadFromGameStateScript(CStringParser& parser) -- reload the entity runtime persistence from a text game-state script: each START_THING_LEVEL block's entity script into the game-persistent table (map UIDs remapped to map numbers), then each START_NEW_DEAD_THING record into the persisted-dead map
// Leaf: ?LoadFromGameStateScript@CEntityRuntimePersistence@NThingManager@@QAEXAAVCStringParser@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005248B0, 1543 bytes, thiscall, `ret 4` (first instruction `sub esp,0CCh` follows int3 padding;
//   ends `ret 4` at 0x00524EB4, int3 padding after).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?LoadFromGameStateScript@CEntityRuntimePersistence@NThingManager@@QAEXAAVCStringParser@@@Z
//   (one CStringParser& argument: `mov ebp,[esp+0E0h]`, `ret 4`); its strings START_THING_LEVEL / END_THING_LEVEL /
//   START_DEAD_THINGS / START_NEW_DEAD_THING / END_NEW_DEAD_THING / END_DEAD_THINGS / UID / SectionNameCRC are the persistence
//   records SaveToGameStateScript writes; the dead-thing insert is the map at this+0x18 (PersistedDeadEntities).
// Callees: Clear 0x005236F0; CMainGameComponent::PMainGameComponent 0x013B86A0 -> PeekWorld (+0x24) -> CWorld::PeekWorldMap (slot 13,
//   +0x34); CStringParser ReadAsStringUntilString 0x009BA330, SkipPastNextItemIfIs 0x009B9E00, SkipUntilWhitespace 0x009B9CE0,
//   ReadAsInteger 0x009BA540, SkipPastString 0x009B9BA0, SkipWhitespace 0x009B9C60, ReadAsStringUntilPastString 0x009BA3E0, the
//   inline CStringParser(const CCharString&, 0, CCharString(""), 0) (CBaseClass 0x0099A2D0, vtable 0x0122D874, Init 0x009B9530) and
//   its implicit destructor (comment list freed with free 0x00BFEA14, ~CBaseClass 0x0099A2E0); CWorldMap::GetMapNumberFromMapUID
//   0x004FB920; CCharString(const char*, long) 0x0099EBF0 / () 0x0099E4B0 / ~ 0x0099EAE0 / operator= 0x0099EFB0; the expanded
//   SetGamePersistentScript (0x005237F0) with vector<CEntityScriptInfo>::resize(n) -> resize(n, CEntityScriptInfo()) expanded
//   (erase -> STLport __copy 0x0052BD10, _M_fill_insert 0x0052C300); CPersistContext(CStringParser&, ...) 0x009BADD0,
//   Transfer<unsigned __int64> 0x0044BB00, Transfer<unsigned long> 0x004106F0; the expanded AddPersistedDeadEntity (0x00520330)
//   with map insert_unique 0x0052A130.
// Shape (chunk 2010): shared headers only (hdr/ overlay opt-ins LOADSCRIPT, SKIPUNTILWHITESPACE, MAPNUMBERFROMMAPUID,
//   TRANSFER_BODY; INLINE_CTOR from chunk 2001). The two small same-TU callees (AddPersistedDeadEntity, SetGamePersistentScript)
//   are defined above the row as retail's TU had them, and VC7.1 expands them in the loops as retail did. The Transfer defaults
//   are plain temporaries (`(unsigned __int64)0`, `(unsigned long)0`): with the Transfer<T> template body visible (FABLE_RETAIL_CPERSISTCONTEXT_
//   TRANSFER_BODY, the body of the landed 0x0044BB00 / 0x004106F0, which this TU's instances reproduce RELOCATION_MATCH) VC7.1
//   knows the out-of-line call only reads them and hoists their zero stores into the loop preheader with the zero constant in
//   edi (0x00524C6B..0x00524C7B); declaration-only Transfer keeps the stores in the loop and colours `this` into edi instead.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_INLINE_CTOR
#define FABLE_RETAIL_CSTRINGPARSER_SKIPUNTILWHITESPACE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_MAPNUMBERFROMMAPUID
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_LOADSCRIPT
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CStringParser.h"
#include "engine/CArray.h"
#include "engine/CPersistContext.h"
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/NThingManager_CEntityRuntimePersistence.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

void NThingManager::CEntityRuntimePersistence::AddPersistedDeadEntity(unsigned __int64 uid, bool gamePersistent, unsigned long sectionNameCRC)
{
    if (uid != 0) {
        CPersistedDeadEntity entity;
        entity.UID = uid;
        entity.GamePersistent = gamePersistent;
        entity.SectionNameCRC = sectionNameCRC;
        PersistedDeadEntities.insert(std::map<unsigned __int64, CPersistedDeadEntity>::value_type(uid, entity));
    }
}

void NThingManager::CEntityRuntimePersistence::SetGamePersistentScript(long index, const CCharString& script)
{
    GamePersistentEntityScripts.resize(MAX((unsigned int)(index + 1), GamePersistentEntityScripts.size()));
    GamePersistentEntityScripts[index].Script = script;
    GamePersistentEntityScripts[index].Valid = true;
}

void NThingManager::CEntityRuntimePersistence::LoadFromGameStateScript(CStringParser& parser)
{
    Clear();
    const CWorldMap& world_map = CMainGameComponent::Get()->PeekWorld().PeekWorldMap();
    parser.ReadAsStringUntilString("START_THING_LEVEL");
    while (parser.SkipPastNextItemIfIs("START_THING_LEVEL")) {
        parser.SkipUntilWhitespace();
        unsigned long level = parser.ReadAsInteger();
        if (level >= (unsigned long)world_map.GetNoMaps()) {
            long map_number = world_map.GetMapNumberFromMapUID(level);
            if (map_number > 0)
                level = map_number;
        }
        CCharString script = parser.ReadAsStringUntilString("END_THING_LEVEL");
        parser.SkipPastString("END_THING_LEVEL");
        CStringParser script_parser(script);
        script_parser.SkipWhitespace();
        SetGamePersistentScript(level, script);
    }
    parser.SkipWhitespace();
    if (parser.SkipPastNextItemIfIs("START_DEAD_THINGS")) {
        while (parser.SkipPastNextItemIfIs("START_NEW_DEAD_THING")) {
            CCharString text = parser.ReadAsStringUntilPastString("END_NEW_DEAD_THING");
            CStringParser dead_parser(text);
            CPersistContext context(dead_parser, 0, false, 0, 0, 0);
            unsigned __int64 uid = 0;
            context.Transfer("UID", uid, (unsigned __int64)0);
            unsigned long section_name_crc = 0;
            context.Transfer("SectionNameCRC", section_name_crc, (unsigned long)0);
            AddPersistedDeadEntity(uid, true, section_name_crc);
        }
    }
    parser.SkipPastString("END_DEAD_THINGS");
}
