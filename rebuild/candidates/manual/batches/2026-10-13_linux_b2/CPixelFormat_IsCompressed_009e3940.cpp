// CPixelFormat::IsCompressed()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E3940, oracle length 33 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx] ; lea eax,[eax+eax*8] ; mov eax,[eax*4+0129BA44h] ; cmp eax,4 ; jl F ; cmp eax,5 ; jle T ; cmp eax,7 ; jne F ; T: mov al,1 ; ret ; F: xor al,al ; ret
// Same 36-byte descriptor table as CPixelFormat::IsSigned (batch linux_b1): encoding codes 4, 5
// and 7 are the block-compressed (DXT) formats; 6 is "signed" and is not compressed.
//
// Lane feedback: `switch` over the channel type with cases {4,5,7} reproduces the retail range-split compare
// ladder; an if-chain or a `||` expression does not. RELOCATION_MATCH.
#include "engine/CPixelFormat.h"

struct CPixelFormatDescriptor {
    long          ChannelType;      // +0x00 ; 4,5,7 == compressed (DXT1/DXT3/DXT5 family), 6 == signed
    unsigned char _rest[0x20];
};

extern CPixelFormatDescriptor g_PixelFormatDescriptors[];   // 0x0129BA44 (.rdata)

struct CPixelFormat_Methods : CPixelFormat {
    bool IsCompressed() const;
};

bool CPixelFormat_Methods::IsCompressed() const
{
    // Retail is a switch: `cmp 4 ; jl` / `cmp 5 ; jle` / `cmp 7 ; jne` is the compiler's
    // range split of the case set {4,5,7}; the equivalent if-chain emits 35 bytes.
    switch (g_PixelFormatDescriptors[this->FormatIndex].ChannelType) {
    case 4: case 5: case 7:
        return true;
    default:
        return false;
    }
}
