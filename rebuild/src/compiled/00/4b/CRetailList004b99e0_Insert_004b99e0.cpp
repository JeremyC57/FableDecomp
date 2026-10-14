// CRetailList_004b99e0::Insert(iterator where, const SValue_004b99e0&) -- STLport-style list insert: build a 0x18-byte node holding a copy of the value, link it before `where`, return the iterator to it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B99E0, length 96 bytes.
// Callees: malloc @ 0x00BFEA0E, CCharString::CCharString(const CCharString&) @ 0x0099EC30 (twice: the value's two names). The chunk label
// ("CCreatureAction_TentacleStrikeLoop::Clone") is a propagated STL label: the body is a list node insert for a 16-byte value of two CCharStrings,
// a byte and a long; no element count is kept and the links are set in the STLport order (next, prev, prev->next, where->prev).
// Shape: the iterator result (a 4-byte class with a user constructor) comes back through the hidden pointer, so `ret 0Ch` covers the result slot,
// the position and the value; the value rides in ebx, the node's value address in edi (placement new null test). `this` is unused.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (landed-trusted)
inline void* operator new(unsigned int, void* p) { return p; }   // placement form: the null test on its result is what retail emits

struct CCharString {                        // modelled locally: the shared header inlines the copy constructor, retail calls it out of line
    void* m_pStringData;
    CCharString(const CCharString& other);   // 0x0099EC30
};
struct SValue_004b99e0 {                    // 16 bytes: two names, a flag byte, a long -- copied memberwise (the user copy constructor gives one move per member)
    CCharString m_name;
    CCharString m_value;
    unsigned char m_flag;
    long m_number;
    SValue_004b99e0(const SValue_004b99e0& o) : m_name(o.m_name), m_value(o.m_value), m_flag(o.m_flag), m_number(o.m_number) { }
};
struct SNode_004b99e0 {                     // 0x18 bytes: next / prev, then the value
    SNode_004b99e0* m_pNext;
    SNode_004b99e0* m_pPrev;
    SValue_004b99e0 m_value;                // +0x08
};
struct SIterator_004b99e0 {                 // a node pointer with a user constructor: returned through the hidden pointer
    SNode_004b99e0* m_pNode;
    SIterator_004b99e0(SNode_004b99e0* node) : m_pNode(node) { }
};

struct CRetailList_004b99e0 {
    SNode_004b99e0* m_pHead;
    SIterator_004b99e0 Insert(SIterator_004b99e0 where, const SValue_004b99e0& value);
};

SIterator_004b99e0 CRetailList_004b99e0::Insert(SIterator_004b99e0 where, const SValue_004b99e0& value)
{
    SNode_004b99e0* node = (SNode_004b99e0*)malloc(sizeof(SNode_004b99e0));
    new (&node->m_value) SValue_004b99e0(value);
    SNode_004b99e0* pos = where.m_pNode;
    SNode_004b99e0* prev = pos->m_pPrev;    // read once, before the node's own links are written (retail keeps it in ecx)
    node->m_pNext = pos;
    node->m_pPrev = prev;
    prev->m_pNext = node;
    pos->m_pPrev = node;
    return SIterator_004b99e0(node);
}