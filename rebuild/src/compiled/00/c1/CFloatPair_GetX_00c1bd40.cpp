// CFloatPair::GetX() const -- returns the first float on the FPU stack
// Leaf: ?GetX@CFloatPair@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BD40, 3 bytes, __thiscall `ret`: fld dword ptr [ecx] ; ret
// Was landed as CHeightMapCell::GetHeight (catalog; propagated). Callers 0x00C3454A (SetPosition) and 0x00C61DF9.
// Naming (sweep1, 2026-10-25): the four accessors 0x00C1BD40 / 0x00C1BD50 / 0x00C1BD60 / 0x00C1BD70 are the ones the
//   landed CPositionHolder::SetPosition 0x00C34530 calls as CFloatPair::GetY / GetX / SetX / SetY on its +0x14 member;
//   the libpk voice setup 0x00C34390 sets the +0x14 and +0x1c pairs with SetX / SetY (calls 0x00C343A4 .. 0x00C343C6,
//   `push 0` for 0.0f). Descriptive (no PDB covers libpk; CFloatPair is that row's name).
struct CFloatPair {                             // 8 bytes: two floats (the voice's +0x14 / +0x1c pairs)
    float m_x;                                  // +0x00
    float m_y;                                  // +0x04
    float GetX() const;                         // 0x00C1BD40
    float GetY() const;                         // 0x00C1BD50
    void SetX(float x);                         // 0x00C1BD60
    void SetY(float y);                         // 0x00C1BD70
};

float CFloatPair::GetX() const
{
    return m_x;
}
