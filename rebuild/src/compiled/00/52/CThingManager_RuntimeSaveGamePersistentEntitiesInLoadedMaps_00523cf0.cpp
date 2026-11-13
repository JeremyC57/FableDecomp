// CThingManager::RuntimeSaveGamePersistentEntitiesInLoadedMaps(bool binary) -- for every loaded world map (index 1 up to
// WorldMap.GetNoMaps()), gather the things to persist on that map (only the living hero when the world is serialising hero state,
// else every game-state-persisted living thing of the class list whose current map is that map), then save them into the map's
// game-persistent slot of the runtime persistence: as a binary stream (the script cleared) or as script text (the binary data
// cleared: SetGamePersistentSaveData(map, 0, 0), which retail expands here).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00523CF0, 799 bytes (0x31F), thiscall `ret 4`. Caller CThingManager::SaveGameState 0x00524381.
// Identity: Ego_r.pdb ?RuntimeSaveGamePersistentEntitiesInLoadedMaps@CThingManager@@AAEX_N@Z (0x0052E950) + FableWin.pdb (private).
// Callees: CWorldMap::IsMapLoaded (vtable slot 10), CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature
//   0x00487DC0, CThing::IsGameStatePersisted 0x004C8280, vector<CThing*>::_M_insert_overflow 0x00436AC0, CMemoryDataOutputStream()
//   0x00996AF0 / ~ 0x00996B30 / ReserveSize 0x00996BC0 / GetLength 0x00994B40 / PeekData 0x00995E70, CThingManager::SaveEntitiesBinary
//   0x00522160, CThingManager::SaveEntitiesToScript 0x00521EB0, CEntityRuntimePersistence::SetGamePersistentSaveData 0x00523860
//   (called in the binary path, expanded in the text path), CEntityRuntimePersistence::SetGamePersistentScript 0x005237F0,
//   vector<CEntityScriptInfo>::resize 0x0052C6A0, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 /
//   Reserve 0x0099EF90 / SetUseFastExtend 0x0099EF70, free 0x00BFEA14; "" 0x0122D70E; CMainGameComponent::PMainGameComponent 0x013B86A0.
// Shape: the same-TU bodies of the two savers, of SetGamePersistentSaveData and of STLport's out-of-line range insert (0x0052B6A0)
//   are visible to the compiler (as in retail's ThingManager.cpp), so neither local vector escapes: the hero path's push_back folds
//   to the direct growth call on the known-empty vector, the list walk keeps _M_finish in ebp, and the size-0 SaveData call expands
//   (resize, UncompressedSize = 0, Valid = true through the strength-reduced map*24 record offset). The empty script is a named
//   local. The thing vector is cleared right after its construction: retail's zero stores run start, end-of-storage, finish.
// Headers: overlay opt-ins listed in hdr/README.md (row 0x00523CF0).
// Leaf: ?RuntimeSaveGamePersistentEntitiesInLoadedMaps@CThingManager@@AAEX_N@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_RESERVE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASSFACTORY_CLASSLIST
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_SAVE
#define FABLE_RETAIL_CTHING_GETSERIALISATIONSECTIONNAME
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_CTHING_SAVEBINARY
#define FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITESTREAM
#define FABLE_RETAIL_STLP_VECTOR_RANGE_INSERT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CCOMPRESSORZLIB_INTERNALCOMPRESS
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_GETNOMAPS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_RESERVESIZE
#define FABLE_RETAIL_CTHINGMANAGER_RUNTIMESAVE
#define FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_SETGAMESCRIPT
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CThingManager.h"

#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CWorld.h"
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CCompressorZlib.h"

// Same-TU bodies (ThingManager.cpp, earlier than this row): the landed CThingManager::SaveEntitiesToScript 0x00521EB0 and
// CThingManager::SaveEntitiesBinary 0x00522160, kept out of line as in retail.
#pragma auto_inline(off)
void CThingManager::SaveEntitiesBinary(CMemoryDataOutputStream& stream, long version,
                                       const std::vector<CThing*>& things, std::vector<CCharString>& names)
{
    if (things.empty())
        return;
    SerialiseVersionNumber = version;
    std::set<CCharString> sections;
    for (std::vector<CThing*>::const_iterator it = things.begin(); it != things.end(); ++it)
        sections.insert((*it)->GetSerialisationSectionName());
    names.clear();
    names.insert(names.end(), sections.begin(), sections.end());
    stream.WriteULONG(sections.size());
    SerialiseVersionNumber = version;
    for (std::set<CCharString>::iterator s = sections.begin(); s != sections.end(); ++s) {
        CCharString section(*s);
        stream.WriteNullTerminatedString(section);
        CMemoryDataOutputStream sectionStream;
        for (std::vector<CThing*>::const_iterator it = things.begin(); it != things.end(); ++it) {
            CThing* thing = *it;
            if (thing->GetSerialisationSectionName() == section) {
                CMemoryDataOutputStream thingStream;
                thingStream.WriteNullTerminatedString(thing->GetTypeName());
                thing->SaveBinary(thingStream);
                sectionStream.WriteULONG(thingStream.GetLength());
                sectionStream.Write(thingStream.PeekData(), thingStream.GetLength());
            }
        }
        sectionStream.WriteULONG(0);
        stream.CDataOutputStream::Write(sectionStream);
    }
}
void CThingManager::SaveEntitiesToScript(CCharString& script, long version, const std::vector<CThing*>& things,
                                         std::vector<CCharString>& names)
{
    if (things.empty())
        return;
    SerialiseVersionNumber = version;
    std::set<CCharString> sections;
    for (std::vector<CThing*>::const_iterator it = things.begin(); it != things.end(); ++it)
        sections.insert((*it)->GetSerialisationSectionName());
    names.clear();
    names.insert(names.end(), sections.begin(), sections.end());
    SerialiseVersionNumber = version;
    for (std::set<CCharString>::iterator s = sections.begin(); s != sections.end(); ++s) {
        CCharString section(*s);
        script += "\r\nXXXSectionStart " + section + ";\r\n";
        for (std::vector<CThing*>::const_iterator it = things.begin(); it != things.end(); ++it) {
            CThing* thing = *it;
            if (thing->GetSerialisationSectionName() == section) {
                CCharString thingScript;
                thingScript.Reserve(0x4000);
                thingScript.SetUseFastExtend(true);
                thingScript += "\r\n";
                thingScript += "NewThing ";
                thingScript += thing->GetTypeName();
                thingScript += ";\r\n";
                thing->Save(thingScript);
                thingScript += "EndThing;\r\n";
                script += thingScript;
            }
        }
        script += "\r\nXXXSectionEnd;\r\n\r\n";
    }
}
#pragma auto_inline(on)

// Same-TU body: the landed NThingManager::CEntityRuntimePersistence::SetGamePersistentSaveData 0x00523860 (ThingManager.cpp,
// earlier than this row); retail expands it in this row's text path (size 0: no compressor) and calls it in the binary path.
void NThingManager::CEntityRuntimePersistence::SetGamePersistentSaveData(long index, const void* data, unsigned long size)
{
    unsigned long needed = index + 1;
    GamePersistentEntityScripts.resize(needed > GamePersistentEntityScripts.size() ? needed : GamePersistentEntityScripts.size());
    CEntityScriptInfo& info = GamePersistentEntityScripts[index];
    info.UncompressedSize = size;
    if (size > 0) {
        CCompressorZlib compressor;
        compressor.InternalCompress(data, size, info.SaveData);
    }
    info.Valid = true;
}

void CThingManager::RuntimeSaveGamePersistentEntitiesInLoadedMaps(bool binary)
{
    for (long map = 1; map < WorldMap.GetNoMaps(); ++map) {
        if (!WorldMap.IsMapLoaded(map))
            continue;
        std::vector<CThing*> things;
        things.clear();     // retail re-stores the empty vector's _M_finish after the construction stores (start, eos, finish)
        if (World.IsSerialisingHeroState()) {
            CThing* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            if (hero && !hero->IsDead() && hero->GetCurrentMapNumber() == map)
                things.push_back(hero);
        } else {
            for (std::list<CClass*>::iterator it = ClassList.begin(); it != ClassList.end(); ++it) {
                CThing* thing = static_cast<CThing*>(*it);
                if (thing->IsGameStatePersisted() && thing && !thing->IsDead() && thing->GetCurrentMapNumber() == map)
                    things.push_back(thing);
            }
        }
        std::vector<CCharString> names;
        SerialisingGameState = true;
        if (binary) {
            CMemoryDataOutputStream stream;
            stream.ReserveSize(0x2000);
            SaveEntitiesBinary(stream, 2, things, names);
            EntityRuntimePersistence.SetGamePersistentSaveData(map, stream.PeekData(), stream.GetLength());
            CCharString noScript("");
            EntityRuntimePersistence.SetGamePersistentScript(map, noScript);
        } else {
            CCharString text;
            text.Reserve(0x8000);
            text.SetUseFastExtend(true);
            SaveEntitiesToScript(text, 2, things, names);
            EntityRuntimePersistence.SetGamePersistentScript(map, text);
            EntityRuntimePersistence.SetGamePersistentSaveData(map, 0, 0);
        }
        SerialisingGameState = false;
    }
}