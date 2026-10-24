// CInputTypeStateRecord::ClearState() -- zero the seven state dwords at +0x0C..+0x24
// Retail 0x0066FA30, 24 bytes: one shared `xor eax,eax` then seven dword stores in the order
// +0x20, +0x1C, +0x18, +0x0C, +0x24, +0x14, +0x10, `ret` (no `this` returned, so a void member,
// not a constructor). Like its neighbours 0x0066F9E0 / 0x0066FA50 it is an out-of-line inline
// member left in the input-type TU with no call site, jump or absolute reference anywhere in
// Fable.exe, so the owning class cannot be pinned; the class/member names are descriptive.
// Shape: VC7.1 keeps plain zero stores in source statement order, so the body lists the
// assignments in retail's store order.
// Leaf: ClearState

class CInputTypeStateRecord
{
public:
    void ClearState();

    unsigned long m_header[3];      // +0x00..+0x0B (not touched)
    long m_state0C;                 // +0x0C
    long m_state10;                 // +0x10
    long m_state14;                 // +0x14
    long m_state18;                 // +0x18
    long m_state1C;                 // +0x1C
    long m_state20;                 // +0x20
    long m_state24;                 // +0x24
};

void CInputTypeStateRecord::ClearState()
{
    m_state20 = 0;
    m_state1C = 0;
    m_state18 = 0;
    m_state0C = 0;
    m_state24 = 0;
    m_state14 = 0;
    m_state10 = 0;
}
