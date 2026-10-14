// CInterpolationInfo::CInterpolationInfo() -- clear the thirteen words of the record (+0..+0x30)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415A60, length 43 bytes.
// Shape: `mov eax,ecx ; xor ecx,ecx` feeding thirteen stores, `ret`.
struct CInterpolationInfo {
    long m_0, m_4, m_8, m_c, m_10, m_14, m_18, m_1c, m_20, m_24, m_28, m_2c, m_30;
    CInterpolationInfo();
};

CInterpolationInfo::CInterpolationInfo()
    : m_0(0), m_4(0), m_8(0), m_c(0), m_10(0), m_14(0), m_18(0), m_1c(0), m_20(0), m_24(0), m_28(0), m_2c(0), m_30(0)
{
}
