// CBufferPairTriple_006b5b80::~CBufferPairTriple_006b5b80() -- destructor of a class whose only destructible member is an array of three buffer pairs at +0x1a8: the pairs are destroyed last to first, each freeing its second buffer then its first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B5B80, length 56 bytes.
// Callees: free @ 0x00BFEA14 (x2 per element). Shape: VC7.1 destroys a member array in reverse: esi starts one element past the end
// (`lea esi,[ecx+1F0h]` = +0x1a8 + 3*0x18), the loop reads the second buffer pointer through `[esi-0Ch]`, steps the pointer down by 0x18,
// then reads the first buffer through `[esi]`; the counter runs in edi from 3. Each 12-byte buffer is a {pointer, size, capacity} record
// whose inline destructor is `if (p) free(p)` (the members of the pair are destroyed in reverse declaration order, hence +0xc before +0).
// The sibling body @ 0x006B5D80 is the same class shape with the array at +0x1ac. The manifest label
// "CEngineWaterBackgroundSubPatch::OnPostRelocation" is propagated; the class is named by address.
#include <stdlib.h>

struct CRawBuffer_006b5b80 {                       // 12 bytes: a malloc'd block with its size and capacity
    void* m_pData;
    long  m_size;
    long  m_capacity;
    ~CRawBuffer_006b5b80() { if (m_pData) free(m_pData); }
};

struct SBufferPair_006b5b80 {                      // 0x18 bytes
    CRawBuffer_006b5b80 m_first;                   // +0x00
    CRawBuffer_006b5b80 m_second;                  // +0x0c
};

struct CBufferPairTriple_006b5b80 {
    unsigned char       _pad[0x1a8];
    SBufferPair_006b5b80 m_pairs[3];               // +0x1a8..+0x1f0
    ~CBufferPairTriple_006b5b80();
};

CBufferPairTriple_006b5b80::~CBufferPairTriple_006b5b80()
{
}
