// CEngineInternalPrimitiveParticleGroup::ClearChildPrimitive(unsigned long id) -- frees child id `id`: erases its handle node from the primitive list, marks the id slot unused and returns the id to the free list (removed first, then appended)
// Grade: RELOCATION_MATCH -- FreePrimitiveIDs typed as the Ego_r.pdb list<long> (the unsigned id binds const long&
//   through a converted temporary, so push_back stores it from edi), direct PrimitiveIDToIterator[id] uses instead of an
//   entry reference, STLport __prev local in the list insert (was DIFFER 175v154)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBCE10, length 154 bytes (`ret 4`); vtable 0x012A59D4 slot 10.
// Callees: the handle's virtual destructor (slot 0 with 0: `node->m_value.~CEnginePrimitiveHandle()` from STLport list::erase),
// free @ 0x00BFEA14, list<long>::remove(const long&) @ 0x0084ACE0 (Ego_r.pdb FreePrimitiveIDs is list<long>; manifest "list<CTCBase*>::remove", same body),
// malloc @ 0x00BFEA0E (push_back node: placement value with the null test, linked before the sentinel).
// Shape: the iterator slot is reset with a temporary `CPrimitiveIterator()` whose constructor clears only the used byte (the node
// word is copied uninitialised from the frame); each PrimitiveIDToIterator[id] use re-forms the scaled index (no entry reference); the
// unsigned id binds remove()/push_back()'s `const long&` through a converted temporary (homed in the dead argument slot,
// `mov [esp+1Ch],edi`), so push_back stores the id straight from edi; the list insert reads the sentinel's prev once (STLport __prev).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

inline void* operator new(unsigned int, void* where) { return where; }

class CEnginePrimitiveHandle {                     // 8 bytes: vptr + counted Data
public:
    virtual ~CEnginePrimitiveHandle();
    void* Data;
};

struct SListNodeBase {
    SListNodeBase* m_pNext;
    SListNodeBase* m_pPrev;
};

struct SHandleListNode : SListNodeBase {
    CEnginePrimitiveHandle m_value;                // +0x08
};

struct CHandleList {                               // STLport list<CEnginePrimitiveHandle>, 4 bytes
    SHandleListNode* m_pNode;
    void erase(SHandleListNode* node)
    {
        SListNodeBase* next = node->m_pNext;
        SListNodeBase* prev = node->m_pPrev;
        prev->m_pNext = next;
        next->m_pPrev = prev;
        node->m_value.~CEnginePrimitiveHandle();
        free(node);
    }
};

struct SIdListNode : SListNodeBase {
    long m_value;                                  // +0x08
};

struct CIdList {                                   // STLport list<long> (Ego_r.pdb FreePrimitiveIDs), 4 bytes
    SIdListNode* m_pNode;
    void remove(const long& value);                // 0x0084ACE0
    void push_back(const long& value)
    {
        SListNodeBase* pos = m_pNode;
        SIdListNode* node = (SIdListNode*)malloc(sizeof(SIdListNode));
        new (&node->m_value) long(value);
        SListNodeBase* prev = pos->m_pPrev;
        node->m_pNext = pos;
        node->m_pPrev = prev;
        prev->m_pNext = node;
        pos->m_pPrev = node;
    }
};

struct CPrimitiveIterator {                        // 8 bytes: in-use flag and the list node
    unsigned char    m_used;
    SHandleListNode* m_pNode;
    CPrimitiveIterator() : m_used(false) { }
};

struct CIteratorArray {                            // CArray<CPrimitiveIterator>: first, last, end
    CPrimitiveIterator* m_pFirst;
    CPrimitiveIterator* m_pLast;
    CPrimitiveIterator* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
    CPrimitiveIterator& operator[](unsigned long index) { return m_pFirst[index]; }
};

class CEngineInternalPrimitiveParticleGroup {
public:
    virtual void v00();
    unsigned char  _pad04[0x44];
    CHandleList    Primitives;                     // +0x48
    CIteratorArray PrimitiveIDToIterator;          // +0x4c
    CIdList        FreePrimitiveIDs;               // +0x58
    void ClearChildPrimitive(unsigned long id);
};

void CEngineInternalPrimitiveParticleGroup::ClearChildPrimitive(unsigned long id)
{
    if (id < PrimitiveIDToIterator.GetCount()) {
        if (PrimitiveIDToIterator[id].m_used) {
            Primitives.erase(PrimitiveIDToIterator[id].m_pNode);
            PrimitiveIDToIterator[id] = CPrimitiveIterator();
            FreePrimitiveIDs.remove(id);
            FreePrimitiveIDs.push_back(id);
        }
    }
}