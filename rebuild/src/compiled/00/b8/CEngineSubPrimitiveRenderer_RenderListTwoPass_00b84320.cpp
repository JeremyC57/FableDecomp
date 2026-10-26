// CEngineSubPrimitiveRenderer::RenderListTwoPass(CRetailEntryVector*, void*, void*) -- for every 8-byte entry of the vector, walk its sub-primitive chain twice: first rendering only the sub-primitives flagged for the first pass with the first params, then every sub-primitive with the second params
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B84320, length 100 bytes (+ int3 padding).
// Callees: virtual slot 5 (+0x14) of each sub-primitive's manager (+0x14 of it; arguments: the sub-primitive, the params).
// Model reused from the landed sibling CEngineSubPrimitiveRenderer::RenderList @ 0x00B842D0 (same vector, entry and chain).
// Shape: a `__stdcall` static (`ret 0Ch`); the count (`sub ; sar 3`) is taken once into ebp, the index runs in ebx, the
// vector is re-read from its stack slot at each outer iteration; both chain walks are do/while (no entry null test),
// the head is kept in esi for the second walk and the first walk runs in edi; the pass flag is bit 2 of the byte at +4.
// `this` is not used, so the engine header (engine/CEngineSubPrimitiveRenderer.h) is not needed.
struct CEngineSubPrimitive_00b84320;

struct CSubPrimitiveManager_00b84320 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void RenderSubPrimitive(CEngineSubPrimitive_00b84320* sub, void* params);   // slot 5 (+0x14)
};

struct CEngineSubPrimitive_00b84320 {
    unsigned char                  _pad[4];
    unsigned char                  m_flags;        // +0x04 (bit 2: rendered in the first pass)
    unsigned char                  _pad5[0xb];
    CEngineSubPrimitive_00b84320*  m_pNext;        // +0x10
    CSubPrimitiveManager_00b84320* m_pManager;     // +0x14
};

struct SRenderEntry_00b84320 { long m_key; CEngineSubPrimitive_00b84320* m_pFirst; };   // 8 bytes

struct CRetailEntryVector_00b84320 {
    SRenderEntry_00b84320* m_pFirst;
    SRenderEntry_00b84320* m_pLast;
    SRenderEntry_00b84320* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
};

class CEngineSubPrimitiveRenderer { public:
    static void __stdcall RenderListTwoPass(CRetailEntryVector_00b84320* list, void* firstPassParams, void* secondPassParams);
};

void __stdcall CEngineSubPrimitiveRenderer::RenderListTwoPass(CRetailEntryVector_00b84320* list, void* firstPassParams, void* secondPassParams)
{
    unsigned long count = list->GetCount();
    for (unsigned long i = 0; i < count; ++i) {
        CEngineSubPrimitive_00b84320* first = list->m_pFirst[i].m_pFirst;   // never null: both walks are do/while
        CEngineSubPrimitive_00b84320* sub = first;
        do {
            if (sub->m_flags & 4)
                sub->m_pManager->RenderSubPrimitive(sub, firstPassParams);
            sub = sub->m_pNext;
        } while (sub);
        sub = first;
        do {
            sub->m_pManager->RenderSubPrimitive(sub, secondPassParams);
            sub = sub->m_pNext;
        } while (sub);
    }
}