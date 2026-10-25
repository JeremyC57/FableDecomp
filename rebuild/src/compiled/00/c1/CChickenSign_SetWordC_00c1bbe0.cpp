// CChickenSign::SetWordC(unsigned short value) -- stores the 16-bit word field at +0x6 of the packed parameter record
// Leaf: ?SetWordC@CChickenSign@@QAEXG@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BBE0, 12 bytes: mov ax,[esp+4] ; mov [ecx+6],ax ; ret 4
// Naming (sweep1, 2026-10-25): was landed as a fake `__fastcall` CThingImpl::SetCurrentMapNumber(void* edx_unused,
//   short) (catalog CThing::SetCurrentMapNumber, propagated); now the plain thiscall member it is (ecx = this, the
//   value on the stack, `ret 4`);
//   descriptive (no PDB covers the libpk library). The record is the packed 20-byte parameter record the landed
//   CChickenSign::Init 0x00C1BC90 resets through its setters (SetWordC 0x00C1BBE0 = +6, SetByteD 0x00C1BBF0 = +3) and
//   whose accessors the landed CMaskedChickenSign rows (GetWordC 0x00C27BC0, SetByteD 0x00C27BF0, MergeWordC 0x00C27D40,
//   MergeByteD 0x00C27D80) declare as `unsigned short GetWordC() const // 0x00C1BAA0`, `void SetWordC(unsigned short)
//   // 0x00C1BBE0` and `void SetByteD(unsigned char) // 0x00C1BBF0` (FINDINGS chunk 435, agent435 NOTES). The class
//   name CChickenSign is the landed family name (see NOTES.md: it is a propagated game name; owner item).
struct CChickenSign {                           // the packed 20-byte parameter record (landed CChickenSign::Init 0x00C1BC90)
    unsigned char  m_level;                     // +0x00
    unsigned char  m_offset;                    // +0x01
    unsigned char  m_flags;                     // +0x02
    unsigned char  m_byteD;                     // +0x03
    short          m_wordB;                     // +0x04
    unsigned short m_wordC;                     // +0x06
    short          m_scale;                     // +0x08
    unsigned short m_wordA;                     // +0x0a
    unsigned short m_wordE;                     // +0x0c
    unsigned short m_time;                      // +0x0e
    unsigned short m_rangeMin;                  // +0x10
    unsigned short m_rangeMax;                  // +0x12
    void SetWordC(unsigned short value);
};

void CChickenSign::SetWordC(unsigned short value)
{
    m_wordC = value;
}
