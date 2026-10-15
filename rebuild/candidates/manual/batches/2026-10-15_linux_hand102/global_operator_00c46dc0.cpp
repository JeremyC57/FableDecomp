// operator==(const C2DVector& a, const C2DVector& b) -- exact component-wise equality of two 2D float vectors
// Leaf: ??8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C46DC0, length 37 bytes; `__fastcall` (a in ecx, b in edx, plain `ret`).
// Callers: 0x00C3727D (two frame vectors, the bool tested with `test al,al`) and the thunk 0x00C466B0 (`jmp`). The catalog
// label "CTCTavernGame::IsBetting" is propagated over this body and rejected (the landed CTCTavernGame::IsBetting
// siblings are integer tests on `this`).
// Shape: `fld [edx] ; fld [ecx] ; fucompp ; test ah,44h ; jp` per component (the left operand is loaded last); the `&&` of
// two float equalities returned as bool materialises the full-register `mov eax,1` / `xor eax,eax` exits.
struct C2DVector {
    float X;
    float Y;
};

bool __fastcall operator==(const C2DVector& a, const C2DVector& b)
{
    return a.X == b.X && a.Y == b.Y;
}
