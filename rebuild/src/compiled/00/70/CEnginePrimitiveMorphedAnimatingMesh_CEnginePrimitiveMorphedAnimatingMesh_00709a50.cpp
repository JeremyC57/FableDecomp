// CEnginePrimitiveMorphedAnimatingMesh::CEnginePrimitiveMorphedAnimatingMesh -- forwards animating-mesh settings, copies texture morphs and hidden materials, and starts an empty decal vector. Retail00709a50,161bytes, complete ret60. No shared class header exists; donor vectors16byte conflict with retail12byte members (morph90,decals9c,seta8). Base size90 from native ctor and landed destructor00709b10. Callees007088f0,005bbdf1,00514400. PDB signature independently identified in editor symbols; native final base boolean forcedtrue.
// Leaf: ??0CEnginePrimitiveMorphedAnimatingMesh
#ifndef _MSC_VER
#define __int64 long long
#endif
class C3DAnimationInterpolator;
class C3DPositionInterpolator;
class C3DOrientationInterpolator;
class CEnginePrimitiveThemeChannelDesc;
enum EEngineInterpolationSet {INTERPOLATION_DEFAULT=0};
class CRGBColour { public:
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