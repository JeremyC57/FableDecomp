// NThingMap::CThingMap::Validate -- traverse every cell and check its node-list tails.
// Retail: 0085cde0,214 bytes.
// Leaf: ?Validate@CThingMap@NThingMap@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_C2DBOXI
#include "engine/NThingMap_CThingMap.h"
void NThingMap::CThingMap::Validate()
{
 for(long c1=0;c1<MaxLayers;++c1) {
  long maxy,maxx;
  long size=MapwhoLevelInfo[c1].Size;
  if(size==-1)maxx=maxy=1;
  else {
   maxx=Area.GetExtents().X/size;
   maxy=Area.GetExtents().Y/size;
  }
  for(long cy=0;cy<maxy;++cy)
   for(long cx=0;cx<maxx;++cx)
    ValidateCell(GetCell((unsigned char)c1,cx,cy));
 }
}