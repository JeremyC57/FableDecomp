// CPixelFormat::CPixelFormat(long rBits, long gBits, long bBits, long aBits) -- pixel format from its RGBA channel widths
// Leaf: ??0CPixelFormat@@QAE@JJJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041C850, length 38 bytes, `ret 10h` (thiscall, returns `this`).
// Name: Ego_r.pdb `??0CPixelFormat@@QAE@JJJJ@Z`; the catalog's "ConstructString" is a propagated label (the body forwards
// four longs plus a constant `false` to the five-argument Initialise and returns `this`). Caller 0x00426705.
// Callee: CPixelFormat::Initialise(long, long, long, long, bool) @ 0x009E3790 (Ego_r.pdb ?Initialise@CPixelFormat@@QAEXJJJJ_N@Z):
// it scans the format table at 0x0129BA44 (0x24-byte entries: type, colour depth, A, R, G, B bits ...) for a type-2
// entry whose R/G/B/A widths equal the arguments and stores the entry index in FormatIndex.
// Layout: CPixelFormat is one long FormatIndex (Ego_r.pdb; engine/CPixelFormat.h holds only that layout and no
// member functions, so the constructor needs this local class of the same shape).
// Shape: /O2; arguments re-pushed in order (right to left) with the trailing `push 0` first.
// Model: local -- engine/CPixelFormat.h exists (FormatIndex +0, same offset), but the generated header is data-only (pack(1), no
// member functions, no virtuals: the vptr and counted pointers are flattened into raw words); the constructor being defined must
// be declared in the class, so it is local.
class CPixelFormat {
public:
    CPixelFormat(long rBits, long gBits, long bBits, long aBits);
    void Initialise(long rBits, long gBits, long bBits, long aBits, bool option);   // 0x009E3790 (fifth argument: meaning unproven)
    long FormatIndex;                                    // +0x00
};
CPixelFormat::CPixelFormat(long rBits, long gBits, long bBits, long aBits)
{
    Initialise(rBits, gBits, bBits, aBits, false);
}