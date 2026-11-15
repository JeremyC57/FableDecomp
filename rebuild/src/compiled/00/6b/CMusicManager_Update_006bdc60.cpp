// 006bdc60 CMusicManager::Update
// Update music from the controlled creature's sound-theme cell and region.
// Leaf: ?Update@CMusicManager@@QAEXXZ
// Retail: 0x006BDC60,492 bytes,thiscall.
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_PASS22_GETDEF_INDEX_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CSOUNDTHEMEDEF_PASS22
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_PASS22_SOUNDTHEME
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_PASS22_FORCEMUSICPLAY
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSoundThemeDef.h"
#include "engine/CMusicManager.h"
#include "engine/CSystemManager.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegionDef.h"
#include "engine/CRegion.h"
void CMusicManager::Update()
{
    bool forcePlay = false;
    float offset;
    if (GFGetSystemManager()->GetForceMusicPlay(offset)) {
        forcePlay = true;
        GFGetSystemManager()->ResetForceMusicPlay();
    }
    if (PendingMusicID)
        PlayMusic(PendingMusicID, *PPendingFadeTimes, PendingLoopCount);
    CThingPlayerCreature* creature = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (creature && !creature->IsDead()) {
        CWorldMap& map = CMainGameComponent::Get()->GetWorld().GetWorldMap();
        long themeIndex = map.GetBlockValue(creature->GetPos().ToC2DCoordI());
        CDefPointer<const CSoundThemeDef> theme;
        long cellMusic = 0;
        if (themeIndex && CGameDefinitionManager::Get().GetDef("SOUND_THEME", themeIndex, &theme)) {
            if (theme->MusicSet)
                cellMusic = theme->MusicSet;
        }
        long mapNumber = map.GetMapNumberAt(creature->GetPos());
        long regionMusic = 0;
        if (mapNumber) {
            long regionNumber = map.GetRegionNumberMapIsIn(mapNumber);
            if (regionNumber) {
                CDefPointer<const CRegionDef> region = map.GetRegion(regionNumber).GetPRegionDef();
                if (!region.IsNull())
                    regionMusic = region->Music;
            }
        }
        CState newState = CurrentState;
        newState.RegionSet = regionMusic;
        newState.MapCellSet = cellMusic;
        newState.Paused = Paused;
        UpdateCurrentMusicOffset();
        InternalUpdate(newState, forcePlay);
    }
}