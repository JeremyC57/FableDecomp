// CEngineInternalPrimitiveParticleGroup::GetChildPrimitive(unsigned long id) -- the primitive behind child id `id`: the id indexes the iterator array (+0x4c), a used slot (byte flag) holds the list node whose handle carries the primitive pointer (+0xc); null when out of range or free
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBC430, length 44 bytes (48 with padding).
// Callees: none. The array's first pointer is re-read for the element address after the range check (`mov ecx,[ecx+4Ch]` again): the count and the element come from two inline CArray accessors. Shape: `(last - first) >> 3` is the array count (8-byte entries, unsigned compare `jae`), the entry address is `lea eax,[ecx+eax*8]`, the flag test is
// `cmp byte ptr [eax],0`, then two loads reach the handle's primitive. Layout: the retail CEngineInternalPrimitiveParticleGroup holds a 4-byte list (+0x48),
// the CArray<CPrimitiveIterator> as three pointers at +0x4c and a 4-byte free-id list at +0x58 (the PDB header puts the array at +0x54 because its STL
// containers are wider); modelled locally with the retail offsets.
struct CEngineInternalPrimitiveBase;

struct CEnginePrimitiveHandle {                    // list value: the handle id and the primitive
    long                          m_id;
    CEngineInternalPrimitiveBase* m_pPrimitive;
};

struct SHandleListNode {                           // STLport-style list node
    SHandleListNode*       m_pNext;
    SHandleListNode*       m_pPrev;
    CEnginePrimitiveHandle m_value;                // +0x08 (primitive at +0x0c)
};

struct CPrimitiveIterator {                        // 8 bytes: in-use flag and the list node
    unsigned char    m_used;
    SHandleListNode* m_pNode;
};

struct CIteratorArray {                            // CArray<CPrimitiveIterator>: first, last, end
    CPrimitiveIterator* m_pFirst;
    CPrimitiveIterator* m_pLast;
    CPrimitiveIterator* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
    CPrimitiveIterator& operator[](unsigned long index) { return m_pFirst[index]; }
};

struct CEngineInternalPrimitiveParticleGroup {
    unsigned char    _pad[0x48];
    SHandleListNode* m_pPrimitives;                // +0x48 list<CEnginePrimitiveHandle>
    CIteratorArray   PrimitiveIDToIterator;        // +0x4c
    CEngineInternalPrimitiveBase* GetChildPrimitive(unsigned long id);
};

CEngineInternalPrimitiveBase* CEngineInternalPrimitiveParticleGroup::GetChildPrimitive(unsigned long id)
{
    if (id < PrimitiveIDToIterator.GetCount()) {
        CPrimitiveIterator& entry = PrimitiveIDToIterator[id];
        if (entry.m_used)
            return entry.m_pNode->m_value.m_pPrimitive;
    }
    return 0;
}