// CChickenSign::GetLevel() -- the packed parameter record's level getter: the byte at +0 (stored 0..127 by SetLevel) as a 0..1 float
// Leaf: GetLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B9C0, 18 bytes, __fastcall, plain `ret`.
// Callers: 0x00C12AE5 (fstp into a frame float) and 0x00C20592 (`test byte ptr [esi],1 ; lea ecx,[esi+8] ; call` --
//   the record sits at +8 of its owner and the getter feeds the owner's SetLevel twin @ 0x00C1BB20).
// Constant: 1/127 (0x3C010204) @ 0x0129F780.
// Shape: `push ecx ; movzx eax,byte [ecx] ; mov [esp],eax ; fild [esp] ; fmul k` -- an unsigned char widened through a
//   frame dword and multiplied by the reciprocal (the inverse of SetLevel's `x * 127`). The record model is the landed
//   CChickenSign::Init one (0x00C1BC90, same translation unit; the manifest name, no RTTI); descriptive member names.
struct CChickenSign {
    unsigned char  m_level;                     // +0x00 0..1 as 0..127
    unsigned char  m_offset;                    // +0x01
    unsigned char  m_flags;                     // +0x02
    unsigned char  m_byteD;                     // +0x03
    short          m_wordB;                     // +0x04
    short          m_wordC;                     // +0x06
    short          m_scale;                     // +0x08
    unsigned short m_wordA;                     // +0x0a
    unsigned short m_wordE;                     // +0x0c
    unsigned short m_time;                      // +0x0e
    unsigned short m_rangeMin;                  // +0x10
    unsigned short m_rangeMax;                  // +0x12

    float GetLevel();
};

float CChickenSign::GetLevel()
{
    return (float)m_level * (1.0f / 127.0f);
}