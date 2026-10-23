// 004eaad0 _STL::vector<CPrizeScoreDef>::operator=
// Flags: /O2 /Oy /W3 /O1 /Oy-
// Leaf: operator=
#include <stdlib.h>
#include <string.h>
#include <new>
#pragma intrinsic(memset)
namespace X_004eaad0 {
// Layout: oracle's pointer differences divided by 0x30; vector pointers at +0,+4,+8.
// Layout: retail RTTI012437d4 is CPrizeScoreDef; PDB Score(float)+40, Mult(int)+44.
// Layout: CSubDefClassBase has packed size37 and aligned derived fields at40 and virtual destructor; target vector has no active header.

#pragma pack(push,1)
struct CSubDefClassBase {virtual ~CSubDefClassBase();char baseData[33];CSubDefClassBase();CSubDefClassBase(const CSubDefClassBase&);CSubDefClassBase& operator=(const CSubDefClassBase&);};

#pragma pack(pop)
struct CPrizeScoreDef:CSubDefClassBase {float Score;int Mult;CPrizeScoreDef();__forceinline CPrizeScoreDef(const CPrizeScoreDef& x):CSubDefClassBase(x),Score(x.Score),Mult(x.Mult){} };
struct __false_type {};
CPrizeScoreDef* __fastcall copy(CPrizeScoreDef* f,CPrizeScoreDef* e,CPrizeScoreDef* o,const __false_type&);
template<class Tag>__declspec(noinline) void __fastcall destroy(CPrizeScoreDef* f,CPrizeScoreDef* e,const Tag&){for(;f!=e;++f)f->~CPrizeScoreDef();}
__declspec(noinline) void __fastcall construct(CPrizeScoreDef* out,const CPrizeScoreDef& x);
__forceinline CPrizeScoreDef* uninitialized_copy(CPrizeScoreDef* f,CPrizeScoreDef* o,CPrizeScoreDef* e){for(;f!=e;++f,++o)construct(o,*f);return o;}
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
template struct vector<CPrizeScoreDef>;
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace X_004eaad0 {
static int destroyed;
__declspec(noinline) void __fastcall construct(CPrizeScoreDef* out,const CPrizeScoreDef& x){if(out)new(out)CPrizeScoreDef(x);}
CPrizeScoreDef* __fastcall copy(CPrizeScoreDef* f,CPrizeScoreDef* e,CPrizeScoreDef* o,const __false_type&){for(;f!=e;++f,++o)*o=*f;return o;}
CSubDefClassBase::CSubDefClassBase(){memset(baseData,0,sizeof(baseData));}
CSubDefClassBase::CSubDefClassBase(const CSubDefClassBase& x){memcpy(baseData,x.baseData,sizeof(baseData));}
CSubDefClassBase& CSubDefClassBase::operator=(const CSubDefClassBase& x){memcpy(baseData,x.baseData,sizeof(baseData));return *this;}
CSubDefClassBase::~CSubDefClassBase(){++destroyed;}
CPrizeScoreDef::CPrizeScoreDef():Score(0),Mult(0){}
template<> CPrizeScoreDef* vector<CPrizeScoreDef>::_M_allocate_and_copy(unsigned n,CPrizeScoreDef*a,CPrizeScoreDef*b){CPrizeScoreDef*p=(CPrizeScoreDef*)malloc(n*sizeof(CPrizeScoreDef));uninitialized_copy(a,p,b);return p;}
template<> void vector<CPrizeScoreDef>::_M_clear(){__false_type t;destroy(first,last,t);free(first);}
}
int main(){using namespace X_004eaad0;assert(sizeof(CSubDefClassBase)==37&&sizeof(CPrizeScoreDef)==48);CPrizeScoreDef layout;assert((char*)&layout.Score-(char*)&layout==40&&(char*)&layout.Mult-(char*)&layout==44);vector<CPrizeScoreDef>a,b;memset(&a,0,sizeof(a));memset(&b,0,sizeof(b));CPrizeScoreDef x[3];for(int k=0;k<3;++k){x[k].Score=2.0f;x[k].Mult=-7;}b.first=x;b.last=x+3;b.limit=x+3;a.operator=(b);assert(a.last-a.first==3&&a.first[2].Score==2.0f&&a.first[2].Mult==-7);a.operator=(a);b.last=x+1;a.operator=(b);assert(a.last-a.first==1&&destroyed==2);b.last=x+3;a.operator=(b);assert(a.last-a.first==3);a._M_clear();assert(destroyed==5);puts("004eaad0_TEST PASS");}