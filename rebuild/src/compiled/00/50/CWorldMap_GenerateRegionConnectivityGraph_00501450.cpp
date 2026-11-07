// CWorldMap::GenerateRegionConnectivityGraph() -- serialize connections among loaded regions.
// Leaf: ?GenerateRegionConnectivityGraph@CWorldMap@@QAEXXZ
// Retail: Fable.exe 0x00501450, RELOCATION_MATCH (chunk 849 rebase of the held Codex pass 13 source; no Codex headers on disk).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CThing::SetInLimbo 0x004C8CF0,
//   CWorldMap::UnloadRegion 0x004FEEC0 / LoadRegion 0x00500540 / GetRegionNumberMapIsIn 0x004FC190, the GetAllThings instances
//   0x0048D400 (CThingFilter_HasTC) and 0x005198B0 (CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>), the const
//   CVectorMap::LowerBound 0x004365B0, CTCDRegionExit::GetNameOfRegionConnectingTo 0x007792D0, set<CCharString> insert_unique
//   0x004AC380 / _M_erase 0x004ABA70, CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, CCharString
//   operators, CDiskFileWin32(const CWideString&,FileAccessType,unsigned long) 0x0099AD80 / Close 0x0099A920, CCharString::SaveToFile.
// Shape: Codex body; the main player is a named `CPlayer& player` local (Codex's PDB locals list has none; without it phero and
//   current_region swap spill slots, 8 lines); the non-const LowerBound is expanded inline (FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND).
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_PLAYER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CMapInfo.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_HasTCOfLeafType.h"
#include "engine/CTCDRegionExit.h"
#include "engine/CTCActionUseScriptedHook.h"
#include "engine/CDiskFileWin32.h"
#include "fable_game.h"
void CWorldMap::GenerateRegionConnectivityGraph()
{
    CPlayer& player=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer();
    CThingPlayerCreature* phero=player.GetPControlledCreature();
    if(phero && !phero->IsDead()) phero->SetInLimbo(true);
    long current_region=LoadedRegion;
    UnloadRegion(current_region,false);
    CCharString graph_data;
    for(long region_index=1;region_index<Regions.size();++region_index) {
        LoadRegion(region_index,WAIT_FOR_LOAD,false);
        std::vector<CThing*> region_exits;
        World.GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(CTCDRegionExit::StatGetInterfaceType()),region_exits);
        std::set<CCharString> regions_connecting_to;
        for(long c1=0;c1<region_exits.size();++c1) {
            if(GetRegionNumberMapIsIn(region_exits[c1]->GetCurrentMapNumber())==region_index) {
                const CTCDRegionExit* ptc_region_exit=0;
                region_exits[c1]->PeekTC(&ptc_region_exit);
                const CCharString& region_connecting_to_name=ptc_region_exit->GetNameOfRegionConnectingTo();
                if(region_connecting_to_name!="") regions_connecting_to.insert(region_connecting_to_name);
            }
        }
        region_exits.clear();
        World.GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>(),region_exits);
        for(long c1=0;c1<region_exits.size();++c1) {
            if(GetRegionNumberMapIsIn(region_exits[c1]->GetCurrentMapNumber())==region_index) {
                const CTCActionUseScriptedHook* ptc_scripted_hook;
                region_exits[c1]->GetTC(&ptc_scripted_hook);
                CThing* pentrance=ptc_scripted_hook->GetPEntranceConnectedTo();
                if(pentrance && !pentrance->IsDead())
                    regions_connecting_to.insert(Regions[GetRegionNumberMapIsIn(GetMapNumberAt(pentrance->GetPos()))].GetName());
            }
        }
        if(!regions_connecting_to.empty()) {
            graph_data+="\""+Regions[region_index].GetName()+"\": ";
            std::set<CCharString>::iterator last_entry=regions_connecting_to.end();
            --last_entry;
            for(std::set<CCharString>::iterator it=regions_connecting_to.begin();it!=regions_connecting_to.end();++it) {
                graph_data+="\""+*it+"\"";
                if(it!=last_entry) graph_data+=", ";
            }
            graph_data+=";\r\n";
        }
        UnloadRegion(region_index,false);
    }
    CDiskFileWin32 graph_file(L"RegionGraph.txt",FILE_ACCESS_CREATE,2);
    graph_data.SaveToFile(graph_file);
    LoadRegion(current_region,WAIT_FOR_LOAD,true);
    if(phero && !phero->IsDead()) phero->SetInLimbo(false);
}