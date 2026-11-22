#include <assert.h>
#include <stdio.h>
// Leaf: ?ConvertRGBToPFDData@CPixelFormat@@QBEXABVCRGBColour@@PAE@Z
// Flags:
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_CONVERT
#define FABLE_RETAIL_CPIXELFORMAT_POSSHIFT_FORMATREF
#include <string.h>
#include "engine/CPixelFormat.h"
#include "engine/CRGBColour.h"

void CPixelFormat::ConvertRGBToPFDData(const CRGBColour& rgb, unsigned char* pfd_data) const
{
    unsigned long pfd_long = ((rgb.GetA() >> GetAScaleShift()) << GetAPosShift())
                           + ((rgb.GetR() >> GetRScaleShift()) << GetRPosShift())
                           + ((rgb.GetG() >> GetGScaleShift()) << GetGPosShift())
                           + ((rgb.GetB() >> GetBScaleShift()) << GetBPosShift());
    unsigned char* pb_pfd = reinterpret_cast<unsigned char*>(&pfd_long);
    memcpy(pfd_data, pb_pfd, GetByteColourDepth());
}


extern const CD3DFormatInfo GD3DFormats[] =
{
    { D3DFMT_A8R8G8B8, PF_TYPE_RGBA, 32, 8, 8, 8, 8, 0, true },
    { D3DFMT_R8G8B8,   PF_TYPE_RGBA, 24, 0, 8, 8, 8, 0, true },
    { D3DFMT_R5G6B5,   PF_TYPE_RGBA, 16, 0, 5, 6, 5, 0, true },
    { D3DFMT_A8,       PF_TYPE_RGBA,  8, 8, 0, 0, 0, 0, true }
};

namespace
{
class TestPixelFormat : public CPixelFormat
{
public:
    explicit TestPixelFormat(long format) { FormatIndex = format; }
};

void Check(long format, const CRGBColour& rgb, const unsigned char* expected, unsigned long count)
{
    const TestPixelFormat pixel_format(format);
    unsigned char bytes[6] = { 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7 };
    pixel_format.ConvertRGBToPFDData(rgb, bytes + 1);
    assert(bytes[0] == 0xe7);
    for (unsigned long i = 0; i < count; ++i)
        assert(bytes[i + 1] == expected[i]);
    for (unsigned long j = count + 1; j < sizeof(bytes); ++j)
        assert(bytes[j] == 0xe7);
}
}

int main()
{
    const CRGBColour rgb(250, 145, 75, 173);
    const unsigned char full[] = { 0x4b, 0x91, 0xfa, 0xad };
    const unsigned char rgb565[] = { 0x89, 0xfc };
    const unsigned char alpha[] = { 0xad };
    const unsigned char zero[] = { 0, 0, 0, 0 };
    Check(0, rgb, full, 4);
    Check(1, rgb, full, 3);
    Check(2, rgb, rgb565, 2);
    Check(3, rgb, alpha, 1);
    Check(0, CRGBColour(0, 0, 0, 0), zero, 4);
    puts("009e3f60_TEST PASS");
}