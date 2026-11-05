// Leaf: ?GetChunkInfo@C3DMeshFileClothChunk_Methods@@QAE?AVCCharString@@XZ
#include "engine/C3DMeshFileClothChunk.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    CCharString(const char* lit, long len);
    int dummy;
};

struct C3DMeshFileClothChunk_Methods : C3DMeshFileClothChunk {
    CCharString GetChunkInfo();
};

CCharString C3DMeshFileClothChunk_Methods::GetChunkInfo()
{
    return CCharString((const char*)0x122d70e, -1);
}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetChunkInfo@C3DMeshFileClothChunk@@UBE?BVCCharString@@XZ); evidence: RTTI vtable slot + PDB name set.
