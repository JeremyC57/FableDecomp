#ifndef FABLE_ENGINE_C3DMESHOBJECT_H
#define FABLE_ENGINE_C3DMESHOBJECT_H
#include "engine/CBaseClass.h"
#include "engine/CMatrix3x4.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#ifdef FABLE_RETAIL_C3DMESHOBJECT_GETNAMES
#include "stlport/fable_containers.h"   // std::vector<CCharString> (NF217 opt-in below)
#endif
#ifdef FABLE_RETAIL_C3DMESHOBJECT_INLINE
#define FABLE_RETAIL_C3DMESHSTATS_ISANIMATED
#include "engine/C3DMeshStats.h"
#endif
class C3DMeshInfo;class C3DMeshStats;class C3DAnimationBlendState;class C3DAnimationManager;
// Original complete120-byte class, both raw PDBs; native GetHelperPoint constructs
// the object at frame48h and accesses Mesh+8/Info+Ch and its two matrices.
class C3DMeshObject : public CBaseClass {
#ifdef FABLE_RETAIL_CARRYABLE_POSE_PASS29
public:
 static CMatrix3x4 FABLE_FASTCALL GetRotationMatrixForTransformFromTo(const CRightHandedSet&,const CRightHandedSet&);
#endif
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
#ifdef FABLE_RETAIL_C3DMESHOBJECT_STATIC_DUMMY_PASS26
 // Ego_r 00B8D180 and FableWin 031F7A90 agree on the class, constness and arguments.
 void GetDummyObjectPositionStatic(const CCharString&, long, C3DVector&, C3DVector&, C3DVector&) const;
#endif
 bool HasHelperPoint(const CCharString&)const;
#ifdef FABLE_RETAIL_GRAPHIC_DUMMY_PASS29
 bool HasDummyObject(const CCharString&)const;
 long GetNoDummyObjects(const CCharString&)const;
#endif
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
#ifdef FABLE_RETAIL_C3DMESHOBJECT_GETNAMES
 // Retail opt-in (NF217, q12368 / q12369 / q12370: rows 0x00A35B30 / 0x00A35AF0 / 0x00A35B50): the packed-name queries, public const
 // over std::vector<CCharString>&, both PDBs: ?GetAllHelperPointNames@C3DMeshObject@@QBEXAAV?$vector@VCCharString@@..@std@@@Z (Ego_r.pdb
 // 0x00B8C700, FableWin.pdb 0x031F77D0), ?GetAllDummyObjectNames@..QBE..@Z (0x00B8C740 / 0x031F89B0),
 // ?GetDummyObjectNamesStartingWith@..QBE..ABVCCharString@@@Z (0x00B8C760 / 0x031F89E0). Each forwards to the C3DMeshStats query of the
 // same name with sort = false. Declarations only, no layout change (std::vector from stlport/fable_containers.h, included above).
 void GetAllHelperPointNames(std::vector<CCharString>& names)const;                                     // 0x00A35AF0
 void GetAllDummyObjectNames(std::vector<CCharString>& names)const;                                     // 0x00A35B30
 void GetDummyObjectNamesStartingWith(std::vector<CCharString>& names,const CCharString& prefix)const;  // 0x00A35B50
#endif
protected:
 const C3DMeshInfo* MeshInfo;
#ifdef FABLE_RETAIL_C3DMESHOBJECT_DUMMY_PAIR_PASS24
public:
 void GetDummyObjectPositionAnimated(const CCharString&, long, const class C3DAnimationBlendState&, const class C3DAnimationBlendState&, float, class C3DAnimationManager&, C3DVector&, C3DVector&, C3DVector&, bool) const;
private:
#endif
 CCountedPointer<const C3DMeshStats> Mesh;
 CMatrix3x4 PositionMatrix;
 bool InversePositionMatrixSetup;
 CMatrix3x4 InversePositionMatrix;
 float Scale;
};
FABLE_STATIC_ASSERT_AT(47, sizeof(C3DMeshObject)==120);
#endif
