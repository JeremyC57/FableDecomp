// C3DDummyObject::C3DDummyObject() -- identity Transform: the nine off-diagonal / translation floats zeroed from the highest offset down,
// then 1.0f into E33 (+0x20), E22 (+0x10) and E11 (+0); BoneIndex (+0x30) is left alone
// Leaf: ??0C3DDummyObject@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E4B0.
// NF39 (2026-11-16): moved off its address-suffixed stand-in (CMatrix3x4_00a2e4b0) onto the shared engine/C3DDummyObject.h, which names
//   ??0C3DDummyObject@@QAE@XZ 0x00A2E4B0 (Ego_r.pdb / FableWin.pdb, out of line in retail; the manifest label C3DDummyObject::C3DDummyObject);
//   used as the C3DMeshStats slot-6 out object (0x00BF1928). The body is engine/CMatrix3x4.h SetIdentity in its twelve-separate-stores
//   form (FableWin.pdb ?SetIdentity@CMatrix3x4@@QAEXXZ, opt-in _SETIDENTITY_SEPARATE: E43 .. E12 zeroed, then E33, E22, E11).
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY_SEPARATE
#include "engine/C3DDummyObject.h"

C3DDummyObject::C3DDummyObject()
{
    Transform.SetIdentity();
}
