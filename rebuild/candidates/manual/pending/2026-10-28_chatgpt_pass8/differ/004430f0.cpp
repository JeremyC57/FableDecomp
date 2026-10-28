// NPlayerGui::CDrawNPCName::ScalarDeletingDestructor(unsigned int) -- Destroy the draw object and optionally release its allocation.
// Leaf: ??_GCDrawNPCName@NPlayerGui@@QAEPAXI@Z
// Identity: probable. Own scalar-deleting cleanup first destroys member40 then inherited handle vector28, conditionally scalar-delete00bfe9bc and returns receiver. Only draw owner with original CWideString at donor44/retail40 and trivial trailingcounter is CDrawNPCName; cached GUI ownership confirms the sibling destruction family. Original wide dtor99b510. Original nonvirtual implicit destructor; no fake vptr store, standard compiler-generated deleting form requested through delete expression.
// Layout: inherited Draw40 and real list4 or CWideString4 member. No target header. Source includes only typed original declarations and compiler destruction.
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CBaseClass{public:virtual ~CBaseClass(){}};class CIVCountedPointeeBase;class CEnginePersistentPrimitiveBase;
template<class T>class CIVCountedPointer{public:CIVCountedPointeeBase* Object;CIVCountedPointer():Object(0){}~CIVCountedPointer();};class CEnginePrimitiveHandle:public CBaseClass{public:CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;virtual ~CEnginePrimitiveHandle();};
struct CBaseObjectPointer;class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{};namespace NParticleEngine{class CParticleEmitter;}
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}__forceinline ~_Vector_base(){if(_M_start)free(_M_start);}};template<class T>__forceinline void _Destroy(T* first,T* last){for(;first!=last;++first)first->~T();}template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:vector(const A& a=A()):_Vector_base<T,A>(a){}__forceinline ~vector();};}
class CPlayerGui;class CPlayerManager;class CWorld;class CMainGameComponent;class CGraphicDataBank;class CPlayerGuiDef;
class CRGBColour{public:unsigned char R,G,B,A;};class CFadeInFadeOutBase{public:bool Active;float FadeInTime,FadeOutTime;bool Closing,Opening;float OpenTimer,CloseTimer;CRGBColour ToColour;};enum EPlayerGuiMode{EPlayerGuiMode_DescriptiveFixture};
namespace NPlayerGui{
class CDrawBase{public:virtual void ClearPrimitivesIfNecessary();virtual void ClearPrimitives();CPlayerGui* PlayerGui;CPlayerManager* PlayerManager;CWorld* World;CMainGameComponent* Component;CGraphicDataBank* Bank;bool Drawing;std::vector<CEnginePrimitiveHandle> PrimitiveHandles;};
class CDrawHealth:public CDrawBase{public:std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;float PreviousHealth;CFadeInFadeOutBase HealthIncreaseFade;float HealthReduction,HealthReductionAlpha,HealthFlashTimer,LowHealthPulseScale,LowHealthPulseDirection;long PreviousEffect;};
class CStatBase{public:virtual void UnprovenStatSlot0()=0;virtual void UnprovenStatSlot1()=0;virtual void UnprovenStatSlot2()=0;CPlayerGuiDef* GuiDef;bool ValueInitialised;float CurrentValue,OldValue,LastOldValue,RecentMaxValue,OldRecentMaxValue;unsigned long FrameCurrentValueLastMoved,FrameFullyOpenedOn;EPlayerGuiMode GuiMode;float FadeInTimer;std::vector<CEnginePrimitiveHandle> PrimitiveHandles;std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;};
}
class CWideStringData;class CWideString{public:CWideStringData* PStringData;CWideString();~CWideString();};
namespace std{struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};template<class T,class A>class _List_base{protected:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > > _M_node;_List_base(const A&);~_List_base();};template<class T,class A=allocator<T> >class list:protected _List_base<T,A>{public:list(const A& a=A()):_List_base<T,A>(a){}};}
namespace NPlayerGui{class CDrawDamage:public CDrawBase{public:class SDamageValue{public:long Duration;float Change;bool Hero;};std::list<SDamageValue> DamageValues;};class CDrawNPCName:public CDrawBase{public:__forceinline ~CDrawNPCName(){}CWideString Name;long ScreenShotMessageCounter;};}
void DefineBaseDestructor(NPlayerGui::CDrawBase* p){p->CDrawBase::~CDrawBase();}
void DefineOwnerDestructor(NPlayerGui::CDrawNPCName* p){p->CDrawNPCName::~CDrawNPCName();}
namespace std{template<class T,class A>__forceinline vector<T,A>::~vector(){_Destroy(this->_M_start,this->_M_finish);}}
// Fidelity: define the implicit owner chain before the vector cleanup body, then request the deleting form.
void DestroyCandidate(NPlayerGui::CDrawNPCName* p){delete p;}