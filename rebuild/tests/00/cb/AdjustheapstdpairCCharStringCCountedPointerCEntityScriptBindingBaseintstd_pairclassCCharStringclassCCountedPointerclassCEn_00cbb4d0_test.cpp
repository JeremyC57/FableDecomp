// 00cbb4d0 AdjustHeap
// Flags: /O2 /Oy /W3 /Gr
// Leaf: AdjustHeap
// Layout: SGI heap iterator stride and by-value element/comparator sizes are taken from oracle indexed accesses and ret cleanup.
#include <new>
#include <string.h>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
#pragma intrinsic(memcpy)
namespace N00cbb4d0 {

struct RefBlock { unsigned int Refs; void (__fastcall *Destroy)(void*); void* Data; };
struct Base { virtual void v00(); ~Base(); };
template<class T> struct Counted {
 T* Data; RefBlock* Block;
 __forceinline Counted():Data(0),Block(0) {}
 __forceinline Counted(const Counted& o) {Data=o.Data;Block=o.Block;if(Block) ++Block->Refs;}
 __forceinline ~Counted() { Release(); Data=0;Block=0; }
 __forceinline RefBlock* GetControl()const{return Block;}
 __forceinline void Release() { if(Block) {--Block->Refs; if(Block->Refs==0) {Block->Destroy(Block->Data); ::operator delete(GetControl());}} }
 __forceinline Counted& operator=(const Counted& o) {RefBlock* block=o.Block; T* data=o.Data; if(Block!=block){Release();Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
template<class T> struct PolyCounted:Base {
 T* Data;RefBlock* Block;
 __forceinline PolyCounted():Data(0),Block(0) {}
 __forceinline PolyCounted(const PolyCounted& o):Data(o.Data),Block(o.Block) {if(Block)++Block->Refs;}
 __forceinline ~PolyCounted(){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=0;Block=0;}
 __forceinline PolyCounted& operator=(const PolyCounted& o){RefBlock* block=o.Block;T* data=o.Data;if(Block!=block){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
// Layout:99ec30 copy shares data and reads refcount+13;99e9b0 releases chararray then17-byte block.
#pragma pack(push,1)
struct StringData{char*Data;unsigned Length,Capacity;unsigned char Fast;int Refs;};
#pragma pack(pop)
extern int StringInstances;
struct String {
 StringData*Data;
 __declspec(noinline) void Clear();
 __declspec(noinline) String();
 __declspec(noinline) String(const String&v);
 __declspec(noinline) ~String();
 __declspec(noinline) String&operator=(const String&v);
 __declspec(noinline) bool FastCompare(const String&v)const;
};
struct WideString {unsigned int Key; WideString();WideString(const WideString&);~WideString();WideString& operator=(const WideString&);};
struct Element {String S;Counted<void> P;};
struct Compare {unsigned int State; __forceinline bool operator()(const Element& a,const Element& b) const {return a.S.FastCompare(b.S);}};

template<class T,class C> void __fastcall PushHeap(T* first,int hole,int top,T value,C comp);
template<class T,class C> void __fastcall AdjustHeap(T* first,int hole,int length,T value,C comp) {
 int top=hole;
 int second=2*hole+2;
 while(second<length) {
  if(comp(first[second],first[second-1])) --second;
  first[hole]=first[second];
  hole=second;second=2*(second+1);
 }
 if(second==length) {first[hole]=first[second-1];hole=second-1;}
 PushHeap(first,hole,top,value,comp);
}

template void __fastcall AdjustHeap<Element,Compare>(Element*,int,int,Element,Compare);
}

namespace N00cbb4d0 {
 __declspec(noinline) void String::Clear(){if(Data){if(--Data->Refs<=0){if(Data->Data){delete[]Data->Data;Data->Data=0;}Data->Length=0;Data->Capacity&=0x80000000;delete Data;}Data=0;}}
 __declspec(noinline) String::String():Data(0){++StringInstances;}
 __declspec(noinline) String::String(const String&v):Data(0){++StringInstances;if(this!=&v){if(Data)Clear();if(v.Data){Data=v.Data;++Data->Refs;}}}
 __declspec(noinline) String::~String(){Clear();--StringInstances;}
 __declspec(noinline) String&String::operator=(const String&v){if(this!=&v){if(Data)Clear();if(v.Data){Data=v.Data;++Data->Refs;}}return *this;}
 __declspec(noinline) bool String::FastCompare(const String&v)const{if(Data==v.Data)return false;if(!Data)return true;if(!v.Data)return false;if(Data->Length<v.Data->Length)return true;if(Data->Length>v.Data->Length)return false;for(unsigned i=0;i<Data->Length;i+=4){unsigned left,right;memcpy(&left,Data->Data+i,4);memcpy(&right,v.Data->Data+i,4);if(left<right)return true;if(left>right)return false;}return false;}

}

#include <assert.h>
#include <stdio.h>
namespace N00cbb4d0 {
int StringInstances=0;static int liveObjects,deletedObjects;
struct Object{unsigned id;Object(unsigned n):id(n){++liveObjects;}~Object(){--liveObjects;++deletedObjects;}};
void __fastcall DeleteObject(void*p){delete (Object*)p;}
void SetText(String&s,const char*text){s.Clear();unsigned n=(unsigned)strlen(text);if(n){s.Data=new StringData;s.Data->Length=n;s.Data->Capacity=(n+4)&~3u;s.Data->Fast=1;s.Data->Refs=1;s.Data->Data=new char[s.Data->Capacity];memset(s.Data->Data,0,s.Data->Capacity);memcpy(s.Data->Data,text,n);}}
void SetValue(Element&e,const char*text,unsigned id){SetText(e.S,text);e.P.Release();Object*p=new Object(id);e.P.Data=p;e.P.Block=new RefBlock;e.P.Block->Refs=1;e.P.Block->Destroy=DeleteObject;e.P.Block->Data=p;}
template<class T,class C>void __fastcall PushHeap(T*first,int hole,int top,T value,C comp){int parent=(hole-1)/2;while(hole>top&&comp(first[parent],value)){first[hole]=first[parent];hole=parent;parent=(hole-1)/2;}first[hole]=value;}
void CheckOwnership(Element**items,unsigned n){for(unsigned i=0;i<n;++i){RefBlock*block=items[i]->P.Block;StringData*text=items[i]->S.Data;unsigned blockCount=0,textCount=0;for(unsigned j=0;j<n;++j){if(items[j]->P.Block==block)++blockCount;if(items[j]->S.Data==text)++textCount;}assert(block&&block->Refs==blockCount&&block->Data==items[i]->P.Data);assert(text&&text->Refs==(int)textCount);}}
void CheckHeap(Element*a,unsigned n){for(unsigned i=1;i<n;++i)assert(!a[(i-1)/2].S.FastCompare(a[i].S));}
}
int main(){using namespace N00cbb4d0;assert(sizeof(StringData)==17&&sizeof(String)==4&&sizeof(Element)==12);
 {
 Element a[7],x,y,z;const char*names[]={"g","f","e","d","c","b","a"};for(unsigned i=0;i<7;++i)SetValue(a[i],names[i],i+1);
 SetValue(x,"a",100);SetValue(y,"j",200);SetValue(z,"t",300);Compare comp;comp.State=0;
 AdjustHeap(a,0,7,x,comp);assert(a[0].S.Data->Data[0]=='f'&&a[1].S.Data->Data[0]=='d'&&a[3].P.Data==x.P.Data);CheckHeap(a,7);
 Element*items[]={a,a+1,a+2,a+3,a+4,a+5,a+6,&x,&y,&z};CheckOwnership(items,10);
 PushHeap(a,6,0,y,comp);assert(a[0].P.Data==y.P.Data);CheckHeap(a,7);CheckOwnership(items,10);
 AdjustHeap(a,0,1,z,comp);assert(a[0].P.Data==z.P.Data);CheckOwnership(items,10);
 // FastCompare uses length first and little-endian unsigned words for equal lengths.
 String shortText,longText,left,right,empty;SetText(shortText,"z");SetText(longText,"aa");assert(shortText.FastCompare(longText));SetText(left,"az");SetText(right,"ba");assert(!left.FastCompare(right)&&right.FastCompare(left));assert(empty.FastCompare(left)&&!left.FastCompare(empty)&&!empty.FastCompare(empty));
 }
 assert(StringInstances==0&&liveObjects==0&&deletedObjects==10);
 puts("00cbb4d0_TEST PASS");return 0;
}