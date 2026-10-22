// std::list<CNavQuadTreeMap>::insert(iterator) -- (label propagated; really an STLport list<SEntry>::insert(iterator where)) insert a default-constructed 0x20-byte entry before `where` and return the new node's iterator
// Leaf: insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A17480, 139 bytes (incl. padding).
// Callees: CCharString::CCharString() 0x0099E4B0, malloc 0x00BFEA0E, entry copy constructor 0x00A16960,
//          CCharString::~CCharString 0x0099EAE0, operator delete 0x00BFE9BC.
//
// Shape: the manifest label is wrong (not in any vtable, `ret 8` with the result in eax = hidden
// iterator pointer plus one argument). The body is STLport `list<T>::insert(where)` =
// `insert(where, T())`: the temporary T (a counted pointer at +0/+4, five plain words, a CCharString
// at +0x1C) is default-constructed (immediate zero stores for the counted pointer, the string's
// out-of-line default constructor), the node (0x28 bytes = next, prev, T) comes from malloc, T is
// placement-copied into it (the null test on `&node->m_data` is the placement new; T's copy
// constructor 0x00A16960 is out of line), the node is linked before `where`, the iterator is stored
// through the hidden pointer, and the temporary is destroyed: string first, then the inline counted
// pointer release (the `--count; if (count == 0)` re-read form).
#include "fable_string.h"

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CCountedPointer {
    void* m_pObject;                        // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct SEntry {
    CCountedPointer m_pThing;               // +0x00
    long m_values[5];                       // +0x08
    CCharString m_name;                     // +0x1c
    SEntry() { }
    SEntry(const SEntry& other);            // 0x00A16960
};

inline void* operator new(unsigned int, void* where) { return where; }

struct SListNode {
    SListNode* m_pNext;                     // +0
    SListNode* m_pPrev;                     // +4
    SEntry m_data;                          // +8
};

struct SListIterator {
    SListNode* m_pNode;
    SListIterator(SListNode* node) : m_pNode(node) { }
};

struct CCombatSequence_WillForcePush {      // really std::list<SEntry> (STLport)
    SListNode* m_pHead;
    SListNode* CreateNode(const SEntry& value)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        new (&node->m_data) SEntry(value);
        return node;
    }
    SListIterator insert(SListIterator where, const SEntry& value)
    {
        SListNode* node = CreateNode(value);
        SListNode* next = where.m_pNode;
        SListNode* prev = next->m_pPrev;
        node->m_pNext = next;
        node->m_pPrev = prev;
        prev->m_pNext = node;
        next->m_pPrev = node;
        return SListIterator(node);
    }
    SListIterator insert(SListIterator where);
};

SListIterator CCombatSequence_WillForcePush::insert(SListIterator where)
{
    return insert(where, SEntry());
}