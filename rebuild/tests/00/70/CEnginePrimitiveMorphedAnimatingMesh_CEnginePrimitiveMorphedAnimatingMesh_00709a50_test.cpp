// CEnginePrimitiveMorphedAnimatingMesh::CEnginePrimitiveMorphedAnimatingMesh -- forwards animating-mesh settings, copies texture morphs and hidden materials, and starts an empty decal vector. Retail00709a50,161bytes, complete ret60. No shared class header exists; donor vectors16byte conflict with retail12byte members (morph90,decals9c,seta8). Base size90 from native ctor and landed destructor00709b10. Callees007088f0,005bbdf1,00514400. PDB signature independently identified in editor symbols; native final base boolean forcedtrue.
// Leaf: ??0CEnginePrimitiveMorphedAnimatingMesh
#ifndef _MSC_VER
#define __int64 long long
#endif
struct C3DAnimationInterpolator;
struct C3DPositionInterpolator;
struct C3DOrientationInterpolator;
struct CEnginePrimitiveThemeChannelDesc;
enum EEngineInterpolationSet {INTERPOLATION_DEFAULT=0};
struct CRGBColour {
 unsigned char B,G,R,A;
 CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a){Set(r,g,b,a);}
 CRGBColour(const CRGBColour& o){Set(o.R,o.G,o.B,o.A);}
 void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a){R=r;G=g;B=b;A=a;}
};
struct CTextureMorphVector {void* First;void* Last;void* End;CTextureMorphVector():First(0),Last(0),End(0){} CTextureMorphVector(const CTextureMorphVector&);}; //005bbdf1
struct CDecalAllocator {CDecalAllocator(){}};
struct CDecalStorage:CDecalAllocator {void* Data; CDecalStorage(const CDecalAllocator& a,void* data):CDecalAllocator(a),Data(data){}};
struct CTextureDecalVector {void* First;void* Last;CDecalStorage End;CTextureDecalVector():First(0),Last(0),End(CDecalAllocator(),0){}};
struct CHiddenMaterialSet {void* Header;unsigned long Count;CHiddenMaterialSet():Header(0),Count(0){} CHiddenMaterialSet(const CHiddenMaterialSet&);}; //00514400
struct CEnginePrimitiveAnimatingMesh {
 unsigned long Type;unsigned char NativeBaseData[0x8c];
 CEnginePrimitiveAnimatingMesh(unsigned __int64,unsigned long,const C3DAnimationInterpolator&,const C3DPositionInterpolator&,const C3DOrientationInterpolator&,float,bool,unsigned char,CRGBColour,float,EEngineInterpolationSet,const CEnginePrimitiveThemeChannelDesc&,bool); //007088f0
};
struct CEnginePrimitiveMorphedAnimatingMesh:CEnginePrimitiveAnimatingMesh {
 CTextureMorphVector TextureMorphs;
 CTextureDecalVector TextureDecals;
 CHiddenMaterialSet HiddenMaterialNames;
 CEnginePrimitiveMorphedAnimatingMesh(unsigned __int64,unsigned long,const CTextureMorphVector&,const CHiddenMaterialSet&,const C3DAnimationInterpolator&,const C3DPositionInterpolator&,const C3DOrientationInterpolator&,float,bool,unsigned char,CRGBColour,float,EEngineInterpolationSet,const CEnginePrimitiveThemeChannelDesc&);
};
CEnginePrimitiveMorphedAnimatingMesh::CEnginePrimitiveMorphedAnimatingMesh(unsigned __int64 id,unsigned long mesh,const CTextureMorphVector& morphs,const CHiddenMaterialSet& hidden,const C3DAnimationInterpolator& animation,const C3DPositionInterpolator& position,const C3DOrientationInterpolator& orientation,float scale,bool dynamic,unsigned char alpha,CRGBColour colour,float drawDistance,EEngineInterpolationSet interpolation,const CEnginePrimitiveThemeChannelDesc& theme)
 :CEnginePrimitiveAnimatingMesh(id,mesh,animation,position,orientation,scale,dynamic,alpha,colour,drawDistance,interpolation,theme,true),TextureMorphs(morphs),TextureDecals(),HiddenMaterialNames(hidden)
{Type=11;}

#include <cstdio>
#include <stddef.h>
struct C3DAnimationInterpolator {int Marker;};
struct C3DPositionInterpolator {int Marker;};
struct C3DOrientationInterpolator {int Marker;};
struct CEnginePrimitiveThemeChannelDesc {int Marker;};
static int failures=0,sequence=0,baseCalls=0,morphCalls=0,setCalls=0;
static const C3DAnimationInterpolator* expectedAnim;
static const C3DPositionInterpolator* expectedPos;
static const C3DOrientationInterpolator* expectedOrient;
static const CEnginePrimitiveThemeChannelDesc* expectedTheme;
static CEnginePrimitiveMorphedAnimatingMesh* current;
static const CTextureMorphVector* expectedMorphs;
static const CHiddenMaterialSet* expectedHidden;
static void check(bool b){if(!b)++failures;}
CEnginePrimitiveAnimatingMesh::CEnginePrimitiveAnimatingMesh(unsigned __int64 id,unsigned long mesh,const C3DAnimationInterpolator& a,const C3DPositionInterpolator& p,const C3DOrientationInterpolator& o,float scale,bool dynamic,unsigned char alpha,CRGBColour colour,float distance,EEngineInterpolationSet interpolation,const CEnginePrimitiveThemeChannelDesc& theme,bool enable)
{
 check(sequence++==0);++baseCalls; current=static_cast<CEnginePrimitiveMorphedAnimatingMesh*>(this);
 check(id==(((unsigned __int64)0x12345678<<32)|0x9abcdef0) && mesh==77);
 check(&a==expectedAnim && &p==expectedPos && &o==expectedOrient && &theme==expectedTheme);
 check(scale==1.25f && dynamic && alpha==173 && distance==8192 && (int)interpolation==3 && enable);
 check(colour.R==0x12 && colour.G==0x34 && colour.B==0x56 && colour.A==0x78);
 Type=99;NativeBaseData[5]=0x6a;
}
CTextureMorphVector::CTextureMorphVector(const CTextureMorphVector& o)
{check(sequence++==1 && &o==expectedMorphs);++morphCalls;First=o.First;Last=o.Last;End=o.End;}
CHiddenMaterialSet::CHiddenMaterialSet(const CHiddenMaterialSet& o)
{
 check(sequence++==2 && &o==expectedHidden);++setCalls;
 check(current->Type==99 && current->NativeBaseData[5]==0x6a);
 check(!current->TextureDecals.First && !current->TextureDecals.Last && !current->TextureDecals.End.Data);
 check(current->TextureMorphs.First==expectedMorphs->First && current->TextureMorphs.Last==expectedMorphs->Last && current->TextureMorphs.End==expectedMorphs->End);
 Header=o.Header;Count=o.Count;
}
int main()
{
#ifdef _MSC_VER
 check(sizeof(CEnginePrimitiveAnimatingMesh)==0x90 && offsetof(CEnginePrimitiveMorphedAnimatingMesh,TextureMorphs)==0x90 && offsetof(CEnginePrimitiveMorphedAnimatingMesh,TextureDecals)==0x9c && offsetof(CEnginePrimitiveMorphedAnimatingMesh,HiddenMaterialNames)==0xa8);
#endif
 C3DAnimationInterpolator a;C3DPositionInterpolator p;C3DOrientationInterpolator o;CEnginePrimitiveThemeChannelDesc theme;
 expectedAnim=&a;expectedPos=&p;expectedOrient=&o;expectedTheme=&theme;
 int entries[4];CTextureMorphVector morphs;morphs.First=entries;morphs.Last=entries+3;morphs.End=entries+4;expectedMorphs=&morphs;
 CHiddenMaterialSet hidden;hidden.Header=entries+2;hidden.Count=17;expectedHidden=&hidden;
 for(int i=0;i<2;++i){
 sequence=0;if(i){morphs.First=morphs.Last=morphs.End=0;hidden.Header=0;hidden.Count=0;}
 CEnginePrimitiveMorphedAnimatingMesh result((((unsigned __int64)0x12345678<<32)|0x9abcdef0),77,morphs,hidden,a,p,o,1.25f,true,173,CRGBColour(0x12,0x34,0x56,0x78),8192,(EEngineInterpolationSet)3,theme);
 check(sequence==3 && result.Type==11 && result.NativeBaseData[5]==0x6a);
 check(result.HiddenMaterialNames.Header==hidden.Header && result.HiddenMaterialNames.Count==hidden.Count);
 check(result.TextureMorphs.First==morphs.First && result.TextureMorphs.Last==morphs.Last && result.TextureMorphs.End==morphs.End);
 }
 check(baseCalls==2 && morphCalls==2 && setCalls==2);
 if(failures){std::printf("00709a50_TEST FAIL %d\n",failures);return 1;}std::printf("00709a50_TEST PASS\n");return 0;
}