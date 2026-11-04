#ifndef FABLE_ENGINE_CRIGHTHANDEDSET_H
#define FABLE_ENGINE_CRIGHTHANDEDSET_H
#include "engine/C3DVector.h"
// Ego_r original class0x9f08: protected Up+0,Forward+12,size24.
// Native CThingCreatureBase::OnHit gets GetRHSet then reads Forward at+12.
#ifndef FABLE_RETAIL_CRIGHTHANDEDSET
class CRightHandedSet { protected: C3DVector Up,Forward; public: const C3DVector& GetForward() const { return Forward; } };
#else
// Retail opt-in (chunk 734, row 0x004A1CB0): Ego_r.pdb / FableWin.pdb ?GetUp@CRightHandedSet@@QBEABVC3DVector@@XZ (inline) and
// ?GetRight@CRightHandedSet@@QBE?AVC3DVector@@XZ, out of line at 0x0049CE00 (landed: Forward x Up) and expanded in
// PreparePrimitivesForEngine (0x004A1D95..0x004A1E01). FableWin.pdb: GetRight (0x01B409F0) is followed by ??TC3DVector (0x01B40A20).
class CRightHandedSet { protected: C3DVector Up,Forward; public: const C3DVector& GetForward() const { return Forward; }
    const C3DVector& GetUp() const { return Up; }
    C3DVector GetRight() const { return Forward ^ Up; }
    // Chunk 739 (row 0x00B0CAD0): FableWin.pdb ?SetForward@CRightHandedSet@@QAEXABVC3DVector@@@Z / ?SetUp@...@QAEXABVC3DVector@@@Z
    // (inline) and ?MakePerpendicular@CRightHandedSet@@QAEXXZ (out of line at 0x00A22E10, landed users 0x00B9DFC0 / 0x00BD2BC0).
    void SetForward(const C3DVector& forward) { Forward = forward; }
    void SetUp(const C3DVector& up) { Up = up; }
    void MakePerpendicular();                                            // 0x00A22E10
};
#endif
#endif
