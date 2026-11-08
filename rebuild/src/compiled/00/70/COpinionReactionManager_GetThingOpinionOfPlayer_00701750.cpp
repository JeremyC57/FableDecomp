// 00701750 COpinionReactionManager::GetThingOpinionOfPlayer
// Leaf: ?GetThingOpinionOfPlayer@COpinionReactionManager@@QBEMABVCThing@@W4EOpinion@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Flags:
#include "fable_compiler.h"
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
#include "engine/CArray.h"
class COpinionSet{protected:CArray<float>OpinionValues;public:float&operator[](long n){return const_cast<float&>(OpinionValues[n]);}COpinionSet():OpinionValues(5){}float operator[](long n)const{return OpinionValues[n];}};
enum EOpinion{};enum ETCInterfaceType{};class CTCBase;
#include "engine/CVectorMap.h"
// Layout: shared flattened CThing fields; the original const PeekTC template reads native bitset20 and map44.
class CThing:public CThingFields{public:template<class T>FABLE_FORCEINLINE bool PeekTC(T**out)const{ETCInterfaceType type=T::StatGetInterfaceType();if(reinterpret_cast<const unsigned long*>(UsingComponents)[type>>5]&(1UL<<(type&31))){ETCInterfaceType key=type;const CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<const CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);const std::pair<ETCInterfaceType,CTCBase*>*i=map.LowerBound(key);if(i==map.end()||type<i->first)i=map.end();*out=reinterpret_cast<T*>(i->second);return true;}return false;}};
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