// 006c4060 std::find<CBaseIntelligentPointer*,CBaseIntelligentPointer>
// Flags: /Gr
// Leaf: ??$find@UCBaseIntelligentPointer@FableFind_006c4060@@U12@@FableFind_006c4060@@YIPAUCBaseIntelligentPointer@0@PAU10@0ABU10@ABUrandom_access_iterator_tag@0@@Z
// Layout: CBaseIntelligentPointer stride8, GetPItem00a01b50 reads pointer+4; SGI random-access find unroll4.
namespace FableFind_006c4060 {
struct CBaseIntelligentPointer {void* __vftable;mutable void* PData;void* GetPItem()const;__forceinline bool operator==(const CBaseIntelligentPointer& p)const{return GetPItem()==p.GetPItem();}};
struct random_access_iterator_tag {random_access_iterator_tag(){}};
template<class T,class V> T* find(T* first,T* last,const V& value,const random_access_iterator_tag&){
 int count=(last-first)>>2;
 for(;count>0;--count){
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
  if(*first==value)return first;++first;
 }
 switch(last-first){
 case 3:if(*first==value)return first;++first;
 case 2:if(*first==value)return first;++first;
 case 1:if(*first==value)return first;
 default:return last;
 }
}
template CBaseIntelligentPointer* find<CBaseIntelligentPointer,CBaseIntelligentPointer>(CBaseIntelligentPointer*,CBaseIntelligentPointer*,const CBaseIntelligentPointer&,const random_access_iterator_tag&);
}

#include <stdio.h>
#include <assert.h>
namespace FableFind_006c4060 {
struct CBaseObjectPointer {void* Object;unsigned int Count;};
void* CBaseIntelligentPointer::GetPItem()const{if(PData){CBaseObjectPointer* p=(CBaseObjectPointer*)PData;if(p->Object)return p->Object;if(--p->Count==0 && !p->Object)delete p;PData=0;}return 0;}
}
int main(){using namespace FableFind_006c4060;CBaseIntelligentPointer v[12],value;CBaseObjectPointer data[13];random_access_iterator_tag tag;int i;
for(i=0;i<13;++i){data[i].Object=(void*)(i+1);data[i].Count=1;}for(i=0;i<12;++i){v[i].__vftable=0;v[i].PData=&data[i];}value.__vftable=0;
for(int n=0;n<=12;++n){value.PData=&data[12];assert(find(v,v+n,value,tag)==v+n);for(i=0;i<n;++i){value.PData=&data[i];assert(find(v,v+n,value,tag)==v+i);}}
CBaseObjectPointer* dead=new CBaseObjectPointer;dead->Object=0;dead->Count=1;v[0].PData=dead;value.PData=0;assert(find(v,v+4,value,tag)==v && v[0].PData==0);
puts("006c4060_TEST PASS");return 0;}