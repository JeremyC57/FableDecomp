#include "engine/C3DMeshFileXVertexDataChunk.h"  // retyped onto the PDB layout; byte parity re-verified

class CCharString { public:
    char* Data;
    CCharString(const char* text, long length);
};

__declspec(noinline) CCharString::CCharString(const char* text, long length)
{
    Data = (char*)text;
    if (length) Data = (char*)text + 1;
}

struct C3DMeshFileXVertexDataChunk_Methods : C3DMeshFileXVertexDataChunk {
    CCharString GetChunkDescription() const;
};

// Literal (NF413, 2026-11-25): HEAD cast the retail address (const char*)0x0129E368; it is now the literal spelled from retail's bytes
//   at 0x0129E368 (narrow "Vertex Buffer Data", NUL-terminated).
CCharString C3DMeshFileXVertexDataChunk_Methods::GetChunkDescription() const
{
    return CCharString("Vertex Buffer Data", -1);
}
