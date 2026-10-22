// CMediaType::CMediaType() -- default constructor of a 0x48-byte libpk record: the whole object zeroed (`rep stosd`, 18 words) then the two scale words at +0x28 and +0x20 set to 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1D00, length 29 bytes.
// Callees: none (memset is the intrinsic `rep stosd`). Shape: `this` moves to edx so ecx can be the stosd count; the constant 1 is shared by the
// two stores (+0x28 first, then +0x20, in source order); `mov eax,edx` returns `this`. The twin @ 0x00CB1AC0 is the same body without the
// `this` return (a void Reset). The manifest label "CEntry::CEntry" is propagated; the class is named by address.
#include <string.h>

struct CCodecState_00cb1d00 {                      // 0x48 bytes
    long m_0, m_4, m_8, m_c, m_10, m_14, m_18, m_1c;
    long m_20;                                     // +0x20 scale, 1 after construction
    long m_24;
    long m_28;                                     // +0x28 scale, 1 after construction
    long m_2c, m_30, m_34, m_38, m_3c, m_40, m_44;
    CCodecState_00cb1d00();
};

CCodecState_00cb1d00::CCodecState_00cb1d00()
{
    memset(this, 0, sizeof(*this));
    m_28 = 1;
    m_20 = 1;
}