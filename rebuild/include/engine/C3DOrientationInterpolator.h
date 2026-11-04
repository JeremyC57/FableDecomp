#ifndef FABLE_ENGINE_C3DORIENTATIONINTERPOLATOR_H
#define FABLE_ENGINE_C3DORIENTATIONINTERPOLATOR_H
// New shared header (chunk 749, row 0x005502E0 NUISystem::CMesh::Draw): Ego_r.pdb C3DOrientationInterpolator, sizeof 0x34
// (RTTI .?AVC3DOrientationInterpolator@@ : CBaseClass): StartOrientation +0x4, EndOrientation +0x1C (CRightHandedSet each).
// Name set: FableWin.pdb ??0..QAE@XZ, ??0..QAE@ABVCRightHandedSet@@0@Z (out of line at 0x005508D0 in the CMesh TU, `ret 8`),
// ??0..QAE@ABV0@@Z, ??1..UAE@XZ. The destructor is inline and empty: every CMesh::Draw temporary is destroyed by a direct
// call of ~CBaseClass 0x0099A2E0.
#include "engine/CBaseClass.h"
#include "engine/CRightHandedSet.h"
class C3DOrientationInterpolator : public CBaseClass {
public:
    C3DOrientationInterpolator();
    C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end);   // 0x005508D0
    C3DOrientationInterpolator(const C3DOrientationInterpolator&);
    virtual ~C3DOrientationInterpolator() { }
    CRightHandedSet StartOrientation;                                    // +0x04
    CRightHandedSet EndOrientation;                                      // +0x1c
};
FABLE_STATIC_ASSERT(sizeof(C3DOrientationInterpolator) == 0x34);
#endif
