#include "engine/C3DMeshFileVerticesChunk.h"  // retyped onto the PDB layout; byte parity re-verified

class CCharString { public:
    char* Data;
    CCharString(const char* text, long length);
};

__declspec(noinline) CCharString::CCharString(const char* text, long length)
{
    Data = (char*)text;
    if (length) Data = (char*)text + 1;
}

struct C3DMeshFileVerticesChunk_Methods : C3DMeshFileVerticesChunk {
    CCharString GetChunkDescription() const;
};

// Literal (NF413, 2026-11-25): HEAD cast the retail address (const char*)0x0129ED24; it is now the literal spelled from retail's bytes
//   at 0x0129ED24 (narrow "Vertex List", NUL-terminated).
CCharString C3DMeshFileVerticesChunk_Methods::GetChunkDescription() const
{
    return CCharString("Vertex List", -1);
}
