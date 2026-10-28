#include <stdio.h>
// std::__unguarded_linear_insert(Iter last, T value, Compare comp) -- Shift a sentinel-bounded run using the original comparator result.
// Leaf: ??$__unguarded_linear_insert@PAPBVCThing@@PBV1@VCCompare_FurthestFromInitPos2D@?A0x
// Flags: /Gr
// Identity: own comparator007483e0 and its full PDB symbol establish const CThing* with CCompare_FurthestFromInitPos2D.
// Layout: named comparator is empty or contains Pos at0,12bytes; owner is not accessed by this algorithm.
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing;class CThingObject;
class C3DVector {public:float X,Y,Z;};
namespace std {
template<class Iter,class T,class Compare>void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp){Iter next=last;--next;while(comp(value,*next)){*last=*next;last=next;--next;}*last=value;}
}
namespace {class CCompare_FurthestFromInitPos2D {public:bool operator()(const CThing*,const CThing*)const;};}
typedef const CThing* Element;
typedef CCompare_FurthestFromInitPos2D Compare;
template void __fastcall std::__unguarded_linear_insert<Element*,Element,Compare>(Element*,Element,Compare);

class CThing {public:int Key;};class CThingObject:public CThing {};static int calls,bad;
namespace {bool CCompare_FurthestFromInitPos2D::operator()(const CThing* a,const CThing* b)const{++calls;return a->Key<b->Key;}}
int main(){CThing objects[5];objects[0].Key=0;objects[1].Key=4;objects[2].Key=9;objects[3].Key=2;objects[4].Key=19;
 Element data[5]={&objects[0],&objects[1],&objects[2],&objects[4],&objects[4]};Compare comp;
 std::__unguarded_linear_insert(data+3,(Element)&objects[3],comp);
 int ok=data[0]==&objects[0] && data[1]==&objects[3] && data[2]==&objects[1] && data[3]==&objects[2] && data[4]==&objects[4] && calls==3 && !bad;
 calls=0;std::__unguarded_linear_insert(data+4,(Element)&objects[4],comp);ok=ok && calls==1 && data[3]==&objects[2] && data[4]==&objects[4];
puts(ok?"00748810_TEST PASS":"00748810_TEST FAIL");return ok?0:1;}