// CPixelFormat::IsSigned()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E3970, oracle length 17 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx] ; lea eax,[eax+eax*8] ; cmp dword ptr [eax*4+0129BA44h],6 ; sete al ; ret
// CPixelFormat is a 4-byte handle (PDB: long FormatIndex) into a static descriptor table in
// .rdata at 0x0129BA44 whose entries are 36 bytes (index*9*4). The member compared against 6 is
// the first dword of the entry; value 6 is the "signed" channel type. The table's real
// element type is not in the PDB export, so a minimal 36-byte descriptor is declared here.
#include "engine/CPixelFormat.h"

struct CPixelFormatDescriptor {
    long          ChannelType;      // +0x00 ; 6 == signed component format
    unsigned char _rest[0x20];      // remaining 32 bytes of the 36-byte descriptor
};

extern CPixelFormatDescriptor g_PixelFormatDescriptors[];   // 0x0129BA44 (.rdata)

struct CPixelFormat_Methods : CPixelFormat {
    bool IsSigned();
};

bool CPixelFormat_Methods::IsSigned()
{
    return g_PixelFormatDescriptors[this->FormatIndex].ChannelType == 6;
}
