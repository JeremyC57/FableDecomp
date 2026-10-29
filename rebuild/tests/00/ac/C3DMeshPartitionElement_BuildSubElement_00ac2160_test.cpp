#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __cdecl
#endif
// 00ac2160 C3DMeshPartitionElement::BuildSubElement
// Flags:
// Leaf: ?BuildSubElement@C3DMeshPartitionElement@@QAEPAV1@J@Z
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;C3DVector(){}__forceinline C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}__forceinline C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
#pragma pack(pop)
class C3DBoundingBox{public:C3DVector MinCoord,MaxCoord;C3DBoundingBox(){}};
class C3DBoundingSphere{public:C3DVector Center;float Radius;__forceinline C3DBoundingSphere():Center(0.0f,0.0f,0.0f),Radius(0.0f){}};
// Layout: original56-byte nonpolymorphic class: box0, StartTriangle24,
// TriangleCount28, SubElements32/36, sphere40. No engine header exists.
class C3DMeshPartitionElement{public:C3DBoundingBox BoundingBox;long StartTriangle,TriangleCount;C3DMeshPartitionElement*SubElements[2];C3DBoundingSphere BoundingSphere;
 C3DMeshPartitionElement(){SubElements[0]=0;SubElements[1]=0;StartTriangle=0;TriangleCount=0;}
 __forceinline ~C3DMeshPartitionElement(){delete SubElements[0];delete SubElements[1];StartTriangle=0;TriangleCount=0;}
 C3DMeshPartitionElement*BuildSubElement(long);
};
C3DMeshPartitionElement*C3DMeshPartitionElement::BuildSubElement(long index){
 C3DMeshPartitionElement*old=SubElements[index];if(old){old->~C3DMeshPartitionElement();::operator delete(old);}
 return SubElements[index]=new C3DMeshPartitionElement;
}

typedef C3DMeshPartitionElement Node;
static bool active,failAllocation,bad;static unsigned int allocationCalls,deleteCalls,expectedCount;static unsigned char fillPattern;static void*expectedDeletes[16];
void*__cdecl operator new(size_t n)throw(std::bad_alloc){if(active){++allocationCalls;if(n!=56||deleteCalls!=expectedCount)bad=true;if(failAllocation)return 0;}void*p=malloc(n);if(p)memset(p,fillPattern,n);return p;}
void __cdecl operator delete(void*p)throw(){
 if(active){if(deleteCalls>=expectedCount||p!=expectedDeletes[deleteCalls])bad=true;long start,count;memcpy(&start,(unsigned char*)p+24,4);memcpy(&count,(unsigned char*)p+28,4);if(start||count)bad=true;++deleteCalls;}free(p);
}
static void collect(Node*p){if(!p)return;collect(p->SubElements[0]);collect(p->SubElements[1]);if(expectedCount>=16)abort();expectedDeletes[expectedCount++]=p;}
static Node*build(unsigned int shape){if(!shape)return 0;Node*p=new Node;unsigned int mask=shape-1;p->StartTriangle=123;p->TriangleCount=456;
 if(mask&1){p->SubElements[0]=new Node;p->SubElements[0]->StartTriangle=12;p->SubElements[0]->TriangleCount=34;if(mask&4){p->SubElements[0]->SubElements[0]=new Node;p->SubElements[0]->SubElements[0]->StartTriangle=56;p->SubElements[0]->SubElements[0]->TriangleCount=78;}}
 if(mask&2){p->SubElements[1]=new Node;p->SubElements[1]->StartTriangle=90;p->SubElements[1]->TriangleCount=123;if(mask&8){p->SubElements[1]->SubElements[1]=new Node;p->SubElements[1]->SubElements[1]->StartTriangle=456;p->SubElements[1]->SubElements[1]->TriangleCount=789;}}return p;
}
int main(){
 if(sizeof(Node)!=56||sizeof(C3DBoundingSphere)!=16||sizeof(C3DBoundingBox)!=24)return 1;unsigned int cases=0;
 for(unsigned int pattern=0;pattern<8;++pattern)for(unsigned int shape=0;shape<17;++shape)for(unsigned int index=0;index<2;++index)for(unsigned int failure=0;failure<2;++failure){
  active=false;fillPattern=(unsigned char)(pattern*31);union Storage{double align;unsigned char bytes[88];}storage;memset(storage.bytes,fillPattern,88);Node*owner=new(storage.bytes+16)Node;
  owner->StartTriangle=9876;owner->TriangleCount=5432;owner->SubElements[index]=build(shape);owner->SubElements[1-index]=new Node;
  expectedCount=0;collect(owner->SubElements[index]);unsigned char before[56];memcpy(before,owner,56);bad=false;allocationCalls=deleteCalls=0;failAllocation=failure!=0;active=true;
  Node*result=owner->BuildSubElement(index);active=false;
  if(bad||allocationCalls!=1||deleteCalls!=expectedCount||result!=owner->SubElements[index]||(result==0)!=failAllocation)return 2;
  unsigned char*after=(unsigned char*)owner;for(unsigned int i=0;i<56;++i)if((i<32+index*4||i>=36+index*4)&&before[i]!=after[i])return 3;
  if(result){unsigned char*bytes=(unsigned char*)result;for(unsigned int i=0;i<24;++i)if(bytes[i]!=fillPattern)return 4;for(unsigned int i=24;i<56;++i)if(bytes[i]!=0)return 5;}
  for(unsigned int i=0;i<16;++i)if(storage.bytes[i]!=fillPattern||storage.bytes[72+i]!=fillPattern)return 6;
  owner->~Node();++cases;
 }
 if(cases!=544)return 7;puts("00ac2160_TEST PASS");return 0;
}