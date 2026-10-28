#include <stdio.h>
#include <stdlib.h>
#include <new>
static int frees,bad,handles,memberDestructions,ownerDeletes;static void* expectedBuffer;static void* expectedOwner;static void TrackFree(void* p){if(p!=expectedBuffer||memberDestructions!=1)bad=1;++frees;free(p);}
#define free TrackFree
#define protected public
// NPlayerGui::CDrawDamage::ScalarDeletingDestructor(unsigned int) -- Destroy the draw object and optionally release its allocation.
// Leaf: ??_GCDrawDamage@NPlayerGui@@QAEPAXI@Z
// Identity: probable. Own scalar-deleting cleanup first destroys member40 then inherited handle vector28, conditionally scalar-delete00bfe9bc and returns receiver. Out-of-line list base destructor43fcf0 frees trivial SDamageValue nodes; own plain dtor441c70 and counted deleter443840 connect matching CDrawDamage creator via Reset4446e0. Original nonvirtual implicit destructor; no fake vptr store, standard compiler-generated deleting form requested through delete expression.
// Layout: inherited Draw40 and real list4 or CWideString4 member. No target header. Source includes only typed original declarations and compiler destruction.
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
class CWideStringData;class CWideString{public:CWideStringData* PStringData;CWideString();~CWideString();};
namespace std{struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};template<class T,class A>class _List_base{protected:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > > _M_node;_List_base(const A&);~_List_base();};template<class T,class A=allocator<T> >class list:protected _List_base<T,A>{public:list(const A& a=A()):_List_base<T,A>(a){}};}
namespace NPlayerGui{class CDrawDamage:public CDrawBase{public:class SDamageValue{public:long Duration;float Change;bool Hero;};std::list<SDamageValue> DamageValues;};class CDrawNPCName:public CDrawBase{public:CWideString Name;long ScreenShotMessageCounter;};}
#pragma inline_depth(0)
void DefineBaseDestructor(NPlayerGui::CDrawBase* p){p->CDrawBase::~CDrawBase();}
void DefineOwnerDestructor(NPlayerGui::CDrawDamage* p){p->CDrawDamage::~CDrawDamage();}
#pragma inline_depth(8)
// Fidelity: qualified destructor emission makes the implicit definitions visible before the deleting form.
void DestroyCandidate(NPlayerGui::CDrawDamage* p){delete p;}
#undef protected
#undef free
template<class T>CIVCountedPointer<T>::~CIVCountedPointer(){}CEnginePrimitiveHandle::~CEnginePrimitiveHandle(){if(memberDestructions!=1)bad=1;++handles;}CBaseIntelligentPointer::~CBaseIntelligentPointer(){}void NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary(){}void NPlayerGui::CDrawBase::ClearPrimitives(){}CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){++memberDestructions;}template<class T,class A>std::_List_base<T,A>::_List_base(const A&):_M_node(std::allocator<std::_List_node<T> >(),0){}template<class T,class A>std::_List_base<T,A>::~_List_base(){++memberDestructions;}void operator delete(void* p)throw(){if(p!=expectedOwner||handles!=2||frees!=1)bad=1;++ownerDeletes;free(p);}
int main(){typedef NPlayerGui::CDrawDamage Owner;Owner* p=new Owner;expectedOwner=p;CEnginePrimitiveHandle* hs=(CEnginePrimitiveHandle*)malloc(2*sizeof(CEnginePrimitiveHandle));new(hs)CEnginePrimitiveHandle;new(hs+1)CEnginePrimitiveHandle;expectedBuffer=hs;p->PrimitiveHandles._M_start=hs;p->PrimitiveHandles._M_finish=hs+2;p->PrimitiveHandles._M_end_of_storage._M_data=hs+2;DestroyCandidate(p);bool ok=!bad&&frees==1&&handles==2&&memberDestructions==1&&ownerDeletes==1;puts(ok?"00442d50_TEST PASS":"00442d50_TEST FAIL");return ok?0:1;}
