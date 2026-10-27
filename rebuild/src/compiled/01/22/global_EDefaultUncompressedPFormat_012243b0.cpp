// _global::$E_DefaultUncompressedPFormat() -- dynamic initializer of DefaultUncompressedPFormat: the inline CPixelFormat(D3DFMT_A8R8G8B8) constructor, i.e. CPixelFormat::Initialise(21)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012243B0, 13 bytes, in `.text$yc`, reached from the .CRT$XCU table, plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb `dynamic initializer for 'DefaultUncompressedPFormat'`, one of the
//   CNavQuadTree::BlockedNode / CMatrix4x4::Identity / DefaultUncompressedPFormat group (FableWin 0x0400B9A0..0x0400BCF0)
//   whose retail initializers are 0x012243D0 / 0x01224340 / this row; the object 0x013D28E4 is a 4-byte CPixelFormat read by
//   CVolumeTexture::InitialiseSlicesFromTextureList 0x00A68690, and 21 is D3DFMT_A8R8G8B8 (uncompressed 32-bit).
// Label: the manifest's "sub_012243b0" is a gap-scan placeholder.
// Callee: CPixelFormat::Initialise(D3DFORMAT) 0x009E3830 (?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z).
// Layout: Ego_r.pdb CPixelFormat { long FormatIndex; } (4 bytes, trivial destructor: no atexit registration).
// Shape: `call ; ret`, not a tail jump: the thiscall callee pops its argument.
enum _D3DFORMAT { D3DFMT_A8R8G8B8 = 21 };
typedef _D3DFORMAT D3DFORMAT;

class CPixelFormat {
public:
    long FormatIndex;                                     // +0x00
    CPixelFormat(D3DFORMAT format) { Initialise(format); }
    void Initialise(D3DFORMAT format);                    // 0x009E3830
};

CPixelFormat DefaultUncompressedPFormat(D3DFMT_A8R8G8B8);   // 0x013D28E4