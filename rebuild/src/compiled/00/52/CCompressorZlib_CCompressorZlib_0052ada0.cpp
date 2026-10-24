// CCompressorZlib::CCompressorZlib() -- construct the zlib compressor: the CACompressor base with zlib's workspace size, the CCompressorZlib vtable, then Init() resets the stream state and installs the allocator callbacks
// Leaf: ??0CCompressorZlib
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052ADA0, 33 bytes (48 with padding), plain `ret` (thiscall, no arguments).
// Callees: CCompressorZlib::GetWorkspaceSize() 0x00A3A7D0 (static; folded `xor eax,eax ; ret`), CACompressor::
//   CACompressor(long) 0x00A3AEA0 (landed), CCompressorZlib::Init() 0x00A3AA10 (landed as "Init": zeroes the stream
//   words, stores the zalloc/zfree callbacks 0x00A3A9C0 / 0x00A3AA00, clears the three flags +0x54..+0x56).
// Callers: none by rel32 call or absolute address in retail (w23/xref.py); the constructor is emitted out of line
//   beside its inline uses (the landed 0x00996990 inlines it).
// Label correction: catalogued as "ConvertPathToFullPath"; vtable 0x01245974 is CCompressorZlib (rtti-vtables.tsv,
//   slots 1..3 at 0x0052ADD0..), and Ego_r.pdb names `??0CCompressorZlib@@QAE@XZ`; the landed CCompressedMemoryDataInput
//   Stream ctor 0x00996990 inlines the same constructor.
// Layout: Ego_r.pdb CCompressorZlib (0x5c) has Stream (z_stream_s) +0x20 and the three bools +0x58..; retail
//   CACompressor is 0x1c bytes (its vector 12 bytes, not the PDB's 16), so Stream sits at +0x1c and the flags at
//   +0x54..+0x56 (Init writes them there): 0x58 bytes. Local model like the landed 0x00996990 (no engine header).
// Shape: the base argument is the static call (ecx still holds `this` but is unused), the derived vtable store sits
//   between the base constructor and the body's Init() call.
struct CACompressor {                                           // landed 0x00A3AEA0 (vtable 0x0129B424)
    virtual ~CACompressor();
    unsigned char _body[0x1c - 4];                              // Workspace, WorkspaceSize, CompressionOn, OwnWorkspace
    CACompressor(long workspaceSize);                           // 0x00A3AEA0
};

class CCompressorZlib : public CACompressor {                   // vtable 0x01245974, retail 0x58 bytes
public:
    unsigned char Stream[0x38];                                 // +0x1c z_stream_s
    bool          ZlibInitialised;                              // +0x54
    bool          Compressing;                                  // +0x55
    bool          ErrorFlag;                                    // +0x56
    static long GetWorkspaceSize();                             // 0x00A3A7D0
    void Init();                                                // 0x00A3AA10
    virtual ~CCompressorZlib();
    CCompressorZlib();
};

CCompressorZlib::CCompressorZlib()
    : CACompressor(GetWorkspaceSize())
{
    Init();
}