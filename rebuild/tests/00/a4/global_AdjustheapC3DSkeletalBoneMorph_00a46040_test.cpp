#include <assert.h>
#include <stdio.h>
// BEGIN exact source snapshot SHA256 85a5083890452c5a3ed8355bf30f6b4925b9aafe2e259b0ed7d7b8fd4b21483a
// 00a46040 _Adjust_heap<C3DSkeletalBoneMorph*>
// Flags: /O2 /Oy /Gr
// Leaf: _Adjust_heap
// Layout: retail stride20: CCharString +0, scalar +4, vector +8. String copy99EC30 assign99EFB0 dtor99EAE0.
struct StringStorage;
struct CCharString {StringStorage* data;CCharString();CCharString(const CCharString&);CCharString& operator=(const CCharString&);~CCharString();};
struct MorphVector {float x,y,z;};
struct C3DSkeletalBoneMorph {CCharString name;float weight;MorphVector displacement; C3DSkeletalBoneMorph(){} C3DSkeletalBoneMorph(const C3DSkeletalBoneMorph&b):name(b.name){weight=b.weight;displacement.x=b.displacement.x;displacement.y=b.displacement.y;displacement.z=b.displacement.z;}};
typedef bool (__fastcall* MorphCompare)(const C3DSkeletalBoneMorph&,const C3DSkeletalBoneMorph&);
template<class T,class Compare> __declspec(noinline) void __fastcall _Push_heap(T*first,int hole,int top,T value,Compare comp){int parent=(hole-1)/2;while(hole>top&&comp(first[parent],value)){first[hole]=first[parent];hole=parent;parent=(hole-1)/2;}first[hole]=value;}
template<class T,class Compare> void __fastcall _Adjust_heap(T*first,int hole,int length,T value,Compare comp){int top=hole;int child=hole*2+2;while(child<length){if(comp(first[child],first[child-1]))--child;first[hole]=first[child];hole=child;child=2*(child+1);}if(child==length){first[hole]=first[child-1];hole=child-1;}_Push_heap(first,hole,top,value,comp);}
template void __fastcall _Adjust_heap(C3DSkeletalBoneMorph*,int,int,C3DSkeletalBoneMorph,MorphCompare);

// END exact source snapshot
struct StringStorage {int refs;int value;};
CCharString::CCharString():data(0){}
CCharString::CCharString(const CCharString&x):data(x.data){if(data)++data->refs;}
CCharString& CCharString::operator=(const CCharString&x){if(&x!=this){if(data&&!--data->refs)delete data;data=x.data;if(data)++data->refs;}return *this;}
CCharString::~CCharString(){if(data&&!--data->refs)delete data;}
bool __fastcall compare(const C3DSkeletalBoneMorph&a,const C3DSkeletalBoneMorph&b){return a.weight<b.weight;}
int main(){C3DSkeletalBoneMorph a[5];int k[5]={1,9,8,7,6};for(int i=0;i<5;++i){a[i].weight=(float)k[i];a[i].name.data=new StringStorage;a[i].name.data->refs=1;a[i].name.data->value=k[i];a[i].displacement.x=(float)k[i];a[i].displacement.y=0.0f;a[i].displacement.z=0.0f;}_Adjust_heap(a,0,5,a[0],compare);assert(a[0].weight==9);for(int i=0;i<5;++i)assert(a[i].weight==a[i].name.data->value&&a[i].displacement.x==a[i].weight);_Adjust_heap(a,0,1,a[0],compare);assert(a[0].weight==9);puts("00a46040_TEST PASS");}