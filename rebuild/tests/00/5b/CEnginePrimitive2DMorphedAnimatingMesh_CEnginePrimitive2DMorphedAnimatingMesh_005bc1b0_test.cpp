#include <stdio.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
// Original morphing mesh descriptor: true primitive hierarchy and container types.
// Leaf: ??0CEnginePrimitive2DMorphedAnimatingMesh@@QAE@_KKABV?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@ABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@2@ABVC3DAnimationBlendState@@ABVC2DBoxF@@ABVC3DBoundingSphere@@ABVC3DOrientationInterpolator@@W4EEngineInterpolationSet@@EVCRGBColour@@@Z
// Flags: /Os /Oy-
#include <stddef.h>
#include "rebuild_abi.h"
#ifdef _MSC_VER
typedef unsigned __int64 PrimitiveIdentifier;
#else
typedef unsigned int PrimitiveIdentifier __attribute__((mode(DI)));
#endif
#pragma pack(push,1)
class C2DVector {public:float X,Y;};
class C3DVector {public:float X,Y,Z;};
#pragma pack(pop)
class C2DBoxF {public:float TLX,TLY,BRX,BRY;};
class C3DBoundingSphere {public:C3DVector Center;float Radius;};
class CRightHandedSet {public:C3DVector Up,Forward;};
class CBaseClass {public:virtual~CBaseClass();};
class CIVCountedPointeeBase;
template<class T>class CIVCountedPointer {public:CIVCountedPointeeBase*Object;~CIVCountedPointer();};
class C3DAnimationBlendState:public CBaseClass {public:class CSharedData;CIVCountedPointer<CSharedData>Data;virtual~C3DAnimationBlendState();};
class C3DOrientationInterpolator:public CBaseClass {public:CRightHandedSet StartOrientation,EndOrientation;virtual~C3DOrientationInterpolator();};
class C3DAnimationInterpolator:public CBaseClass {public:C3DAnimationBlendState StartState,EndState;virtual~C3DAnimationInterpolator();};
class CCharStringData;class CCharString {public:CCharStringData*PStringData;};
// Accessed original anonymous component fields; overlapping union aliases are
// unexported and explicitly omitted, as in independently audited text descriptor.
class CRGBColour {public:unsigned char B,G,R,A;__forceinline void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a){R=r;G=g;B=b;A=a;}CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a){Initialise(r,g,b,a);}__forceinline CRGBColour(const CRGBColour&x){Initialise(x.R,x.G,x.B,x.A);}};
enum EEnginePrimitiveType {};enum EEngineInterpolationSet {};enum ECompositeBlendType {};
class CEnginePrimitiveThemeChannelDesc {public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){}};
class CEnginePrimitive {public:EEnginePrimitiveType Type;EEngineInterpolationSet InterpolationSet;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;};
namespace std {
typedef ::size_t size_t;
template<class T>class allocator {public:typedef T value_type;template<class U>struct rebind{typedef allocator<U>other;};};
template<class T,class A>struct _Alloc_traits {typedef A _Orig;typedef A allocator_type;};
template<class T,class U>struct _Alloc_traits<T,allocator<U> >{typedef allocator<U>_Orig;typedef allocator<T>allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class T,class A>class _Vector_base {public:typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,allocator_type>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:vector(const vector&);~vector();};
template<class A,class B,class R>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};
template<class T>struct less:public binary_function<T,T,bool>{};template<class T>struct _Identity:public unary_function<T,T>{};
struct _Rb_tree_node_base {bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base {V _M_value_field;};
template<class V,class A>struct _Rb_tree_base {protected:typedef typename _Alloc_traits<_Rb_tree_node<V>,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,_M_node_allocator_type>_M_header;};
template<class K,class V,class S,class C,class A>class _Rb_tree:public _Rb_tree_base<V,A> {protected:size_t _M_node_count;C _M_key_compare;};
template<class K,class C=less<K>,class A=allocator<K> >class set {private:typedef _Rb_tree<K,K,_Identity<K>,C,A>_Rep_type;_Rep_type _M_t;public:set(const set&);~set();};
}
class CCompositeBlendDesc {public:unsigned long TextureIndex;ECompositeBlendType BlendType;CRGBColour DiffuseColour,SpecularColour;};
class CEngineTextureMorph {public:long OriginalTextureIndex,MorphID;std::vector<CCompositeBlendDesc>Blends;};
class CEnginePrimitive2DAnimatingMesh:public CEnginePrimitive {public:C2DBoxF ClipBox;unsigned long MeshIndex;unsigned char Alpha;C3DOrientationInterpolator Orientation;C3DAnimationInterpolator Anim;C3DBoundingSphere BoundingSphere;CRGBColour TintColour;~CEnginePrimitive2DAnimatingMesh();CEnginePrimitive2DAnimatingMesh(PrimitiveIdentifier,unsigned long,const C3DAnimationBlendState&,const C2DBoxF&,const C3DBoundingSphere&,const C3DOrientationInterpolator&,EEngineInterpolationSet,unsigned char,CRGBColour);};
class CEnginePrimitive2DMorphedAnimatingMesh:public CEnginePrimitive2DAnimatingMesh {public:std::vector<CEngineTextureMorph>TextureMorphs;std::set<CCharString>HiddenMaterialNames;CEnginePrimitive2DMorphedAnimatingMesh(PrimitiveIdentifier,unsigned long,const std::vector<CEngineTextureMorph>&,const std::set<CCharString>&,const C3DAnimationBlendState&,const C2DBoxF&,const C3DBoundingSphere&,const C3DOrientationInterpolator&,EEngineInterpolationSet,unsigned char,CRGBColour);};
FABLE_STATIC_ASSERT(sizeof(C3DOrientationInterpolator)==52);FABLE_STATIC_ASSERT(sizeof(C3DAnimationInterpolator)==20);FABLE_STATIC_ASSERT(sizeof(C3DAnimationBlendState)==8);FABLE_STATIC_ASSERT(sizeof(CEngineTextureMorph)==20);FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive2DAnimatingMesh)==128);FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive2DMorphedAnimatingMesh)==152);FABLE_STATIC_ASSERT(sizeof(std::set<CCharString>)==12);
CEnginePrimitive2DMorphedAnimatingMesh::CEnginePrimitive2DMorphedAnimatingMesh(PrimitiveIdentifier id,unsigned long mesh,const std::vector<CEngineTextureMorph>&morphs,const std::set<CCharString>&hidden,const C3DAnimationBlendState&anim,const C2DBoxF&box,const C3DBoundingSphere&sphere,const C3DOrientationInterpolator&orientation,EEngineInterpolationSet interpolation,unsigned char alpha,CRGBColour colour):CEnginePrimitive2DAnimatingMesh(id,mesh,anim,box,sphere,orientation,interpolation,alpha,colour),TextureMorphs(morphs),HiddenMaterialNames(hidden){Type=static_cast<EEnginePrimitiveType>(0x26);}

static PrimitiveIdentifier expectedID;static unsigned long expectedMesh;static const C3DAnimationBlendState*expectedAnim;static const C2DBoxF*expectedBox;static const C3DBoundingSphere*expectedSphere;static const C3DOrientationInterpolator*expectedOrientation;static EEngineInterpolationSet expectedSet;static unsigned char expectedAlpha;static unsigned char expectedColour[4];static const void*expectedMorphs,*expectedHidden;static void*expectedDestination;static int calls;static bool bad;
CBaseClass::~CBaseClass(){}
C3DAnimationBlendState::~C3DAnimationBlendState(){}
C3DOrientationInterpolator::~C3DOrientationInterpolator(){}
C3DAnimationInterpolator::~C3DAnimationInterpolator(){}
CEnginePrimitive2DAnimatingMesh::~CEnginePrimitive2DAnimatingMesh(){}
template<class T>CIVCountedPointer<T>::~CIVCountedPointer(){}
CEnginePrimitive2DAnimatingMesh::CEnginePrimitive2DAnimatingMesh(PrimitiveIdentifier id,unsigned long mesh,const C3DAnimationBlendState&anim,const C2DBoxF&box,const C3DBoundingSphere&sphere,const C3DOrientationInterpolator&orientation,EEngineInterpolationSet interpolation,unsigned char alpha,CRGBColour colour):TintColour(colour){if(calls++||id!=expectedID||mesh!=expectedMesh||&anim!=expectedAnim||&box!=expectedBox||&sphere!=expectedSphere||&orientation!=expectedOrientation||interpolation!=expectedSet||alpha!=expectedAlpha||memcmp(&colour,expectedColour,4)||this!=expectedDestination)bad=true;Type=static_cast<EEnginePrimitiveType>(0x25);InterpolationSet=interpolation;ClipBox=box;MeshIndex=mesh;Alpha=alpha;Orientation.StartOrientation=orientation.StartOrientation;Orientation.EndOrientation=orientation.EndOrientation;BoundingSphere=sphere;}
namespace std {
template<class T,class A>vector<T,A>::~vector(){}
template<class K,class C,class A>set<K,C,A>::~set(){}
template<class T,class A>vector<T,A>::vector(const vector&x){if(calls++!=1||&x!=expectedMorphs||reinterpret_cast<unsigned char*>(this)!=reinterpret_cast<unsigned char*>(expectedDestination)+128)bad=true;memcpy(this,&x,sizeof x);}
template<class K,class C,class A>set<K,C,A>::set(const set&x){if(calls++!=2||&x!=expectedHidden||reinterpret_cast<unsigned char*>(this)!=reinterpret_cast<unsigned char*>(expectedDestination)+140)bad=true;memcpy(this,&x,sizeof x);}
}
// Library constructor definitions above are explicit dependency mocks; the
// fixture executes the actual descriptor constructor and typed argument copies.
int main(){union MorphInput{void*align;unsigned char bytes[12];}morph;union HiddenInput{void*align;unsigned char bytes[12];}hidden;memset(&morph,0x31,sizeof morph);memset(&hidden,0x72,sizeof hidden);const std::vector<CEngineTextureMorph>&mv=*reinterpret_cast<const std::vector<CEngineTextureMorph>*>(morph.bytes);const std::set<CCharString>&hv=*reinterpret_cast<const std::set<CCharString>*>(hidden.bytes);expectedMorphs=&mv;expectedHidden=&hv;C3DAnimationBlendState anim;C2DBoxF box;box.TLX=1;box.TLY=-2;box.BRX=3;box.BRY=4;C3DBoundingSphere sphere;sphere.Center.X=5;sphere.Center.Y=6;sphere.Center.Z=7;sphere.Radius=8;C3DOrientationInterpolator orientation;memset(&orientation.StartOrientation,0x47,48);expectedAnim=&anim;expectedBox=&box;expectedSphere=&sphere;expectedOrientation=&orientation;expectedMesh=0x879abcde;expectedSet=static_cast<EEngineInterpolationSet>(3);expectedAlpha=0xbc;expectedID=static_cast<PrimitiveIdentifier>(0x12345678UL)*static_cast<PrimitiveIdentifier>(0x10000UL)*static_cast<PrimitiveIdentifier>(0x10000UL)+0x9abcdef0UL;CRGBColour colour(0x11,0x22,0x33,0x44);memcpy(expectedColour,&colour,4);calls=0;bad=false;{union Destination{double align;unsigned char bytes[152];}storage;expectedDestination=storage.bytes;CEnginePrimitive2DMorphedAnimatingMesh&object=*new(static_cast<void*>(storage.bytes))CEnginePrimitive2DMorphedAnimatingMesh(expectedID,expectedMesh,mv,hv,anim,box,sphere,orientation,expectedSet,expectedAlpha,colour);if(bad||calls!=3||static_cast<unsigned>(object.Type)!=0x26||object.InterpolationSet!=expectedSet||object.MeshIndex!=expectedMesh||object.Alpha!=expectedAlpha||memcmp(&object.TintColour,&colour,4)||memcmp(&object.TextureMorphs,&mv,12)||memcmp(&object.HiddenMaterialNames,&hv,12)||memcmp(&object.ClipBox,&box,16)||memcmp(&object.BoundingSphere,&sphere,16))return 1;}puts("005bc1b0_TEST PASS");return 0;}