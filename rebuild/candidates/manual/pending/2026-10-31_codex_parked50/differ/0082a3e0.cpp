// 0082a3e0 CMiniMapDisplay::CMarkerInfo::IsReversedOnMap -- read the marked entrance's minimap orientation
// Codex parked-50 best attempt: DIFFER(118v118); residue: 118/118 bytes; map/key address formation order and final component pointer scratch register differ.
// Leaf: ?IsReversedOnMap@CMarkerInfo@CMiniMapDisplay@@QBE_NXZ
// Flags:
#ifdef _MSC_VER
#define CThing CThing_Layout
#define CTCGraphicAppearance CGraphicAppearanceLayoutPointer
#include "engine/_quarantine/CThing.h"
#undef CTCGraphicAppearance
#undef CThing
#else
// Layout: GNU observes the opaque152byte raw-layout alias; the unused native UID spelling is compiler-specific.
class CThing_Layout { unsigned char unobservedNativeThingLayout[0x98]; };
#endif
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define CClass CClass_Layout
#include "engine/CClass.h"
#undef CClass
#define CTCBase CTCBase_Layout
#include "engine/CTCBase.h"
#undef CTCBase
#define CMiniMapDisplay CMiniMapDisplay_Layout
#include "engine/CMiniMapDisplay.h"
#undef CMiniMapDisplay
#include "engine/CBaseClassNonCopyable.h"
#pragma pack(push,1)
class CClass:public CBaseObject {
public:
    // Layout: no CClass members are read here; preserve the imported17byte prefix.
    unsigned char unobservedClassState[sizeof(CClass_Layout)-sizeof(CBaseObject)];
};
class CTCBase:public CBaseClassNonCopyable {
protected:CThing&ParentThing;private:bool ToBeRemoved;bool IsInGlobalUpdateTCs;unsigned char unobservedTCAlignment[2];
};
class CTCDriverBase:public CTCBase { };
class CTCDRegionExit:public CTCDriverBase {
public:
    // Layout: original fields before ReversedOnMiniMap are not read.
    unsigned char unobservedExitState[0x28-sizeof(CTCDriverBase)];
private:bool ReversedOnMiniMap;
public:bool IsReversedOnMiniMap()const{return ReversedOnMiniMap;}
};
class CTCActionUseScriptedHook:public CTCBase {
public:
    // Layout: original fields before ReversedOnMiniMap are not read.
    unsigned char unobservedHookState[0x4c-sizeof(CTCBase)];
private:bool ReversedOnMiniMap;
public:bool IsReversedOnMiniMap()const{return ReversedOnMiniMap;}
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
private:
    C Compare;bool Dirty;
public:
    const std::pair<K,V>* LowerBound(const K&)const;
    const std::pair<K,V>* Find(const K& key)const {
        const std::pair<K,V>* i=LowerBound(key);
        if(i==this->_M_finish || key<i->first)return this->_M_finish;
        return i;
    }
};
// Fidelity: explicit specialization is visible before the caller, unlike a delayed template body.
template<> __declspec(noinline) const std::pair<ETCInterfaceType,CTCBase*>*
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType&key)const{
 long count=this->_M_finish-this->_M_start;
 const std::pair<ETCInterfaceType,CTCBase*>*first=this->_M_start;
 while(count>0){long half=count/2;const std::pair<ETCInterfaceType,CTCBase*>*middle=first+half;
  if(middle->first<key){first=middle+1;count-=half+1;}else count=half;
 }
 return first;
}
#pragma pack(push,1)
class CThing:public CClass {
protected:
    // Layout: unread identity fields extend the prefix to the component bitset at20.
    unsigned char unobservedIdentity[0x20-sizeof(CClass)];
    std::bitset<274> UsingComponents;
    CVectorMap<ETCInterfaceType,CTCBase*> Components;
    unsigned char unobservedComponentLists[8];
    CTCDriverBase* PTCDriver;
    unsigned char unobservedThingState[0x91-0x60];
    unsigned char Dead:1;
    unsigned char unobservedThingFlags:7;
    // Layout: remaining original tail state is unread.
    unsigned char unobservedThingTail[sizeof(CThing_Layout)-0x92];
public:
    bool IsDead()const{return Dead;}
    const CTCDriverBase&GetTCDriver()const{return *PTCDriver;}
    bool HasTC(ETCInterfaceType type)const { if(UsingComponents._M_w[type>>5]&(1UL<<(type&31)))return true;return false; }
    __forceinline const CTCBase& GetTC(ETCInterfaceType type)const {return *Components.Find(type)->second;}

};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CThing)==sizeof(CThing_Layout));
class CBaseIntelligentPointer {
public:unsigned char PData[4];virtual ~CBaseIntelligentPointer();CBaseObject* GetPItem()const;
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {
public:T* Get()const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CMiniMapDisplay:public CBaseClassNonCopyable {
public:
    // Layout: the selected nested-class method never reads its outer object.
    unsigned char unobservedDisplayState[sizeof(CMiniMapDisplay_Layout)-sizeof(CBaseClassNonCopyable)];
    class CMarkerInfo {
    public:
        CIntelligentPointer<const CThing> PThing;
        long GraphicIndex;bool AutoMarker;
        bool IsReversedOnMap()const;
    };
};
bool CMiniMapDisplay::CMarkerInfo::IsReversedOnMap()const {
    if(PThing.Get() && !PThing.Get()->IsDead()) {
        const CThing* driven=PThing.Get();
        if(driven->HasTC((ETCInterfaceType)0x64))
            return static_cast<const CTCDRegionExit&>(driven->GetTCDriver()).IsReversedOnMiniMap();
        const CThing* thing=PThing.Get();
        if(thing->HasTC((ETCInterfaceType)0xc2))
            return static_cast<const CTCActionUseScriptedHook&>(thing->GetTC((ETCInterfaceType)0xc2)).IsReversedOnMiniMap();
    }
    return false;
}
