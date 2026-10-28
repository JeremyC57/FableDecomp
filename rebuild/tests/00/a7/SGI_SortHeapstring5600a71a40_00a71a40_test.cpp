#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 c4593a94d8e26b17a0fa12b1a67e242842b74cbd042ce8927bf20949c41616b4
// 00a71a40 SGI::SortHeap_string56_00a71a40
// Flags: /O2 /Oy /Gr
// Leaf: SortHeap
// Layout: no engine CCharString header exists; retail holds one data pointer, ctor 0099ec30, assignment 0099efb0, dtor 0099eae0.
class CCharString {public: void* data; CCharString():data(0){} CCharString(const CCharString&);~CCharString();const CCharString& operator=(const CCharString&);};
struct Payload_00a71a40 {int data[13];};
template<class K,class V> struct Pair_00a71a40 { K first;V second;Pair_00a71a40(){}Pair_00a71a40(const Pair_00a71a40& p):first(p.first),second(p.second){} };
typedef Pair_00a71a40<CCharString,Payload_00a71a40> Record_00a71a40;
struct Compare_00a71a40 {int state;bool operator()(const Record_00a71a40&,const Record_00a71a40&)const;};

template<class T,class C> void PushHeap(T* first,int hole,int top,T value,C comp) {
 int parent=(hole-1)/2;
 while(hole>top && comp(first[parent],value)) {
  first[hole]=first[parent];hole=parent;parent=(hole-1)/2;
 }
 first[hole]=value;
}
template<class T,class C> __declspec(noinline) void AdjustHeap(T* first,int hole,int length,T value,C comp) {
 int top=hole;int second=2*hole+2;
 while(second<length){
  if(comp(first[second],first[second-1]))--second;
  first[hole]=first[second];hole=second;second=2*(second+1);
 }
 if(second==length){first[hole]=first[second-1];hole=second-1;}
 PushHeap(first,hole,top,value,comp);
}
template<class T,class C> __forceinline void PopHeap(T* first,T* last,T* dest,T value,C comp) {
 *dest=*first;AdjustHeap(first,0,int(last-first),value,comp);
}
template<class T,class C> void SortHeap(T* first,T* last,C comp) {
 for(;last-first>1;--last)PopHeap(first,last-1,last-1,*(last-1),comp);
}
template void SortHeap<Record_00a71a40,Compare_00a71a40>(Record_00a71a40*,Record_00a71a40*,Compare_00a71a40);

// END exact source snapshot
bool Compare_00a71a40::operator()(const Record_00a71a40&a,const Record_00a71a40&b)const{return *(int*)a.first.data<*(int*)b.first.data;}
CCharString::CCharString(const CCharString& p):data(p.data){} CCharString::~CCharString(){} const CCharString& CCharString::operator=(const CCharString& p){data=p.data;return *this;}

int main(){const int n=5;int keys[n]={7,5,6,3,4};Record_00a71a40 v[n];Compare_00a71a40 comp;comp.state=0;int i;
for(i=0;i<n;++i){v[i].first.data=&keys[i];for(int j=0;j<13;++j)v[i].second.data[j]=keys[i]*10+j;}

SortHeap(v,v,comp);SortHeap(v,v+1,comp);SortHeap(v,v+n,comp);for(i=1;i<n;++i)assert(!comp(v[i],v[i-1]));
for(i=0;i<n;++i)for(int j=0;j<13;++j)assert(v[i].second.data[j]==*(int*)v[i].first.data*10+j);puts("00a71a40_TEST PASS");return 0;}