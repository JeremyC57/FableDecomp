// CEngineInternalPrimitiveGroup::GetBoundingBoxWorldSpace(C3DBoundingBox&) -- union the successful child world bounds
// Retail 00BC5FF0, 266 bytes. Actual header Primitives begins+50; native CArray has three pointers (12, donor16).
// Only Start/Finish are used, so a two-pointer projection into the actual named member preserves both ABIs.
// Child virtual GetBoundingBoxWorldSpace is slot17 (+44); PMF dispatch preserves native member ABI.
// Handle first word is opaque donor padding; only counted Data pointer at+4 is read, never refcounted here.
// Leaf: GetBoundingBoxWorldSpace
#include "engine/CEngineInternalPrimitiveGroup.h"
#include "engine/CEngineInternalPrimitiveBase.h"
struct C3DBoundingBox { float MinX,MinY,MinZ,MaxX,MaxY,MaxZ; };
struct CEngineInternalPrimitiveBase_Methods : CEngineInternalPrimitiveBase {
 typedef bool (CEngineInternalPrimitiveBase_Methods::*BoundsMethod)(C3DBoundingBox&);
 bool DispatchBounds(C3DBoundingBox& bounds) {
  return (this->*static_cast<BoundsMethod*>(__vftable)[17])(bounds);
 }
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveBase_Methods::BoundsMethod)==4);
#endif
struct CEnginePrimitiveHandleView { unsigned int UnusedHandleWord; CEngineInternalPrimitiveBase_Methods* Primitive; };
struct CPrimitiveArrayBounds { CEnginePrimitiveHandleView* Start; CEnginePrimitiveHandleView* Finish; };
struct CEngineInternalPrimitiveGroup_Methods : CEngineInternalPrimitiveGroup {
 CPrimitiveArrayBounds& Children() { return *reinterpret_cast<CPrimitiveArrayBounds*>(Primitives); }
 bool GetBoundingBoxWorldSpace(C3DBoundingBox& bounds);
};
bool CEngineInternalPrimitiveGroup_Methods::GetBoundingBoxWorldSpace(C3DBoundingBox& bounds) {
 bool found=false;
 for(CEnginePrimitiveHandleView* p=Children().Start;p!=Children().Finish;++p) {
  C3DBoundingBox box;
  if(p->Primitive && p->Primitive->DispatchBounds(box)) {
   if(found) {
    if(bounds.MinX>box.MinX) bounds.MinX=box.MinX;
    if(bounds.MinY>box.MinY) bounds.MinY=box.MinY;
    if(bounds.MinZ>box.MinZ) bounds.MinZ=box.MinZ;
    if(bounds.MaxX<box.MaxX) bounds.MaxX=box.MaxX;
    if(bounds.MaxY<box.MaxY) bounds.MaxY=box.MaxY;
    if(bounds.MaxZ<box.MaxZ) bounds.MaxZ=box.MaxZ;
   } else { bounds=box; found=true; }
  }
 }
 return found;
}