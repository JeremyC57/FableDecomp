// CTCOverheadDisplay::~CTCOverheadDisplay() -- destroy the primitive-handle vector (each handle's virtual destructor, then the block), release the overhead display def, destroy the renown/repair/damage value lists, then the thing-component base
// Leaf: ??1CTCOverheadDisplay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B80B0, length 105 bytes (tail `jmp` into ~CTCBase).
// Label correction: the manifest calls this ~CTCTargetingBase (propagated); its only caller is the deleting destructor 0x007B8090, slot 0 of
// vtable 0x0126DA94 = CTCOverheadDisplay (rtti-vtables.tsv), and the members fit Ego_r.pdb CTCOverheadDisplay with retail 4-byte lists (the
// landed ctor 0x007B7BC0).
// Callees: CEnginePrimitiveHandle virtual destructor (slot 0, explicit call with flags 0) per 8-byte element, free @ 0x00BFEA14, the def's
// virtual slot 1 (destroy), std::list<pair<long,float> >::~list @ 0x007B7E70 (x3; manifest "CGuiControlTreePane::SortTreeRecursively" is
// propagated), CTCBase::~CTCBase @ 0x00686830 (tail jump; manifest "~CChunkedFileChunk").
// Layout (retail): DamageValues +0x0C, RepairValues +0x10, RenownValues +0x14 (one-pointer STLport lists), PDef +0x18
// (CDefPointer<const COverheadDisplayDef>), PrimitiveHandles +0x1C/+0x20/+0x24 (vector<CEnginePrimitiveHandle>, 8-byte polymorphic
// handles: vptr + handle word).
// Shape: no vtable store, so the class is `__declspec(novtable)` over CTCBase's out-of-line virtual destructor; members die in reverse
// order: the vector (range destroy with `last` read first, then the null-tested free), the def pointer (`if (p) { p->Release(); p = 0; }`,
// Release a member of the def), the three lists out of line.
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
class CThing;
class CTCBase { public:                                // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual ~CTCBase();                         // 0x00686830
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CEnginePrimitiveHandle { public:                 // 8 bytes: vptr + handle
    virtual ~CEnginePrimitiveHandle();
    unsigned long m_handle;
};
struct CPrimitiveHandleVector {                 // retail std::vector: first, last, end (12 bytes)
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    static void Destroy(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last)
    {
        for (; first != last; ++first)
            first->~CEnginePrimitiveHandle();
    }
    ~CPrimitiveHandleVector()
    {
        Destroy(m_pFirst, m_pLast);
        if (m_pFirst)
            free(m_pFirst);
    }
};
class COverheadDisplayDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CDefPointer_COverheadDisplayDef {
    const COverheadDisplayDef* m_pDef;
    ~CDefPointer_COverheadDisplayDef()
    {
        if (m_pDef) {
            const_cast<COverheadDisplayDef*>(m_pDef)->Release();
            m_pDef = 0;
        }
    }
};
struct CValueList {                             // std::list<pair<long,float> >: one sentinel pointer
    void* head;
    ~CValueList();                              // 0x007B7E70
};
struct __declspec(novtable) CTCOverheadDisplay : CTCBase {
    CValueList DamageValues;                    // +0x0c
    CValueList RepairValues;                    // +0x10
    CValueList RenownValues;                    // +0x14
    CDefPointer_COverheadDisplayDef PDef;       // +0x18
    CPrimitiveHandleVector PrimitiveHandles;    // +0x1c
    ~CTCOverheadDisplay();
};

CTCOverheadDisplay::~CTCOverheadDisplay()
{
}