#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 622d950b6d126db7d6fdd71b29d0cc8e6369783b584ee9b6d566a57eb7761301
// 00a6fa20 SGI::AdjustHeap_string20_00a6fa20
// Flags: /O2 /Oy /Gr
// Leaf: AdjustHeap
// Layout: no engine CCharString header exists; retail holds one data pointer, ctor 0099ec30, assignment 0099efb0, dtor 0099eae0.
class CCharString {public: void* data; CCharString():data(0){} CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);bool FastCompare(const CCharString&)const;};
struct Payload_00a6fa20 {int data[4];Payload_00a6fa20(){}__forceinline Payload_00a6fa20(const Payload_00a6fa20& o){data[0]=o.data[0];data[1]=o.data[1];data[2]=o.data[2];data[3]=o.data[3];}};
template<class K,class V> struct Pair_00a6fa20 { K first;V second;Pair_00a6fa20(){}Pair_00a6fa20(const Pair_00a6fa20& p):first(p.first),second(p.second){} };
typedef Pair_00a6fa20<CCharString,Payload_00a6fa20> Record_00a6fa20;
struct Compare_00a6fa20 {int state;bool operator()(const Record_00a6fa20&a,const Record_00a6fa20&b)const{return a.first.FastCompare(b.first);}};

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
template void AdjustHeap<Record_00a6fa20,Compare_00a6fa20>(Record_00a6fa20*,int,int,Record_00a6fa20,Compare_00a6fa20);

// END exact source snapshot
struct TestString {unsigned int key;};
struct TestStringData {TestString* text;unsigned int size;};
bool CCharString::FastCompare(const CCharString& b)const{const TestStringData* x=(const TestStringData*)data;const TestStringData* y=(const TestStringData*)b.data;if(x==y)return false;if(!x)return true;if(!y)return false;if(x->size!=y->size)return x->size<y->size;for(unsigned int i=0;i<x->size;i+=4){unsigned int l=*(const unsigned int*)((const char*)x->text+i),r=*(const unsigned int*)((const char*)y->text+i);if(l!=r)return l<r;}return false;}
CCharString::CCharString(const CCharString& p):data(p.data){} CCharString::~CCharString(){} CCharString& CCharString::operator=(const CCharString& p){data=p.data;return *this;}

int main(){const int n=5;int keys[n]={7,5,6,3,4};TestString items[n];TestStringData text[n];Record_00a6fa20 v[n];Compare_00a6fa20 comp;comp.state=0;int i;
for(i=0;i<n;++i){items[i].key=keys[i];text[i].text=&items[i];text[i].size=4;v[i].first.data=&text[i];for(int j=0;j<4;++j)v[i].second.data[j]=keys[i]*10+j;}

Record_00a6fa20 value;TestString vk;vk.key=8;TestStringData valueData;valueData.text=&vk;valueData.size=4;value.first.data=&valueData;for(int j=0;j<4;++j)value.second.data[j]=80+j; AdjustHeap(v,0,n,value,comp);assert(((TestStringData*)v[0].first.data)->text->key==8);
for(i=0;i<n;++i)for(int j=0;j<4;++j)assert(v[i].second.data[j]==((TestStringData*)v[i].first.data)->text->key*10+j);CCharString empty;assert(empty.FastCompare(v[0].first));assert(!v[0].first.FastCompare(empty));assert(!v[0].first.FastCompare(v[0].first));TestStringData shorter;shorter.text=&vk;shorter.size=0;CCharString shortString;shortString.data=&shorter;assert(shortString.FastCompare(v[0].first));puts("00a6fa20_TEST PASS");return 0;}