// 008091a0 CThingFilter_IsTargetableBehind::operator() -- determine whether a targeting point permits a rear target
// Leaf: ??RCThingFilter_IsTargetableBehind@?A0x
// Flags:
#define CTCTargeted CTCTargeted_Layout
#include "engine/CTCTargeted.h"
#undef CTCTargeted
#ifdef _MSC_VER
#define CThing CThing_Layout
#define CTCGraphicAppearance CGraphicAppearanceLayoutPointer
#include "engine/_quarantine/CThing.h"
#undef CTCGraphicAppearance
#undef CThing
#else
// Layout: the imported native header's unread UID uses an MSVC integer spelling. GNU observes only this152byte opaque raw-layout alias.
class CThing_Layout { unsigned char unobservedNativeThingLayout[0x98]; };
#endif
#define CBaseObject CBaseObject_Layout
#include "engine/CBaseObject.h"
#undef CBaseObject
#define CClass CClass_Layout
#include "engine/CClass.h"
#undef CClass
#define CTCBase CTCBase_Layout
#include "engine/CTCBase.h"
#undef CTCBase
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable:public CBaseClass { protected: virtual ~CBaseClassNonCopyable(); };
class CBaseObject:public CBaseClass {
public:CBaseObjectPointer* IntelligentPointer;
protected:virtual ~CBaseObject();
};
#pragma pack(push,1)
class CClass:public CBaseObject {
public:
    // Layout: no CClass members are read here; preserve the imported17byte prefix.
    unsigned char unobservedClassState[sizeof(CClass_Layout)-sizeof(CBaseObject)];
};
class CTCBase:public CBaseClassNonCopyable {
public:CThing* ParentThing;bool ToBeRemoved;bool IsInGlobalUpdateTCs;unsigned char unobservedTCAlignment[2];
};
#pragma pack(pop)
enum ETCInterfaceType { };
namespace std {
template<class T>class allocator { };
template<class V,class T,class A>class _STLP_alloc_proxy:public A { public:V _M_data; };
template<class T,class A>class _Vector_base { public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage; };
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> { };
template<class K,class V>struct pair { K first;V second; };
template<unsigned long N>struct _Base_bitset { unsigned long _M_w[N]; };
template<unsigned long N>class bitset:public _Base_bitset<(N+31)/32> { };
}
template<class T>class CArray:public std::vector<T> { };
template<class K,class V>class CKeyPairCompareLess { };
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> > {
public:
    C Compare;bool Dirty;
    const std::pair<K,V>* LowerBound(const K&)const;
};
#pragma pack(push,1)
class CThing:public CClass {
public:
    // Layout: unread identity fields extend the prefix to the component bitset at20.
    unsigned char unobservedIdentity[0x20-sizeof(CClass)];
    std::bitset<274> UsingComponents;
    CVectorMap<ETCInterfaceType,CTCBase*> Components;
    // Layout: all original fields beyond the component map are unread.
    unsigned char unobservedThingTail[sizeof(CThing_Layout)-0x54];
    bool HasTC(ETCInterfaceType type)const { if(UsingComponents._M_w[type>>5]&(1UL<<(type&31)))return true;return false; }
    template<class T>__forceinline bool GetTC(ETCInterfaceType type,T** out)const {
        if(HasTC(type)) {
            ETCInterfaceType key=type;
            const CVectorMap<ETCInterfaceType,CTCBase*>& map=Components;
            const std::pair<ETCInterfaceType,CTCBase*>* i=map.LowerBound(key);
            if(i==map._M_finish || type<i->first) i=map._M_finish;
            *out=(T*)i->second;
            return true;
        }
        return false;
    }
};
#pragma pack(pop)
class CBaseIntelligentPointer {
public:
    unsigned char PData[4];
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem()const;
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {
public:T* Get()const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
#pragma pack(push,1)
class C3DVector {
public:float X,Y,Z;C3DVector() { }C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z) { }
};
#pragma pack(pop)
namespace NPlayerTargeting {
class CTargetPoint {
public:CIntelligentPointer<CThing> PThing;long Index;C3DVector TargetingPosition;long FrameRejected;
};
}
class CTCTargeted:public CTCBase {
public:
    unsigned long TargetingFlags;
    // Layout: remaining original targeted-component state is unread.
    unsigned char unobservedTargetedState[8];
    bool IsZTargetable()const { if(TargetingFlags&1) return true;return false; }
};
FABLE_STATIC_ASSERT(sizeof(CTCTargeted)==sizeof(CTCTargeted_Layout));
FABLE_STATIC_ASSERT(sizeof(CThing)==sizeof(CThing_Layout));
namespace NTargeting { bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(const CThing&,const CThing&); }
namespace {
class CThingFilter_IsTargetableBehind {
public:
    const CThing* Targeter;
    const CThing* PTargetedThing;
    CThingFilter_IsTargetableBehind(const CThing& targeter,const CThing* targeted):Targeter(&targeter),PTargetedThing(targeted) { }
    bool operator()(const NPlayerTargeting::CTargetPoint&)const;
};
bool CThingFilter_IsTargetableBehind::operator()(const NPlayerTargeting::CTargetPoint& point)const {
    const CThing* thing=point.PThing.Get();
    if(NTargeting::IsThingVulnerableToMeleeStrikesFromThing(*thing,*Targeter)) return true;
    if(PTargetedThing==thing) return true;
    const CTCTargeted* targeted;
    if(thing->GetTC((ETCInterfaceType)9,&targeted)) {
        if(targeted->IsZTargetable()) return true;
    }
    return false;
}
}