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