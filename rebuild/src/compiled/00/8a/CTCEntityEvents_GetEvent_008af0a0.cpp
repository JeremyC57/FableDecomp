// CTCEntityEvents::GetEvent(Event** out,const Filter& filter) const -- Find the newest event of the requested type.
// Leaf: ??$GetEvent@VCEventExpressionPerformedTo@NEntityEvents@@VCEventFilter_None@2@@CTCEntityEvents@@QBE_NPAPAVCEventExpressionPerformedTo@NEntityEvents@@ABVCEventFilter_None@2@@Z
// Identity: original mutable event template instance in PDB; full retail RTTI01274240 GetType slot1=0084b070 returns3, proving CEventExpressionPerformedTo rather than propagated CEventHitBy.
#define CTCEntityEvents CTCEntityEvents_HeaderLayout
#include "engine/CTCEntityEvents.h"
#undef CTCEntityEvents
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
void __cdecl operator delete(void*);
// Layout: original counted block RefCount+0, DeleteFunc+4, Data+8; wrapper Data+0,Info+4. No active shared headers exist.
class CCPPointerInfo {public:long RefCount;void (__fastcall* DeleteFunc)(void*);void* Data;};
template<class T>class CCountedPointer {public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){} __forceinline CCountedPointer(const CCountedPointer& p):Data(p.Data),Info(p.Info){if(Info)++Info->RefCount;} __forceinline ~CCountedPointer(){if(Info && --Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}} T* operator->()const{return Data;}};
namespace NEntityEvents {
enum EEventType {EVENT_EXPRESSION_PERFORMED_TO=3,EVENT_APOLOGY_RECEIVED=4,EVENT_HIT_THING=29};
// Layout: original CEventBase virtual destructor slot0/GetType slot1,WorldFrame+4. Unused event-specific data does not enter this lookup.
class CEventBase {public:virtual ~CEventBase(){}virtual EEventType GetType()const=0;long WorldFrame;};
class CEventHitThing:public CEventBase {public:enum{EventType=29};};
class CEventExpressionPerformedTo:public CEventBase {public:enum{EventType=3};};
class CEventApologyReceived:public CEventBase {public:enum{EventType=4};};
class CEventFilter_None {public:bool operator()(const CEventBase*)const{return true;}};
}
namespace std {
template<class T>class allocator{};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T>struct _Const_traits{typedef const T& reference;};
template<class T>struct _Nonconst_traits{typedef T& reference;};
template<class T,class Traits>struct _List_iterator {typedef typename Traits::reference reference;_List_node_base* _M_node;_List_iterator(_List_node_base* p):_M_node(p){}reference operator*()const{return ((_List_node<T>*)_M_node)->_M_data;} _List_iterator& operator--(){_M_node=_M_node->_M_prev;return *this;}bool operator!=(const _List_iterator& p)const{return _M_node!=p._M_node;}};
template<class Iterator>class reverse_iterator {public:Iterator current;reverse_iterator(Iterator it):current(it){}typename Iterator::reference operator*()const{Iterator tmp=current;return *--tmp;}reverse_iterator& operator++(){--current;return *this;}bool operator!=(const reverse_iterator& p)const{return current!=p.current;}};
template<class T,class A=allocator<T> >class list{public:_List_node_base* _M_node;typedef _List_iterator<T,_Const_traits<T> > const_iterator;typedef reverse_iterator<const_iterator> const_reverse_iterator;const_iterator begin()const{return const_iterator(_M_node->_M_next);}const_iterator end()const{return const_iterator(_M_node);}const_reverse_iterator rbegin()const{return const_reverse_iterator(end());}const_reverse_iterator rend()const{return const_reverse_iterator(begin());}};
}
// Layout: shared header provides Events+0x0c; interpret its opaque storage as the original4-byte STLport list, not an invented container.
class CTCEntityEvents:public CTCEntityEvents_HeaderLayout {public:typedef std::list<CCountedPointer<NEntityEvents::CEventBase> > EventList;const EventList& GetEvents()const{return *(const EventList*)Events;}template<class Event,class Filter>bool GetEvent(Event**,const Filter&)const;};
template<class Event,class Filter>bool CTCEntityEvents::GetEvent(Event** out,const Filter& filter)const{for(EventList::const_reverse_iterator it=GetEvents().rbegin();it!=GetEvents().rend();++it){CCountedPointer<NEntityEvents::CEventBase> event=*it;if(event->GetType()==Event::EventType && filter((Event*)event.Data)){*out=(Event*)event.Data;return true;}}return false;}
template bool CTCEntityEvents::GetEvent<NEntityEvents::CEventExpressionPerformedTo,NEntityEvents::CEventFilter_None>(NEntityEvents::CEventExpressionPerformedTo**,const NEntityEvents::CEventFilter_None&)const;