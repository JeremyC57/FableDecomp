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