// LHNetworkLib::LHHttp2::ResetDocumentRecv() -- clears the receive state (header/document cursors and buffers, lengths, chunk mode) and then frees the received document parts
// Leaf: ResetDocumentRecv
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083EB90, 64 bytes, tail-jumps to ResetDocumentParts.
// Callee: 0x0083EB00 LHHttp2::ResetDocumentParts() (thiscall; frees each LHHttpDocumentParts and empties the vector).
// Identity: the manifest's `CPersistContext_TransferableVector<CReverbEnvironmentEntry>::Resize` is propagated. The body
// works on LHHttp2 fields next to ResetDocumentParts (0x0083EB00, same TU) and is called from two LHHttp2 request paths
// (0x0083F182, 0x0083F3C2); of the Ego_r.pdb LHHttp2 methods (Reset, ResetRequest, ResetDocumentRecv, ...) it is the
// receive reset.
// Layout: engine/LHHttp2.h is the Ego_r.pdb layout (Dinkumware strings of 0x1C bytes) and does not match retail, whose
// fields sit lower (CurrentHeaderPart +0x70 v PDB +0xB8, CurrentDocumentPart +0x89C v +0x8F8; the vector ResetDocumentParts
// walks is at +0x880), so this is a local model of the retail offsets the body touches, with the PDB member names.
// Shape: the three cursor/length stores share one zero register (ebx) and run first; the two 2048-byte buffers are
// intrinsic memsets (`rep stosd` of 0x200 dwords through eax); InChunkMode = false; the final call is a tail jump.
// Layout (supersedes any local-model note above): engine/LHHttp2.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define LHHttp2 LHHttp2Donor
#include "engine/LHHttp2.h"
#undef LHHttp2
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)

namespace LHNetworkLib {

class LHHttp2 : public LHHttp2Donor {
 public:
    void ResetDocumentParts();                     // 0x0083EB00
    void ResetDocumentRecv();
};

void LHHttp2::ResetDocumentRecv()
{
    CurrentHeaderPos = 0;
    DocumentEstimatedLength = 0;
    DocumentLength = 0;
    memset(CurrentHeaderPart, 0, sizeof(CurrentHeaderPart));
    memset(CurrentDocumentPart, 0, sizeof(CurrentDocumentPart));
    InChunkMode = false;
    ResetDocumentParts();
}

}