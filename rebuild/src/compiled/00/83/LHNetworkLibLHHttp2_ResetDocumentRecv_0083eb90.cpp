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
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)

namespace LHNetworkLib {

class LHHttp2 { public:
    unsigned char _pad0[0x70];                     // +0x00 Host, Hostname, Port, PreparedRequest, headers, timers
    char          CurrentHeaderPart[2048];         // +0x70
    unsigned long CurrentHeaderPos;                // +0x870
    unsigned char _pad874[0x20];                   // +0x874 ProxyHost, ProxyPort, DocumentParts (+0x880), DocumentEncoding
    unsigned long DocumentEstimatedLength;         // +0x894
    unsigned long DocumentLength;                  // +0x898
    char          CurrentDocumentPart[2048];       // +0x89C
    unsigned long CurrentDocumentPos;              // +0x109C
    bool          InChunkMode;                     // +0x10A0
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