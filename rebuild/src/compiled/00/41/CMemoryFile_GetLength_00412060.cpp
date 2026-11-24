// Leaf: ?GetLength@CMemoryFile@@UBEJXZ

#define FABLE_RETAIL_CMEMORYFILE
#include "engine/CMemoryFile.h"  // retyped onto the PDB layout; byte parity re-verified
long CMemoryFile::GetLength() const
{
    const CMemoryFile* file = this;
    return file->Length;
}
// Flatten route (flatten_pilot v4, NF324): CMemoryFile_GetLength -> CMemoryFile::GetLength, the RTTI-placed PDB member; class model: engine/CMemoryFile.h under FABLE_RETAIL_CMEMORYFILE.
