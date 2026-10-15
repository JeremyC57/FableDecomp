// CEngineInternalPrimitiveParticleGroup::ClearChildPrimitive(unsigned long id) -- frees child id `id`: erases its handle node from the primitive list, marks the id slot unused and returns the id to the free list (removed first, then appended)
// Grade: DIFFER(175v154) -- register allocation: the lane caches `id*8` in ebx (`lea ebx,[ebp*8]`) and keeps the id in ebp with `this` in edi; retail keeps this/first/id in ebx/esi/edi and re-forms `[eax+edi*8]` (no scaled-index CSE, `mov ecx,esi ; lea eax,[ecx+edi*8] ; cmp byte ptr [eax],0`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBCE10, length 154 bytes (`ret 4`); vtable 0x012A59D4 slot 10.
// Callees: the handle's virtual destructor (slot 0 with 0: `node->m_value.~CEnginePrimitiveHandle()` from STLport list::erase),
// free @ 0x00BFEA14, list<unsigned long>::remove(const unsigned long&) @ 0x0084ACE0 (manifest "list<CTCBase*>::remove", same body),
// malloc @ 0x00BFEA0E (push_back node: placement value with the null test, linked before the sentinel).
// Shape: the iterator slot is reset with a temporary `CPrimitiveIterator()` whose constructor clears only the used byte (the node
// word is copied uninitialised from the frame); the id handed to remove() by reference is the by-value parameter of an inline
// helper, homed in the caller's dead argument slot (`mov [esp+1Ch],edi` before the call) -- that part reproduces with the
// `__forceinline ReleaseID(id)` helper. Tried: plain body (first re-read, no slot store), inline helper (not inlined),
// __forceinline helper (this form), iterator-object operator[] (no change).
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
    unsigned long m_value;                         // +0x08
};

struct CIdList {                                   // STLport list<unsigned long>, 4 bytes
    SIdListNode* m_pNode;
    void remove(const unsigned long& value);       // 0x0084ACE0
    void push_back(const unsigned long& value)
    {
        SListNodeBase* pos = m_pNode;
        SIdListNode* node = (SIdListNode*)malloc(sizeof(SIdListNode));
        new (&node->m_value) unsigned long(value);
        node->m_pNext = pos;
        node->m_pPrev = pos->m_pPrev;
        pos->m_pPrev->m_pNext = node;
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
    __forceinline void ReleaseID(unsigned long id)
    {
        FreePrimitiveIDs.remove(id);
        FreePrimitiveIDs.push_back(id);
    }
    void ClearChildPrimitive(unsigned long id);
};

void CEngineInternalPrimitiveParticleGroup::ClearChildPrimitive(unsigned long id)
{
    if (id < PrimitiveIDToIterator.GetCount()) {
        CPrimitiveIterator& entry = PrimitiveIDToIterator[id];
        if (entry.m_used) {
            Primitives.erase(entry.m_pNode);
            PrimitiveIDToIterator[id] = CPrimitiveIterator();
            ReleaseID(id);
        }
    }
}
