#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do {if(!(x)){printf("failed case %d line %d\n",k,__LINE__);exit(1);}}while(0)
static int k;
namespace std {
template<class T>struct list {typedef const T* const_iterator;T data[10];int count;
 const T*begin()const{return data;}const T*end()const{return data+count;}int size()const{return count;}const T&front()const{return data[0];}};
template<class T>struct vector {typedef T* iterator;T data[16];int count;T*begin(){return data;}T*end(){return data+count;}};
}
struct C3DVector{float X,Y,Z;};struct CNavWayPoint{C3DVector Position;};
struct CRGBColour{unsigned r,g,b;CRGBColour(unsigned x,unsigned y,unsigned z):r(x),g(y),b(z){}};
enum {ENGINE_PRIMITIVE_BLEND_SOLID,ENGINE_INTERPOLATION_SET_0=0,ENGINE_PRIMITIVE_BLEND_OP_ADD=0};
struct CEnginePrimitiveLine{C3DVector Start,End;CRGBColour Colour;
 CEnginePrimitiveLine():Colour(0,0,0){}
 CEnginePrimitiveLine(unsigned __int64 id,const C3DVector&a,const C3DVector&b,CRGBColour c,float w,int mode,unsigned bank,bool use,int interp,int op):Start(a),End(b),Colour(c){
 CHECK(!id&&w==0.05f&&!mode&&!bank&&use&&!interp&&!op);}};
struct CIEngine{CEnginePrimitiveLine lines[50];int n;void AddPrimitive(const CEnginePrimitiveLine&l){CHECK(n<50);lines[n++]=l;}};
struct CEnginePrimitivePrepareInfo{CIEngine&Engine;};struct CNavigatorBase{unsigned type;};
static int paths,heights,positions;
struct CNavigatorAStar:CNavigatorBase{std::list<CNavWayPoint> route;std::vector<C3DVector> extras;
 const std::list<CNavWayPoint>*GetRoute(){return &route;}void CreateLineList(std::vector<C3DVector>&v){++paths;v=extras;}};
template<class F,class T>T GFDebugPDynamicCast(F p,T){return static_cast<T>(p);}
struct Creature{C3DVector pos;const C3DVector&GetPos(){++positions;return pos;}float GetHeightZBelowAt(const C3DVector&v,bool a,bool b){
 CHECK(v.Z==1000&&!a&&b);++heights;return 2*v.X-v.Y;}};
const unsigned long NAVIGATOR_TYPE_A_STAR=3;
struct CTCCreatureNavigation{bool DrawNavRoutes;CNavigatorBase*PNavigator;Creature parent;
 unsigned GetNavigatorType(){return PNavigator->type;}Creature&GetParentCreature(){return parent;}
 void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);};
void CTCCreatureNavigation::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    static float vertical_offset = 1.0f;
    if (!DrawNavRoutes || GetNavigatorType() != NAVIGATOR_TYPE_A_STAR) return;
    CNavigatorAStar* a_star = GFDebugPDynamicCast(PNavigator,(CNavigatorAStar*)0);
    const std::list<CNavWayPoint>* route = a_star->GetRoute();
    if (!route->size()) return;
    if(false) {} else for (std::list<CNavWayPoint>::const_iterator point=route->begin(); point!=route->end(); ++point) {
        std::list<CNavWayPoint>::const_iterator next=point;
        if (++next==route->end()) break;
 C3DVector start,end;
 start.X=point->Position.X; start.Y=point->Position.Y; start.Z=1000.0f; start.Z=GetParentCreature().GetHeightZBelowAt(start,false,true)+vertical_offset;
 end.X=next->Position.X; end.Y=next->Position.Y; end.Z=1000.0f; end.Z=GetParentCreature().GetHeightZBelowAt(end,false,true)+vertical_offset;
 CEnginePrimitiveLine line(0,start,end,CRGBColour(0,255,0),0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_0,ENGINE_PRIMITIVE_BLEND_OP_ADD);
 info.Engine.AddPrimitive(line);
 end=start; end.Z+=0.4f; line.Start=start; line.End=end; line.Colour=CRGBColour(0,0,255); info.Engine.AddPrimitive(line);
 }
 C3DVector start,end;
 start.X=GetParentCreature().GetPos().X; start.Y=GetParentCreature().GetPos().Y; start.Z=1000.0f; start.Z=GetParentCreature().GetHeightZBelowAt(start,false,true)+vertical_offset;
 end.X=route->front().Position.X; end.Y=route->front().Position.Y; end.Z=1000.0f; end.Z=GetParentCreature().GetHeightZBelowAt(end,false,true)+vertical_offset;
 CEnginePrimitiveLine line(0,start,end,CRGBColour(255,0,0),0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_0,ENGINE_PRIMITIVE_BLEND_OP_ADD);
 info.Engine.AddPrimitive(line);
 end=start; end.Z+=vertical_offset; line.Start=start; line.End=end; line.Colour=CRGBColour(255,0,0); info.Engine.AddPrimitive(line);
 std::vector<C3DVector> line_list; a_star->CreateLineList(line_list);
 if(false) {} else for (std::vector<C3DVector>::iterator line_it=line_list.begin();line_it!=line_list.end();++line_it) {
 C3DVector start_pos=*line_it++; start_pos.Z=1000.0f; start_pos.Z=GetParentCreature().GetHeightZBelowAt(start_pos,false,true)+vertical_offset;
 C3DVector end_pos=*line_it; end_pos.Z=1000.0f; end_pos.Z=GetParentCreature().GetHeightZBelowAt(end_pos,false,true)+vertical_offset;
 CEnginePrimitiveLine loop_line(0,start_pos,end_pos,CRGBColour(0,255,0),0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_0,ENGINE_PRIMITIVE_BLEND_OP_ADD);
 info.Engine.AddPrimitive(loop_line);
 }
}
static C3DVector projected(C3DVector v){v.Z=2*v.X-v.Y+1;return v;}
static bool same(C3DVector a,C3DVector b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
static void verify(CIEngine&e,int i,C3DVector a,C3DVector b,unsigned r,unsigned g,unsigned blue){
 CHECK(i<e.n);const CEnginePrimitiveLine&l=e.lines[i];CHECK(same(l.Start,a)&&same(l.End,b));CHECK(l.Colour.r==r&&l.Colour.g==g&&l.Colour.b==blue);}
int main(){for(k=0;k<16384;++k){
 CIEngine e;e.n=0;CEnginePrimitivePrepareInfo info={e};CTCCreatureNavigation nav;CNavigatorAStar n;nav.PNavigator=&n;
 nav.DrawNavRoutes=(k&1)!=0;n.type=(k&2)?3:5;n.route.count=(k>>2)%10;n.extras.count=2*((k>>6)%8);
 nav.parent.pos.X=(float)(121-k%39);nav.parent.pos.Y=(float)(-72+k%17);nav.parent.pos.Z=-3;
 int j;for(j=0;j<n.route.count;++j){C3DVector v={(float)(j*7-k%61),(float)(k%29-2*j),40};n.route.data[j].Position=v;}
 for(j=0;j<n.extras.count;++j){C3DVector v={(float)(j*3+k%31),(float)(k%53-j),-100};n.extras.data[j]=v;}
 heights=paths=positions=0;nav.DrawPreparePrimitivesForRendering(info);int out=0;
 if(nav.DrawNavRoutes&&n.type==3&&n.route.count){CHECK(paths==1&&positions==2&&heights==2*n.route.count+n.extras.count);
 for(j=0;j+1<n.route.count;++j){C3DVector a=projected(n.route.data[j].Position),b=projected(n.route.data[j+1].Position);verify(e,out++,a,b,0,255,0);b=a;b.Z+=0.4f;verify(e,out++,a,b,0,0,255);}
 C3DVector a=projected(nav.parent.pos),b=projected(n.route.data[0].Position);verify(e,out++,a,b,255,0,0);b=a;b.Z+=1;verify(e,out++,a,b,255,0,0);
 for(j=0;j<n.extras.count;j+=2)verify(e,out++,projected(n.extras.data[j]),projected(n.extras.data[j+1]),0,255,0);
 }else CHECK(!paths&&!positions&&!heights);CHECK(out==e.n);
 }puts("006cc270_TEST PASS");return 0;}