#ifndef FABLE_ENGINE_C3DMESHOBJECT_H
#define FABLE_ENGINE_C3DMESHOBJECT_H
#include "engine/CBaseClass.h"
#include "engine/CMatrix3x4.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#ifdef FABLE_RETAIL_C3DMESHOBJECT_INLINE
#define FABLE_RETAIL_C3DMESHSTATS_ISANIMATED
#include "engine/C3DMeshStats.h"
#endif
class C3DMeshInfo;class C3DMeshStats;class C3DAnimationBlendState;class C3DAnimationManager;
// Original complete120-byte class, both raw PDBs; native GetHelperPoint constructs
// the object at frame48h and accesses Mesh+8/Info+Ch and its two matrices.
class C3DMeshObject : public CBaseClass {
public:
 #ifdef FABLE_RETAIL_C3DMESHOBJECT_INLINE
 // Retail opt-in (row 0x004C0560 GetHelperPoint): ctor / IsAnimated expanded in place; dtor body visible (expanded at the
 // loop continue, called as COMDAT 0x004C3200 at the returns).
 C3DMeshObject() : InversePositionMatrixSetup(false) {}
 virtual ~C3DMeshObject() {}
 #else
 C3DMeshObject();
 virtual ~C3DMeshObject();
 #endif
 bool HasHelperPoint(const CCharString&)const;
 long GetNoHelperPoints(const CCharString&)const;
 #ifdef FABLE_RETAIL_C3DMESHOBJECT_INLINE
 bool IsAnimated()const { return Mesh->IsAnimated(); }
 #else
 bool IsAnimated()const;
 #endif
 C3DVector GetHelperPointPositionStatic(const CCharString&,long)const;
 C3DVector GetHelperPointPositionAnimated(const CCharString&,long,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&)const;
#ifdef FABLE_RETAIL_C3DMESHOBJECT_POSITIONMATRIXFROMRHSET
 // Retail opt-in (wf17, row 0x0075B000): FableWin.pdb ?GetPositionMatrixFromRHSet@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@@Z
 // (static __fastcall, landed at 0x00A358F0). Declaration only.
 static CMatrix3x4 __fastcall GetPositionMatrixFromRHSet(const class CRightHandedSet& set);   // 0x00A358F0
#endif
protected:
 const C3DMeshInfo* MeshInfo;
 CCountedPointer<const C3DMeshStats> Mesh;
 CMatrix3x4 PositionMatrix;
 bool InversePositionMatrixSetup;
 CMatrix3x4 InversePositionMatrix;
 float Scale;
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshObject)==120);
#endif
