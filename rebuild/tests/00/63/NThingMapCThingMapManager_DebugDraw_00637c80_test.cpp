#include <cstdio>
#include <cmath>
#define FABLE_FASTCALL
class C2DCoordI {public:long X,Y;C2DCoordI(long x=0,long y=0):X(x),Y(y){}};
class C2DVector {public:float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){}};
class C2DBoxI {public:long TLX,TLY,BRX,BRY;C2DCoordI GetTLPos()const{return C2DCoordI(TLX,TLY);}};
class C2DBoxF {public:float TLX,TLY,BRX,BRY;C2DBoxF(float a=0,float b=0,float c=0,float d=0):TLX(a),TLY(b),BRX(c),BRY(d){}};
class CRGBColour {public:unsigned char B,G,R,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b):B(b),G(g),R(r),A(255){}};
static int gMapCalls=0,gBoxCalls=0;static bool gColourOK=true;static long gLayer;static C2DVector gOrigin;static C2DBoxF gBox;
namespace NThingMap {class CThingMap{public:C2DBoxI Area;const C2DBoxI& GetArea()const{return Area;}void DebugDraw(const C2DVector& origin,long layer){++gMapCalls;gOrigin=origin;gLayer=layer;}};class CThingMapManager{public:CThingMap* GetThingMapAt(const C2DCoordI&)const;void DebugDraw(const C2DCoordI&,long)const;};}
static NThingMap::CThingMap* gMap=0;
NThingMap::CThingMap* NThingMap::CThingMapManager::GetThingMapAt(const C2DCoordI&)const{return gMap;}
class CRenderManager2D{public:bool Draw2DBox(const C2DBoxF& box,const CRGBColour& colour,unsigned long flags){++gBoxCalls;gBox=box;gColourOK &= colour.R==255&&colour.G==255&&colour.B==255&&colour.A==255&&flags==0;return true;}};
static CRenderManager2D gRender;CRenderManager2D* g_pRenderManager2D=&gRender;
double VIRTUAL_SCREEN_WIDTH=100.0,VIRTUAL_SCREEN_HEIGHT=200.0;
float GFAXToVX(float x){return x*2.0f;}float GFAYToVY(float y){return y*3.0f;}
void NThingMap::CThingMapManager::DebugDraw(const C2DCoordI& pos,long layer) const
{
 CThingMap* map=GetThingMapAt(pos);
 C2DVector origin((float)(VIRTUAL_SCREEN_WIDTH/20.0),(float)(VIRTUAL_SCREEN_HEIGHT/20.0));
 if(map){
  map->DebugDraw(origin,layer);
  static long count=0;
  if(count%16>8){
   float halfWidth=GFAXToVX(3.0f);
   float halfHeight=GFAYToVY(3.0f);
   C2DCoordI local(pos.X-map->GetArea().GetTLPos().X,pos.Y-map->GetArea().GetTLPos().Y);
   C2DVector point(GFAXToVX((float)local.X)+origin.X,GFAYToVY((float)local.Y)+origin.Y);
   g_pRenderManager2D->Draw2DBox(C2DBoxF(point.X-halfWidth,point.Y-halfHeight,point.X+halfWidth,point.Y+halfHeight),CRGBColour(255,255,255),0);
  }
  ++count;
 }
}

static bool Close(float a,float b){return std::fabs(a-b)<(1.0f/131072.0f);}
int main(){NThingMap::CThingMap map;map.Area.TLX=10;map.Area.TLY=20;map.Area.BRX=50;map.Area.BRY=60;NThingMap::CThingMapManager first,second;C2DCoordI pos(15,27);int ok=1,cases=0;first.DebugDraw(pos,-1);ok &= gMapCalls==0&&gBoxCalls==0;++cases;gMap=&map;
int boxes=0;for(int frame=0;frame<32;++frame){int before=gBoxCalls;const NThingMap::CThingMapManager& manager=(frame&1)?first:second;manager.DebugDraw(pos,frame);bool marker=frame%16>8;boxes+=marker?1:0;ok &= gMapCalls==frame+1&&gBoxCalls==boxes&&gLayer==frame;ok &= Close(gOrigin.X,5)&&Close(gOrigin.Y,10);if(marker){ok &= gBoxCalls==before+1;ok &= Close(gBox.TLX,9)&&Close(gBox.BRX,21)&&Close(gBox.TLY,22)&&Close(gBox.BRY,40);}++cases;if(frame%8==0){gMap=0;manager.DebugDraw(pos,99);ok &= gMapCalls==frame+1&&gBoxCalls==boxes;gMap=&map;++cases;}}
ok &= boxes==14&&cases==37&&gColourOK;std::printf(ok?"00637c80_TEST PASS\n":"00637c80_TEST FAIL\n");return ok?0:1;}