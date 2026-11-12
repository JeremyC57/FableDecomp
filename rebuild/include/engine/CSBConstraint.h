#ifndef FABLE_ENGINE_CSBCONSTRAINT_H
#define FABLE_ENGINE_CSBCONSTRAINT_H
// New header (chunk Q17, row 0x00A87700). RTTI .?AVCSBConstraint@@ : CSBInput : CSB (Fable.exe rtti_bases.tsv). CSBInput adds the input
// kinematic at +0x0C (FableWin.pdb ?SetInput@CSBInput@@QAEXVCKinematic@@@Z; the member name Input is descriptive: the Ego_r.pdb layout
// row is unnamed padding), CSBConstraint adds MoveBeforeOrientation +0x14 (Ego_r.pdb; FableWin ?GetMoveBeforeOrientation@CSBConstraint@@QAE_NXZ).
// Needs FABLE_RETAIL_CSB (engine/CSB.h).
#include "engine/CSB.h"
class CSBInput : public CSB {
public:
    void SetInput(CKinematic input) { Input = input; }
protected:
    CKinematic Input;                                                    // +0x0c
};
class CSBConstraint : public CSBInput {
public:
    bool GetMoveBeforeOrientation() { return MoveBeforeOrientation; }
protected:
    bool MoveBeforeOrientation;                                          // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CSBConstraint) == 0x18);
#endif
