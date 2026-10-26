// CEngineSubPrimitiveRenderer::DeleteList(CRetailEntryVector*) -- delete every sub-primitive of every entry's chain (virtual deleting destructor), then empty the entry vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B84400, length 97 bytes (+ int3 padding).
// Callees: virtual slot 0 (the scalar deleting destructor, argument 1) of each sub-primitive.
// Model reused from the landed sibling CEngineSubPrimitiveRenderer::RenderList @ 0x00B842D0 (same vector, entry and chain).
// Shape: a `__stdcall` static (`ret 4`); the count is taken once (edi), the index runs in ebp, the vector's first
// pointer is re-read every outer iteration; the chain walk is a do/while that reads the successor (esi) BEFORE the
// virtual call and has no null test before it (a plain virtual call with 1, not `delete`). The clear is the inlined
// SGI `erase(begin(), end())`: `copy(end, end, begin)` as a signed `for (n = last - first; n > 0; --n)` loop moving
// 8-byte entries (two dword moves), then `m_pLast = result`.
// `this` is not used, so the engine header (engine/CEngineSubPrimitiveRenderer.h) is not needed.
struct CEngineSubPrimitive_00b84400 {
    virtual void Destroy(unsigned int flags);      // slot 0: scalar deleting destructor
    unsigned char                 _pad4[0xc];
    CEngineSubPrimitive_00b84400* m_pNext;         // +0x10
};

struct SRenderEntry_00b84400 { long m_key; CEngineSubPrimitive_00b84400* m_pFirst; };   // 8 bytes

struct CRetailEntryVector_00b84400 {
    SRenderEntry_00b84400* m_pFirst;
    SRenderEntry_00b84400* m_pLast;
    SRenderEntry_00b84400* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
    static SRenderEntry_00b84400* Copy(SRenderEntry_00b84400* first, SRenderEntry_00b84400* last, SRenderEntry_00b84400* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    void Erase(SRenderEntry_00b84400* first, SRenderEntry_00b84400* last)
    {
        SRenderEntry_00b84400* kept = Copy(last, m_pLast, first);
        m_pLast = kept;
    }
};

class CEngineSubPrimitiveRenderer { public:
    static void __stdcall DeleteList(CRetailEntryVector_00b84400* list);
};

void __stdcall CEngineSubPrimitiveRenderer::DeleteList(CRetailEntryVector_00b84400* list)
{
    unsigned long count = list->GetCount();
    for (unsigned long i = 0; i < count; ++i) {
        CEngineSubPrimitive_00b84400* sub = list->m_pFirst[i].m_pFirst;   // never null: do/while
        do {
            CEngineSubPrimitive_00b84400* next = sub->m_pNext;
            sub->Destroy(1);
            sub = next;
        } while (sub);
    }
    list->Erase(list->m_pFirst, list->m_pLast);
}