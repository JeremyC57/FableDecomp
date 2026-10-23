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