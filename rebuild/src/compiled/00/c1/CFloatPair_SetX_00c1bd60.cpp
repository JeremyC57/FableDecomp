// CFloatPair::SetX(float x) -- stores the first float
// Leaf: ?SetX@CFloatPair@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BD60, 9 bytes, __thiscall `ret 4`: mov eax,[esp+4] ; mov [ecx],eax ; ret 4
// Was landed as CClass_00c1bd60::__EH_epilog3(long) (FINDINGS chunk 437: "labelled __EH_epilog3 is a plain setter"). Callers 0x00C343A4, 0x00C343BB and 0x00C34569 (SetPosition).
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

void CFloatPair::SetX(float x)
{
    m_x = x;
}
