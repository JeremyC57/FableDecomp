// Behavior-only fixture for the parked source; target and lookup bodies unchanged.
// Unused interfaces and tails are reduced. All receivers, map entries and stream
// boundary objects are constructed C++ objects. Native dispatch/layout remains
// a separate evidence obligation; this fixture makes no byte-match claim.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
#define private public
#define protected public
static int cases,resetCalls,addCalls,ctorCalls,dtorCalls;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CTCParamsBase;class CThing;
enum ETCInterfaceType{};enum EHeroAbility{};
class CBaseClass{public:virtual~CBaseClass(){}};
class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable(){}};
class CTCBase:public CBaseClassNonCopyable{protected:CThing&ParentThing;public:CTCBase(CThing&t):ParentThing(t){}};
namespace std{
template<class T>class allocator{};
template<class A,class B>struct pair{A first;B second;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef T*iterator;iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}};
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{private:C Compare;bool Dirty;public:typedef typename std::vector<std::pair<K,V> >::iterator iterator;iterator LowerBound(const K&);};

#pragma auto_inline(off)
template<class K,class V,class C>typename CVectorMap<K,V,C>::iterator CVectorMap<K,V,C>::LowerBound(const K&key){iterator first=this->begin();long count=this->end()-first;while(count>0){long half=count>>1;iterator middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first;}
#pragma auto_inline(on)

class CTCSummonSpell:public CTCBase{public:CTCSummonSpell(CThing&t):CTCBase(t){}void ResetMinionPos();static ETCInterfaceType __fastcall StatGetInterfaceType(){return static_cast<ETCInterfaceType>(0x91);}};
class CThing{public:unsigned long UsingComponents[8];CVectorMap<ETCInterfaceType,CTCBase*>Components;CTCBase*AddTC(const CCharString&,bool,const CTCParamsBase*);
 bool HasTC(ETCInterfaceType type)const{return (reinterpret_cast<const unsigned long*>(UsingComponents)[static_cast<unsigned int>(type)>>5]&(1UL<<(static_cast<unsigned int>(type)&31)))!=0;}
 __forceinline CTCBase&GetTC(ETCInterfaceType type){CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(type);if(it==map.end()||type<it->first)it=map.end();return *it->second;}
 template<class T>__forceinline bool GetTC(T**out){if(HasTC(T::StatGetInterfaceType())){*out=static_cast<T*>(&GetTC(T::StatGetInterfaceType()));return true;}return false;}

};
class CThingPhysical:public CThing{};class CThingGameObject:public CThingPhysical{};class CThingCreatureBase:public CThingGameObject{};
class CTCSpecialAbilities:public CTCBase{private:CThingCreatureBase&Creature;public:CTCSpecialAbilities(CThingCreatureBase&t):CTCBase(t),Creature(t){}bool SetupSummonSpell();};
bool CTCSpecialAbilities::SetupSummonSpell(){CTCSummonSpell*spell;if(static_cast<CThing&>(Creature).GetTC(&spell)){spell->ResetMinionPos();return true;}{CCharString name("CTCSummonSpell");static_cast<CThing&>(Creature).AddTC(name,true,0);}return true;}

#undef private
#undef protected
static const CTCSummonSpell*expectedSpell;static CThing*expectedCreature;static const CCharString*liveString;
CCharString::CCharString(const char*p,long size):PStringData(0){CHECK(strcmp(p,"CTCSummonSpell")==0&&size==-1);CHECK(!liveString);liveString=this;++ctorCalls;}
CCharString::~CCharString(){CHECK(liveString==this);liveString=0;++dtorCalls;}
void CTCSummonSpell::ResetMinionPos(){CHECK(this==expectedSpell);++resetCalls;}
CTCBase*CThing::AddTC(const CCharString&name,bool create,const CTCParamsBase*params){CHECK(this==expectedCreature&&&name==liveString&&create&&params==0);++addCalls;return const_cast<CTCSummonSpell*>(expectedSpell);}
int main(){
 for(int n=1;n<=9;++n)for(int place=0;place<n;++place)for(int has=0;has<2;++has)for(int bits=0;bits<4;++bits)for(int seed=0;seed<3;++seed){
  CThingCreatureBase creature;CTCSummonSpell spell(creature),other(creature);CTCSpecialAbilities abilities(creature);
  std::pair<ETCInterfaceType,CTCBase*>entries[9];
  for(int i=0;i<8;++i)creature.UsingComponents[i]=0;
  creature.UsingComponents[0]=(unsigned long)(bits+seed*16);creature.UsingComponents[4]=(has?0x20000UL:0UL)|(unsigned long)(bits<<2);
  for(int j=0;j<n;++j){entries[j].first=(ETCInterfaceType)(0x91+j-place);entries[j].second=j==place?static_cast<CTCBase*>(&spell):static_cast<CTCBase*>(&other);}
  creature.Components._M_start=entries;creature.Components._M_finish=entries+n;creature.Components._M_end_of_storage._M_data=entries+n;
  expectedSpell=&spell;expectedCreature=&creature;liveString=0;resetCalls=addCalls=ctorCalls=dtorCalls=0;
  CHECK(abilities.SetupSummonSpell());
  CHECK(resetCalls==has&&addCalls==1-has&&ctorCalls==1-has&&dtorCalls==1-has&&liveString==0);
  CHECK(creature.UsingComponents[4]==((has?0x20000UL:0UL)|(unsigned long)(bits<<2)));
  for(int k=0;k<n;++k)CHECK(entries[k].first==(ETCInterfaceType)(0x91+k-place)&&entries[k].second==(k==place?static_cast<CTCBase*>(&spell):static_cast<CTCBase*>(&other)));
  ++cases;
 }
 CHECK(cases==1080);puts("0072b330_TEST PASS");return 0;
}
