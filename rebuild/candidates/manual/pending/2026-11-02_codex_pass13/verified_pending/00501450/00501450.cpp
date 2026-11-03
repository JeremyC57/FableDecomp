// CWorldMap::GenerateRegionConnectivityGraph() -- serialize connections among loaded regions.
// Leaf: ?GenerateRegionConnectivityGraph@CWorldMap@@QAEXXZ
#define FABLE_RETAIL_CVECTORMAP_ORDINARY_LOWERBOUND
#define FABLE_RETAIL_CTHINGFILTER_HASTC_EXTERNAL
#define FABLE_RETAIL_STLPORT_TREE_INSERT_BODY
#define FABLE_RETAIL_CVECTORMAP_SIZE_LOWERBOUND
#define FABLE_RETAIL_REGION_CONNECTIVITY_TYPES
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
#define FABLE_RETAIL_CPLAYER_TYPED
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHING_NAVIGATION
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALL_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
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
    CThingPlayerCreature* phero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
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
