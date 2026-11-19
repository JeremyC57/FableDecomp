// Behaviour test for CTCEntityEvents::GetEvent<const CEventHitBy, CFilter_Bind3<CIsHitEventHitByMelee, unary_negate<CIsHitEventBlocked>,
// CIsHitEventHitBy> > (0x00738A80): the row body over models of the event list (STLport list walked by reverse_iterator), the counted
// pointer (shared count block, DeleteFunc + delete on the last release) and the event / filter callees. Kills: oldest-first search, the
// type test dropped, any filter dropped / negation lost, the wrong thing compared, *out not stored or stored on a miss, the wrong return,
// and a leaked or extra reference (every copy released on both exits).
#include <stdio.h>
#include <stdlib.h>
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
class CThing { public: int id; };
static int deletes;
struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteEvent(void*){++deletes;}
template<class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer():Data(0),Info(0){}
    CCountedPointer(const CCountedPointer& o):Data(o.Data),Info(o.Info){ if(Info) ++Info->RefCount; }
    ~CCountedPointer(){ if(Info){ --Info->RefCount; if(Info->RefCount==0){ Info->DeleteFunc(Info->Data); delete Info; } } Data=0; Info=0; }
    T* operator->() const { return Data; } T* get() const { return Data; }
};
namespace NEntityEvents {
class CEventBase { public: long Type; virtual long GetType() const { return Type; } };
class CEventHitBase : public CEventBase { public: bool Melee, Blocked; CThing* HitBy; CThing* GetPHitBy() const { return HitBy; } };
class CEventHitBy : public CEventHitBase { public: static long StatGetType() { return 1; } };
class CIsHitEventHitByMelee { public: bool operator()(const CEventHitBase* e) const { return e->Melee; } };
class CIsHitEventBlocked { public: typedef const CEventHitBase* argument_type; bool operator()(const CEventHitBase* e) const { return e->Blocked; } };
class CIsHitEventHitBy { public: const CThing* HitBy; bool operator()(const CEventHitBase* pevent) const { CThing* phit_by = pevent->GetPHitBy(); return phit_by == HitBy; } };
}
template<class P> class unary_negate { public: P pred; bool operator()(const typename P::argument_type& x) const { return !pred(x); } };
template<class A,class B,class C> class CFilter_Bind3 { public: const A& F1; const B& F2; const C& F3;
    CFilter_Bind3(const A& a,const B& b,const C& c):F1(a),F2(b),F3(c){}
    template<class T> bool operator()(const T& t) const { return F1(t) && F2(t) && F3(t); } };
template<class E,class D> inline E* GFDebugPDynamicCast(D* p, E*) { return static_cast<E*>(p); }
struct Node { Node* next; Node* prev; CCountedPointer<NEntityEvents::CEventBase> data; };
struct EventList { Node* head;   // sentinel
    struct rit { Node* cur; const CCountedPointer<NEntityEvents::CEventBase>& operator*() const { return cur->prev->data; }
                 rit& operator++() { cur = cur->prev; return *this; } bool operator!=(const rit& o) const { return cur != o.cur; } };
    rit rbegin() const { rit r; r.cur = head; return r; } rit rend() const { rit r; r.cur = head->next; return r; } };
class CTCEntityEvents { public: EventList Events;
    template <class TEvent, class TFilter> bool GetEvent(TEvent** out, const TFilter& filter) const;
};
template <class TEvent, class TFilter>
bool CTCEntityEvents::GetEvent(TEvent** out, const TFilter& filter) const
{
    for (EventList::rit it = Events.rbegin(); it != Events.rend(); ++it)
    {
        CCountedPointer<NEntityEvents::CEventBase> event = *it;
        if (event->GetType() == TEvent::StatGetType())
        {
            TEvent* typed_event = GFDebugPDynamicCast(event.get(), (TEvent*)0);
            if (filter(typed_event))
            {
                *out = typed_event;
                return true;
            }
        }
    }
    return false;
}
using namespace NEntityEvents;
int main(){
 CThing a,b; Node sentinel; Node nodes[6]; CEventHitBy ev[6]; CCPPointerInfo* info[6]; long cases=0;
 for(unsigned mask=0;mask<4096;mask+=7){
  // six events, oldest first; each event's type / melee / blocked / hitter from the mask
  int n=(mask%6)+1; sentinel.next=&sentinel; sentinel.prev=&sentinel; Node* last=&sentinel;
  for(int i=0;i!=n;++i){ unsigned m=(mask>>(i*2))^(i*5);
   ev[i].Type=(m&1)?1:3; ev[i].Melee=(m&2)!=0; ev[i].Blocked=(m&4)!=0; ev[i].HitBy=(m&8)?&b:&a;
   info[i]=new CCPPointerInfo; info[i]->RefCount=1; info[i]->DeleteFunc=DeleteEvent; info[i]->Data=&ev[i];
   nodes[i].data.Data=&ev[i]; nodes[i].data.Info=info[i];
   nodes[i].prev=last; nodes[i].next=&sentinel; last->next=&nodes[i]; sentinel.prev=&nodes[i]; last=&nodes[i]; }
  CTCEntityEvents events; events.Events.head=&sentinel;
  CIsHitEventHitByMelee melee; unary_negate<CIsHitEventBlocked> not_blocked; CIsHitEventHitBy hitby; hitby.HitBy=&a;
  CFilter_Bind3<CIsHitEventHitByMelee, unary_negate<CIsHitEventBlocked>, CIsHitEventHitBy> filter(melee, not_blocked, hitby);
  const CEventHitBy* out=0; deletes=0;
  bool got=events.GetEvent(&out, filter);
  int want=-1; for(int i=n-1;i>=0;--i){ if(ev[i].Type==1&&ev[i].Melee&&!ev[i].Blocked&&ev[i].HitBy==&a){want=i;break;} }
  check(got==(want>=0)); check(want>=0 ? out==&ev[want] : out==0);
  check(deletes==0); for(int i=0;i!=n;++i){ check(info[i]->RefCount==1); delete info[i]; nodes[i].data.Info=0; }
  ++cases;
 }
 check(cases==586);puts("00738a80_TEST PASS");return 0;
}