#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#define FABLE_ENGINE_CAISTATEGROUP_CREATUREUSECOVERBASE_H
struct CAIStateGroup_CreatureUseCoverBase {void* __vftable;void* CoverList[3];long FailedToFindCoverAt_Z;};
#endif
// CAIStateGroup_CreatureUseCoverBase::RefreshCoverList() -- refill retained cover handles from a filtered temporary list.
// Retail009710D0,250 bytes. Retail vector is12 bytes versus donor16: ValidPositionsFound consequently lives at48 (donor FailedToFindCoverAt_Z).
#include <stddef.h>
#include <stdlib.h>
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
inline void* operator new(size_t,void* p){return p;}
struct CThingRetail;
struct CIntelligentPointerBaseRetail {virtual ~CIntelligentPointerBaseRetail();void* Data; CIntelligentPointerBaseRetail(); CThingRetail* GetPItem()const;void SetPItem(CThingRetail*);}; //A01B10,A01B50,A01B90,A01C10
struct CIntelligentPointerRetail:CIntelligentPointerBaseRetail {
 __forceinline CIntelligentPointerRetail(CThingRetail* p){SetPItem(p);}
 __forceinline CIntelligentPointerRetail(const CIntelligentPointerRetail& p){SetPItem(p.GetPItem());}
};
struct CAllocatorTag {CAllocatorTag(){}};
struct CHandleVector {CIntelligentPointerRetail* first;CIntelligentPointerRetail* last;CIntelligentPointerRetail* capacity;
 void Erase(CIntelligentPointerRetail*,CIntelligentPointerRetail*); //004AC0F0
 void InsertOverflow(CIntelligentPointerRetail*,const CIntelligentPointerRetail&,const CAllocatorTag&,unsigned long,bool); //004AC940
 __forceinline void clear(){Erase(first,last);}
 __forceinline void push_back(const CIntelligentPointerRetail& p){if(last!=capacity){if(last)new(last)CIntelligentPointerRetail(p);++last;}else InsertOverflow(last,p,CAllocatorTag(),1,true);}
};
struct CThingVector {CThingRetail** first;CThingRetail** last;CThingRetail** capacity;
 CThingVector():first(0),last(0),capacity(0){}
 ~CThingVector(){if(first)free(first);}
 bool empty()const{return first==last;}
};
struct CCoverDispatch {
 virtual void s00();virtual void s01();virtual void s02();virtual void s03();virtual void s04();virtual void s05();virtual void s06();virtual void s07();virtual void s08();virtual void s09();virtual void s10();virtual void s11();virtual void s12();virtual void s13();virtual void s14();virtual void s15();virtual void s16();virtual void s17();virtual void s18();virtual void s19();virtual void s20();virtual void s21();
 virtual void FilterCoverList(CThingVector&);virtual void GetCoverThingList(CThingVector&);
};
struct UseCoverMethods:CAIStateGroup_CreatureUseCoverBase {void RefreshCoverList(); __forceinline CHandleVector& Covers(){return *(CHandleVector*)CoverList;} __forceinline long& RetailValidPositionsFound(){return *(long*)&FailedToFindCoverAt_Z;}};
void UseCoverMethods::RefreshCoverList(){
 Covers().clear(); RetailValidPositionsFound()=0;
 CThingVector things;
 ((CCoverDispatch*)this)->GetCoverThingList(things);
 if(!things.empty()){
  ((CCoverDispatch*)this)->FilterCoverList(things);
  for(CThingRetail** it=things.first;it!=things.last;++it)Covers().push_back(CIntelligentPointerRetail(*it));
 }
}

struct CThingRetail {int id,refs;};
static int errors,erases,getCalls,filters,overflows,ctors,dtors,mode;
static UseCoverMethods* group;
static CThingRetail a={1,0},b={2,0},c={3,0},old={4,0};
CIntelligentPointerBaseRetail::CIntelligentPointerBaseRetail():Data(0){++ctors;}
CIntelligentPointerBaseRetail::~CIntelligentPointerBaseRetail(){if(Data)--((CThingRetail*)Data)->refs;++dtors;}
CThingRetail* CIntelligentPointerBaseRetail::GetPItem()const{return (CThingRetail*)Data;}
void CIntelligentPointerBaseRetail::SetPItem(CThingRetail* p){if(Data)--((CThingRetail*)Data)->refs;Data=p;if(p)++p->refs;}
void CHandleVector::Erase(CIntelligentPointerRetail* begin,CIntelligentPointerRetail* end){++erases;if(begin!=first||end!=last)++errors;for(CIntelligentPointerRetail* p=begin;p!=end;++p)p->~CIntelligentPointerRetail();last=first;}
void CHandleVector::InsertOverflow(CIntelligentPointerRetail* at,const CIntelligentPointerRetail& item,const CAllocatorTag&,unsigned long n,bool copy){
 ++overflows;if(at!=last||n!=1||!copy)++errors;
 size_t count=last-first;CIntelligentPointerRetail* fresh=(CIntelligentPointerRetail*)malloc((count+4)*sizeof(CIntelligentPointerRetail));
 for(size_t i=0;i<count;++i){new(fresh+i)CIntelligentPointerRetail(first[i]);first[i].~CIntelligentPointerRetail();}
 free(first);first=fresh;last=fresh+count;capacity=fresh+count+4;new(last)CIntelligentPointerRetail(item);++last;
}
void CCoverDispatch::s00(){}
void CCoverDispatch::s01(){}
void CCoverDispatch::s02(){}
void CCoverDispatch::s03(){}
void CCoverDispatch::s04(){}
void CCoverDispatch::s05(){}
void CCoverDispatch::s06(){}
void CCoverDispatch::s07(){}
void CCoverDispatch::s08(){}
void CCoverDispatch::s09(){}
void CCoverDispatch::s10(){}
void CCoverDispatch::s11(){}
void CCoverDispatch::s12(){}
void CCoverDispatch::s13(){}
void CCoverDispatch::s14(){}
void CCoverDispatch::s15(){}
void CCoverDispatch::s16(){}
void CCoverDispatch::s17(){}
void CCoverDispatch::s18(){}
void CCoverDispatch::s19(){}
void CCoverDispatch::s20(){}
void CCoverDispatch::s21(){}
void CCoverDispatch::GetCoverThingList(CThingVector& v){
 ++getCalls;if(group->Covers().first!=group->Covers().last||group->RetailValidPositionsFound()!=0||v.first||v.last||v.capacity)++errors;
 if(mode==0)return;
 v.first=(CThingRetail**)malloc(3*sizeof(CThingRetail*));v.last=v.capacity=v.first+3;v.first[0]=&a;v.first[1]=&b;v.first[2]=&c;
}
void CCoverDispatch::FilterCoverList(CThingVector& v){++filters;if(mode==2)v.last=v.first;else if(mode==3){v.first[0]=&c;v.first[1]=&a;v.last=v.first+2;}else if(mode==4){v.first[0]=0;v.first[1]=&a;v.first[2]=&a;}}
int main(){
 CCoverDispatch dispatch;
 for(mode=0;mode<5;++mode){
  UseCoverMethods object;group=&object;object.__vftable=*(void**)&dispatch;
  CHandleVector& handles=object.Covers();size_t capacity=mode==1?1:4;
  handles.first=(CIntelligentPointerRetail*)malloc(capacity*sizeof(CIntelligentPointerRetail));handles.last=handles.first;handles.capacity=handles.first+capacity;
  new(handles.last++)CIntelligentPointerRetail(&old);object.RetailValidPositionsFound()=99;
  int oldGets=getCalls,oldFilters=filters,oldErases=erases,oldOverflow=overflows;
  object.RefreshCoverList();
  int expected=mode==0||mode==2?0:mode==3?2:3;
  if(handles.last-handles.first!=expected||getCalls!=oldGets+1||erases!=oldErases+1||filters!=oldFilters+(mode!=0)||old.refs||object.RetailValidPositionsFound()!=0)++errors;
  if(mode==1&&(handles.first[0].GetPItem()!=&a||handles.first[1].GetPItem()!=&b||handles.first[2].GetPItem()!=&c||overflows!=oldOverflow+1))++errors;
  if(mode==3&&(handles.first[0].GetPItem()!=&c||handles.first[1].GetPItem()!=&a))++errors;
  if(mode==4&&(handles.first[0].GetPItem()!=0||a.refs!=2||b.refs||c.refs))++errors;
  handles.Erase(handles.first,handles.last);free(handles.first);
  if(a.refs||b.refs||c.refs||old.refs||ctors!=dtors)++errors;
 }
 if(errors){std::printf("009710d0_TEST FAIL %d\n",errors);return 1;}std::printf("009710d0_TEST PASS\n");return 0;
}