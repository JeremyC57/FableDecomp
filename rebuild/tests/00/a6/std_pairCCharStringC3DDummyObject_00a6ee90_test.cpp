#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#pragma pack(push,1)
template<class T>class CBasicString{public:T*PData;unsigned long StringLength;unsigned long DataLength:31;unsigned long:1;bool UseFastExtend:1;};
class CCharStringData{public:CBasicString<char>Data;long NoRefs;};
#pragma pack(pop)
static int copies;
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const CCharString&v):PStringData(v.PStringData){++copies;if(PStringData)++PStringData->NoRefs;}
CCharString::~CCharString(){if(PStringData)--PStringData->NoRefs;}
CMatrix3x4::CMatrix3x4():E11(1),E12(0),E13(0),E21(0),E22(1),E23(0),E31(0),E32(0),E33(1),E41(0),E42(0),E43(0){}
C3DDummyObject::C3DDummyObject():BoneIndex(-1){}
static void check(bool p){if(!p)abort();}
typedef std::pair<CCharString,C3DDummyObject>Pair;
union Space{double align;unsigned char bytes[56*11];};
int main(){check(sizeof(Pair)==56&&sizeof(CCharStringData)==17);unsigned cases=0;for(unsigned count=0;count<=8;++count)for(int shared=0;shared<2;++shared)for(int pattern=0;pattern<4;++pattern){Space storage;memset(&storage,0xa5,sizeof storage);Pair*first=reinterpret_cast<Pair*>(storage.bytes+56);CCharStringData block;block.Data.PData=const_cast<char*>("dummy");block.Data.StringLength=5;block.Data.DataLength=6;block.Data.UseFastExtend=false;block.NoRefs=shared?2:1;Pair value;value.first.PStringData=shared?&block:0;float fields[12];for(int i=0;i<12;++i)fields[i]=float(pattern*23+i*3-17)*0.5f;memcpy(&value.second.Transform,fields,sizeof fields);value.second.BoneIndex=pattern-2;Pair before=value;int baseline=block.NoRefs;copies=0;std::__false_type tag;Pair*end=std::__uninitialized_fill_n(first,count,value,tag);check(end==first+count&&copies==int(count)&&block.NoRefs==baseline+(shared?int(count):0));for(unsigned i=0;i<count;++i){check(first[i].first.PStringData==value.first.PStringData&&memcmp(&first[i].second,&value.second,sizeof(C3DDummyObject))==0);}for(unsigned i=0;i<56;++i)check(storage.bytes[i]==0xa5);for(unsigned i=56*(count+1);i<sizeof storage.bytes;++i)check(storage.bytes[i]==0xa5);check(value.first.PStringData==before.first.PStringData&&memcmp(&value.second,&before.second,sizeof value.second)==0);for(unsigned i=0;i<count;++i)first[i].~Pair();check(block.NoRefs==baseline);copies=0;check(std::__uninitialized_fill_n(static_cast<Pair*>(0),0U,value,tag)==0&&copies==0);++cases;}check(cases==72);puts("00a6ee90_TEST PASS");return 0;}