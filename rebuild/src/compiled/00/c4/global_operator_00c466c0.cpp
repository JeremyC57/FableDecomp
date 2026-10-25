// operator<(const SRegionLookupKey& a, const SRegionLookupKey& b) -- order of the 12-byte region lookup keys: by the cached length, equal lengths ordered by the position vector (C2DVector operator<, reached by a tail jump)
// Leaf: ??M@YI_NABUSRegionLookupKey@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C466C0, 42 bytes (+ int3 padding), `__fastcall` (a in ecx, b in edx), `ret`, bool.
// Key layout: the landed constructor 0x00C46550 (SRegionLookupKey: C2DVector Position +0, float Length +8 =
//   Position.GetLength()). Callers: 0x00C46C6E and 0x00C46CAB (the lookup table's compare). The manifest's
//   "ComparisonSort_FloatCompare" describes it.
// Callee: C2DVector operator< 0x00C46E20 (`__fastcall`, defined in another translation unit, so the equal-length arm
//   becomes `jmp` with a and b still in ecx/edx: Position is the key's first member).
// Shape: `fld [edx+8] ; fld [ecx+8] ; fucompp ; test ah,44h ; jp` then the tail jump; the `<` arm returns through
//   `fnstsw ; test ah,5 ; jp` with full-register 1/0.
struct C2DVector {
    float X;                                    // +0x00
    float Y;                                    // +0x04
};

bool __fastcall operator<(const C2DVector& a, const C2DVector& b);   // 0x00C46E20

struct SRegionLookupKey {
    C2DVector Position;                         // +0x00
    float     Length;                           // +0x08
};

bool __fastcall operator<(const SRegionLookupKey& a, const SRegionLookupKey& b)
{
    if (a.Length == b.Length)
        return a.Position < b.Position;
    return a.Length < b.Length;
}