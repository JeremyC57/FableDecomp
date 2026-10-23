// 00cbcfc0 PartialSort
// Flags: /O2 /Oy /W3 /Gr
// Leaf: PartialSort
// Layout: SGI template iterator stride, comparator dword and ret cleanup from oracle.tsv.
#include <new>
namespace N00cbcfc0 {

struct Compare {unsigned int State;};

struct Block {unsigned int Count;void (__fastcall *Destroy)(void*);void* Object;};
struct Pointer {void* Object;Block* Control; __forceinline Block* GetControl()const{return Control;}Pointer():Object(0),Control(0){} __forceinline Pointer(const Pointer& o){Object=o.Object;Control=o.Control;if(Control)++Control->Count;} __forceinline void Release(){if(Control){--Control->Count;if(Control->Count==0){Control->Destroy(Control->Object);::operator delete(GetControl());}}} __forceinline ~Pointer(){Release();} __forceinline Pointer& operator=(const Pointer& o){Block* b=o.Control;void* p=o.Object;if(Control!=b){Release();Object=p;Control=b;if(b)++b->Count;}return *this;}};
struct Element {unsigned __int64 Key;Pointer P;};
#pragma auto_inline(off)
template<class T,class C> __declspec(noinline) void __fastcall AdjustHeap(T* first,int hole,int length,T value,C comp){int top=hole;int child=hole*2+2;while(child<length){if(first[child].Key<first[child-1].Key)--child;first[hole]=first[child];hole=child;child=hole*2+2;}if(child==length){first[hole]=first[child-1];hole=child-1;}int parent=(hole-1)/2;while(hole>top&&first[parent].Key<value.Key){first[hole]=first[parent];hole=parent;parent=(hole-1)/2;}first[hole]=value;}
template<class T,class C> __declspec(noinline) void __fastcall MakeHeap(T* first,T* last,C c,int*,int*){int n=int(last-first);if(n<2)return;int parent=(n-2)/2;for(;;){AdjustHeap(first,parent,n,first[parent],c);if(parent==0)return;--parent;}}
template<class T,class C> __declspec(noinline) void __fastcall SortHeap(T* first,T* last,C c){while(last-first>1){--last;T value=*last;*last=*first;AdjustHeap(first,0,int(last-first),value,c);}}
#pragma auto_inline(on)
template<class T,class C> void __fastcall PartialSort(T* first,T* middle,T* last,int*,C comp) {
 MakeHeap(first,middle,comp,(int*)0,(int*)0);
 for(T* p=middle;p<last;++p) if(p->Key<first->Key) {T value=*p;*p=*first;AdjustHeap(first,0,int(middle-first),value,comp);}
 SortHeap(first,middle,comp);
}
template void __fastcall PartialSort<Element,Compare>(Element*,Element*,Element*,int*,Compare);

}

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
namespace N00cbcfc0 {
static int live,deleted;
struct Payload{unsigned __int64 key;};
void __fastcall DestroyPayload(void*p){assert(p);--live;++deleted;delete(Payload*)p;}
void Init(Element&e,unsigned __int64 key){e.Key=key;e.P.Object=new Payload;((Payload*)e.P.Object)->key=key;e.P.Control=new Block;e.P.Control->Count=1;e.P.Control->Destroy=DestroyPayload;e.P.Control->Object=e.P.Object;++live;}
void Check(Element*first,Element*last){for(;first!=last;++first){assert(first->P.Object&&first->P.Control&&first->P.Control->Count==1);assert(first->P.Control->Object==first->P.Object);assert(((Payload*)first->P.Object)->key==first->Key);}}
}
int main(){using namespace N00cbcfc0;assert(sizeof(Element)==16&&sizeof(Pointer)==8&&sizeof(Block)==12&&offsetof(Element,P)==8);Compare c;c.State=0;
{Element a[8];unsigned __int64 high=((unsigned __int64)1<<32);unsigned __int64 keys[8]={high+4,2,high+2,1,high+1,3,0,high};for(int i=0;i<8;++i)Init(a[i],keys[i]);PartialSort(a,a+4,a+8,(int*)0,c);for(int j=0;j<4;++j)assert(a[j].Key==(unsigned)j);Check(a,a+8);assert(live==8&&!deleted);
PartialSort(a,a+8,a+8,(int*)0,c);for(int k=0;k<4;++k)assert(a[k].Key==(unsigned)k);assert(a[4].Key==high&&a[5].Key==high+1&&a[6].Key==high+2&&a[7].Key==high+4);Check(a,a+8);}
assert(!live&&deleted==8);
{Element one;Init(one,7);PartialSort(&one,&one+1,&one+1,(int*)0,c);Check(&one,&one+1);assert(one.Key==7);}
{Element two[2];Init(two[0],9);Init(two[1],4);PartialSort(two,two+1,two+2,(int*)0,c);assert(two[0].Key==4&&two[1].Key==9);Check(two,two+2);}
{Element empty;PartialSort(&empty,&empty,&empty,(int*)0,c);}
assert(!live&&deleted==11);puts("00cbcfc0_TEST PASS");return 0;}