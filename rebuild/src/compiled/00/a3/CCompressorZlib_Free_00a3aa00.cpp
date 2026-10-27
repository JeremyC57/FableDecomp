// CCompressorZlib::Free(void*, void*) -- zlib zfree callback: frees the block
// Retail: Fable.exe 0x00A3AA00, 12 bytes: `mov eax,[esp+8] ; push eax ; call free ; pop ecx ; ret`.
// Identity: Ego_r.pdb ?Free@CCompressorZlib@@CAXPAX0@Z (private static, cdecl); stored as the z_stream zfree
// (z_stream +0x24) next to Alloc 0x00A3A9C0 by CCompressorZlib::Initialise 0x00A3AA10 and the Internal(De)Compress
// local streams. The opaque argument is unused.
// Callees: free thunk 0x00BFEA14.
// Leaf: ?Free@CCompressorZlib@@CAXPAX0@Z
extern "C" void __cdecl free(void* p);   // 0x00BFEA14

#define CCompressorZlib DonorCCompressorZlib
#include "engine/CCompressorZlib.h"
#undef CCompressorZlib
class CCompressorZlib : public DonorCCompressorZlib {
private:
    static void Free(void* opaque, void* address);
};

void CCompressorZlib::Free(void* opaque, void* address)
{
    free(address);
}