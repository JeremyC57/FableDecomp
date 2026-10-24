// std::_Construct<CRGBColour, CRGBColour>(CRGBColour* p, const CRGBColour& value) -- STLport placement copy: when p is not null, copy-construct the colour there
// Leaf: _Construct
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045BB8F, length 40 bytes, `__fastcall` (p in ecx, value in edx), plain `ret`.
// Callers: the vector<CRGBColour> copy loops (0x0045BAAA operator=, 0x0045BBC5 _M_allocate_and_copy, this chunk) and the
// landed __uninitialized_fill_n 0x00469149; free template functions of these TUs are `__fastcall` (/Gr).
// Model: CRGBColour is the PDB's 4-byte B, G, R, A union; its user copy ctor goes through an inline Set(r, g, b, a) (the
// landed 0x0064A450 model): the arguments evaluate right to left, so the bytes load A, B, G, R (B spilled to the frame)
// and store R, G, B, A. The `new (p)` null test is the placement-new check.
// Shape: frame pointer + one spill slot = size TU with /Oy off: #pragma optimize("y", off) + ("s", on).
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* where) { return where; }

struct CRGBColour {                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

namespace std {
template <class T1, class T2> inline void __fastcall _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}
template void __fastcall _Construct<CRGBColour, CRGBColour>(CRGBColour* p, const CRGBColour& value);
}