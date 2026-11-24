// Leaf: ?IsSuperChunk@CChunkedFileSuperChunk@@UBEHXZ
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#include "engine/CChunkedFileSuperChunk.h"
int CChunkedFileSuperChunk::IsSuperChunk() const
{
    return 1;
}
// Flatten route (flatten_pilot v4, NF324): CChunkedFileSuperChunk_IsSuperChunk -> CChunkedFileSuperChunk::IsSuperChunk, the RTTI-placed PDB member; class model: engine/CChunkedFileSuperChunk.h under FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK (preamble of CCountedPointer_DeleteData_00ac1460.cpp).
