// CPixelFormat::IsCompressed()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E3940, oracle length 33 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx] ; lea eax,[eax+eax*8] ; mov eax,[eax*4+0129BA44h] ; cmp eax,4 ; jl F ; cmp eax,5 ; jle T ; cmp eax,7 ; jne F ; T: mov al,1 ; ret ; F: xor al,al ; ret
// Same 36-byte descriptor table as CPixelFormat::IsSigned (batch linux_b1): encoding codes 4, 5
// and 7 are the block-compressed (DXT) formats; 6 is "signed" and is not compressed.
#include "engine/CPixelFormat.h"

struct CPixelFormatDescriptor {
    long          ChannelType;      // +0x00 ; 4,5,7 == compressed, 6 == signed
    unsigned char _rest[0x20];
};

extern CPixelFormatDescriptor g_PixelFormatDescriptors[];   // 0x0129BA44 (.rdata)

struct CPixelFormat_Methods : CPixelFormat {
    bool IsCompressed();
};

bool CPixelFormat_Methods::IsCompressed()
{
    long type = g_PixelFormatDescriptors[this->FormatIndex].ChannelType;
    if (type < 4)
        return false;
    if (type <= 5)
        return true;
    if (type != 7)
        return false;
    return true;
}
