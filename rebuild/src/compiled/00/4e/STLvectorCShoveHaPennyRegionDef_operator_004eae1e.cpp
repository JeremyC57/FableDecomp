// 004eae1e _STL::vector<CShoveHaPennyRegionDef>::operator=
// Flags: /O2 /Oy /W3 /O1 /Oy-
// Leaf: operator=
#include <stdlib.h>
#include <string.h>
#include <new>
#pragma intrinsic(memset)
namespace X_004eae1e {
// Layout: oracle's pointer differences divided by 0x30; vector pointers at +0,+4,+8.
// Layout: PDB CShoveHaPennyRegionDef size48; Start+40, Score+44; inherited virtual def data first40.
// PDB sibling base layout: packed37 bytes; derived fields align to40.
#pragma pack(push,1)
struct CSubDefClassBase {virtual ~CSubDefClassBase();char baseData[33];CSubDefClassBase();CSubDefClassBase(const CSubDefClassBase&);CSubDefClassBase& operator=(const CSubDefClassBase&);};

#pragma pack(pop)
struct CShoveHaPennyRegionDef:CSubDefClassBase {float Start,Score;CShoveHaPennyRegionDef();__forceinline CShoveHaPennyRegionDef(const CShoveHaPennyRegionDef& x):CSubDefClassBase(x),Start(x.Start),Score(x.Score){} };
struct __false_type {};
CShoveHaPennyRegionDef* __fastcall copy(CShoveHaPennyRegionDef* f,CShoveHaPennyRegionDef* e,CShoveHaPennyRegionDef* o,const __false_type&);
template<class Tag>__declspec(noinline) void __fastcall destroy(CShoveHaPennyRegionDef* f,CShoveHaPennyRegionDef* e,const Tag&){for(;f!=e;++f)f->~CShoveHaPennyRegionDef();}
__declspec(noinline) void __fastcall construct(CShoveHaPennyRegionDef* out,const CShoveHaPennyRegionDef& x);
__forceinline CShoveHaPennyRegionDef* uninitialized_copy(CShoveHaPennyRegionDef* f,CShoveHaPennyRegionDef* e,CShoveHaPennyRegionDef* o){for(;;){if(f==e)break;construct(o,*f);++f;++o;}return o;}
template<class T> __forceinline void _UninitializedCopy(T* out,T* first,T* last){while(first!=last){construct(out,*first);++first;++out;}}
template<class T> struct vector {
 T *first,*last,*limit;
 T* _M_allocate_and_copy(unsigned,T*,T*);
 void _M_clear();
 vector& operator=(const vector&);
};
template<class T> vector<T>& vector<T>::operator=(const vector& rhs) {
 if(&rhs!=this){
 T* finish=rhs.last;T* start=rhs.first;unsigned count=finish-start;
 if(count>unsigned(limit-first)) {T* fresh=_M_allocate_and_copy(count,start,finish);_M_clear();first=fresh;limit=fresh+count;}
 else if(unsigned(last-first)>=count){T* end=copy(start,finish,first,__false_type());destroy(end,last,__false_type());}
 else{copy(start,start+(last-first),first,__false_type());_UninitializedCopy(last,rhs.first+(last-first),rhs.last);}
 last=first+count;
 }return *this;
}
template struct vector<CShoveHaPennyRegionDef>;
}