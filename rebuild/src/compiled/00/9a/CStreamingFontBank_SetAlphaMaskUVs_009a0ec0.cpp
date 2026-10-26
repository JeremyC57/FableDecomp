// CStreamingFontBank::SetAlphaMaskUVs(const C2DVector& uv0, const C2DVector& uv1) -- stores the two alpha-mask texture coordinates the streaming font renderer uses
// Leaf: ?SetAlphaMaskUVs@CStreamingFontBank@@SIXABVC2DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A0EC0, 33 bytes (+ int3 padding), plain `ret`. Catalog row "_global
// SetAlphaMaskUVs"; the Ego_r PDB name is `?SetAlphaMaskUVs@CStreamingFontBank@@SIXABVC2DVector@@0@Z`, a static
// `__fastcall` member (uv0 in ecx, uv1 in edx).
// Globals: the two UV pairs @ 0x013CA2F8 and 0x013CA300 (file statics of the streaming-font module; no PDB names).
// Shape: two whole-object assignments of the 8-byte C2DVector POD (two floats, implicit copy) are dword moves
// through eax/ecx; a user operator= on the float members would go through the x87.
class C2DVector { public:
    float X;                                    // +0x00
    float Y;                                    // +0x04
};

extern C2DVector g_StreamingFontAlphaMaskUV0;   // 0x013CA2F8
extern C2DVector g_StreamingFontAlphaMaskUV1;   // 0x013CA300

class CStreamingFontBank {
public:
    static void __fastcall SetAlphaMaskUVs(const C2DVector& uv0, const C2DVector& uv1);
};

void __fastcall CStreamingFontBank::SetAlphaMaskUVs(const C2DVector& uv0, const C2DVector& uv1)
{
    g_StreamingFontAlphaMaskUV0 = uv0;
    g_StreamingFontAlphaMaskUV1 = uv1;
}