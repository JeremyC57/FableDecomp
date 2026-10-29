#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
#include <string.h>
extern "C" int puts(const char*);
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
class CBaseClass { public: virtual ~CBaseClass(); };
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

static bool fixture_vulnerable=false;
static int fixture_get_calls=0,fixture_vulnerability_calls=0,fixture_lookup_calls=0;
static const CThing* fixture_seen_thing=0;
static const CThing* fixture_seen_targeter=0;
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CBaseObject::~CBaseObject() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() { CBaseObject* p=0;memcpy(PData,&p,sizeof(p)); }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CBaseObject* CBaseIntelligentPointer::GetPItem()const { ++fixture_get_calls;CBaseObject* p;memcpy(&p,PData,sizeof(p));return p; }
namespace NTargeting {
bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(const CThing& thing,const CThing& targeter) {
    ++fixture_vulnerability_calls;fixture_seen_thing=&thing;fixture_seen_targeter=&targeter;return fixture_vulnerable;
}
}
template<class K,class V,class C> const std::pair<K,V>* CVectorMap<K,V,C>::LowerBound(const K& key)const {
    ++fixture_lookup_calls;
    const std::pair<K,V>* p=this->_M_start;
    while(p!=this->_M_finish && p->first<key)++p;
    return p;
}
int main() {
    if(sizeof(NPlayerTargeting::CTargetPoint)!=28 || sizeof(CBaseIntelligentPointer)!=8 || sizeof(CTCTargeted)!=24)return 1;
    CThing thing,targeter,other;
    for(int i=0;i<9;++i){thing.UsingComponents._M_w[i]=0;targeter.UsingComponents._M_w[i]=0;other.UsingComponents._M_w[i]=0;}
    CTCTargeted targeted;
    std::pair<ETCInterfaceType,CTCBase*> entries[3];
    entries[0].first=(ETCInterfaceType)8;entries[0].second=0;
    entries[1].first=(ETCInterfaceType)9;entries[1].second=&targeted;
    entries[2].first=(ETCInterfaceType)10;entries[2].second=0;
    thing.Components._M_start=entries;thing.Components._M_finish=entries+2;thing.Components._M_end_of_storage._M_data=entries+3;
    NPlayerTargeting::CTargetPoint point;
    CBaseObject* base=&thing;memcpy(point.PThing.PData,&base,sizeof(base));
    point.Index=-17;point.TargetingPosition.X=1;point.TargetingPosition.Y=2;point.TargetingPosition.Z=3;point.FrameRejected=91;
    for(int vulnerable=0;vulnerable<2;++vulnerable)for(int matched=0;matched<2;++matched)for(int present=0;present<2;++present)for(unsigned long flags=0;flags<8;++flags) {
        fixture_vulnerable=vulnerable!=0;
        CThingFilter_IsTargetableBehind predicate(targeter,matched?&thing:&other);
        thing.UsingComponents._M_w[0]=present?(1UL<<9):0;
        targeted.TargetingFlags=flags;
        fixture_get_calls=0;fixture_vulnerability_calls=0;fixture_lookup_calls=0;
        bool result=predicate(point);
        bool expected=vulnerable || matched || (present && (flags&1));
        if(result!=expected || fixture_get_calls!=1 || fixture_vulnerability_calls!=1)return 2;
        if(fixture_lookup_calls!=(!vulnerable && !matched && present?1:0))return 3;
        if(fixture_seen_thing!=&thing || fixture_seen_targeter!=&targeter)return 4;
        if(point.Index!=-17 || point.TargetingPosition.X!=1 || point.TargetingPosition.Y!=2 || point.TargetingPosition.Z!=3 || point.FrameRejected!=91)return 5;
        if(targeted.TargetingFlags!=flags || predicate.Targeter!=&targeter || predicate.PTargetedThing!=(matched?&thing:&other))return 6;
    }
    puts("008091a0_TEST PASS");return 0;
}