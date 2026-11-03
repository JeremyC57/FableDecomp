// CThingManager::RuntimeLoadEntities(long map, const set<CCharString>& names, vector<CThing*>& things) -- unless the editor is active, load a map's runtime-persisted entities: the level-persistent entity script, the (non-empty) game-persistent entity script, then the game-persistent binary save data, flagging the manager as loading while it runs
// Leaf: ?RuntimeLoadEntities@CThingManager@@QAEXJABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@AAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@3@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00522720, length 349 bytes (+3 int3), `ret 0Ch`. Name and signature in Ego_r +
//   FableWin (?RuntimeLoadEntities@CThingManager@@QAEXJABV?$set@VCCharString@@...@AAV?$vector@PAVCThing@@...@Z: the name set
//   and the thing vector are references).
// Callees: CMainGameComponent::Get() (PMainGameComponent 0x013B86A0) then virtual slot 9 IsEditorActive (+0x24),
//   CThingManager::LoadEntitiesFromScript 0x00520D00, NThingManager::CEntityRuntimePersistence::GetGamePersistentSaveData
//   0x00522530, CMemoryDataInputStream(const void*, unsigned long) 0x00994930, ~CDataStream 0x00993B10 (the derived stream
//   dtors are compiler-generated), CThingManager::LoadEntitiesBinary 0x00521240, free 0x00BFEA14.
// Shape: the level/game script accessors are the inline GetLevelPersistentScript / GetGamePersistentScript, the empty test is
//   the inline CCharString::IsEmpty. Rebased (2026-11-03) from row-local CThingManager / CEntityRuntimePersistence /
//   CCharString / CMainGameComponent / CMemoryDataInputStream models onto the shared headers.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "stlport/fable_containers.h"
#include "engine/CThingManager.h"
#include "fable_game.h"
#include "engine/CMemoryDataInputStream.h"

void CThingManager::RuntimeLoadEntities(long map, const std::set<CCharString>& names, std::vector<CThing*>& things)
{
    if (CMainGameComponent::Get()->IsEditorActive())
        return;
    LoadingEntitiesFromSaveGame = true;
    const CCharString* levelScript = EntityRuntimePersistence.GetLevelPersistentScript(map);
    if (levelScript)
        LoadEntitiesFromScript(*levelScript, EntityRuntimePersistence.SaveFormatVersion, map, names, 0, 0, things);
    const CCharString* gameScript = EntityRuntimePersistence.GetGamePersistentScript(map);
    if (gameScript && !gameScript->IsEmpty())
        LoadEntitiesFromScript(*gameScript, EntityRuntimePersistence.SaveFormatVersion, map, names, 0, 0, things);
    CArray<unsigned char> data;
    if (EntityRuntimePersistence.GetGamePersistentSaveData(map, data) && !data.empty()) {
        CMemoryDataInputStream stream(data.GetAsCArray(), data.size());
        LoadEntitiesBinary(stream, EntityRuntimePersistence.SaveFormatVersion, map, names, 0, 0, things);
    }
    LoadingEntitiesFromSaveGame = false;
}