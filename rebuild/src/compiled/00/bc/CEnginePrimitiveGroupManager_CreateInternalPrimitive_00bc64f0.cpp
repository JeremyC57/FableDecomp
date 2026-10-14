// CEnginePrimitiveGroupManager::CreateInternalPrimitive(long) -- allocate and construct a group primitive (0x60 bytes: base type 1, flags 0, two cleared words, the empty handle vector, cleared primitive flags, post-render enabled); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC64F0, length 80 bytes.
// Callees: operator new @ 0x00BFEA1A (non-throwing: `cmp esi,edi ; je` on the zero register), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @ 0x00B8FF00
// (type 1, flags 0). Vtable 0x012A66E4 = CEngineInternalPrimitiveGroup (rtti-vtables.tsv). Shape: the inlined group constructor zeroes +0x48 and +0x4c, stores the
// vtable, zeroes the handle vector (+0x50..+0x58) and the flags (+0x5c) from edi, and ORs bit 3 into the byte at +0x34; the argument is unused.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

struct CEngineInternalPrimitiveBase {              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x30];                    // +0x04 .. +0x34
    unsigned char PostRenderEnable;                // +0x34 bitfield group: bit 3 = post-render enabled
    unsigned char _pad35[0x13];                    // +0x35 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

struct CEnginePrimitiveHandle { long m_id; CEngineInternalPrimitiveBase* m_pPrimitive; };

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CEngineInternalPrimitiveGroup : CEngineInternalPrimitiveBase {   // vtable 0x012A66E4; 0x60 bytes
    long                m_a;                       // +0x48
    long                m_b;                       // +0x4c
    CRetailHandleVector Primitives;                // +0x50
    unsigned long       PrimitiveFlags;            // +0x5c
    virtual void v00();
    CEngineInternalPrimitiveGroup()
        : CEngineInternalPrimitiveBase(1, 0), m_a(0), m_b(0), Primitives(), PrimitiveFlags(0)
    {
        PostRenderEnable |= 8;
    }
};

struct CEnginePrimitiveGroupManager {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveGroupManager::CreateInternalPrimitive(long type)
{
    return new CEngineInternalPrimitiveGroup;
}