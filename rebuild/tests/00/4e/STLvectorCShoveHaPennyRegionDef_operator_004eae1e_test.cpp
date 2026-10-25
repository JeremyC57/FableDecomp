// 004eae1e std::vector<CShoveHaPennyRegionDef>::operator=
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

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace X_004eae1e {
static int destroyed;
__declspec(noinline) void __fastcall construct(CShoveHaPennyRegionDef* out,const CShoveHaPennyRegionDef& x){if(out)new(out)CShoveHaPennyRegionDef(x);}
CShoveHaPennyRegionDef* __fastcall copy(CShoveHaPennyRegionDef* f,CShoveHaPennyRegionDef* e,CShoveHaPennyRegionDef* o,const __false_type&){for(;f!=e;++f,++o)*o=*f;return o;}
CSubDefClassBase::CSubDefClassBase(){memset(baseData,0,sizeof(baseData));}
CSubDefClassBase::CSubDefClassBase(const CSubDefClassBase& x){memcpy(baseData,x.baseData,sizeof(baseData));}
CSubDefClassBase& CSubDefClassBase::operator=(const CSubDefClassBase& x){memcpy(baseData,x.baseData,sizeof(baseData));return *this;}
CSubDefClassBase::~CSubDefClassBase(){++destroyed;}
CShoveHaPennyRegionDef::CShoveHaPennyRegionDef():Start(0),Score(0){}
template<> CShoveHaPennyRegionDef* vector<CShoveHaPennyRegionDef>::_M_allocate_and_copy(unsigned n,CShoveHaPennyRegionDef*a,CShoveHaPennyRegionDef*b){CShoveHaPennyRegionDef*p=(CShoveHaPennyRegionDef*)malloc(n*sizeof(CShoveHaPennyRegionDef));uninitialized_copy(a,b,p);return p;}
template<> void vector<CShoveHaPennyRegionDef>::_M_clear(){__false_type t;destroy(first,last,t);free(first);}
}
int main(){using namespace X_004eae1e;vector<CShoveHaPennyRegionDef>a,b;memset(&a,0,sizeof(a));memset(&b,0,sizeof(b));CShoveHaPennyRegionDef x[3];for(int k=0;k<3;++k){x[k].Start=2;x[k].Score=7;}b.first=x;b.last=x+3;b.limit=x+3;a.operator=(b);assert(a.last-a.first==3&&a.first[2].Score==7);a.operator=(a);b.last=x+1;a.operator=(b);assert(a.last-a.first==1&&destroyed==2);b.last=x+3;a.operator=(b);assert(a.last-a.first==3);a._M_clear();assert(destroyed==5);puts("004eae1e_TEST PASS");}