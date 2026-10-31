// 00738a80 CTCEntityEvents::GetEvent
// Codex parked-50 best attempt: DIFFER(177v177); residue: 177/177 bytes; event object and counted-pointer control block use swapped ESI/EDI throughout traversal and release.
// Leaf: ??$GetEvent@$$CBVCEventHitBy@NEntityEvents@@V?$CFilter_Bind3@VCIsHitEventHitByMelee@NEntityEvents@@V?$unary_negate@VCIsHitEventBlocked@NEntityEvents@@@std@@VCIsHitEventHitBy@2@@@@CTCEntityEvents@@QBE_NPAPBVCEventHitBy@NEntityEvents@@ABV?$CFilter_Bind3@VCIsHitEventHitByMelee@NEntityEvents@@V?$unary_negate@VCIsHitEventBlocked@NEntityEvents@@@std@@VCIsHitEventHitBy@2@@@@Z
// Flags:
#include <stddef.h>
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCEntityEvents CTCEntityEventsLayout
#include "engine/CTCEntityEvents.h"
#undef CTCEntityEvents
#define CCharString CCharStringLayout
#include "fable_string.h"
#undef CCharString
class CCharString{
    unsigned char unobservedString[sizeof(CCharStringLayout)];
public:CCharString(const char*,long=-1);~CCharString();
};
#include "engine/CBaseClassNonCopyable.h"
class CThing;class CTCParamsBase;
class C2DVector;class CRightHandedSet;class CWorld;enum ETCInterfaceType{};
class CPersistContext;class CEnginePrimitivePrepareInfo;namespace NTCMessages{class CMessageBase;}
class CThingManager;
class CTCBase:public CBaseClassNonCopyable{
protected:virtual void OnSerialise(CPersistContext&,bool);CWorld&GetWorld()const;
public:
    virtual void OnRemove();
    virtual void ConstructFromParams(const CTCParamsBase&);
    virtual void OnCreate();virtual void InitialActivate();virtual void OnInitialActivate();
    virtual void RenderUpdate(float);virtual bool HasFrameUpdate()const;virtual void FrameUpdate();virtual void BulletTimeFrameUpdate();
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    virtual void Copy(const CTCBase&);virtual void OnDie();virtual void OnKill();virtual void OnLevelUnload();virtual void OnSetInLimbo(bool);
    virtual bool Validate(CCharString&)const;virtual bool IsFreeForDeletion()const;virtual bool IsSerialiseCreatable()const;virtual bool IsSerialisableForHeroState()const;
    virtual ETCInterfaceType GetInterfaceType()const=0;virtual CCharString GetClassTypeName()const=0;
    virtual bool IsComponentSerialisable()const;virtual bool IsDriver()const;virtual bool IsCopyable()const;virtual void GlobalUpdate(bool);
    virtual void HandleMessage(const NTCMessages::CMessageBase&);
protected:
    CThing&ParentThing;
private:
    bool ToBeRemoved,IsInGlobalUpdateTCs;unsigned char unobservedAlignment[2];
};

#include "engine/CCountedPointer.h"
namespace std{
template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};
template<class P>class unary_negate:public unary_function<typename P::argument_type,bool>{
protected:P _M_pred;
public:explicit unary_negate(const P&p):_M_pred(p){}bool operator()(const typename P::argument_type&x)const{return!_M_pred(x);}
};
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};
template<class C,class T,class D=ptrdiff_t,class P=T*,class R=T&>struct iterator{typedef C iterator_category;typedef T value_type;typedef D difference_type;typedef P pointer;typedef R reference;};
template<class T>struct _Nonconst_traits;
template<class T>struct _Const_traits{typedef T value_type;typedef const T&reference;typedef const T*pointer;typedef _Nonconst_traits<T> _Non_const_traits;};
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;typedef _Nonconst_traits<T> _Non_const_traits;};
struct _List_node_base{_List_node_base*_M_next;_List_node_base*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base{
    typedef size_t size_type;typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag iterator_category;
    _List_node_base*_M_node;
    _List_iterator_base(_List_node_base*x):_M_node(x){} _List_iterator_base(){}
    void _M_incr(){_M_node=_M_node->_M_next;}void _M_decr(){_M_node=_M_node->_M_prev;}
    bool operator==(const _List_iterator_base&x)const{return _M_node==x._M_node;}bool operator!=(const _List_iterator_base&x)const{return _M_node!=x._M_node;}
};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{
    typedef T value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
    typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag iterator_category;
    typedef _List_iterator<T,_Nonconst_traits<T> > iterator;typedef _List_node<T> _Node;
    _List_iterator(_Node*x):_List_iterator_base(x){} _List_iterator(){}
    _List_iterator(const iterator&x):_List_iterator_base(x._M_node){}
    reference operator*()const{return ((_Node*)_M_node)->_M_data;}
    _List_iterator&operator--(){this->_M_decr();return *this;}
};
template<class I>class reverse_iterator:public iterator<typename I::iterator_category,typename I::value_type,typename I::difference_type,typename I::pointer,typename I::reference>{
protected:I current;
public:typedef typename I::reference reference;
    reverse_iterator(){}explicit reverse_iterator(I x):current(x){}reverse_iterator(const reverse_iterator&x):current(x.current){}
    I base()const{return current;}reference operator*()const{I tmp=current;return *--tmp;}
    reverse_iterator&operator++(){--current;return *this;}
};
template<class I>inline bool __cdecl operator==(const reverse_iterator<I>&x,const reverse_iterator<I>&y){return x.base()==y.base();}
template<class I>inline bool __cdecl operator!=(const reverse_iterator<I>&x,const reverse_iterator<I>&y){return !(x==y);}
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T,class A>class _List_base{protected:typedef _List_node<T> _Node;public:_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> >_M_node;_List_base(const A&);~_List_base();};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{
public:typedef _List_node<T>_Node;typedef _List_iterator<T,_Const_traits<T> >const_iterator;typedef reverse_iterator<const_iterator>const_reverse_iterator;
    explicit list(const A&a=A()):_List_base<T,A>(a){}
    const_iterator begin()const{return const_iterator((_Node*)this->_M_node._M_data->_M_next);}const_iterator end()const{return this->_M_node._M_data;}
    const_reverse_iterator rbegin()const{return const_reverse_iterator(end());}const_reverse_iterator rend()const{return const_reverse_iterator(begin());}
};
}
class CTCEntityEvents;class CHitParameters;
namespace NEntityEvents{
enum EEventType{};
class CEventBase{
public:CEventBase();virtual ~CEventBase();virtual EEventType GetType()const=0;virtual void OnAdd(const CTCEntityEvents&);virtual long GetLifeTime()const;virtual bool IsReadyToDie()const;
protected:long WorldFrame;
};
class CEventHitBase:public CEventBase{
public:explicit CEventHitBase(const CHitParameters&);virtual ~CEventHitBase();CThing*GetPHitBy()const;virtual bool IsFlourish()const;virtual long GetLifeTime()const;
protected:CCountedPointer<CHitParameters>PHitParams;
};
class CEventHitBy:public CEventHitBase{
public:explicit CEventHitBy(const CHitParameters&);virtual ~CEventHitBy();static EEventType __fastcall StatGetType(){return (EEventType)1;}virtual EEventType GetType()const;
};
class CIsHitEventHitByMelee{public:CIsHitEventHitByMelee(){}bool operator()(const CEventHitBase*)const;};
class CIsHitEventBlocked:public std::unary_function<const CEventHitBase*,bool>{public:CIsHitEventBlocked(){}bool operator()(const CEventHitBase*)const;};
class CIsHitEventHitBy{
    const CThing&HitBy;
public:explicit CIsHitEventHitBy(const CThing&x):HitBy(x){}bool operator()(const CEventHitBase*event)const{return event->GetPHitBy()==&HitBy;}
};
}
template<class A,class B,class C>class CFilter_Bind3{
protected:const A&F1;const B&F2;const C&F3;
public:CFilter_Bind3(const A&a,const B&b,const C&c):F1(a),F2(b),F3(c){}
    template<class T>bool operator()(const T&x)const{return F1(x)&&F2(x)&&F3(x);}
};
class CTCEntityEvents:public CTCBase{
protected:std::list<CCountedPointer<NEntityEvents::CEventBase> >Events;
public:explicit CTCEntityEvents(CThing&);virtual ~CTCEntityEvents();virtual ETCInterfaceType GetInterfaceType()const;virtual CCharString GetClassTypeName()const;
    template<class TEvent,class TFilter>bool GetEvent(TEvent**out,const TFilter&filter)const;
};
template<class TEvent,class TFilter>bool CTCEntityEvents::GetEvent(TEvent**out,const TFilter&filter)const{
    for(std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_reverse_iterator it=Events.rbegin();it!=Events.rend();++it){
        const CCountedPointer<NEntityEvents::CEventBase>&stored=*it;
        CCountedPointer<NEntityEvents::CEventBase>event(stored);
        if(event->GetType()==TEvent::StatGetType()&&filter(static_cast<TEvent*>(event.Data))){*out=static_cast<TEvent*>(event.Data);return true;}
    }
    return false;
}
template bool CTCEntityEvents::GetEvent<const NEntityEvents::CEventHitBy,CFilter_Bind3<NEntityEvents::CIsHitEventHitByMelee,std::unary_negate<NEntityEvents::CIsHitEventBlocked>,NEntityEvents::CIsHitEventHitBy> >(const NEntityEvents::CEventHitBy**,const CFilter_Bind3<NEntityEvents::CIsHitEventHitByMelee,std::unary_negate<NEntityEvents::CIsHitEventBlocked>,NEntityEvents::CIsHitEventHitBy>&)const;
