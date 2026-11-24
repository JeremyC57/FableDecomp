// Leaf: ?NeedsBufferedRead@CDataInputStream@@EAE_NJ@Z
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "engine/CDataInputStream.h"
bool CDataInputStream::NeedsBufferedRead(long)
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CDataInputStream_NeedsBufferedRead -> CDataInputStream::NeedsBufferedRead, the RTTI-placed PDB member; class model: engine/CDataInputStream.h under FABLE_RETAIL_CDATAINPUTSTREAM.
