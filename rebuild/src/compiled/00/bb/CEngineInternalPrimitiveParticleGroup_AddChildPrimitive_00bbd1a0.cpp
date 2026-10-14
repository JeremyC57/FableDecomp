// CEngineInternalPrimitiveParticleGroup::AddChildPrimitive(const CEnginePrimitive& primitive, unsigned long flags) -- allocates a child id (a recycled one from the free list at +0x58, else a new slot appended to the iterator array at +0x4c) and adds the primitive under it (virtual slot 8); returns the id
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBD1A0, length 86 bytes (96 with padding).
// Callees: CArray<CPrimitiveIterator>::Resize(unsigned long) @ 0x00BBDD40 (on the array at +0x4c: count + 1), free @ 0x00BFEA14 (the popped list node),
// virtual slot 8 (+0x20, AddChildPrimitiveWithID(id, primitive, flags)). Shape: the free list is a 4-byte STLport list (sentinel node; `cmp [eax],eax` is
// empty()); pop_front reads the node's successor, predecessor and value, relinks the neighbours and frees the node; the id rides in esi through both arms.
// Layout: the retail group holds a 4-byte list at +0x48, the three-pointer CArray at +0x4c and the free-id list at +0x58 (the PDB header's STL containers
// are wider); modelled locally with the retail offsets.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CEnginePrimitive;

struct SIdListNode {                               // STLport list node: next, prev, value
    SIdListNode*  m_pNext;
    SIdListNode*  m_pPrev;
    unsigned long m_id;
};

struct CIdList {                                   // 4 bytes: the sentinel node
    SIdListNode* m_pNode;
    bool empty() const { return m_pNode->m_pNext == m_pNode; }
    unsigned long PopFront()
    {
        SIdListNode* node = m_pNode->m_pNext;
        SIdListNode* next = node->m_pNext;
        SIdListNode* prev = node->m_pPrev;
        unsigned long id = node->m_id;
        prev->m_pNext = next;
        next->m_pPrev = prev;
        free(node);
        return id;
    }
};

struct CPrimitiveIterator { unsigned char m_used; void* m_pNode; };

struct CIteratorArray {                            // CArray<CPrimitiveIterator>: first, last, end
    CPrimitiveIterator* m_pFirst;
    CPrimitiveIterator* m_pLast;
    CPrimitiveIterator* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void Resize(unsigned long count);              // 0x00BBDD40
};

struct CEngineInternalPrimitiveParticleGroup {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual unsigned long AddChildPrimitiveWithID(unsigned long id, const CEnginePrimitive& primitive, unsigned long flags);   // slot 8 (+0x20)
    unsigned char  _pad04[0x44];
    void*          m_pPrimitives;                  // +0x48 list<CEnginePrimitiveHandle>
    CIteratorArray PrimitiveIDToIterator;          // +0x4c
    CIdList        FreePrimitiveIDs;               // +0x58
    unsigned long AddChildPrimitive(const CEnginePrimitive& primitive, unsigned long flags);
};

unsigned long CEngineInternalPrimitiveParticleGroup::AddChildPrimitive(const CEnginePrimitive& primitive, unsigned long flags)
{
    unsigned long id;
    if (FreePrimitiveIDs.empty()) {
        id = PrimitiveIDToIterator.GetCount();
        PrimitiveIDToIterator.Resize(id + 1);
    } else {
        id = FreePrimitiveIDs.PopFront();
    }
    AddChildPrimitiveWithID(id, primitive, flags);
    return id;
}