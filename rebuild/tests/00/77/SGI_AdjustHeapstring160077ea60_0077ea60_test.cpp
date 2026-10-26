#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 9684f90ce1c9799b436f57cb12df990813603b633b15610941495352e1754926
// 0077ea60 SGI::AdjustHeap_string16_0077ea60
// Flags: /O2 /Oy /Gr
// Leaf: AdjustHeap
// Layout: no engine CCharString header exists; retail holds one data pointer, ctor 0099ec30, assignment 0099efb0, dtor 0099eae0.
class CCharString {public: void* data; CCharString():data(0){} CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
struct Payload_0077ea60 {int data[3];Payload_0077ea60(){}__forceinline Payload_0077ea60(const Payload_0077ea60& o){data[0]=o.data[0];data[1]=o.data[1];data[2]=o.data[2];}};
template<class K,class V> struct Pair_0077ea60 { K first;V second;Pair_0077ea60(){}Pair_0077ea60(const Pair_0077ea60& p):first(p.first),second(p.second){} };
typedef Pair_0077ea60<CCharString,Payload_0077ea60> Record_0077ea60;
// Layout: string-data comparison helper is retail 00429950; null and identity tests are inline.
template<class Char>class CBasicString { public:const Char* text;bool operator<(const CBasicString&)const;};
struct Compare_0077ea60 {int state;bool operator()(const Record_0077ea60&a,const Record_0077ea60&b)const {CBasicString<char>* y=(CBasicString<char>*)b.first.data;CBasicString<char>* x=(CBasicString<char>*)a.first.data;return y!=x && (!x || (y && *x<*y));}};

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
template void AdjustHeap<Record_0077ea60,Compare_0077ea60>(Record_0077ea60*,int,int,Record_0077ea60,Compare_0077ea60);

// END exact source snapshot
template<class Char>bool CBasicString<Char>::operator<(const CBasicString& p)const{const signed char* a=(const signed char*)text;const signed char* b=(const signed char*)p.text;for(;*a || *b;++a,++b){if(*a!=*b)return *a<*b;}return false;}
CCharString::CCharString(const CCharString& p):data(p.data){} CCharString::~CCharString(){} CCharString& CCharString::operator=(const CCharString& p){data=p.data;return *this;}

int main(){const int n=5;int keys[n]={7,5,6,3,4};char letters[n][2];CBasicString<char> names[n];Record_0077ea60 v[n];Compare_0077ea60 comp;comp.state=0;int i;
for(i=0;i<n;++i){letters[i][0]=char(keys[i]+'0');letters[i][1]=0;names[i].text=letters[i];v[i].first.data=&names[i];for(int j=0;j<3;++j)v[i].second.data[j]=keys[i]*10+j;}

Record_0077ea60 value;CBasicString<char> vk;vk.text="8";value.first.data=&vk;for(int j=0;j<3;++j)value.second.data[j]=80+j; AdjustHeap(v,0,n,value,comp);assert(((CBasicString<char>*)v[0].first.data)->text[0]=='8');
for(i=0;i<n;++i)for(int j=0;j<3;++j)assert(v[i].second.data[j]==(((CBasicString<char>*)v[i].first.data)->text[0]-'0')*10+j);Record_0077ea60 empty;empty.first.data=0;assert(comp(empty,v[0]));assert(!comp(v[0],empty));assert(!comp(v[0],v[0]));puts("0077ea60_TEST PASS");return 0;}