// CChickenSign::SetByteD(unsigned char value) -- stores the byte field at +0x3 of the packed parameter record
// Leaf: ?SetByteD@CChickenSign@@QAEXE@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BBF0, 10 bytes: mov al,[esp+4] ; mov [ecx+3],al ; ret 4
// Naming (sweep1, 2026-10-25): was landed as the global module "EnableNavigator" over a C00c1bbf0::M stand-in (catalog
//   _global::SetMember_byte0x3_00c1bbf0, manifest CTCPhysicsNavigator::EnableNavigator, propagated; its "// Leaf:
//   EnableNavigator" named no symbol of its object);
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
    void SetByteD(unsigned char value);
};

void CChickenSign::SetByteD(unsigned char value)
{
    m_byteD = value;
}
