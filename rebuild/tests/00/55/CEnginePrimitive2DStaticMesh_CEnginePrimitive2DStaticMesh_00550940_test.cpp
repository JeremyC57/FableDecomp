#ifndef _MSC_VER
#define __int64 long long
#endif
// CEnginePrimitive2DStaticMesh::CEnginePrimitive2DStaticMesh -- constructs mesh index, clip box, alpha, orientation, invalid bounding sphere and tint; retail00550940 192bytes. PDB wrapper layout0x6c confirmed at offsets0xc/0x10/0x20/0x24/0x58/0x68; no matching wrapper or composite headers exist. Orientation copy installs class vtable01239824; other members copied fieldwise.
class C3DVector { public: float X,Y,Z; C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){} C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} };
class CRightHandedSet { public: C3DVector Up,Forward; CRightHandedSet(const CRightHandedSet& o):Up(o.Up),Forward(o.Forward){} CRightHandedSet(const C3DVector& up,const C3DVector& forward):Up(up),Forward(forward){} };
class C3DOrientationInterpolator { public:
 virtual ~C3DOrientationInterpolator(); // RTTI vtable01239824 slot0 -> deleting destructor004c2130
 CRightHandedSet StartOrientation,EndOrientation;
 C3DOrientationInterpolator(const CRightHandedSet& start,const CRightHandedSet& end):StartOrientation(start),EndOrientation(end){}
 C3DOrientationInterpolator(const C3DOrientationInterpolator& o):StartOrientation(o.StartOrientation),EndOrientation(o.EndOrientation){}
};
class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF(float x,float y,float xx,float yy):TLX(x),TLY(y),BRX(xx),BRY(yy){} C2DBoxF(const C2DBoxF& o):TLX(o.TLX),TLY(o.TLY),BRX(o.BRX),BRY(o.BRY){} };
class CRGBColour { public: unsigned char B,G,R,A; };
struct CMeshTint { unsigned char B,G,R,A; CMeshTint(const CRGBColour& o) { Set(o.R,o.G,o.B,o.A); } void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; } };
class CEnginePrimitiveThemeChannelDesc { public: unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad; CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){} };
class CEnginePrimitive { public: long Type,InterpolationSet; CEnginePrimitiveThemeChannelDesc ThemeChannelDesc; CEnginePrimitive(long type,long interpolation):Type(type),InterpolationSet(interpolation){} };
class C3DBoundingSphere { public: C3DVector Center; float Radius; C3DBoundingSphere():Center(0,0,0),Radius(-1){} };
struct CEnginePrimitive2DStaticMesh : CEnginePrimitive {
 unsigned long MeshIndex;
 C2DBoxF ClipBox;
 unsigned char Alpha;
 C3DOrientationInterpolator Orientation;
 C3DBoundingSphere BoundingSphere;
 CMeshTint TintColour;
 CEnginePrimitive2DStaticMesh(unsigned __int64 unused,unsigned long mesh,const C2DBoxF* box,const C3DOrientationInterpolator* orientation,long interpolation,unsigned char alpha,CRGBColour tint);
};
CEnginePrimitive2DStaticMesh::CEnginePrimitive2DStaticMesh(unsigned __int64 unused,unsigned long mesh,const C2DBoxF* box,const C3DOrientationInterpolator* orientation,long interpolation,unsigned char alpha,CRGBColour tint)
 : CEnginePrimitive(0x24,interpolation),MeshIndex(mesh),ClipBox(*box),Alpha(alpha),Orientation(*orientation),BoundingSphere(),TintColour(tint)
{ (void)unused; }

#include <cstdio>
#include <cstddef>
C3DOrientationInterpolator::~C3DOrientationInterpolator() {}
static int check(bool ok) { return ok?0:1; }
static bool same(const C3DVector& a,const C3DVector& b) { return a.X==b.X && a.Y==b.Y && a.Z==b.Z; }
int main()
{
 int failures=0;
 C3DVector a(-1.0f,2.0f,-3.0f),b(4.0f,-5.0f,6.0f),c(-7.0f,8.0f,-9.0f),d(10.0f,-11.0f,12.0f);
 CRightHandedSet start(a,b),end(c,d);
 C3DOrientationInterpolator orientation(start,end);
 C2DBoxF box(-15.0f,0.25f,23.0f,17.5f);
 for(unsigned int i=0;i<2;++i) {
  CRGBColour tint={i?0x12:0,i?0x34:0,i?0x56:0,i?0x78:0};
  const unsigned long mesh=i?0xfedcba98UL:0;
  const long interpolation=i?0x76543210L:0;
  const unsigned char alpha=i?0xa5:0;
  CEnginePrimitive2DStaticMesh sprite(~static_cast<unsigned __int64>(0),mesh,&box,&orientation,interpolation,alpha,tint);
  failures+=check(sprite.Type==0x24 && sprite.InterpolationSet==interpolation);
  failures+=check(sprite.ThemeChannelDesc.ChannelIndex0==0 && sprite.ThemeChannelDesc.ChannelIndex1==0 && sprite.ThemeChannelDesc.ChannelBlendFactor==0);
  failures+=check(sprite.MeshIndex==mesh && sprite.Alpha==alpha);
  failures+=check(sprite.ClipBox.TLX==box.TLX && sprite.ClipBox.TLY==box.TLY && sprite.ClipBox.BRX==box.BRX && sprite.ClipBox.BRY==box.BRY);
  failures+=check(same(sprite.Orientation.StartOrientation.Up,orientation.StartOrientation.Up) && same(sprite.Orientation.StartOrientation.Forward,orientation.StartOrientation.Forward));
  failures+=check(same(sprite.Orientation.EndOrientation.Up,orientation.EndOrientation.Up) && same(sprite.Orientation.EndOrientation.Forward,orientation.EndOrientation.Forward));
  failures+=check(sprite.BoundingSphere.Center.X==0 && sprite.BoundingSphere.Center.Y==0 && sprite.BoundingSphere.Center.Z==0 && sprite.BoundingSphere.Radius==-1.0f);
  failures+=check(sprite.TintColour.R==tint.R && sprite.TintColour.G==tint.G && sprite.TintColour.B==tint.B && sprite.TintColour.A==tint.A);
  float saved=sprite.Orientation.StartOrientation.Up.X;
  orientation.StartOrientation.Up.X+=1.0f;
  failures+=check(sprite.Orientation.StartOrientation.Up.X==saved);
 }
#ifdef _MSC_VER
 failures+=check(sizeof(CEnginePrimitive2DStaticMesh)==0x6c && offsetof(CEnginePrimitive2DStaticMesh,Orientation)==0x24 && offsetof(CEnginePrimitive2DStaticMesh,BoundingSphere)==0x58 && offsetof(CEnginePrimitive2DStaticMesh,TintColour)==0x68);
#endif
 if(failures) { std::printf("00550940_TEST FAIL %d\n",failures); return 1; }
 std::printf("00550940_TEST PASS\n");return 0;
}