// Leaf: ?GetPosition@CMemoryFile@@UBEJXZ

#define FABLE_RETAIL_CMEMORYFILE
#include "engine/CMemoryFile.h"  // retyped onto the PDB layout; byte parity re-verified
long CMemoryFile::GetPosition() const
{
    const CMemoryFile* file = this;
    return file->Position;
}
// Flatten route (flatten_pilot v4, NF324): CMemoryFile_GetPosition -> CMemoryFile::GetPosition, the RTTI-placed PDB member; class model: engine/CMemoryFile.h under FABLE_RETAIL_CMEMORYFILE.
