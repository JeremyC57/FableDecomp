// CMaskedChickenSign::SetWordC(unsigned short mask, unsigned short value) -- record the word field's override mask (+4) and store the value in the embedded record (+8)
// Leaf: ?SetWordC@CMaskedChickenSign@@QAEXGG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C27BA0, 23 bytes (`ret 8`): mov ax,[esp+4] ; mov edx,[esp+8] ; mov [ecx+4],ax ;
//   push edx ; add ecx,8 ; call 0x00C1BBE0 ; ret 8.
// Naming (sweep1, 2026-10-25): was landed as a fake `__fastcall` vector_EWeaponClass_Umove(WVec* self, int edx, ...)
//   stand-in (catalog std::vector<EWeaponClass>::_Umove, propagated); descriptive (no PDB covers the libpk audio
//   library). It is the word twin of the landed CMaskedChickenSign::SetByteD 0x00C27BF0 (same shape: the mask to +4
//   instead of +6, CChickenSign::SetWordC 0x00C1BBE0 instead of SetByteD 0x00C1BBF0), which declares this address as
//   `void SetWordC(unsigned short mask, unsigned short value)`; its callers are the landed MergeWordC 0x00C27D40
//   (0x00C27D6B) and 0x00C27D19 (FINDINGS chunk 435, agent435 NOTES). Now the plain thiscall member it is.

struct CChickenSign {                           // the packed parameter record (chunk 433 model, landed 0x00C1BC90)
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
    unsigned short GetWordC() const;            // 0x00C1BAA0 (`mov ax,[ecx+6]`)
    unsigned short GetByteD() const;            // 0x00C1BAB0 (`movzx ax,byte [ecx+3]`)
    void SetWordC(unsigned short value);        // 0x00C1BBE0
    void SetByteD(unsigned char value);         // 0x00C1BBF0
};

struct CMaskedChickenSign {                     // a CChickenSign plus per-field override masks
    unsigned long  m_flags;                     // +0x00
    unsigned short m_wordCMask;                 // +0x04 bits of m_wordC this record owns
    unsigned short m_byteDMask;                 // +0x06 bits of m_byteD this record owns
    CChickenSign   m_sign;                      // +0x08
    void SetWordC(unsigned short mask, unsigned short value);
    void SetByteD(unsigned short mask, unsigned char value);    // 0x00C27BF0
};

void CMaskedChickenSign::SetWordC(unsigned short mask, unsigned short value)
{
    m_wordCMask = mask;
    m_sign.SetWordC(value);
}