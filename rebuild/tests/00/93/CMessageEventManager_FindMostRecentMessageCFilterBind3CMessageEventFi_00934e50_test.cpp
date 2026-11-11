#include <cstdio>
#include <cassert>
#include <list>
#include <functional>
static unsigned int type_calls,get_calls,not_calls,range_calls;
class CBaseObject {public:virtual ~CBaseObject(){}};
class CThing:public CBaseObject {public:unsigned int ThingTypeFlags;bool Nearby;unsigned long GetTypeFlags()const{return ThingTypeFlags;} };
class CThingAICreature:public CThing {public:static unsigned long StatGetTypeFlags(){return 0x1f;} };
class CBaseIntelligentPointer {public:CBaseObject* Item;mutable unsigned int Reads;unsigned int ExpireAt;CBaseIntelligentPointer():Item(0),Reads(0),ExpireAt(0){} CBaseObject* GetPItem()const{++get_calls;++Reads;return ExpireAt&&Reads>=ExpireAt?0:Item;} };
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {
 T* GetPointer()const{return static_cast<T*>(GetPItem());}
public:T* Get()const{return GetPointer();}T* operator->()const{return GetPointer();}
};
template<class T>inline bool operator!=(const CIntelligentPointer<T>& pointer,int){return pointer.Get()!=0;}
template<class From,class To>inline To GFDebugPDynamicCast(From value,To){return static_cast<To>(value);}
class CMessageEvent {public:int Type;CIntelligentPointer<CThing> POriginator;
 template<class T>T* GetPOriginator()const{
        T* ret=0;
        if(POriginator!=0){
            unsigned long things_flags=POriginator->GetTypeFlags();
            unsigned long cast_to_flags=T::StatGetTypeFlags();
            unsigned long xored=things_flags^cast_to_flags;
            if(!(xored&cast_to_flags))ret=GFDebugPDynamicCast(POriginator.Get(),(T*)0);
        }
        return ret;
 }
};
class CMessageEventFilter_IsOfType:public std::unary_function<CMessageEvent,bool>{int Type;public:CMessageEventFilter_IsOfType(int t):Type(t){}bool operator()(const CMessageEvent& event)const{++type_calls;return Type==event.Type;}};
class CThingFilter_IsNot:public std::unary_function<CThing,bool>{const CThing& Thing;public:CThingFilter_IsNot(const CThing& thing):Thing(thing){}bool operator()(const CThing& thing)const{++not_calls;return &thing!=&Thing;}};
class CThingFilter_In2DRange:public std::unary_function<CThing,bool>{public:bool operator()(const CThing& thing)const{++range_calls;return thing.Nearby;}};
template<class T>class CMessageEventFilter_ThingSearchFilter:public std::unary_function<CMessageEvent,bool>{const T* Filter;
public:CMessageEventFilter_ThingSearchFilter(const T& filter):Filter(&filter){}
 bool operator()(const CMessageEvent& event)const{
  CThingAICreature* thing=event.GetPOriginator<CThingAICreature>();
  if(!thing)return false;
  return (*Filter)(*thing);
 }
};
template<class TF1,class TF2,class TF3> class CFilter_Bind3 {
protected:
 const TF1& F1;
 const TF2& F2;
 const TF3& F3;
public:
 CFilter_Bind3(const TF1& f1,const TF2& f2,const TF3& f3):F1(f1),F2(f2),F3(f3){}
 template<class T> bool operator()(const T& t)const{return F1(t)&&F2(t)&&F3(t);}
};

class CMessageEventManager {public:std::list<CMessageEvent> Messages;
 template<class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter_func) const {
  for (std::list<CMessageEvent>::const_iterator it = Messages.begin(); it != Messages.end(); ++it) {
   const CMessageEvent& event = *it;
   if (filter_func(event))
    return &event;
  }
  return 0;
 }

};
int main(){
 CThingAICreature self,nearby,faraway,wrong;self.ThingTypeFlags=nearby.ThingTypeFlags=faraway.ThingTypeFlags=0x1f;wrong.ThingTypeFlags=0x1e;
 self.Nearby=nearby.Nearby=wrong.Nearby=true;faraway.Nearby=false;
 CMessageEventFilter_IsOfType type(7);CThingFilter_IsNot isnot(self);CThingFilter_In2DRange range;
 CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNot> first(isnot);
 CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> second(range);
 CFilter_Bind3<CMessageEventFilter_IsOfType,CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNot>,CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> > filter(type,first,second);
 unsigned int cases=0;
 // Eight states: type miss,null,wrong type,self,far,success,expires on first result,expires before second search.
 for(unsigned int pattern=0;pattern<4096;++pattern){
  CMessageEventManager manager;unsigned int code=pattern;unsigned int kinds[4];
  for(int i=0;i<4;++i){unsigned int kind=code%8;code/=8;kinds[i]=kind;manager.Messages.push_back(CMessageEvent());CMessageEvent& e=manager.Messages.back();e.Type=kind==0?0:7;e.POriginator.Item=kind==1?0:kind==2?&wrong:kind==3?&self:kind==4?&faraway:&nearby;e.POriginator.ExpireAt=kind==6?3:kind==7?4:0;}
  const CMessageEvent* expected=0;unsigned int types=0,gets=0,nots=0,ranges=0;unsigned int index=0;
  for(std::list<CMessageEvent>::const_iterator it=manager.Messages.begin();it!=manager.Messages.end();++it,++index){
   ++types;unsigned int k=kinds[index];if(k==0)continue;
   if(k==1){gets+=1;continue;}if(k==2){gets+=2;continue;}gets+=3;if(k==6)continue;
   ++nots;if(k==3)continue;if(k==7){++gets;continue;}
   gets+=3;++ranges;if(k==4)continue;expected=&*it;break;
  }
  type_calls=get_calls=not_calls=range_calls=0;
  assert(manager.FindMostRecentMessage(filter)==expected);assert(type_calls==types&&get_calls==gets&&not_calls==nots&&range_calls==ranges);++cases;
 }
 CMessageEventManager empty;type_calls=get_calls=not_calls=range_calls=0;assert(empty.FindMostRecentMessage(filter)==0);assert(type_calls==0&&get_calls==0&&not_calls==0&&range_calls==0);++cases;
 assert(cases==4097);std::puts("00934e50_TEST PASS");return 0;
}