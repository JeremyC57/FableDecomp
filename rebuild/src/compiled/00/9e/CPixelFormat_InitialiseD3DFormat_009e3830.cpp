// CPixelFormat::Initialise(_D3DFORMAT format) -- set FormatIndex to the format's row in the D3D format table
// Retail 0x009E3830, 50 bytes (Ego_r.pdb ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z). Walks the 36-byte
// CD3DFormatInfo table @ 0x0129BA40 (D3DFormat +0, PFType +4, BitDepth +8 ...; BitDepth -1 ends it, as the
// landed CDisplayManager format pickers read it) and stores the matching row index, or -1 when the format
// is not listed. The walk pointer is kept on the BitDepth field (`mov eax,0x0129BA48`, compare at [eax-8]).
// Shape: the first row is compared before any terminator test (a do/while: the extern table rules out a
// folded entry test), the advance and index increment sit between the next BitDepth load and its test. The
// search is an inline helper returning the index or -1 whose result is stored once: VC7.1 duplicates the
// store into both exits and the miss value goes through a register (`or eax,-1 ; mov [ecx],eax`); storing
// -1 directly in the member gives `mov dword ptr [ecx],-1` (51 v 50).
// Model: a local CPixelFormat carrying the PDB member (FormatIndex +0) and this member function;
// engine/CPixelFormat.h is a data-only struct (a member definition needs the class itself to keep the PDB
// symbol ?Initialise@CPixelFormat@@).
// Leaf: ?Initialise@CPixelFormat


#include "engine/CD3DFormatInfo.h"   // CD3DFormatInfo + GD3DFormats 0x0129BA40 (NF28: was a local struct model and g_D3DFormatInfo)


class CPixelFormat
{
public:
    long FormatIndex;                              // +0x00
    void Initialise(_D3DFORMAT format);
};

static inline long FindFormatIndex(_D3DFORMAT format)
{
    long index = 0;
    do
    {
        if (GD3DFormats[index].D3DFormat == format)
            return index;
        ++index;
    } while (GD3DFormats[index].BitDepth != -1);
    return -1;
}

void CPixelFormat::Initialise(_D3DFORMAT format)
{
    FormatIndex = FindFormatIndex(format);
}
