// CMaskedChickenSign::SetByteD(unsigned short mask, unsigned char value) -- record the byte field's override mask and store the value in the embedded record
// Leaf: ?SetByteD@CMaskedChickenSign@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C27BF0, 25 bytes (`ret 8`). Descriptive names (no PDB covers the libpk audio
//   library): the manifest's "Thiscall_NavigatorAssign" is propagated. Callers: 0x00C27D2E and MergeByteD 0x00C27DAB.
// Callee: CChickenSign::SetByteD @ 0x00C1BBF0 (landed as "EnableNavigator"). The word twin is 0x00C27BA0.

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
    void SetWordC(unsigned short mask, unsigned short value);   // 0x00C27BA0 (landed as "vector_EWeaponClass_Umove")
    void SetByteD(unsigned short mask, unsigned char value);    // 0x00C27BF0
};

void CMaskedChickenSign::SetByteD(unsigned short mask, unsigned char value)
{
    m_byteDMask = mask;
    m_sign.SetByteD(value);
}