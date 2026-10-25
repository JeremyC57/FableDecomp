// CPositionHolder::SetPosition(float x, float y) -- stores a new 2D position in the member pair at +0x14 and reports whether it changed
// Leaf: SetPosition
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34530, 80 bytes, __fastcall `ret 8`, al = changed.
//   Descriptive name (no PDB covers this library): the catalog's `CHeightMapCell_OnHeightChanged` is propagated.
//   The member at +0x14 is a pair of floats with out-of-line accessors, each an identical-code-folded body:
//   GetX 0x00C1BD40 (float +0), GetY 0x00C1BD50 (float +4), SetX 0x00C1BD60 (+0), SetY 0x00C1BD70 (+4).
// Shape: y is compared first (call, fld arg, fucompp, `test ah,44h; jp` to the store block), then x; both equal
//   returns 0 on the fall-through.
struct CFloatPair {
    float m_x;                              // +0x00
    float m_y;                              // +0x04
    float GetX() const;                     // 0x00C1BD40
    float GetY() const;                     // 0x00C1BD50
    void SetX(float x);                     // 0x00C1BD60
    void SetY(float y);                     // 0x00C1BD70
};

struct CPositionHolder {
    unsigned char _pad00[0x14];
    CFloatPair m_position;                  // +0x14
    bool SetPosition(float x, float y);
};

bool CPositionHolder::SetPosition(float x, float y)
{
    if (m_position.GetY() == y && m_position.GetX() == x)
        return false;
    m_position.SetX(x);
    m_position.SetY(y);
    return true;
}