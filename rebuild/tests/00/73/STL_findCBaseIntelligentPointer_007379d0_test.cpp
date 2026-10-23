// 007379d0 _STL::find<CBaseIntelligentPointer*>
// Flags: /O2 /Oy /W3 
// Leaf: find
// Layout: CBaseIntelligentPointer is two dwords; GetPItem at00a01b50 is a landed symbol.
struct CBaseObjectPointer{void*object;unsigned refcount;};
struct CBaseIntelligentPointer{virtual ~CBaseIntelligentPointer();CBaseObjectPointer*PData;CBaseIntelligentPointer();void*GetPItem();void SetPItem(void*);};
struct random_access_iterator_tag {random_access_iterator_tag(){}};
namespace _STL {
CBaseIntelligentPointer* __fastcall find(CBaseIntelligentPointer*first,CBaseIntelligentPointer*last,CBaseIntelligentPointer&value,const random_access_iterator_tag&){
 int count=(last-first)>>2;
 for(;count>0;--count){
  if(first->GetPItem()==value.GetPItem())return first;++first;
  if(first->GetPItem()==value.GetPItem())return first;++first;
  if(first->GetPItem()==value.GetPItem())return first;++first;
  if(first->GetPItem()==value.GetPItem())return first;++first;
 }
 switch(last-first){
  case 3:if(first->GetPItem()==value.GetPItem())return first;++first;
  case 2:if(first->GetPItem()==value.GetPItem())return first;++first;
  case 1:if(first->GetPItem()==value.GetPItem())return first;
  default:return last;
 }
}
}

#include <stdio.h>
#include <assert.h>
static int liveLinks;
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){if(PData&&!--PData->refcount&&!PData->object){delete PData;--liveLinks;}}
void*CBaseIntelligentPointer::GetPItem(){if(!PData)return 0;if(PData->object)return PData->object;CBaseObjectPointer*p=PData;PData=0;if(!--p->refcount){delete p;--liveLinks;}return 0;}
void CBaseIntelligentPointer::SetPItem(void*p){assert(!PData);if(p){PData=new CBaseObjectPointer;PData->object=p;PData->refcount=1;++liveLinks;}}
int main(){assert(sizeof(CBaseIntelligentPointer)==8);{CBaseIntelligentPointer a[9],v;int values[10];for(int i=0;i<9;++i)a[i].SetPItem(&values[i]);random_access_iterator_tag t;for(int n=0;n<=9;++n){for(int k=0;k<10;++k){CBaseIntelligentPointer needle;needle.SetPItem(&values[k]);assert(_STL::find(a,a+n,needle,t)==(k<n?a+k:a+n));needle.PData->object=0;}}for(int q=0;q<9;++q)a[q].PData->object=0;assert(_STL::find(a,a+9,v,t)==a);for(int z=0;z<9;++z)assert(!a[z].GetPItem());}assert(!liveLinks);puts("007379d0_TEST PASS");}