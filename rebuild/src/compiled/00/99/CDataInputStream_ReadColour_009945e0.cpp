// CDataInputStream::ReadColour() -- read a 4-byte CRGBColour from the stream (zero colour if the position would leave [0, 0x7FFFFFFF]) and return it by value
// Leaf: ?ReadColour@CDataInputStream@@QAE?AVCRGBColour@@XZ
// Name: FableWin.pdb / Ego_r.pdb ?ReadColour@CDataInputStream@@QAE?AVCRGBColour@@XZ (hidden result, `ret 4`); the callers 0x00B068F0,
//   0x00B193D0 and 0x00AC0010 push the result address (`lea ; push ; call`).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009945E0, length 138 bytes (`ret 4`: the hidden result pointer).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (landed) when the current source
// chunk holds fewer than 4 bytes.
// Layout: engine/CDataInputStream.h FABLE_RETAIL_CDATAINPUTSTREAM (the inline Read; StreamPos +4, CurrentSourcePtr +0x0c,
//   SourceChunkBytesRemaining +0x14). Re-modelled 2026-11-03 (landing h): the row used to define it on a local CDataInputStreamReader.
// Shape: the inlined Read(buffer, size) of the landed TransferBinaryIn family (0x007AD7F0: 64-bit position range check
// through an inline predicate, chunk memcpy, `+=` bookkeeping); the colour local is zeroed byte by byte by its default
// constructor (one zero register, ebx, which then serves the range check) and returned through CRGBColour's user copy
// constructor, which copies R, G, B, A in that order (stores +2, +1, +0, +3; the D3DCOLOR layout is B, G, R, A).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "engine/CDataInputStream.h"

class CRGBColour { public:                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour() : B(0), G(0), R(0), A(0) { }
    CRGBColour(const CRGBColour& o) { R = o.R; G = o.G; B = o.B; A = o.A; }
};

CRGBColour CDataInputStream::ReadColour()
{
    CRGBColour colour;
    Read(&colour, 4);
    return colour;
}
