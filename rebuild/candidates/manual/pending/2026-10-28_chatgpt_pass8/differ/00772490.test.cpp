#include <stdio.h>
// Layout: engine/CIVCountedPointeeBase.h flattens the vptr. Retail release reads the count at Object+4 and calls Delete at vtable+4; the local real polymorphic base preserves those offsets and const API.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define NOINLINE __attribute__((noinline))
typedef __SIZE_TYPE__ PlacementSize;
#else
#define NOINLINE __declspec(noinline)
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
class CIVCountedPointeeBase { public: virtual ~CIVCountedPointeeBase() {} virtual void Delete() const; mutable long IVRefCount; void AddRef() const { ++IVRefCount; } void Release() const { if(--IVRefCount==0) Delete(); } };
template<class T> class CDefPointer { public: T* Object;
 T* operator->() const { return Object; }
 CDefPointer() : Object(0) {} CDefPointer(const CDefPointer& other) : Object(other.Object) { if(Object) ((const CIVCountedPointeeBase*)Object)->AddRef(); }
 __forceinline ~CDefPointer() { ReleaseData(); }
 __forceinline void ReleaseData() { if(Object) { ((const CIVCountedPointeeBase*)Object)->Release(); Object=0; } }
 __forceinline CDefPointer& operator=(const T* object) { if(Object!=object) { ReleaseData(); Object=object; if(Object) ((const CIVCountedPointeeBase*)Object)->AddRef(); } return *this; }
 CDefPointer& operator=(const CDefPointer& other) { if(Object!=other.Object) { ReleaseData(); Object=other.Object; if(Object) ((const CIVCountedPointeeBase*)Object)->AddRef(); } return *this; }
};
template<class T> class CIVCountedPointer { public: T* Object;
 CIVCountedPointer() : Object(0) {} CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if(Object) ((const CIVCountedPointeeBase*)Object)->AddRef(); }
 __forceinline ~CIVCountedPointer() { ReleaseData(); }
 __forceinline void ReleaseData() { if(Object) { ((const CIVCountedPointeeBase*)Object)->Release(); Object=0; } }
 CIVCountedPointer& operator=(const CIVCountedPointer& other) { if(Object!=other.Object) { ReleaseData(); Object=other.Object; if(Object) ((const CIVCountedPointeeBase*)Object)->AddRef(); } return *this; }
};
class CThingBaseDef; class CTattooDef; class CMeleeCombatAbilityDef; class CCreatureGenerationFamilyDef; class CTextureReplacementDef; class CManagedTextureData;
namespace NGameText { class CGroupEntry; class CTextEntry; class CDataBankGroupEntry { public: CIVCountedPointer<CGroupEntry> PGroup; }; class CDataBankTextEntry { public: CIVCountedPointer<CTextEntry> PText; long NarratorIndex; }; }
namespace N3DPrimitive { class CTriangleList; }
namespace std {
struct __false_type { __false_type() {} };
struct input_iterator_tag {}; struct forward_iterator_tag:input_iterator_tag {}; struct bidirectional_iterator_tag:forward_iterator_tag {}; struct random_access_iterator_tag:bidirectional_iterator_tag {};
template<class T> class allocator { public: T* allocate(unsigned int n); void deallocate(T*,unsigned int); };
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T> __forceinline void _Destroy(T* p) { p->~T(); }
template<class Iter> void __fastcall __destroy_aux(Iter first,Iter last,const __false_type&) { for(;first!=last;++first) _Destroy(&*first); }
template<class Input,class Output,class Distance> Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*) { for(Distance n=last-first;n>0;--n) { *result=*first; ++first; ++result; } return result; }
template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); }
template<class Input,class Output> inline Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&) { Output current=result; for(;first!=last;++first,++current) _Construct(&*current,*first); return current; }
template<class T,class Alloc=allocator<T> > class vector { public:
 typedef unsigned int size_type; typedef T* pointer; T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage;
 template<class Input> T* _M_allocate_and_copy(size_type n,Input first,Input last);
 void reserve(size_type n);
};
}
class CDefClassBase:public CIVCountedPointeeBase { public: template<class T> bool GetSubDef(CDefPointer<const T>*) const; }; class CThingBaseDef:public CDefClassBase {};
class CShopDef:public CDefClassBase { public: unsigned char _unmodeled08[0x99-8]; bool IsBarberShop,IsTattooShop,IsTitleShop; };
class CDefinitionManager { public: CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long) const; template<class T> NOINLINE bool GetDef(long,CDefPointer<const T>*) const; };
template<class T> NOINLINE bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* out) const { if(index>0) { CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index); if(def.Object) { *out=(const T*)def.Object; return true; } } return false; }
class CGameDefinitionManager:public CDefinitionManager {};
class CThing { public: virtual ~CThing() {} unsigned char _unmodeled04[0x88]; unsigned short DefGlobalIndex; };
class CBaseClass { public: virtual ~CBaseClass() {} }; class CBaseClassNonCopyable:public CBaseClass {};
class CTCBase:public CBaseClassNonCopyable { public: CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; protected: const CGameDefinitionManager& GetDefinitionManager() const; };
class CTCShop:public CTCBase { public: bool IsBarberShop() const; };
bool CTCShop::IsBarberShop() const { CDefPointer<const CThingBaseDef> thingDef; GetDefinitionManager().GetDef(ParentThing->DefGlobalIndex,&thingDef); CDefPointer<const CShopDef> shopDef; return thingDef->GetSubDef(&shopDef) && shopDef->IsBarberShop; }

static int deleted=0;
void CIVCountedPointeeBase::Delete() const { ++deleted; }
class TestPointee : public CIVCountedPointeeBase { public: TestPointee(long count) { IVRefCount=count; } };
static CGameDefinitionManager manager; static CThingBaseDef thingDef; static CShopDef shopDef; static const CTCShop* expected; static bool subResult,nonnull; static int calls,bad;
const CGameDefinitionManager& CTCBase::GetDefinitionManager() const { if(this!=expected || calls++) bad=1; return manager; }
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index) const { if(this!=&manager || index!=319 || calls++!=1) bad=1; CDefPointer<const CDefClassBase> p; p.Object=&thingDef; return p; }
template<class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const { if(this!=&thingDef || out->Object || calls++!=2) bad=1; out->Object=nonnull?(const T*)&shopDef:0; if(nonnull) ++shopDef.IVRefCount; return subResult; }
int main() { CTCShop shop; CThing parent; parent.DefGlobalIndex=319; shop.ParentThing=&parent; expected=&shop;
 for(int i=0;i<4;++i) { bool flag=i==0; subResult=i<2; nonnull=i!=2; shopDef.IsBarberShop=flag; shopDef.IVRefCount=0; thingDef.IVRefCount=1; calls=bad=deleted=0; bool result=shop.IsBarberShop(); if(bad || calls!=3 || result!=(subResult&&flag) || deleted!=(nonnull?2:1) || shopDef.IVRefCount || thingDef.IVRefCount) goto fail; } puts("00772490_TEST PASS"); return 0; fail: puts("00772490_TEST FAIL"); return 1; }
