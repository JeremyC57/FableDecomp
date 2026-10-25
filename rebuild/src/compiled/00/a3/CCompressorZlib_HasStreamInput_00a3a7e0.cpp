// CCompressorZlib::HasStreamInput() const -- true while the zlib stream still has an input pointer set
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3A7E0, 11 bytes: `mov edx,[ecx+20h] ; xor eax,eax ; test edx,edx ; setne al ; ret`.
// Identity: Ego_r.pdb lists ?GetRequiredWorkspaceSize@CCompressorZlib@@SIKXZ, ?HasStreamInput@CCompressorZlib@@QBE_NXZ,
//   ?CompressStream@CCompressorZlib@@QAEKPAXK@Z in that order; retail has 0x00A3A7D0 (`xor eax,eax ; ret`), this body,
//   then the landed CCompressorZlib::CompressStream 0x00A3A7F0. Ego_r.pdb CCompressorZlib::Stream (z_stream_s) starts at
//   +0x20, so +0x20 is Stream.next_in (engine/CCompressorZlib.h: Stream_next_in +0x20). Not C3DMeshInfo::HasPhysicsMesh
//   (a propagated byte-twin label). FINDINGS chunk 411.
// Layout: engine/CCompressorZlib.h is data-only (no member functions), so the class is modelled locally with its offsets.
// Leaf: ?HasStreamInput@CCompressorZlib@@QBE_NXZ
struct z_stream_s {
    const unsigned char* next_in;               // +0x00
    unsigned int         avail_in;              // +0x04
};

class CCompressorZlib {
public:
    unsigned char _pad00[0x20];                 // +0x00 vptr and the compressor base state
    z_stream_s    Stream;                       // +0x20 (first two fields)
    bool HasStreamInput() const;
};

bool CCompressorZlib::HasStreamInput() const
{
    return Stream.next_in != 0;
}
