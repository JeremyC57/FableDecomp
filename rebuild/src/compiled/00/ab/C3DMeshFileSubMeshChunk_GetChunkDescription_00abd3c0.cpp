#include "engine/C3DMeshFileSubMeshChunk.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public: char* Data; int Length; CCharString(const char* text, long length); };
struct C3DMeshFileSubMeshChunk_Methods : C3DMeshFileSubMeshChunk {
    CCharString GetChunkDescription() const;
};
CCharString C3DMeshFileSubMeshChunk_Methods::GetChunkDescription() const
{
    return CCharString("SubMesh", -1);
}
