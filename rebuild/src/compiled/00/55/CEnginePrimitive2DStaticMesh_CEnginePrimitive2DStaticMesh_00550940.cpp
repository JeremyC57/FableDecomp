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