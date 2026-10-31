// 007e0d00 CTCEffectOnDie::OnDie
// Codex parked-50 best attempt: DIFFER(112v112); residue: 112/112 bytes; actual emitter Create ABI and bit-test shape restored, but sound receiver load and LEA scratch-register order differ.
// Flags:
// Leaf: ?OnDie@CTCEffectOnDie@@UAEXXZ
#define CThing CThingLayout
#define CTCGraphicAppearance CTCGraphicAppearanceLayout
#include "engine/_quarantine/CThing.h"
#undef CTCGraphicAppearance
#undef CThing
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
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
    std::pair<K,V>* LowerBound(const K&);
    std::pair<K,V>* Find(const K& key) {
        std::pair<K,V>* i=LowerBound(key);
        if(i==this->_M_finish || key<i->first)return this->_M_finish;
        return i;
    }
};
template<> __declspec(noinline) std::pair<ETCInterfaceType,CTCBase*>*
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType&key){
 long count=this->_M_finish-this->_M_start;
 std::pair<ETCInterfaceType,CTCBase*>*first=this->_M_start;
 while(count>0){long half=count/2;std::pair<ETCInterfaceType,CTCBase*>*middle=first+half;
  if(middle->first<key){first=middle+1;count-=half+1;}else count=half;
 }
 return first;
}

#pragma pack(push,1)
class CClass:public CBaseObject {unsigned char unreadClass[sizeof(CClassLayout)-sizeof(CBaseObject)];};
class C3DVector;
// Layout: imported CThing native bitset20/map44; unread remainder preserves extent.
class CThing:public CClass {
 unsigned char unreadIdentity[0x20-sizeof(CClass)];std::bitset<274>UsingComponents;
 CVectorMap<ETCInterfaceType,CTCBase*>Components;
 unsigned char unreadThingTail[sizeof(CThingLayout)-0x54];
public:
 bool HasTC(ETCInterfaceType t)const{if(UsingComponents._M_w[t>>5]&(1UL<<(t&31)))return true;return false;}
 __forceinline CTCBase&GetTC(ETCInterfaceType t){return *Components.Find(t)->second;}
 const C3DVector&GetPos()const;
};
#pragma pack(pop)
class CCharStringData;
class CCharString {CCharStringData*PStringData;};
// Layout: original sound component36; private LastMaterialIndex32.
class CTCSoundPlayer:public CTCBase {
 unsigned char unreadSound[0x20-sizeof(CTCBase)];long LastMaterialIndex;
public:unsigned long PlayAttachedCriteria(const CCharString&,bool,bool,bool,long);
};
class CTCDParticleEmitter:public CTCBase {
public:static CThing*__fastcall Create(long,const C3DVector&,bool);
};
// Layout: original fields and native offsets c/10/14/18/1c/20/21.
class CTCEffectOnDie:public CTCBase {
 bool StartedNonRecoverableFadeout;long Effect,Timer,FadeOutTimer;CCharString DeathSoundCriteria;bool Pausable,Paused;
public:virtual void OnDie();
};
void CTCEffectOnDie::OnDie(){
 if(!Paused){
  if(ParentThing.HasTC((ETCInterfaceType)16)){
   static_cast<CTCSoundPlayer&>(ParentThing.GetTC((ETCInterfaceType)16)).PlayAttachedCriteria(DeathSoundCriteria,true,false,false,-1);
  }
  if(Effect)CTCDParticleEmitter::Create(Effect,ParentThing.GetPos(),false);
 }
}
