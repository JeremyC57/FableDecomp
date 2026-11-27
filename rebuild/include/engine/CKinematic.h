#ifndef FABLE_ENGINE_CKINEMATIC_H
#define FABLE_ENGINE_CKINEMATIC_H
// New header (chunk Q17, row 0x00A87700 CSBCAgents::Process). Ego_r.pdb CKinematic (struct_layouts_egor.tsv: sizeof 8, C2DVector Vector
// +0). Member functions: the FableWin.pdb name set (??0CKinematic@@QAE@XZ, ??0CKinematic@@QAE@ABVC2DVector@@@Z, ?GetVector@CKinematic@@QBEABVC2DVector@@XZ,
// ?SetVector@CKinematic@@QAEXABVC2DVector@@@Z, ??4 -- all inline: no Ego_r.pdb or Fable.exe copy; the copy assignment is the implicit one).
#include "engine/C2DVector.h"
class CKinematic {
public:
#ifdef FABLE_RETAIL_KINEMATIC_ASSIGN_PASS28
    // FableWin 031B5770 calls GetVector on its argument, then SetVector on this.
    const CKinematic& operator=(const CKinematic& other) {
        SetVector(other.GetVector());
        return *this;
    }
#endif
    CKinematic() { }
#ifdef FABLE_RETAIL_STEER_KINEMATIC_CONSTRUCTOR_PASS29
    CKinematic(const C2DVector& vector) { SetVector(vector); }
#else
#ifdef FABLE_RETAIL_SB_CORRIDOR_PASS29
    // FableWin constructs Vector first, then invokes the original SetVector.
    CKinematic(const C2DVector& vector) { SetVector(vector); }
#else
    CKinematic(const C2DVector& vector) : Vector(vector) { }
#endif
#endif
    const C2DVector& GetVector() const { return Vector; }
    void SetVector(const C2DVector& vector) { Vector = vector; }
private:
    C2DVector Vector;                                                    // +0x00
};
FABLE_STATIC_ASSERT_AT(16,sizeof(CKinematic) == 8);
#endif
