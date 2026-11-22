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