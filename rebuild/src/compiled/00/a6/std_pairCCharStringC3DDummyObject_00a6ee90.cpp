// 00a6ee90 std::__uninitialized_fill_n
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@VCCharString@@VC3DDummyObject@@@std@@IU12@@std@@YIPAU?$pair@VCCharString@@VC3DDummyObject@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
#include <new>
#include "rebuild_abi.h"
#ifndef _MSC_VER
#define __fastcall
#endif
class CCharStringData;
// Layout: original CCharString PStringData0, size4; CMatrix3x4 anonymous scalar view E11..E43, size30.
class CCharString{public:CCharStringData*PStringData;CCharString();CCharString(const CCharString&);~CCharString();};
class CMatrix3x4{public:float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;CMatrix3x4();};
// Layout: original C3DDummyObject Transform0 and BoneIndex30, size34. Scalar matrix view preserves anonymous union layout and all named fields used here.
class C3DDummyObject{public:CMatrix3x4 Transform;long BoneIndex;C3DDummyObject();};
namespace std{
struct __false_type{__false_type(){}};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}pair(const pair&p):first(p.first),second(p.second){}template<class C,class D>pair(const pair<C,D>&p):first(p.first),second(p.second){}};
template<class A,class B>inline void __fastcall _Construct(A*p,const B&v){new(p)A(v);}
template<class I,class N,class T>I __fastcall __uninitialized_fill_n(I first,N count,const T&value,const __false_type&){I current=first;for(;count>0;--count,++current)_Construct(&*current,value);return current;}
template pair<CCharString,C3DDummyObject>*__fastcall __uninitialized_fill_n(pair<CCharString,C3DDummyObject>*,unsigned int,const pair<CCharString,C3DDummyObject>&,const __false_type&);
}
FABLE_STATIC_ASSERT(sizeof(CMatrix3x4)==48);FABLE_STATIC_ASSERT(sizeof(C3DDummyObject)==52);FABLE_STATIC_ASSERT(sizeof(std::pair<CCharString,C3DDummyObject>)==56);