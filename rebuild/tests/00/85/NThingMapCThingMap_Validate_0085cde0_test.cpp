#include <stdio.h>
#include <vector>
static int failed,line_failed,visits,heads;static unsigned long expected_level[256],expected_x[256],expected_y[256];static int expected_visits;
#define CHECK(x) do{if(!(x)){failed=1;line_failed=__LINE__;return;}}while(0)
class C2DExtentsI{public:long X,Y;C2DExtentsI(long x=0,long y=0):X(x),Y(y){}};
class C2DBoxI{public:long TLX,TLY,BRX,BRY;C2DExtentsI GetExtents()const{return C2DExtentsI(BRX-TLX,BRY-TLY);}};
class CTCMapwho{};
namespace NThingMap{
long MaxLayers;
class CThingMapCell;
class CMapwhoNode{public:CTCMapwho*PMapwho;CMapwhoNode*PNext;CMapwhoNode*PPrevious;CThingMapCell*PCell;};
class CThingMapCell{public:CMapwhoNode*PFirstNode;const CMapwhoNode*GetPFirstNode()const{++heads;return PFirstNode;}};
class CMapwhoLevelInfo{public:long Size;C2DExtentsI Extents;long ScaleShift,NoThings;};
class CThingMap{public:std::vector<CMapwhoLevelInfo>MapwhoLevelInfo;std::vector<std::vector<CThingMapCell> >Cells;C2DBoxI Area;long NoThings;
 CThingMapCell&GetCell(unsigned char level,unsigned long x,unsigned long y){if(visits>=expected_visits||level!=expected_level[visits]||x!=expected_x[visits]||y!=expected_y[visits])failed=1;++visits;return Cells[level][MapwhoLevelInfo[level].Extents.X*y+x];}
 void ValidateCell(const CThingMapCell&cell)const{
 for(const CMapwhoNode*ploop_node1=cell.GetPFirstNode();ploop_node1;ploop_node1=ploop_node1->PNext){long c1=0;for(const CMapwhoNode*ploop_node2=ploop_node1->PNext;ploop_node2;ploop_node2=ploop_node2->PNext)++c1;}
 }
 void Validate();
};
}
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

static void one(int layers,int config,int width,long height,int pattern)
{
 using namespace NThingMap;
 CThingMap map;map.MapwhoLevelInfo.resize(4);map.Cells.resize(4);map.Area.TLX=-3;map.Area.TLY=5;map.Area.BRX=-3+width;map.Area.BRY=5+height;map.NoThings=123;
 CMapwhoNode nodes[4][64][3];CTCMapwho things[3];long sizes[]={-1,1,2,4};int expected_lengths[4][64];
 for(int l=0;l<4;++l){map.MapwhoLevelInfo[l].Size=sizes[(config>>(2*l))&3];map.MapwhoLevelInfo[l].Extents=C2DExtentsI(8,8);map.MapwhoLevelInfo[l].ScaleShift=l;map.MapwhoLevelInfo[l].NoThings=17+l;map.Cells[l].resize(64);
 for(int c=0;c<64;++c){int length=(c+l+pattern)%4;expected_lengths[l][c]=length;map.Cells[l][c].PFirstNode=length?&nodes[l][c][0]:0;for(int n=0;n<3;++n){CMapwhoNode&node=nodes[l][c][n];node.PMapwho=&things[n];node.PPrevious=n?&nodes[l][c][n-1]:0;node.PNext=n+1<length?&nodes[l][c][n+1]:0;node.PCell=&map.Cells[l][c];}}}
 MaxLayers=layers;visits=heads=expected_visits=0;failed=0;
 for(int l=0;l<layers;++l){long divisor=map.MapwhoLevelInfo[l].Size;long nx=divisor==-1?1:width/divisor,ny=divisor==-1?1:height/divisor;for(long cell=0;nx>0&&cell<nx*(ny>0?ny:0);++cell){expected_level[expected_visits]=l;expected_x[expected_visits]=cell%nx;expected_y[expected_visits]=cell/nx;++expected_visits;}}
 map.Validate();CHECK(!failed);CHECK(visits==expected_visits&&heads==expected_visits);CHECK(MaxLayers==layers&&map.NoThings==123);CHECK(map.Area.TLX==-3&&map.Area.TLY==5&&map.Area.BRX==-3+width&&map.Area.BRY==5+height);
 for(int l=0;l<4;++l){CHECK(map.MapwhoLevelInfo[l].Size==sizes[(config>>(2*l))&3]);CHECK(map.MapwhoLevelInfo[l].Extents.X==8&&map.MapwhoLevelInfo[l].Extents.Y==8);CHECK(map.MapwhoLevelInfo[l].NoThings==17+l&&map.MapwhoLevelInfo[l].ScaleShift==l);CHECK(map.Cells[l].size()==64);for(int c=0;c<64;++c){int length=expected_lengths[l][c];CHECK(map.Cells[l][c].PFirstNode==(length?&nodes[l][c][0]:0));for(int n=0;n<3;++n){const CMapwhoNode&node=nodes[l][c][n];CHECK(node.PMapwho==&things[n]);CHECK(node.PPrevious==(n?&nodes[l][c][n-1]:0));CHECK(node.PNext==(n+1<length?&nodes[l][c][n+1]:0));CHECK(node.PCell==&map.Cells[l][c]);}}}
}
int main(){int layers[]={-2,0,1,2,4};int widths[]={-2,0,3,8};int cases=0;for(int l=0;l<5;++l)for(int c=0;c<256;++c)for(int w=0;w<4;++w)for(int h=0;h<4;++h)for(int p=0;p<4;++p){one(layers[l],c,widths[w],widths[h],p);if(failed){printf("0085cde0_TEST FAIL line%d case%d\n",line_failed,cases);return 1;}++cases;}if(cases!=81920)return 2;puts("0085cde0_TEST PASS");return 0;}