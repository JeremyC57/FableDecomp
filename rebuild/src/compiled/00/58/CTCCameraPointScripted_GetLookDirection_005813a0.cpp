// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#pragma optimize("s",on)
#include "engine/C3DVector.h"
class CRightHandedSet { public: float m[9]; };

// callee at 0x581266 (cdecl): takes (out, rhs, &flag)
void __stdcall GetLookDir_helper(C3DVector* out, CRightHandedSet* rhs, char* flag);

class CTCCameraPointScripted { public:
    CTCCameraPointScripted* GetLookDirection(C3DVector* out, CRightHandedSet* rhs);
};

CTCCameraPointScripted* CTCCameraPointScripted::GetLookDirection(C3DVector* out, CRightHandedSet* rhs)
{
    char flag;
    GetLookDir_helper(out, rhs, &flag);
    return this;
}