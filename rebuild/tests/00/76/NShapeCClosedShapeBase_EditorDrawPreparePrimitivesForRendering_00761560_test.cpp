#include <cstdio>
#include <vector>
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector(const C3DVector& p):X(p.X),Y(p.Y),Z(p.Z){}};
class CEnginePrimitivePrepareInfo{};class CWorldMap{};class CRGBColour{};
static const CEnginePrimitivePrepareInfo* gInfo;static const CWorldMap* gWorld;static const C3DVector* gLod;static const CRGBColour* gColour;static C3DVector gFrom[16],gTo[16];static int gLines,gBadArgs;
namespace NShape {class CClosedShapeBase{public:std::vector<C3DVector> Points;void EditorDrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&,const CWorldMap&,const C3DVector&,const CRGBColour&);};
static void DrawPrimitiveLineOverLandscape(const CEnginePrimitivePrepareInfo& info,const CWorldMap& wmap,const C3DVector& from,const C3DVector& to,const C3DVector& lod,const CRGBColour& colour){gBadArgs|=&info!=gInfo||&wmap!=gWorld||&lod!=gLod||&colour!=gColour;if(gLines<16){gFrom[gLines]=from;gTo[gLines]=to;}else gBadArgs=1;++gLines;}}
void NShape::CClosedShapeBase::EditorDrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info,const CWorldMap& wmap,const C3DVector& lod_centre,const CRGBColour& colour)
{
 if(!Points.empty()){
  std::vector<C3DVector>::const_iterator it=Points.begin();
  C3DVector start_pos=*it++;
  while(it!=Points.begin()){
   C3DVector end_pos;
   if(it==Points.end()){
    it=Points.begin();
    end_pos=*it;
   }else end_pos=*it++;
   DrawPrimitiveLineOverLandscape(info,wmap,start_pos,end_pos,lod_centre,colour);
   start_pos=end_pos;
  }
 }
}

static bool Equal(const C3DVector& a,const C3DVector& b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
int main(){int ok=1,cases=0;CEnginePrimitivePrepareInfo info;CWorldMap world;CRGBColour colour;C3DVector lod(2,4,8);gInfo=&info;gWorld=&world;gColour=&colour;gLod=&lod;for(int n=0;n<16;++n)for(int seed=0;seed<4;++seed){NShape::CClosedShapeBase shape;for(int i=0;i<n;++i)shape.Points.push_back(C3DVector((float)(i+seed),(float)(i*i-seed),float(seed-i)/4));std::vector<C3DVector> before=shape.Points;gLines=gBadArgs=0;shape.EditorDrawPreparePrimitivesForRendering(info,world,lod,colour);ok &= gLines==n&&!gBadArgs&&shape.Points.size()==before.size();for(int i=0;i<n;++i)ok &= Equal(gFrom[i],before[i])&&Equal(gTo[i],before[(i+1)%n])&&Equal(shape.Points[i],before[i]);++cases;}ok &= cases==64;std::printf(ok?"00761560_TEST PASS\n":"00761560_TEST FAIL\n");return ok?0:1;}