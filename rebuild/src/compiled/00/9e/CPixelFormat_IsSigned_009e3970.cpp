// CPixelFormat::IsSigned()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E3970, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx] ; lea eax,[eax+eax*8] ; cmp dword ptr [eax*4+0129BA44h],6 ; sete al ; ret
// CPixelFormat is a 4-byte handle (PDB: long FormatIndex) into a static descriptor table in
// .rdata at 0x0129BA44 whose entries are 36 bytes (index*9*4). The member compared against 6 is
// the first dword of the entry; value 6 is the "signed" channel type. The table's real
// element type is not in the PDB export, so a minimal 36-byte descriptor is declared here.
//
// Lane feedback: `return a == b;` for a bool result is 6 bytes longer (register-width flag then truncate);
// `return (a == b) ? true : false;` reproduces the retail `sete al` directly. RELOCATION_MATCH.
#include "engine/CPixelFormat.h"

#include "engine/CD3DFormatInfo.h"   // CD3DFormatInfo + GD3DFormats 0x0129BA40 (name-fix NF29, both PDBs ?GD3DFormats@@3QBVCD3DFormatInfo@@B: was the local CPixelFormatDescriptor table g_PixelFormatDescriptors)


struct CPixelFormat_Methods : CPixelFormat {
    bool IsSigned() const;
};

bool CPixelFormat_Methods::IsSigned() const
{
    // `cmp mem,6 ; sete al` only comes out of the `?:` form; a bare `return x == 6;`
    // materialises the flag in a full register first (xor ecx,ecx ; sete cl ; mov al,cl).
    return (GD3DFormats[this->FormatIndex].PFType == 6) ? true : false;
}