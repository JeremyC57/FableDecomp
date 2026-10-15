// CTextureManager::Uninitialise() -- clears the Initialised flag and drops the texture-buffer pointer
// Retail: Fable.exe @ 0x00A696D0, 22 bytes. No callees.
// Layout: the retail CTextureManager is 0x40 bytes shorter than the PDB header (engine/CTextureManager.h): its
// CPreallocTexturePool is 0x54 bytes (FrameEnd @ 0x00A698B0 steps the pool array by 0x54), so FailedAllocations
// sits at +0x58c and TextureBuffer at +0x5d0 (header: +0x5cc / +0x610). Modelled locally with the real member names.
// Shape: `if (TextureBuffer) TextureBuffer = 0;` -- the shared zero register is compared against the pointer and
// stored into both the flag byte and the pointer (`xor eax,eax ; cmp edx,eax ; mov [ecx+4],al ; je ; mov [..],eax`).
struct CPreallocTexturePool { unsigned char _body[0x54]; void FrameEnd(); };   // FrameEnd 0x009FC150 (labelled CResourceList::FrameEnd)
struct CTextureManager {
    void*                __vftable;                    // +0x00
    bool                 Initialised;                  // +0x04
    unsigned long        PoolCount;                    // +0x08
    unsigned long        PreallocSizes[16];            // +0x0c
    CPreallocTexturePool PreallocPools[16];            // +0x4c (retail stride 0x54)
    unsigned long        FailedAllocations[16];        // +0x58c
    long                 BufferSize;                   // +0x5cc
    void*                TextureBuffer;                // +0x5d0
    void Uninitialise();
};

void CTextureManager::Uninitialise()
{
    Initialised = false;
    if (TextureBuffer)
        TextureBuffer = 0;
}
