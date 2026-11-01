// 00701750 COpinionReactionManager::GetThingOpinionOfPlayer
// Leaf: ?GetThingOpinionOfPlayer@COpinionReactionManager@@QBEMABVCThing@@W4EOpinion@@@Z
// Flags:
#include <stddef.h>
extern "C" void*__cdecl malloc(size_t);extern "C" void __cdecl free(void*);
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
#define CPlayer CPlayerFields
#include "engine/CPlayer.h"
#undef CPlayer
namespace std{
template<class T>class allocator{public:T*allocate(size_t n){return static_cast<T*>(malloc(n*sizeof(T)));}void deallocate(T*p,size_t){free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class Out,class N,class T>inline Out fill_n(Out first,N n,const T&v){for(;n>0;--n,++first)*first=v;return first;}
struct __true_type{};
template<class I,class N,class T>inline I __uninitialized_fill_n(I first,N n,const T&v,const __true_type&){return fill_n(first,n,v);}
template<class I,class N,class T>inline I uninitialized_fill_n(I first,N n,const T&v){return __uninitialized_fill_n(first,n,v,__true_type());}
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:explicit vector(size_t n):_Vector_base<T,A>(n,A()){this->_M_finish=uninitialized_fill_n(this->_M_start,n,T());}vector&operator=(const vector&x){if(this!=&x){size_t n=x._M_finish-x._M_start;if(n>size_t(this->_M_end_of_storage._M_data-this->_M_start)){T*tmp=this->_M_end_of_storage.allocate(n);for(size_t i=0;i<n;++i)tmp[i]=x._M_start[i];if(this->_M_start)this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);this->_M_start=tmp;this->_M_end_of_storage._M_data=tmp+n;}else{for(size_t i=0;i<n;++i)this->_M_start[i]=x._M_start[i];}this->_M_finish=this->_M_start+n;}return *this;}const T&operator[](size_t n)const{return this->_M_start[n];}const T*begin()const{return this->_M_start;}const T*end()const{return this->_M_finish;}};
template<class A,class B>struct pair{A first;B second;};
}
template<class T>class CArray:public std::vector<T>{public:explicit CArray(unsigned int n):std::vector<T>(n){}const T&operator[](unsigned int n)const{return std::vector<T>::operator[](n);}T&operator[](unsigned int n){return const_cast<T&>(static_cast<const std::vector<T>&>(*this)[n]);} };
class COpinionSet{protected:CArray<float>OpinionValues;public:float&operator[](long n){return const_cast<float&>(OpinionValues[n]);}COpinionSet():OpinionValues(5){}float operator[](long n)const{return OpinionValues[n];}};
enum EOpinion{};enum ETCInterfaceType{};class CTCBase;
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{C Compare;bool Dirty;public:__declspec(noinline)const std::pair<K,V>*LowerBound(const K&key)const{const std::pair<K,V>*first=this->begin();long count=this->end()-first;while(count>0){long half=count/2;const std::pair<K,V>*middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first;}};
// Layout: shared flattened CThing fields; the original const PeekTC template reads native bitset20 and map44.
class CThing:public CThingFields{public:template<class T>__forceinline bool PeekTC(T**out)const{ETCInterfaceType type=T::StatGetInterfaceType();if(reinterpret_cast<const unsigned long*>(UsingComponents)[type>>5]&(1UL<<(type&31))){ETCInterfaceType key=type;const CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<const CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);const std::pair<ETCInterfaceType,CTCBase*>*i=map.LowerBound(key);if(i==map.end()||type<i->first)i=map.end();*out=reinterpret_cast<T*>(i->second);return true;}return false;}};
class CThingPlayerCreature;
class CPlayer:public CPlayerFields{public:const CThingPlayerCreature*PeekPControlledCreature()const;};
class CWorld:public CWorldFields{public:const CPlayer*PeekMyPlayer()const;};
#define CTCCreatureOpinionOfHero CTCCreatureOpinionOfHeroFields
#include "engine/CTCCreatureOpinionOfHero.h"
#undef CTCCreatureOpinionOfHero
class CTCCreatureOpinionOfHero:public CTCCreatureOpinionOfHeroFields{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0x4f;}};
class CTCOpinionSource{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0x50;}const COpinionSet&PeekOpinionProduced()const;};class COpinionReactionManagerDef;
// Layout: original nonpolymorphic manager PDef0 World4; no dedicated header supplied.
class COpinionReactionManager{private:CDefPointer<const COpinionReactionManagerDef>PDef;const CWorld&World;public:float GetThingOpinionOfPlayer(const CThing&,EOpinion)const;private:void GetOpinions(const CTCCreatureOpinionOfHero*,COpinionSet&,const CTCOpinionSource*)const;};
// Visibility-only stand-in: read both pointee inputs and the entire five-element IN/OUT set; never retain its address.
#pragma auto_inline(off)
__declspec(noinline) void COpinionReactionManager::GetOpinions(const CTCCreatureOpinionOfHero*hero,COpinionSet&opinions,const CTCOpinionSource*source)const{
 float contribution=(hero?*reinterpret_cast<const unsigned char*>(hero):0)+(source?*reinterpret_cast<const unsigned char*>(source):0);
 if(source)opinions=source->PeekOpinionProduced();
 for(unsigned int i=0;i<5;++i)opinions[i]+=contribution;
}
#pragma auto_inline(on)
float COpinionReactionManager::GetThingOpinionOfPlayer(const CThing&thing,EOpinion opinion)const{
 const CTCCreatureOpinionOfHero*opinionOfHero=0;thing.PeekTC(&opinionOfHero);
 COpinionSet opinions;
 const CThingPlayerCreature*hero=World.PeekMyPlayer()->PeekPControlledCreature();const CTCOpinionSource*source=0;
 if(hero)reinterpret_cast<const CThing*>(hero)->PeekTC(&source);
 GetOpinions(opinionOfHero,opinions,source);return opinions[opinion];
}