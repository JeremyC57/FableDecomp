#include "engine/C3DMeshFileTransformationChunk.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    CCharString(const char* lit, long len);
    int dummy;
};

struct C3DMeshFileTransformationChunk_Methods : C3DMeshFileTransformationChunk {
    CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileTransformationChunk_Methods::GetChunkInfo() const
{
    return CCharString((const char*)0x122d70e, -1);
}