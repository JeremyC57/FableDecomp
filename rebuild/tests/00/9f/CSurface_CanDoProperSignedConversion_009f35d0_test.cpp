// Behaviour test for CSurface::CanDoProperSignedConversion (0x009F35D0).
#include <cstdio>

struct C2DBoxI { long left, top, right, bottom; };
struct CPixelFormat {
    long FormatIndex;                         // test encoding: sign in bit 8, bytes in the low byte
    CPixelFormat(unsigned long f) : FormatIndex((long)f) {}
    bool IsSigned() const { return (FormatIndex & 0x100) != 0; }
    unsigned long GetByteColourDepth() const { return FormatIndex & 0xff; }
};
struct CSurface {
    unsigned long m_format, m_width, m_height;
    CPixelFormat GetPixelFormat() const { return CPixelFormat(m_format); }
    unsigned long GetWidth() const { return m_width; }
    unsigned long GetHeight() const { return m_height; }
    bool CanDoProperSignedConversion(const C2DBoxI* destinationBox, const C2DBoxI* sourceBox, const CPixelFormat& sourceFormat)
    {
        CPixelFormat format = GetPixelFormat();
        if (format.IsSigned() || sourceFormat.IsSigned()) {
            if (!destinationBox || (destinationBox->left == 0 && destinationBox->top == 0
                                    && destinationBox->right == (long)GetWidth() && destinationBox->bottom == (long)GetHeight())) {
                if (!sourceBox || (sourceBox->left == 0 && sourceBox->top == 0
                                   && sourceBox->right == (long)GetWidth() && sourceBox->bottom == (long)GetHeight())) {
                    if (!sourceFormat.IsSigned() && sourceFormat.GetByteColourDepth() == 4
                        && GetPixelFormat().IsSigned() && GetPixelFormat().GetByteColourDepth() == 2)
                        return true;
                }
            }
        }
        return false;
    }
};

int main()
{
    int ok = 1;
    CSurface s; s.m_format = 0x102; s.m_width = 64; s.m_height = 32;      // signed 2-byte surface
    CPixelFormat src4(4), src2(2), srcSigned(0x104);
    C2DBoxI full = { 0, 0, 64, 32 }, part = { 0, 0, 32, 32 }, off = { 1, 0, 64, 32 };
    if (!s.CanDoProperSignedConversion(0, 0, src4)) ok = 0;
    if (!s.CanDoProperSignedConversion(&full, &full, src4)) ok = 0;
    if (s.CanDoProperSignedConversion(&part, 0, src4)) ok = 0;
    if (s.CanDoProperSignedConversion(0, &off, src4)) ok = 0;
    if (s.CanDoProperSignedConversion(0, 0, src2)) ok = 0;            // source must be 4 bytes
    if (s.CanDoProperSignedConversion(0, 0, srcSigned)) ok = 0;       // source must be unsigned
    CSurface u; u.m_format = 0x002; u.m_width = 64; u.m_height = 32; // unsigned surface, unsigned source
    if (u.CanDoProperSignedConversion(0, 0, src4)) ok = 0;
    CSurface w; w.m_format = 0x104; w.m_width = 8; w.m_height = 8;    // signed but 4 bytes
    if (w.CanDoProperSignedConversion(0, 0, src4)) ok = 0;
    if (!ok) { std::printf("009f35d0_TEST FAIL\n"); return 1; }
    std::printf("009f35d0_TEST PASS\n");
    return 0;
}