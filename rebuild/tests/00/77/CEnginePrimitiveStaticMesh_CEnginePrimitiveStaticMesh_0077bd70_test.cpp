// CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh -- initialize static mesh packet. Retail0077bd70 complete177bytes ret0x34 +15int3. PDB native layout0x4c agrees with stores; no wrapper/composite engine headers exist. Base12bytes Type=8/interpolation/default channel, mesh+c position+10,size+1c,fade+20,RHS+24,nullableMeshEffect+3c,channel+40,tint+44,alpha+48,litbit+49,bulletTime+4a. Unused64bit identifier preserved in signature.
#ifndef _MSC_VER
#define __int64 long long
#endif
class C3DVector { public:float X,Y,Z;C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){} C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class CRightHandedSet { public:C3DVector Up,Forward;CRightHandedSet(const CRightHandedSet& o):Up(o.Up),Forward(o.Forward){} CRightHandedSet(const C3DVector& u,const C3DVector& f):Up(u),Forward(f){}};
class CRGBColour { public:unsigned char B,G,R,A;CRGBColour(){} CRGBColour(const CRGBColour& c){Set(c.R,c.G,c.B,c.A);} void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a){R=r;G=g;B=b;A=a;}};
class CEnginePrimitiveThemeChannelDesc { public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){}};
class CEnginePrimitive { public:long Type,InterpolationSet;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CEnginePrimitive(long type,long interp):Type(type),InterpolationSet(interp){}};
class CEngineMeshEffect;
struct CEngineMeshEffectPointer {CEngineMeshEffect* Object;CEngineMeshEffectPointer():Object(0){}};
enum EEngineInterpolationSet {INTERPOLATION_DEFAULT=0};
struct CEnginePrimitiveStaticMesh:CEnginePrimitive {
 unsigned long MeshIndex;C3DVector WorldPos;float Size,FadeStart;CRightHandedSet RightHandedSet;CEngineMeshEffectPointer MeshEffect;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CRGBColour TintColour;unsigned char Alpha;unsigned char Lit:1;unsigned char Reserved:7;bool BulletTimeAffectsCloth;
 CEnginePrimitiveStaticMesh(unsigned __int64,unsigned long,const C3DVector&,const CRightHandedSet&,float,bool,unsigned char,CRGBColour,float,EEngineInterpolationSet,const CEnginePrimitiveThemeChannelDesc&,bool);
};
CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh(unsigned __int64 unused,unsigned long mesh,const C3DVector& pos,const CRightHandedSet& rhs,float size,bool lit,unsigned char alpha,CRGBColour tint,float fade,EEngineInterpolationSet interp,const CEnginePrimitiveThemeChannelDesc& channel,bool bullet)
:CEnginePrimitive(8,interp),MeshIndex(mesh),WorldPos(pos),Size(size),FadeStart(fade),RightHandedSet(rhs),MeshEffect(),ThemeChannelDesc(channel),TintColour(tint),Alpha(alpha),Lit(lit),BulletTimeAffectsCloth(bullet)
{(void)unused;}

#include <cstdio>
#include <new>
#include <cstring>
#include <stddef.h>
static int failures=0;static void check(bool b){if(!b)++failures;}
static void run(bool lit,bool bullet,unsigned char alpha,unsigned char seed){
 union Storage {long double align;unsigned char bytes[sizeof(CEnginePrimitiveStaticMesh)];} storage;std::memset(storage.bytes,seed,sizeof(storage.bytes));
 C3DVector p(1.5f,-2.75f,3.25f),up(4,5,6),forward(-7,-8,-9);CRightHandedSet rhs(up,forward);CRGBColour color;color.B=0x12;color.G=0x34;color.R=0x56;color.A=0x78;CEnginePrimitiveThemeChannelDesc channel;channel.ChannelIndex0=19;channel.ChannelIndex1=29;channel.ChannelBlendFactor=39;channel.Pad=49;
 CEnginePrimitiveStaticMesh* mesh=new(storage.bytes)CEnginePrimitiveStaticMesh(0x12345678,785,p,rhs,-4.25f,lit,alpha,color,9.5f,(EEngineInterpolationSet)97,channel,bullet);
 check(mesh->Type==8 && mesh->InterpolationSet==97 && mesh->MeshIndex==785);
 check(mesh->CEnginePrimitive::ThemeChannelDesc.ChannelIndex0==0 && mesh->CEnginePrimitive::ThemeChannelDesc.ChannelIndex1==0 && mesh->CEnginePrimitive::ThemeChannelDesc.ChannelBlendFactor==0);
 check(mesh->WorldPos.X==1.5f && mesh->WorldPos.Y==-2.75f && mesh->WorldPos.Z==3.25f && mesh->Size==-4.25f && mesh->FadeStart==9.5f);
 check(mesh->RightHandedSet.Up.X==4 && mesh->RightHandedSet.Up.Y==5 && mesh->RightHandedSet.Up.Z==6 && mesh->RightHandedSet.Forward.X==-7 && mesh->RightHandedSet.Forward.Y==-8 && mesh->RightHandedSet.Forward.Z==-9);
 check(!mesh->MeshEffect.Object && mesh->ThemeChannelDesc.ChannelIndex0==19 && mesh->ThemeChannelDesc.ChannelIndex1==29 && mesh->ThemeChannelDesc.ChannelBlendFactor==39 && mesh->ThemeChannelDesc.Pad==49);
 check(mesh->TintColour.B==0x12 && mesh->TintColour.G==0x34 && mesh->TintColour.R==0x56 && mesh->TintColour.A==0x78 && mesh->Alpha==alpha && mesh->Lit==lit && mesh->BulletTimeAffectsCloth==bullet);
 p.X=100;rhs.Up.X=100;color.B=100;channel.ChannelIndex0=100;check(mesh->WorldPos.X==1.5f && mesh->RightHandedSet.Up.X==4 && mesh->TintColour.B==0x12 && mesh->ThemeChannelDesc.ChannelIndex0==19);
#ifdef _MSC_VER
 check(mesh->CEnginePrimitive::ThemeChannelDesc.Pad==seed && mesh->Reserved==(seed>>1));
#endif
 mesh->~CEnginePrimitiveStaticMesh();
}
int main(){
#ifdef _MSC_VER
 check(sizeof(CEnginePrimitiveStaticMesh)==0x4c && offsetof(CEnginePrimitiveStaticMesh,MeshIndex)==0xc && offsetof(CEnginePrimitiveStaticMesh,WorldPos)==0x10 && offsetof(CEnginePrimitiveStaticMesh,Alpha)==0x48);
#endif
 run(false,false,0,0xfe);run(true,false,255,0xaa);run(false,true,45,0xff);run(true,true,123,0);
 if(failures){std::printf("0077bd70_TEST FAIL %d\n",failures);return 1;}std::printf("0077bd70_TEST PASS\n");return 0;}