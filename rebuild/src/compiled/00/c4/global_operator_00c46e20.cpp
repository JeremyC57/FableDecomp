// operator<(const C2DVector& a, const C2DVector& b) -- strict lexicographic order of two 2D float vectors: by X, ties broken by Y
// Leaf: ??M@YI_NABUC2DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C46E20, 41 bytes, `__fastcall` (a in ecx, b in edx), plain `ret`, bool.
// Only reference: the tail `jmp` of the region-lookup key's operator< 0x00C466C0 (equal lengths are ordered by the
//   position vector). Sibling of the landed `__fastcall` C2DVector operator== 0x00C46DC0. The manifest's
//   "ComparisonSort_FloatPairComparator" describes it.
// Shape: `fld [edx] ; fld [ecx] ; fucompp ; test ah,44h ; jp` for the X equality; both `<` arms (`fld [ecx+n] ;
//   fcomp [edx+n]`) share one `fnstsw ; test ah,5 ; jp` tail with full-register `mov eax,1` / `xor eax,eax` exits.
struct C2DVector {
    float X;                                    // +0x00
    float Y;                                    // +0x04
};

bool __fastcall operator<(const C2DVector& a, const C2DVector& b)
{
    if (a.X == b.X)
        return a.Y < b.Y;
    return a.X < b.X;
}