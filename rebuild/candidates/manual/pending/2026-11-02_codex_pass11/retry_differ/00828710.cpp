// CRegionConnectionGraph::InitialiseGraph(CCharString const &,CWorldMap const &) -- initialize the region connection graph for the world map.
// Leaf: ?InitialiseGraph@CRegionConnectionGraph@@QAEXABVCCharString@@ABVCWorldMap@@@Z
// Grade: DIFFER(1225v1289) -- CVectorMap<long,CCountedPointer<CRegionConnectionNode>>::Insert stays an out-of-line call where retail expands it (pair + counted-pointer construction + push path), frame 0x78 vs 0x80; 10 shapes (explicit/named counted-pointer argument, raw pointer local, connections placement, block form, Get(), named progress string, reserve order, two Insert body models in an overlay) leave it. Graded in Codex's header context
// Flags: /Gr
#define FABLE_RETAIL_REGION_GRAPH
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_QUEST_COMPLETION
#include "rebuild_abi.h"
#include "engine/CRegionConnectionGraph.h"
#include "engine/CStringParser.h"
#include "engine/CWorldMap.h"
namespace NProgressDisplay {void __fastcall DisplayProgress(const CCharString&,float,bool,bool);}
void CRegionConnectionGraph::InitialiseGraph(const CCharString& graph_data,const CWorldMap& world_map)
{
 GraphNodes.clear();GraphNodes.reserve(80);
 CStringParser parser(graph_data);
 while(!parser.AtEnd()&&parser.ReadAsSymbol()=="\""){
  NProgressDisplay::DisplayProgress("Initialise Region Graph",-1.0f,false,false);
  CCharString region_name=parser.ReadAsStringUntilString("\"");
  long region_id=world_map.GetRegionNumberFromName(region_name);
  if(region_id>0){
   GraphNodes.Insert(region_id,new CRegionConnectionNode(region_id));
   std::vector<long> connections;
   if(parser.SkipPastString("\":")){
    while(parser.SkipPastNextItemIfIs("\"")){
     CCharString connecting_region_name=parser.ReadAsStringUntilString("\"");
     long connected_region_id=world_map.GetRegionNumberFromName(connecting_region_name);
     if(connected_region_id>0)connections.push_back(connected_region_id);
     if(!parser.SkipPastString("\"")){GraphNodes.clear();return;}
     parser.SkipPastNextItemIfIs(",");
    }
    GraphNodes.back().second->InitialiseConnections(connections);
   }
  }
  parser.SkipPastNewline();
 }
 GraphNodes.Update();
}
