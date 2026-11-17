// CEngineWorldMap::OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE state) -- switch the world's static maps to a new open state: build the grid, then either open every map's development static map in place (state 2), or open the static-map bank file, hand it to the water renderer, and load the whole bank at once when it carries the global entry, else open every map from it
// Leaf: ?OpenStaticMaps@CEngineWorldMap@@AAEXW4STATIC_MAP_OPEN_STATE@CEngineMap@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B42750, length 400 bytes (with int3 padding; body 0x17f), `ret 4`.
// PDB: ?OpenStaticMaps@CEngineWorldMap@@AAEXW4STATIC_MAP_OPEN_STATE@CEngineMap@@@Z.
// Callees: AutoBuildWorldMapGrid @ 0x00B40070; CCharString(const char*, long) @ 0x0099EBF0 / copy @ 0x0099EC30 /
//   ~CCharString @ 0x0099EAE0; __fastcall NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool)
//   @ 0x009E9F40; CEngineMap::OpenDevelopmentStaticMap(CStaticMapBankFile&, STATIC_MAP_OPEN_STATE) @ 0x00B42530; the bank
//   file's virtual slot 3 (open by name with flags 0x62); its memory-pool setter @ 0x00B3E820 (by-value counted pointer;
//   manifest "CMeshDataBank::SetAnimationMemoryPool", propagated); CEngineWaterRenderer::OpenStaticMapForUse
//   (CStaticMapBankFile*, CCharString) @ 0x00B6D4D0 (global @ 0x01436E54); the bank entry lookup @ 0x009CCDC0 (const
//   CCharString&, as the landed FindEntry callers); CEngineWorldMap::OpenStaticMapsUsingCommonRetailHeader(long) @ 0x00B420F0 (Ego_r.pdb
//   0x009E75F0, private; was the manifest's OpenMapFile).
// Globals: the engine @ 0x01436E98 (memory pool counted pointer at +0x8e0), two CCharString globals @ 0x01436EC4 (the
//   water bank symbol) and 0x01436ECC (the global map entry symbol). String @ 0x012A1F84 "SetStaticMapFileForUse: OpenStaticMap".
// Layout: retail CEngineWorldMap (as DeleteMap/AddMap/SetWorld): MapList +0x20, StaticMapBankName (CWideString) +0x30,
//   StaticMapFile +0x34, StaticMapOpenState +0x1a8.
// Shape: the progress text is an implicit conversion temporary; the state-2 loop passes the folded constant 2; the open
//   result is kept in bl across the pool and water calls; the single state store after the if/else is duplicated by VC
//   into each exit (storing ebp, not the propagated 2), which separate early-return stores do not reproduce.
// NF46 (round 7): the 0x01436E98 stand-in CEngineGlobal_01436e98 / g_pEngine_01436e98 is NEngineInternal::GPResourceManager
//   (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) on engine/CEngineResourceManager.h; its +0x8e0 LoadingMemoryPool is read
//   through FableWin.pdb's inline GetLoadingMemoryPool (overlay opt-in _GETLOADINGMEMORYPOOL). The local CCharString / CWideString moved to
//   fable_string.h (included by the header; FABLE_RETAIL_CCHARSTRING for the defaulted length).
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x01436EC4 SeaStaticMapBankFileName (global; initializer 0x01224680), was g_WaterBankSymbol.
//   0x01436ECC CommonStaticMapHeaderEntryName (global; initializer 0x012246C0), was g_GlobalMapEntrySymbol.
// NF65 (round 7): ported onto the shared headers: engine/CEngineWorldMap.h (overlay opt-ins FABLE_RETAIL_CENGINEWORLDMAP_STATICMAPFILE +
//   _OPENSTATICMAPS: MapList, the CWideString StaticMapBankName, the typed StaticMapFile, StaticMapOpenState +0x1A8 and the private members),
//   engine/CStaticMapBankFile.h (overlay class opt-in) on engine/CBankFileAsync.h (FABLE_RETAIL_CBANKFILEASYNC_SETLOADINGMEMORY: q10425
//   ?SetLoadingMemoryPool@CBankFileAsync@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z) / CBankFile.h (slot 3 OpenFileReadOnly,
//   ENTRIES FindIndexBySymbol), engine/CEngineMap.h (overlay opt-in _OPENDEVELOPMENTSTATICMAP), engine/CEngineWaterRenderer.h (overlay
//   declaration opt-in _OPENSTATICMAPFORUSE_DECL), CEngineResourceManager.h, CCountedPointer.h, CArray.h, fable_string.h. The local CBankFile /
//   CBankFileAsync / CStaticMapBankFile / CEngineWaterRenderer / CEngineMap / CEngineWorldMap models are gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETLOADINGMEMORYPOOL
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINEWORLDMAP_STATICMAPFILE
#define FABLE_RETAIL_CENGINEWORLDMAP_OPENSTATICMAPS
#define FABLE_RETAIL_CENGINEMAP_OPENDEVELOPMENTSTATICMAP
#define FABLE_RETAIL_CENGINEWATERRENDERER_OPENSTATICMAPFORUSE_DECL
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CBANKFILEASYNC_SETLOADINGMEMORY
#define FABLE_RETAIL_CSTATICMAPBANKFILE_CLASS
#include "fable_string.h"
#include "engine/CEngineResourceManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"
#include "engine/CStaticMapBankFile.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineWorldMap.h"
extern CEngineWaterRenderer* g_pEngineWaterRenderer;   // 0x01436E54
extern CCharString SeaStaticMapBankFileName;                  // 0x01436EC4
extern CCharString CommonStaticMapHeaderEntryName;             // 0x01436ECC

void CEngineWorldMap::OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE state)
{
    if (StaticMapOpenState == state)
        return;
    AutoBuildWorldMapGrid();
    if (state == CEngineMap::STATIC_MAP_OPEN_STATE_OPEN_FOR_UPDATE) {
        for (unsigned long i = 0; i < MapList.size(); ++i) {
            if (MapList[i]) {
                NProgressDisplay::DisplayProgress("SetStaticMapFileForUse: OpenStaticMap", -1.0f, false, false);
                MapList[i]->OpenDevelopmentStaticMap(StaticMapFile, state);
            }
        }
    }
    else {
        bool opened = StaticMapFile.OpenFileReadOnly(StaticMapBankName, 0x62);
        StaticMapFile.SetLoadingMemoryPool(NEngineInternal::GPResourceManager->GetLoadingMemoryPool());
        if (!opened)
            return;
        g_pEngineWaterRenderer->OpenStaticMapForUse(&StaticMapFile, SeaStaticMapBankFileName);
        long entry = StaticMapFile.FindIndexBySymbol(CommonStaticMapHeaderEntryName);
        if (entry > 0)
            OpenStaticMapsUsingCommonRetailHeader(entry);
        else {
            for (unsigned long i = 0; i < MapList.size(); ++i) {
                if (MapList[i]) {
                    NProgressDisplay::DisplayProgress("SetStaticMapFileForUse: OpenStaticMap", -1.0f, false, false);
                    MapList[i]->OpenDevelopmentStaticMap(StaticMapFile, state);
                }
            }
        }
    }
    StaticMapOpenState = state;
}
