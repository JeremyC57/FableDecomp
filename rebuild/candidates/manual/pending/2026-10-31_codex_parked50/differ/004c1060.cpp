// 004c1060 NGraphicAppearance::CTCGraphicAppearance::GetFocalPos
// Codex parked-50 best attempt: DIFFER(126v126); residue: 126/126 bytes; GetHelperPoint OUT-address scratch register and vector-copy load/store scheduling differ.
// Flags:
// Leaf: ?GetFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@M@Z
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
#include "engine/CBaseClassNonCopyable.h"
class CPersistContext;class CTCParamsBase;class CEnginePrimitivePrepareInfo;
class CTCBase : public CBaseClassNonCopyable {
protected:
    CThing& ParentThing;
private:
    bool ToBeRemoved,IsInGlobalUpdateTCs;
    // Real virtual prefix through slot12 Copy. Later unused slots are omitted from this projection.
protected:
    virtual void OnSerialise(CPersistContext&,bool); // original protected virtual slot1
public:
    virtual void OnRemove();
    virtual void ConstructFromParams(const CTCParamsBase&);
    virtual void OnCreate();
    virtual void InitialActivate();
    virtual void OnInitialActivate();
    virtual void RenderUpdate(float);
    virtual bool HasFrameUpdate() const;
    virtual void FrameUpdate();
    virtual void BulletTimeFrameUpdate();
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    virtual void Copy(const CTCBase&);
};
FABLE_STATIC_ASSERT(sizeof(CTCBase)==sizeof(CTCBaseLayout));
#pragma pack(push,2)
class CEngineGraphic {
public:
    unsigned long BankIndex;float AnimStep,RenderSizeX;bool AdditiveAlpha;unsigned char Type;
};
#pragma pack(pop)
#pragma pack(push,1) // Original vector alignment: CProng::Prong+1 and pair<C3DVector,bool>13; see packing evidence ledger.
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}const C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);} };
#pragma pack(pop)
class CTCAnimationBase;
class CRGBColour {public:unsigned char B,G,R,A;};
class CEnginePrimitiveThemeChannelDesc {public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;};
namespace std {
template<class T> class allocator {};
template<class V,class T,class A> class _STLP_alloc_proxy : public A {public:V _M_data;};
template<class A,class B> struct pair {typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A& a,const B& b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>& p):first(p.first),second(p.second){}pair(const pair<A,B>& p):first(p.first),second(p.second){} };
template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};
template<class A,class B,class R>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:public binary_function<T,T,bool> {bool operator()(const T& x,const T& y)const{return x<y;}};
template<class T>struct _Select1st:public unary_function<T,typename T::first_type> {const typename T::first_type& operator()(const T& x)const{return x.first;}};
struct _Rb_tree_node_base {bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};
template<class T> struct _Rb_tree_node : public _Rb_tree_node_base {T _M_value_field;};
template<class T,class A> struct _Rb_tree_base {protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > > _M_header;};
template<class K,class V,class Select,class Compare,class A> class _Rb_tree : public _Rb_tree_base<V,A> {protected:unsigned int _M_node_count;Compare _M_key_compare;};
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > > class map {private:_Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,Compare,A> _M_t;};
struct _List_node_base {_List_node_base* _M_next;_List_node_base* _M_prev;};
template<class T> struct _List_node : public _List_node_base {T _M_data;};
template<class T,class A> class _List_base {public:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > > _M_node;};
template<class T,class A=allocator<T> > class list : public _List_base<T,A> {};
}
class CCharString;
extern const CCharString MAIN_FOCAL_POSITION_NAME;
extern const CCharString TARGETING_CURSOR_POSITION_NAME;
namespace {C3DVector __fastcall GetRenderPositionForThing(const CThing&,float);} // original anonymous namespace; compiler-generated hash is not a recovered name
namespace NGraphicAppearance {
class CPrimitive;class CAGraphicAppearanceComponent;
class CTCGraphicAppearance : public CTCBase {
public:
    struct CMeshEffectPriorityEntry;
    std::map<void*,CMeshEffectPriorityEntry> MeshEffects;
    std::map<void*,CRGBColour> TintColours;
    CTCAnimationBase* PTCAnimation;
    C3DVector CachedStaticFocalPosOffset;
    float Scale;
    std::list<CPrimitive> Primitives;
    std::list<CAGraphicAppearanceComponent*> Components;
    CEnginePrimitiveThemeChannelDesc LightingChannel;
    CEngineGraphic MainGraphic;
    unsigned short ActivePersistenceFlags;
    long FrameUpdatedWasRenderedFlag;
    unsigned char UpdatePrimitiveReasons;
    unsigned char SelfIlluminating:1;
    unsigned char ForceResendEveryRenderFrame:1;
    unsigned char ResendPrimsEveryRenderFrame:1;
    unsigned char DisableOcclusionTesting:1;
    unsigned char DrawAfterWater:1;
    unsigned char CachedStaticFocalPos:1;
    unsigned char RenderedLastFrame:1;
    unsigned char PrimitivesDrawn:1;
    unsigned char Alpha;
    signed char ByPassColorFilter;
    bool GetHelperPoint(const CCharString&,long,unsigned long,C3DVector&,float) const;
    C3DVector GetFocalPos(float) const;
    C3DVector GetStaticFocalPos() const;
    virtual void Copy(const CTCBase&);
};
// Fidelity: analysis-only non-escaping stand-in for the existing helper symbol.
// It reads each input and writes only the output vector; no helper row is claimed.
#pragma auto_inline(off)
bool CTCGraphicAppearance::GetHelperPoint(const CCharString&name,long index,unsigned long flags,C3DVector&out,float time)const {
 if(*(const void*const*)&name==0 || &ParentThing==0) return false;
 out=GetRenderPositionForThing(ParentThing,time);
 out.X+=(float)index;out.Y+=(float)flags;
 return true;
}
#pragma auto_inline(on)
C3DVector CTCGraphicAppearance::GetFocalPos(float time) const
{
 C3DVector pos;
 if(!GetHelperPoint(MAIN_FOCAL_POSITION_NAME,0,0,pos,time) && !GetHelperPoint(TARGETING_CURSOR_POSITION_NAME,0,0,pos,time))
  pos=GetRenderPositionForThing(ParentThing,time);
 return pos;
}
}
