#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define __int64 __attribute__((__mode__(__DI__))) long
#endif
#define PASS9_TEST
int main();
// 00aed180 NParticleEngine::CParticleEmitter::DeleteAllParticles
// Leaf: ?DeleteAllParticles@CParticleEmitter@NParticleEngine@@QAEXPAVCIEngine@@@Z
// Flags:
#include "rebuild_abi.h"
#define CParticleEmitter CParticleEmitterFields
#include "engine/CParticleEmitter.h"
#undef CParticleEmitter
class CIEngine;class CBaseClass{public:virtual ~CBaseClass();};class CBaseObjectPointer;class CBaseObject:public CBaseClass{CBaseObjectPointer*IntelligentPointer;protected:virtual ~CBaseObject();};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;T*operator->()const{return Data;}};
namespace std{template<class T>class allocator{};template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;};template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef T*iterator;iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}};}
namespace NParticleEngine{
// Layout: CParticleSystem is only an opaque call receiver here; real CBaseClass
// ancestry and native112extent were independently established in AF56E0.
class CParticleSystem:public CBaseClass{unsigned char unread[108];public:void DeleteAllParticles();};
class CParticleEmitter:public CBaseObject{protected:std::vector<CCountedPointer<CParticleSystem> >ParticleSystems;private:unsigned char unreadTail[344];
#ifdef PASS9_TEST
friend int ::main();
#endif
public:void DeleteAllParticles(CIEngine*);};
void CParticleEmitter::DeleteAllParticles(CIEngine*engine){(void)engine;for(std::vector<CCountedPointer<CParticleSystem> >::iterator it=ParticleSystems.begin();it!=ParticleSystems.end();++it)(*it)->DeleteAllParticles();}
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleEmitter)==sizeof(CParticleEmitterFields));

CBaseClass::~CBaseClass(){}CBaseObject::~CBaseObject(){}
using namespace NParticleEngine;static CParticleSystem*objects;static unsigned calls,order[8],stopAfter,errors;static CCountedPointer<CParticleSystem>**finishSlot;static CCountedPointer<CParticleSystem>*values;
void CParticleSystem::DeleteAllParticles(){unsigned id=this-objects;if(id>=4||calls==8){++errors;return;}order[calls++]=id;if(stopAfter&&calls==stopAfter)*finishSlot=values+calls;}
int main(){unsigned cases=0;for(unsigned n=0;n<=8;++n)for(unsigned seed=0;seed<256;++seed)for(unsigned stop=0;stop<=n;++stop){CParticleSystem particleObjects[4];objects=particleObjects;CCountedPointer<CParticleSystem>entries[8],beforeEntries[8];unsigned expected[8];for(unsigned i=0;i<8;++i){unsigned id=(seed>>(i%7))&3;entries[i].Data=particleObjects+id;entries[i].Info=0;expected[i]=id;}memcpy(beforeEntries,entries,sizeof(entries));struct Guard{unsigned before;CParticleEmitter owner;unsigned after;}guard;guard.before=0x12345678;guard.after=0xa5b6c7d8;CCountedPointer<CParticleSystem>*state[3]={entries,entries+n,entries+8};memcpy(&guard.owner.ParticleSystems,state,12);unsigned char beforeOwner[sizeof(guard.owner)];memcpy(beforeOwner,&guard.owner,sizeof(guard.owner));values=entries;finishSlot=reinterpret_cast<CCountedPointer<CParticleSystem>**>(&guard.owner.ParticleSystems)+1;calls=errors=0;stopAfter=stop;guard.owner.DeleteAllParticles(0);unsigned wanted=stop?stop:n;if(errors||calls!=wanted||memcmp(order,expected,wanted*sizeof(unsigned))||memcmp(beforeEntries,entries,sizeof(entries))||guard.before!=0x12345678||guard.after!=0xa5b6c7d8)return 1;for(unsigned i=0;i<sizeof(guard.owner);++i)if((i<12||i>=16)&&reinterpret_cast<unsigned char*>(&guard.owner)[i]!=beforeOwner[i])return 2;if(*finishSlot!=entries+wanted)return 3;++cases;}if(cases!=11520)return 4;puts("00aed180_TEST PASS");return 0;}