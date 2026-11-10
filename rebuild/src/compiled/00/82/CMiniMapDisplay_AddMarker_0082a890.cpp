// 0082a890 CMiniMapDisplay::AddMarker -- append a manual marker unless its thing/graphic pair already exists.
// Leaf: ?AddMarker@CMiniMapDisplay@@QAEXABVCThing@@J@Z
// Flags: none
// Retail:0082a890,206 bytes.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_CMINIMAPDISPLAY_ADDMARKER
#define FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CMiniMapDisplay.h"
void CMiniMapDisplay::AddMarker(const CThing& thing,long graphic_index)
{
 if(graphic_index>0){
  for(std::list<CMarkerInfo>::const_iterator it=Markers.begin();it!=Markers.end();++it){
   const CMarkerInfo& marker_info=*it;
   if(marker_info.GetPThing()==&thing&&marker_info.GetGraphicIndex()==graphic_index)return;
  }
  Markers.push_back(CMarkerInfo(thing,graphic_index,false));
 }
}