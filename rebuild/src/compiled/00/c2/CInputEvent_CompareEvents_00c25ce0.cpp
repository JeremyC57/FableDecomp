// CInputEvent::CompareEvents(const SEventKey& a, const SEventKey& b) -- equality of two 16-byte event keys (static __fastcall)
// Leaf: CompareEvents
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C25CE0, length 112 bytes.
// Callees: the key's first-word getter @ 0x00C25C60 (`mov eax,[ecx] ; ret`, catalogued as CInputEvent::GetKey).
// Callers: the 0x00C24760 / 0x00C247A0 `__stdcall` wrappers pass `&x->key` (+0x18) of two records in ecx/edx.
// Shape: a `__fastcall` static (a in ecx, b in edx, plain `ret`). The getter is defined out of line ABOVE the
// comparator under `auto_inline(off)`, so VC7.1 knows it only touches eax and keeps b in edx across both calls.
// Key layout: +0 a block pointer (its +4 word is the identity compared for low indices), +4 a reference pointer,
// +8 an index (below 0x8000 = compare the blocks' ids; 0 = compare the references, else the block pointers),
// +0xc the kind.
#pragma auto_inline(off)
struct SEventBlock {
    unsigned long m_0;
    unsigned long m_id;                            // +0x04
};
struct SEventKey {
    SEventBlock*  m_pBlock;                        // +0x00
    void*         m_pRef;                          // +0x04
    unsigned long m_index;                         // +0x08
    unsigned long m_kind;                          // +0x0c
    SEventBlock* GetBlock() const;                 // 0x00C25C60
    static bool __fastcall CompareEvents(const SEventKey& a, const SEventKey& b);
};

SEventBlock* SEventKey::GetBlock() const
{
    return m_pBlock;
}

bool __fastcall SEventKey::CompareEvents(const SEventKey& a, const SEventKey& b)
{
    if (a.m_kind != b.m_kind)
        return false;
    if (a.m_index != b.m_index)
        return false;
    if (a.m_index < 0x8000 && a.GetBlock()->m_id != b.GetBlock()->m_id)
        return false;
    if (a.m_index == 0) {
        if (a.m_pRef && b.m_pRef)
            return a.m_pRef == b.m_pRef;
        return a.m_pBlock == b.m_pBlock;
    }
    return true;
}