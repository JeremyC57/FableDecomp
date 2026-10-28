#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 97cb33e416753897274c0041268bd484c8dba6d17ef2c57226ca7fe51b404762
// 00cb9fa0 SGI::AdjustHeap_script24_00cb9fa0
// Flags: /O2 /Oy /Gr
// Leaf: AdjustHeap
// Layout: no engine CCharString header exists; retail holds one data pointer, ctor 0099ec30, assignment 0099efb0, dtor 0099eae0.
class CCharString {public: void* data; CCharString():data(0){} CCharString(const CCharString&);~CCharString();const CCharString& operator=(const CCharString&);};
template<class K,class V> struct Pair_00cb9fa0 { K first;V second;Pair_00cb9fa0(){}Pair_00cb9fa0(const Pair_00cb9fa0& p):first(p.first),second(p.second){} };
// Layout: measured pair stride24; UID at +0, three-word persist info at +8; final string at +16 has out-of-line assignment.
struct Persist_00cb9fa0 {int a,b;CCharString name;};typedef Pair_00cb9fa0<unsigned __int64,Persist_00cb9fa0> Record_00cb9fa0;
struct Compare_00cb9fa0 {int state;bool operator()(const Record_00cb9fa0&a,const Record_00cb9fa0&b)const{return a.first<b.first;} };

template<class T,class C> void PushHeap(T* first,int hole,int top,T value,C comp) {
 int parent=(hole-1)/2;
 while(hole>top && comp(first[parent],value)) {
  first[hole]=first[parent];hole=parent;parent=(hole-1)/2;
 }
 first[hole]=value;
}
template<class T,class C> void AdjustHeap(T* first,int hole,int length,T value,C comp) {
 int top=hole;int second=2*hole+2;
 while(second<length){
  if(comp(first[second],first[second-1]))--second;
  first[hole]=first[second];hole=second;second=2*(second+1);
 }
 if(second==length){first[hole]=first[second-1];hole=second-1;}
 PushHeap(first,hole,top,value,comp);
}
template<class T,class C> inline void PopHeap(T* first,T* last,T* dest,T value,C comp) {
 *dest=*first;AdjustHeap(first,0,int(last-first),value,comp);
}
template<class T,class C> void SortHeap(T* first,T* last,C comp) {
 for(;last-first>1;--last)PopHeap(first,last-1,last-1,*(last-1),comp);
}
template void AdjustHeap<Record_00cb9fa0,Compare_00cb9fa0>(Record_00cb9fa0*,int,int,Record_00cb9fa0,Compare_00cb9fa0);

// END exact source snapshot
CCharString::CCharString(const CCharString& p):data(p.data){} CCharString::~CCharString(){} const CCharString& CCharString::operator=(const CCharString& p){data=p.data;return *this;}

int main(){const int n=5;unsigned __int64 base=((unsigned __int64)1)<<40;unsigned __int64 keys[n]={base+7,base+5,base+6,base+3,base+4};Record_00cb9fa0 v[n];Compare_00cb9fa0 comp;comp.state=0;int i;
for(i=0;i<n;++i){v[i].first=keys[i];v[i].second.a=int(keys[i]-base)*10;v[i].second.b=int(keys[i]-base)*20;v[i].second.name.data=&keys[i];}

Record_00cb9fa0 value;unsigned __int64 newKey=base+8;value.first=newKey;value.second.a=80;value.second.b=160;value.second.name.data=&newKey; AdjustHeap(v,0,n,value,comp);assert(v[0].first==base+8);
for(i=0;i<n;++i){assert(v[i].second.a==int(v[i].first-base)*10);assert(v[i].second.b==int(v[i].first-base)*20);assert(*(unsigned __int64*)v[i].second.name.data==v[i].first);}puts("00cb9fa0_TEST PASS");return 0;}