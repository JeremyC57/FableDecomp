// CLocalDetailCacheMap::CObjectTypeCollection::RelocateData(long blockType, void* oldBase, void* newBase, long size) -- a cached primitive of this collection was moved: find it in the primitive list, re-base the pointer, let it fix itself up and relink it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDDE0, length 78 bytes.
// Leaf: RelocateData
// Callees: the primitive's virtual slot 7 (+0x1c, its post-relocation fix-up).
// Name: vtable 0x012A7D9C slot 1; FableWin.pdb ?RelocateData@CObjectTypeCollection@CLocalDetailCacheMap@@UAEXJPAX0J@Z
// (void; long, void*, void*, long: `ret 0x10`, the first and last arguments unused).
// Layout (retail, as the landed RenderUpdate 0x00BDD860): first primitive +8, primitives linked through +4.
// Shape: a list walk with a trailing `prev` (edi, zeroed up front) that leaves at the moved block; the inline
// relocation helper (in place, integer arithmetic: `if (p) p = (T*)((long)p + ((long)newBase - (long)oldBase))` -- the
// char* return-value form reassociates to `p - old + new` and moves p out of esi) keeps its own null test, then the virtual fix-up and
// the relink through `prev` or the list head; three separate `ret 0x10` exits.
struct CLocalDetailPrimitiveBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual void OnPostRelocation();                        // slot 7 (+0x1c)
    CLocalDetailPrimitiveBase* m_pNext;                     // +0x04
};

template <class T> inline void RelocatePointer(T*& p, void* oldBase, void* newBase)
{
    if (p)
        p = (T*)((long)p + ((long)newBase - (long)oldBase));
}

struct CLocalDetailObjectCollectionType;
struct CObjectTypeCollection {                              // CLocalDetailCacheMap::CObjectTypeCollection
    virtual ~CObjectTypeCollection();                       // slot 0
    CLocalDetailObjectCollectionType* m_pType;              // +0x04
    CLocalDetailPrimitiveBase*        m_pFirstPrimitive;    // +0x08
    void RelocateData(long blockType, void* oldBase, void* newBase, long size);
};

void CObjectTypeCollection::RelocateData(long blockType, void* oldBase, void* newBase, long size)
{
    CLocalDetailPrimitiveBase* prev = 0;
    for (CLocalDetailPrimitiveBase* p = m_pFirstPrimitive; p; p = p->m_pNext) {
        if (p == oldBase) {
            RelocatePointer(p, oldBase, newBase);
            p->OnPostRelocation();
            if (prev)
                prev->m_pNext = p;
            else
                m_pFirstPrimitive = p;
            return;
        }
        prev = p;
    }
}