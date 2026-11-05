// 00ac2160 C3DMeshPartitionElement::BuildSubElement
// Flags:
// Leaf: ?BuildSubElement@C3DMeshPartitionElement@@QAEPAV1@J@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#pragma pack(push,1)
#include "engine/C3DVector.h"
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