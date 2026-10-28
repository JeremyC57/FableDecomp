#include "engine/C3DAnimFileXAllocationSizeChunk.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public: char* Data; CCharString(const char* text, long length); };
struct C3DAnimFileXAllocationSizeChunk_Methods : C3DAnimFileXAllocationSizeChunk {
    CCharString GetChunkDescription() const;
};
CCharString C3DAnimFileXAllocationSizeChunk_Methods::GetChunkDescription() const
{
    return CCharString("AllocationSize", -1);
}
