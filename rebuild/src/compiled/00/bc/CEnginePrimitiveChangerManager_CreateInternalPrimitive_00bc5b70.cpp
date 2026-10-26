// CEnginePrimitiveChangerManager::CreateInternalPrimitive(long) -- allocate and construct a changer primitive (0x64 bytes: base type 2, flags 0, the empty handle vector, no current child, post-render enabled); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC5B70, length 80 bytes.
// Callees: operator new @ 0x00BFEA1A (non-throwing: `cmp esi,edi ; je` on the zero register), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @ 0x00B8FF00
// (type 2, flags 0). Vtable 0x012A662C = CEngineInternalPrimitiveChanger (rtti-vtables.tsv). Shape: the inlined changer constructor stores the vtable, zeroes the
// handle vector (+0x48..+0x54) and the flags (+0x60) from edi, ORs bit 3 into the byte at +0x34 (`mov al ; or al,8 ; mov`), and stores -1 into the current index
// (+0x58) last; the argument is unused.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

class CEngineInternalPrimitiveBase { public:              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x30];                    // +0x04 .. +0x34
    unsigned char PostRenderEnable;                // +0x34 bitfield group: bit 3 = post-render enabled
    unsigned char _pad35[0x13];                    // +0x35 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

class CEnginePrimitiveHandle { public: long m_id; CEngineInternalPrimitiveBase* m_pPrimitive; };

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle> plus a fourth word the changer clears with it
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    long                    m_reserved;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0), m_reserved(0) { }
};

struct CEngineInternalPrimitiveChanger : CEngineInternalPrimitiveBase {   // vtable 0x012A662C; 0x64 bytes
    CRetailHandleVector Primitives;                // +0x48 .. +0x58 (four words)
    long                m_currentIndex;            // +0x58 (-1 = none)
    long                _pad5c;                    // +0x5c (not initialised)
    unsigned long       m_flags;                   // +0x60
    virtual void v00();
    CEngineInternalPrimitiveChanger()
        : CEngineInternalPrimitiveBase(2, 0), Primitives(), m_flags(0)
    {
        PostRenderEnable |= 8;
        m_currentIndex = -1;
    }
};

class CEnginePrimitiveChangerManager { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveChangerManager::CreateInternalPrimitive(long type)
{
    return new CEngineInternalPrimitiveChanger;
}