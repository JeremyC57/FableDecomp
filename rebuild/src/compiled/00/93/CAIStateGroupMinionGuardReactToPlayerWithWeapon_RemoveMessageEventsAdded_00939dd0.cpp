// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::RemoveMessageEventsAdded() -- drain the world message queue of every "guard warning" event (type 27) this guard's own creature originated: find the most recent one, remove it from the manager's list, repeat until none is left
// Leaf: ?RemoveMessageEventsAdded@CAIStateGroup_MinionGuardReactToPlayerWithWeapon@@AAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe @ 0x00939DD0, 150 bytes. Ego_r/FableWin: private ?RemoveMessageEventsAdded@...@@AAEXXZ. Callees:
//   CMessageEventFilter_IsOriginatedByThing ctor 0x006AF2B0, CAIStateGroupBase::PeekWorld 0x008FCFA0 (catalog label
//   "CPlayer::GetCurrentMode" is propagated), FindMostRecentMessage<CFilter_Bind2<IsOfType,IsOriginatedByThing>> 0x0090DEE0,
//   CAIStateGroupBase::GetWorld 0x008FCFC0, free 0x00BFEA14 (STLport node deallocate); CMessageEventManager::RemoveMessage inlined.
// Shape: the lever ChatGPT pass 10 lacked is what the TU knows about the callees. Retail's TU sees the FindMostRecentMessage
//   template body (so VC7.1 hoists the binder's F1 = &type store above the loop) but NOT the two predicate bodies, which are
//   out of line at 0x006AF1C0 / 0x006AF200. With the predicate bodies visible (pass 10's __declspec(noinline) bodies) the
//   receiver loads straight into ecx (149v150); with the FindMostRecentMessage body hidden the F1 store is not hoisted (155v150).
//   Declared-only predicates + visible template = retail mov eax,[eax+60h] / lea edx / push edx / mov ecx,eax.
// Layout: engine/CWorld.h (PMessageEventManager_Data +0x60), engine/CAIStateGroupBase.h, engine/CAIBrain.h; CMessageEvent is
//   the Ego_r TPI record (88 bytes); STLport 4.6 list node model reduced to the members used.
#include <stddef.h>
extern "C" void __cdecl free(void*);
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CAIBrain.h"
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
class CThing;class CFaction;class CBaseObjectPointer;
class CBaseIntelligentPointer{private:CBaseObjectPointer*PData;public:virtual~CBaseIntelligentPointer();};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{};
#include "engine/C3DVector.h"
enum EMessageEventType{};enum EReactionSpeechType{};
// Layout: original TPI CMessageEvent is88 bytes; base8, Type8, Pos12, UIDs32/40, pointers72/80.
class CMessageEvent:public CBaseObject{
public:virtual~CMessageEvent();EMessageEventType Type;C3DVector Pos;float SightRadius,HearingRadius;unsigned __int64 OriginatorUID,TargetUID;const CFaction*PTargetFaction;long TimeStamp;unsigned long Lifespan;long ExtraData;unsigned long MessageID;EReactionSpeechType ReactionSpeechType;
private:CIntelligentPointer<CThing>POriginator,PTarget;
};
namespace std{template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};}
namespace std{
template<class T>class allocator{public:void deallocate(T*p,size_t){free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base*_M_next;_List_node_base*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base{_List_node_base*_M_node;_List_iterator_base(_List_node_base*p):_M_node(p){}void _M_incr(){_M_node=_M_node->_M_next;}bool operator!=(const _List_iterator_base&v)const{return _M_node!=v._M_node;}};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{
 typedef _List_iterator<T,_Nonconst_traits<T> >iterator;typedef _List_iterator<T,Traits>_Self;
 _List_iterator(_List_node<T>*n):_List_iterator_base(n){} _List_iterator(const iterator&v):_List_iterator_base(v._M_node){}
 typename Traits::reference operator*()const{return ((_List_node<T>*)_M_node)->_M_data;}
 _Self&operator++(){this->_M_incr();return *this;}
};
template<class T>inline void _Destroy(T*p){p->~T();}
template<class T,class A>class _List_base{public:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{
public:typedef _List_iterator<T,_Nonconst_traits<T> >iterator;
 iterator begin(){return (_List_node<T>*)this->_M_node._M_data->_M_next;}iterator end(){return this->_M_node._M_data;}
 iterator erase(iterator position){_List_node_base*next=position._M_node->_M_next;_List_node_base*prev=position._M_node->_M_prev;_List_node<T>*node=(_List_node<T>*)position._M_node;prev->_M_next=next;next->_M_prev=prev;_Destroy(&node->_M_data);this->_M_node.deallocate(node,1);return (_List_node<T>*)next;}
};
}

class CMessageEventFilter_IsOfType:public std::unary_function<CMessageEvent,bool>{private:EMessageEventType Type;public:CMessageEventFilter_IsOfType(EMessageEventType t):Type(t){}bool operator()(const CMessageEvent&e)const;};
class CMessageEventFilter_IsOriginatedByThing:public std::unary_function<CMessageEvent,bool>{private:unsigned __int64 UID;public:CMessageEventFilter_IsOriginatedByThing(const CThing&);bool operator()(const CMessageEvent&e)const;};
template<class A,class B>class CFilter_Bind2{protected:const A&F1;const B&F2;public:CFilter_Bind2(const A&a,const B&b):F1(a),F2(b){}template<class T>bool operator()(const T&e)const{return F1(e)&&F2(e);}};
template<class A,class B>inline CFilter_Bind2<A,B>__fastcall GFFilter_Bind2(const A&a,const B&b){return CFilter_Bind2<A,B>(a,b);}
class CMessageEventManager{
private:const CWorld&World;std::list<CMessageEvent>Messages;
public:template<class F>const CMessageEvent*FindMostRecentMessage(const F&)const;
 void RemoveMessage(const CMessageEvent*event){for(std::list<CMessageEvent>::iterator i=Messages.begin();i!=Messages.end();++i){if(&*i==event){Messages.erase(i);return;}}}
};
template<class F>const CMessageEvent*CMessageEventManager::FindMostRecentMessage(const F&filter)const{
 for(const std::_List_node_base*p=Messages._M_node._M_data->_M_next;p!=Messages._M_node._M_data;p=p->_M_next){const CMessageEvent&e=static_cast<const std::_List_node<CMessageEvent>*>(p)->_M_data;if(filter(e))return &e;}return 0;
}
// Layout: imported CWorld provides message manager at60; no new fields.
class CWorld:public CWorldLayout{public:const CMessageEventManager&GetMessageEventManager()const{return *PMessageEventManager_Data;}CMessageEventManager&GetMessageEventManager(){return *PMessageEventManager_Data;}};
// Layout: imported CAIStateGroupBase PBrain at4, CAIBrain Creature at18.
class CAIStateGroupBase:public CAIStateGroupBaseLayout{protected:const CWorld&PeekWorld()const;CWorld&GetWorld()const;CThingAICreature&GetCreature()const{return *PBrain->Creature;}};
class CAIStateGroup_MinionGuardReactToPlayerWithWeapon:public CAIStateGroupBase{private:void RemoveMessageEventsAdded();};
void CAIStateGroup_MinionGuardReactToPlayerWithWeapon::RemoveMessageEventsAdded(){
 const CMessageEvent*event;
 do{
  event=PeekWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind2(CMessageEventFilter_IsOfType((EMessageEventType)27),CMessageEventFilter_IsOriginatedByThing(reinterpret_cast<const CThing&>(GetCreature()))));
  if(event)GetWorld().GetMessageEventManager().RemoveMessage(event);
 }while(event);
}