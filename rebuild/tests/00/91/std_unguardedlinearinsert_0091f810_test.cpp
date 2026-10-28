#include <stdio.h>
// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Shift a sentinel-bounded run using the original comparator result.
// Leaf: ??$__unguarded_linear_insert@PAPBVCThingObject@@PBV1@VCThingCompare_Nearest@@@std@@YIXPAPBVCThingObject@@PBV1@VCThingCompare_Nearest@@@Z
// Flags: /Gr
// Identity: own comparator0091f740 and its full PDB symbol establish const CThingObject* with CThingCompare_Nearest.
// Layout: named comparator is empty or contains Pos at0,12bytes; owner is not accessed by this algorithm.
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing;class CThingObject;
class C3DVector {public:float X,Y,Z;};
namespace std {
template<class Iter,class T,class Compare>void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp){Iter next=last;--next;while(comp(value,*next)){*last=*next;last=next;--next;}*last=value;}
}
class CThingCompare_Nearest {public:C3DVector Pos;template<class T>T* operator()(T*,T*)const;};
typedef const CThingObject* Element;
typedef CThingCompare_Nearest Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);

class CThing {public:int Key;};class CThingObject:public CThing {};static int calls,bad;
template<class T>T* CThingCompare_Nearest::operator()(T* a,T* b)const{++calls;if(Pos.X!=1.5f || Pos.Y!=-2.0f || Pos.Z!=4.0f)bad=1;if(!a)return b;if(!b)return a;return a->Key<b->Key?a:b;}
int main(){// Retail consumes a pointer-valued selector as a condition. Use a null sentinel and null incoming value to test its actual bounded behavior, not an invented bool comparator.
 CThingObject objects[3];objects[0].Key=1;objects[1].Key=2;objects[2].Key=3;
 Element data[5]={0,&objects[0],&objects[1],&objects[2],&objects[2]};Compare comp;comp.Pos.X=1.5f;comp.Pos.Y=-2.0f;comp.Pos.Z=4.0f;
 std::__unguarded_linear_insert(data+3,(Element)0,comp);
 int ok=!data[0] && !data[1] && data[2]==&objects[0] && data[3]==&objects[1] && data[4]==&objects[2] && calls==3 && !bad;
 calls=0;std::__unguarded_linear_insert(data+1,(Element)0,comp);ok=ok && calls==1 && !data[0] && !data[1];
puts(ok?"0091f810_TEST PASS":"0091f810_TEST FAIL");return ok?0:1;}