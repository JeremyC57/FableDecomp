// 00cb68c0 N00cb68c0::__adjust_heap<Element,CKeyPairCompareLess> (STLport; catalog: _Adjust_heap<std::pair<CCharString,CScriptInfo>*,int,std::_Adjust_heap<std::pair<CCharString,CScriptInfo>*,int,std::pair<class_CCharString,class_CScriptInfo>,CKeyPairCompareLess<CCharString,CScriptInf)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: __adjust_heap
// Layout: cb68e3-cb68fc compares shared payload pointers then calls CBasicString at429950 with payload receiver. PDB lines6834-6843: wrapper4, packed BasicString13 and NoRefs32 at13; Element stride24.
#include <new>
namespace N00cb68c0 {

struct RefBlock { unsigned int Refs; void (__fastcall *Destroy)(void*); void* Data; };
struct Base { virtual void v00(); ~Base(); };
template<class T> struct Counted {
 T* Data; RefBlock* Block;
 __forceinline Counted():Data(0),Block(0) {}
 __forceinline Counted(const Counted& o):Data(o.Data),Block(o.Block) {if(Block) ++Block->Refs;}
 __forceinline ~Counted() { Release(); Data=0;Block=0; }
 __forceinline void Release() { if(Block) {--Block->Refs; if(Block->Refs==0) {Block->Destroy(Block->Data); ::operator delete(Block);}} }
 __forceinline Counted& operator=(const Counted& o) {T* data=o.Data; RefBlock* block=o.Block; if(Block!=block){Release();Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
template<class T> struct PolyCounted:Base {
 T* Data;RefBlock* Block;
 __forceinline PolyCounted():Data(0),Block(0) {}
 __forceinline PolyCounted(const PolyCounted& o):Data(o.Data),Block(o.Block) {if(Block)++Block->Refs;}
 __forceinline ~PolyCounted(){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=0;Block=0;}
 __forceinline PolyCounted& operator=(const PolyCounted& o){T* data=o.Data;RefBlock* block=o.Block;if(Block!=block){if(Block){--Block->Refs;if(Block->Refs==0){Block->Destroy(Block->Data);::operator delete(Block);}}Data=data;Block=block;if(block)++block->Refs;}return *this;}
};
#pragma pack(push,1)
struct CBasicString {char* PData;unsigned int StringLength;unsigned int DataLength:31;unsigned int Reserved:1;unsigned char UseFastExtend:1;bool operator<(const CBasicString&)const;};
struct StringData {CBasicString Data;unsigned int NoRefs;};
#pragma pack(pop)
struct CCharString {StringData* Data;CCharString();CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);
 __forceinline bool operator<(const CCharString& other)const {if(other.Data==Data)return false;if(!Data)return true;if(!other.Data)return false;return Data->Data<other.Data->Data;}
};
struct CWideString {unsigned int Key; CWideString();CWideString(const CWideString&);~CWideString();CWideString& operator=(const CWideString&);};
struct Element {CCharString S;unsigned int A,B;CCharString Other;unsigned int C;unsigned char Flag;};
struct CKeyPairCompareLess {unsigned int State; __forceinline bool operator()(const Element& a,const Element& b) const {return a.S<b.S;}};

template<class T,class C> void __fastcall __push_heap(T* first,int hole,int top,T value,C comp);
template<class T,class C> void __fastcall __adjust_heap(T* first,int hole,int length,T value,C comp) {
 int top=hole;
 int second=2*hole+2;
 while(second<length) {
  if(comp(first[second],first[second-1])) --second;
  first[hole]=first[second];
  hole=second;second=2*(second+1);
 }
 if(second==length) {first[hole]=first[second-1];hole=second-1;}
 __push_heap(first,hole,top,value,comp);
}

template void __fastcall __adjust_heap<Element,CKeyPairCompareLess>(Element*,int,int,Element,CKeyPairCompareLess);
}

void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);

#include <cstdio>
#include <cstring>
#include <cstdlib>
namespace N00cb68c0 {
static int liveBlocks,lessCalls,liveTextArrays,releasedSlow,releasedFast;
CCharString::CCharString():Data(0){}CCharString::CCharString(const CCharString& o):Data(o.Data){if(Data)++Data->NoRefs;}
void Release(StringData* p){if(p&&(int)--p->NoRefs<=0){if(p->Data.PData){if(p->Data.UseFastExtend)++releasedFast;else ++releasedSlow;delete[] p->Data.PData;--liveTextArrays;p->Data.PData=0;}delete p;--liveBlocks;}}
CCharString::~CCharString(){Release(Data);}
CCharString& CCharString::operator=(const CCharString& o){if(Data!=o.Data){if(o.Data)++o.Data->NoRefs;Release(Data);Data=o.Data;}return *this;}
bool CBasicString::operator<(const CBasicString& o)const{++lessCalls;const signed char*a=(const signed char*)PData;const signed char*b=(const signed char*)o.PData;while(*a||*b){if(*a<*b)return true;if(*a>*b)return false;++a;++b;}return false;}
void SetString(CCharString& s,const char* text){CCharString fresh;fresh.Data=new StringData;std::memset(fresh.Data,0,sizeof *fresh.Data);unsigned int n=(unsigned int)std::strlen(text);fresh.Data->Data.PData=new char[n+1];++liveTextArrays;std::memcpy(fresh.Data->Data.PData,text,n+1);fresh.Data->Data.StringLength=n;fresh.Data->Data.DataLength=n+1;fresh.Data->Data.UseFastExtend=0;fresh.Data->NoRefs=1;++liveBlocks;s=fresh;}
void Init(Element& e,const char* key,int id){SetString(e.S,key);SetString(e.Other,"payload");e.A=100+id;e.B=200+id;e.C=300+id;e.Flag=(unsigned char)id;}
bool Is(const CCharString& s,const char* text){return s.Data&&std::strcmp(s.Data->Data.PData,text)==0;}
template<class T,class C>void __fastcall __push_heap(T* first,int hole,int top,T value,C comp){int parent=(hole-1)/2;while(hole>top&&comp(first[parent],value)){first[hole]=first[parent];hole=parent;parent=(hole-1)/2;}first[hole]=value;}
}

namespace N00cb68c0 {
bool Cross255(){int initial=liveBlocks;{CCharString root;SetString(root,"many shared copies");CCharString* copies=(CCharString*)::operator new(300*sizeof(CCharString));for(int i=0;i<300;++i)new(copies+i)CCharString(root);if(root.Data->NoRefs!=301)return false;copies[0]=root;if(root.Data->NoRefs!=301)return false;for(int j=0;j<45;++j)copies[j].~CCharString();if(root.Data->NoRefs!=256)return false;copies[45].~CCharString();if(root.Data->NoRefs!=255)return false;for(int k=46;k<300;++k)copies[k].~CCharString();if(root.Data->NoRefs!=1)return false;::operator delete(copies);}return liveBlocks==initial;}
}
int main(){using namespace N00cb68c0;
 if(!Cross255())return 100;
 {CCharString low,high,plain;SetString(low,"\x80");SetString(high,"\xff");SetString(plain,"A");
 if(!(low<high)||!(high<plain)||plain<high)return 101;
 low.Data->Data.UseFastExtend=0;high.Data->Data.UseFastExtend=1;
 if(!low.Data->Data.PData||!high.Data->Data.PData)return 102;
 }
 if(liveBlocks||liveTextArrays||!releasedFast||!releasedSlow)return 103;
 {Element h[3];Init(h[0],"A",1);Init(h[1],"\xff",2);Init(h[2],"\x80",3);Element x=h[2];CKeyPairCompareLess comp;comp.State=0;
 __adjust_heap(h,0,3,x,comp);if(!Is(h[0].S,"\xff")||!Is(h[1].S,"\x80")||h[1].A!=103||h[1].Other.Data!=x.Other.Data)return 104;
 Element v=h[1];SetString(v.S,"B");__push_heap(h,2,0,v,comp);if(!Is(h[0].S,"B"))return 105;
 }
 if(liveBlocks)return 106;
 if(sizeof(CBasicString)!=13||sizeof(StringData)!=17||sizeof(Element)!=24)return 1;
 {CCharString empty,one,two,same;SetString(one,"10");SetString(two,"2");same=one;int before=lessCalls;
 if(empty<empty||one<empty||one<same||!(empty<one)||lessCalls!=before)return 2;
 if(!(one<two)||two<one||lessCalls!=before+2)return 3;if(one.Data->NoRefs!=2)return 4;}
 if(liveBlocks!=0)return 5;
 {Element a[7];const char* keys[7]={"7","6","5","4","3","2","1"};for(int i=0;i<7;++i)Init(a[i],keys[i],i);CKeyPairCompareLess comp;comp.State=0;
 Element x=a[0];SetString(x.S,"1");__adjust_heap(a,0,7,x,comp);
 if(!Is(a[0].S,"6")||!Is(a[1].S,"4")||!Is(a[3].S,"1")||a[3].S.Data!=x.S.Data||a[3].Other.Data!=x.Other.Data||a[3].A!=100||a[3].B!=200||a[3].C!=300||a[3].Flag!=0)return 6;
 Element y=a[0];SetString(y.S,"z");__push_heap(a,6,0,y,comp);if(!Is(a[0].S,"z")||a[0].S.Data!=y.S.Data)return 7;
 Element z=a[0];SetString(z.S,"zz");__adjust_heap(a,0,1,z,comp);if(!Is(a[0].S,"zz")||a[0].S.Data!=z.S.Data)return 8;
 Element b[2];Init(b[0],"b",8);Init(b[1],"a",9);Element v=b[0];SetString(v.S,"0");__adjust_heap(b,0,2,v,comp);if(!Is(b[0].S,"a")||!Is(b[1].S,"0"))return 9;
 }
 if(liveBlocks!=0||liveTextArrays!=0)return 10;
 std::printf("00cb68c0_TEST PASS\n");return 0;}