#include <stdio.h>
#include <stdlib.h>
#include <new>
static int frees,bad,handles,effects;static void* expectedFree[2];static void TrackFree(void* p){if(frees>=2||p!=expectedFree[frees])bad=1;++frees;free(p);}
#define free TrackFree
#define protected public
// NPlayerGui::CDrawHealth::~CDrawHealth() -- Destroy effect references and owned primitive handles.
// Leaf: ??1CDrawHealth@NPlayerGui@@QAE@XZ
// Identity: proven. Own scalar-dtor00442960 and counted deleter00443500 -> Reset00444050 -> CPlayerGui constructor0043bdc2, matching CDrawHealth ctor0064e1e4 and own typed vectors28/40. PDB original nonvirtual compiler-generated destructor; own class intentionally declares no destructor, so no invented vptr store. Genuine STLport vector/base/proxy, original polymorphic CEnginePrimitiveHandle:CBaseClass and CIntelligentPointer:CBaseIntelligentPointer element types. Original named fields all retained; unused constructors/APIs omitted.
// Layout: base Draw40, Health108, Stat72 in retail; two element types8 each. No matching target engine header exists.
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CBaseClass{public:virtual ~CBaseClass(){}};class CIVCountedPointeeBase;class CEnginePersistentPrimitiveBase;
template<class T>class CIVCountedPointer{public:CIVCountedPointeeBase* Object;CIVCountedPointer():Object(0){}~CIVCountedPointer();};class CEnginePrimitiveHandle:public CBaseClass{public:CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;virtual ~CEnginePrimitiveHandle();};
struct CBaseObjectPointer;class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{};namespace NParticleEngine{class CParticleEmitter;}
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}__forceinline ~_Vector_base(){if(_M_start)free(_M_start);}};template<class T>__forceinline void _Destroy(T* first,T* last){for(;first!=last;++first)first->~T();}template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:vector(const A& a=A()):_Vector_base<T,A>(a){}__forceinline ~vector(){_Destroy(this->_M_start,this->_M_finish);}};}
class CPlayerGui;class CPlayerManager;class CWorld;class CMainGameComponent;class CGraphicDataBank;class CPlayerGuiDef;
class CRGBColour{public:unsigned char R,G,B,A;};class CFadeInFadeOutBase{public:bool Active;float FadeInTime,FadeOutTime;bool Closing,Opening;float OpenTimer,CloseTimer;CRGBColour ToColour;};enum EPlayerGuiMode{EPlayerGuiMode_DescriptiveFixture};
namespace NPlayerGui{
class CDrawBase{public:virtual void ClearPrimitivesIfNecessary();virtual void ClearPrimitives();CPlayerGui* PlayerGui;CPlayerManager* PlayerManager;CWorld* World;CMainGameComponent* Component;CGraphicDataBank* Bank;bool Drawing;std::vector<CEnginePrimitiveHandle> PrimitiveHandles;};
class CDrawHealth:public CDrawBase{public:std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;float PreviousHealth;CFadeInFadeOutBase HealthIncreaseFade;float HealthReduction,HealthReductionAlpha,HealthFlashTimer,LowHealthPulseScale,LowHealthPulseDirection;long PreviousEffect;};
class CStatBase{public:virtual void UnprovenStatSlot0()=0;virtual void UnprovenStatSlot1()=0;virtual void UnprovenStatSlot2()=0;CPlayerGuiDef* GuiDef;bool ValueInitialised;float CurrentValue,OldValue,LastOldValue,RecentMaxValue,OldRecentMaxValue;unsigned long FrameCurrentValueLastMoved,FrameFullyOpenedOn;EPlayerGuiMode GuiMode;float FadeInTimer;std::vector<CEnginePrimitiveHandle> PrimitiveHandles;std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;};
}
void DestroyCandidate(NPlayerGui::CDrawHealth* p){p->CDrawHealth::~CDrawHealth();}

#undef protected
#undef free
template<class T>CIVCountedPointer<T>::~CIVCountedPointer(){}CEnginePrimitiveHandle::~CEnginePrimitiveHandle(){if(effects!=2)bad=1;++handles;}CBaseIntelligentPointer::~CBaseIntelligentPointer(){if(handles)bad=1;++effects;}void NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary(){}void NPlayerGui::CDrawBase::ClearPrimitives(){}
int main(){typedef NPlayerGui::CDrawHealth Owner;typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> Effect;void* storage=malloc(sizeof(Owner));Owner* p=new(storage)Owner;CEnginePrimitiveHandle* hs=(CEnginePrimitiveHandle*)malloc(3*sizeof(CEnginePrimitiveHandle));Effect* es=(Effect*)malloc(2*sizeof(Effect));for(int i=0;i<3;++i)new(hs+i)CEnginePrimitiveHandle;for(int i=0;i<2;++i)new(es+i)Effect;p->PrimitiveHandles._M_start=hs;p->PrimitiveHandles._M_finish=hs+3;p->PrimitiveHandles._M_end_of_storage._M_data=hs+3;p->Effects._M_start=es;p->Effects._M_finish=es+2;p->Effects._M_end_of_storage._M_data=es+2;expectedFree[0]=es;expectedFree[1]=hs;DestroyCandidate(p);bool ok=!bad&&frees==2&&effects==2&&handles==3;free(storage);storage=malloc(sizeof(Owner));p=new(storage)Owner;DestroyCandidate(p);ok=ok&&!bad&&frees==2&&effects==2&&handles==3;free(storage);puts(ok?"00441800_TEST PASS":"00441800_TEST FAIL");return ok?0:1;}