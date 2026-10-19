// CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh -- initialize static mesh packet. Retail0077bd70 complete177bytes ret0x34 +15int3. PDB native layout0x4c agrees with stores; no wrapper/composite engine headers exist. Base12bytes Type=8/interpolation/default channel, mesh+c position+10,size+1c,fade+20,RHS+24,nullableMeshEffect+3c,channel+40,tint+44,alpha+48,litbit+49,bulletTime+4a. Unused64bit identifier preserved in signature.
#ifndef _MSC_VER
#define __int64 long long
#endif
struct C3DVector {float X,Y,Z;C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){} C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
struct CRightHandedSet {C3DVector Up,Forward;CRightHandedSet(const CRightHandedSet& o):Up(o.Up),Forward(o.Forward){} CRightHandedSet(const C3DVector& u,const C3DVector& f):Up(u),Forward(f){}};
struct CRGBColour {unsigned char B,G,R,A;CRGBColour(){} CRGBColour(const CRGBColour& c){Set(c.R,c.G,c.B,c.A);} void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a){R=r;G=g;B=b;A=a;}};
struct CEnginePrimitiveThemeChannelDesc {unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){}};
struct CEnginePrimitive {long Type,InterpolationSet;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CEnginePrimitive(long type,long interp):Type(type),InterpolationSet(interp){}};
struct CEngineMeshEffect;
struct CEngineMeshEffectPointer {CEngineMeshEffect* Object;CEngineMeshEffectPointer():Object(0){}};
enum EEngineInterpolationSet {INTERPOLATION_DEFAULT=0};
struct CEnginePrimitiveStaticMesh:CEnginePrimitive {
 unsigned long MeshIndex;C3DVector WorldPos;float Size,FadeStart;CRightHandedSet RightHandedSet;CEngineMeshEffectPointer MeshEffect;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CRGBColour TintColour;unsigned char Alpha;unsigned char Lit:1;unsigned char Reserved:7;bool BulletTimeAffectsCloth;
 CEnginePrimitiveStaticMesh(unsigned __int64,unsigned long,const C3DVector&,const CRightHandedSet&,float,bool,unsigned char,CRGBColour,float,EEngineInterpolationSet,const CEnginePrimitiveThemeChannelDesc&,bool);
};
CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh(unsigned __int64 unused,unsigned long mesh,const C3DVector& pos,const CRightHandedSet& rhs,float size,bool lit,unsigned char alpha,CRGBColour tint,float fade,EEngineInterpolationSet interp,const CEnginePrimitiveThemeChannelDesc& channel,bool bullet)
:CEnginePrimitive(8,interp),MeshIndex(mesh),WorldPos(pos),Size(size),FadeStart(fade),RightHandedSet(rhs),MeshEffect(),ThemeChannelDesc(channel),TintColour(tint),Alpha(alpha),Lit(lit),BulletTimeAffectsCloth(bullet)
{(void)unused;}