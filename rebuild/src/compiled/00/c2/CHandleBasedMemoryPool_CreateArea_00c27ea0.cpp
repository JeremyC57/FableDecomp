// CHandleBasedMemoryPool_012b4098::CreateArea(void* address, unsigned long size, CHeader* prev, CHeader* next) -- build a free area header at the address covering size bytes (header included), link it between its physical neighbours and return it
// Leaf: ?CreateArea@CHandleBasedMemoryPool_Methods@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C27EA0, 65 bytes (`ret 10h`; `this` is not used). The libpk build of the engine's
//   CHandleBasedMemoryPool_012b4098 (the Ego_r.pdb name set holds `CreateArea(void*, unsigned long, CHeader*, CHeader*)`
//   returning CHeader*); the manifest's "WordWrap_SetCallback" is propagated. Callers: Initialise 0x00C28240 (the
//   whole buffer, no neighbours), ResizeArea 0x00C28290 (the spare tail after an area).
// Shape: the header words are stored in the engine's CreateInitialFreeArea order (size - 0x20, padding, 'N' status,
//   lock flag, both physical links, the resource last), then each non-null neighbour is pointed back at the area.
struct CHeader {                                // CHandleBasedMemoryPool_012b4098::CHeader, 32 bytes in front of every area (landed 0x00C27F50 names)
    void*          Resource;                    // +0x00
    long           Handle;                      // +0x04
    long           Size;                        // +0x08
    short          Padding;                     // +0x0c
    unsigned char  Status;                      // +0x0e ('N' free)
    unsigned char  Locked;                      // +0x0f
    CHeader**      RefAvailable;                // +0x10 (the free-list link that points at this area)
    CHeader*       NextAvailable;               // +0x14
    CHeader*       PrevContiguous;              // +0x18
    CHeader*       NextContiguous;              // +0x1c
};
#include "engine/CHandleBasedMemoryPool.h"

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    CHeader* CreateArea(void* address, unsigned long size, CHeader* prev, CHeader* next);
};

CHeader* CHandleBasedMemoryPool_Methods::CreateArea(void* address, unsigned long size, CHeader* prev, CHeader* next)
{
    CHeader* area = (CHeader*)address;
    area->Size = size - sizeof(CHeader);
    area->Padding = 0;
    area->Status = 'N';
    area->Locked = 0;
    area->PrevContiguous = prev;
    area->NextContiguous = next;
    area->Resource = 0;
    if (prev)
        prev->NextContiguous = area;
    if (next)
        next->PrevContiguous = area;
    return area;
}