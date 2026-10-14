// CEngineSubPrimitiveRenderer::RenderList(CRetailEntryVector* list, void* params) -- for every 8-byte entry of the vector, walk the sub-primitive chain from its second word (+0x10 links) and render each through its manager's virtual slot 5 with the params
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B842D0, length 69 bytes.
// Callees: virtual slot 5 (+0x14) of the sub-primitive's manager (+0x14 of it; arguments: the sub-primitive, the params). Shape: the count (`sub ; sar 3`) is taken
// once into edi, the index runs in ebp, the vector is re-read from the stack at each outer iteration (`jmp` into the loop with the first read hoisted), the
// params stay in ebx; no `this` (a `__stdcall` static), `ret 8`.
struct CEngineSubPrimitive_00b842d0;

struct CSubPrimitiveManager_00b842d0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void RenderSubPrimitive(CEngineSubPrimitive_00b842d0* sub, void* params);   // slot 5 (+0x14)
};

struct CEngineSubPrimitive_00b842d0 {
    unsigned char                 _pad[0x10];
    CEngineSubPrimitive_00b842d0* m_pNext;         // +0x10
    CSubPrimitiveManager_00b842d0* m_pManager;     // +0x14
};

struct SRenderEntry_00b842d0 { long m_key; CEngineSubPrimitive_00b842d0* m_pFirst; };   // 8 bytes

struct CRetailEntryVector_00b842d0 {
    SRenderEntry_00b842d0* m_pFirst;
    SRenderEntry_00b842d0* m_pLast;
    SRenderEntry_00b842d0* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
};

struct CEngineSubPrimitiveRenderer {
    static void __stdcall RenderList(CRetailEntryVector_00b842d0* list, void* params);
};

void __stdcall CEngineSubPrimitiveRenderer::RenderList(CRetailEntryVector_00b842d0* list, void* params)
{
    unsigned long count = list->GetCount();
    for (unsigned long i = 0; i < count; ++i) {
        CEngineSubPrimitive_00b842d0* sub = list->m_pFirst[i].m_pFirst;   // never null: the chain is walked with a do/while
        do {
            sub->m_pManager->RenderSubPrimitive(sub, params);
            sub = sub->m_pNext;
        } while (sub);
    }
}