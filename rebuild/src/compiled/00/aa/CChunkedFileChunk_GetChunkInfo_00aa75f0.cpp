#include "engine/CChunkedFileChunk.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    CCharString(const char* lit, long len);
    int dummy;
};

struct CChunkedFileChunk_Methods : CChunkedFileChunk {
    CCharString GetChunkInfo() const;
};

CCharString CChunkedFileChunk_Methods::GetChunkInfo() const
{
    return CCharString((const char*)0x122d70e, -1);
}