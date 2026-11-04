// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#pragma optimize("y",off)
#pragma optimize("s",on)
#include "engine/C3DVector.h"
class CRightHandedSet { public: float m[9]; };

void __stdcall GetLookDirHelper(C3DVector* out, CRightHandedSet* rhs, unsigned char* pb);

class CTCCameraPointScripted { public:
    CTCCameraPointScripted* GetLookDirection(C3DVector* out, CRightHandedSet* rhs);
};

CTCCameraPointScripted* CTCCameraPointScripted::GetLookDirection(C3DVector* out, CRightHandedSet* rhs)
{
    unsigned char local;
    GetLookDirHelper(out, rhs, &local);
    return this;
}