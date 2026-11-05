// CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip() -- destroys the intelligent pointer at +0x2c, frees the blocks of the two array records at +0x18/+0x0c, then tail-jumps to the base destructor
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DDF6F, 47 bytes. Pragma: optimize("s") (`pop ecx` cleanup after each free).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10, free @ 0x00BFEA14 (CRT),
// base destructor @ 0x00686830 (labelled "~CChunkedFileChunk"; the 0x0c-byte base whose destructor is out of line).
// Shape: the intelligent pointer is destroyed BEFORE the two frees, so the frees are not body statements (a body
// runs before any member destructor): all three are MEMBER objects destroyed in reverse declaration order -- two
// 12-byte array records whose inline destructor frees their first word (`__forceinline`: a size TU would call an
// inline destructor out of line), then the intelligent pointer declared last. The dtor label is propagated.
#pragma optimize("s", on)

extern "C" void __cdecl free(void* memory);         // 0x00BFEA14

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CHeapArray {                                 // 12-byte array record: data, size, capacity
    void* m_pData;
    long  m_size;
    long  m_capacity;
    __forceinline ~CHeapArray() { if (m_pData != 0) free(m_pData); }
};

struct CChunkedFileChunkBase {
    unsigned char m_baseData[0x0c];
    ~CChunkedFileChunkBase();                       // 0x00686830
};

class CPatchTesselationEdgeStrip : public CChunkedFileChunkBase { public:
    CHeapArray              m_arrayA;               // +0x0c
    CHeapArray              m_arrayB;               // +0x18
    unsigned char           m_pad24[0x08];
    CBaseIntelligentPointer m_pOwner;               // +0x2c
    ~CPatchTesselationEdgeStrip();
};

CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip()
{
}