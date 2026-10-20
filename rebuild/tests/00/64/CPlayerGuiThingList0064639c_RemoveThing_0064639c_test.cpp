// Behaviour test for 0064639c: src/0064639c.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CPlayerGuiThingList_0064639c::RemoveThing(CThingCreatureBase* creature)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPlayerGuiThingList_0064639c::RemoveThing(CThingCreatureBase* creature) -- find the creature in the list at +0x3c and erase its entry when present
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064639C, length 69 bytes; a frame-pointer size TU
// (`push ebp ; mov ebp,esp`, `leave`, the result and key temporaries homed in the dead argument slot
// [ebp+8]): `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
// Callees: SGI `find(first, last, value)` @ 0x00655CDC (__fastcall under /Gr: hidden result in ecx,
// the value reference in edx, the two 4-byte iterators by value on the stack; it forwards to
// `__find` @ 0x0065575F with an input-iterator tag), list::erase(iterator) @ 0x006550E2 (thiscall,
// hidden result + the iterator by value; it unlinks the node, destroys the CIntelligentPointer
// element through its vtable and frees the node).
// Identity: reached only through the thunk @ 0x00437900 (`mov ecx,[ecx+1B8h] ; jmp`), i.e. a forwarder
// of the player GUI to one of its draw-object members; the sibling @ 0x0064A2DB (thunk 0x004378F0) is
// the matching "add when absent" (find, then push_back of a CIntelligentPointer built from the thing).
// The manifest label "CTCBoastingCrowdControl::OnKillCrowd" is propagated (that component is 0x24
// bytes and has no list at +0x3c); the class is named by what it holds.
// Shape: the key is a `CThing* const&` bound to a derived `CThingCreatureBase*` argument, so VC converts
// it into a temporary -- homed in the argument's own (dead) slot (`mov eax,[ebp+8] ; mov [ebp+8],eax`); the end
// iterator is pushed first (right to left); the erase result is discarded into the same dead slot.
// Leaf: RemoveThing
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CThing { long m_dummy; };
struct CThingCreatureBase : CThing { };

struct SThingListNode {                                     // STLport list node: next, prev, CIntelligentPointer<CThing>
    SThingListNode* m_pNext;
    SThingListNode* m_pPrev;
    unsigned char   m_value[8];
};

struct CThingListIterator {                                 // 4-byte iterator with a user copy constructor
    SThingListNode* m_pNode;
    CThingListIterator(SThingListNode* node) : m_pNode(node) { }
    CThingListIterator(const CThingListIterator& other) : m_pNode(other.m_pNode) { }
    bool operator!=(const CThingListIterator& other) const { return m_pNode != other.m_pNode; }
};

struct CThingList {                                         // list<CIntelligentPointer<CThing>>: the sentinel node
    SThingListNode* m_pHead;
    CThingListIterator begin() { return CThingListIterator(m_pHead->m_pNext); }
    CThingListIterator end() { return CThingListIterator(m_pHead); }
    CThingListIterator erase(CThingListIterator where);     // 0x006550E2
};
void __fastcall ReleaseListNode_s7(SThingListNode* node);   // stand-in: the item's destructor + node free
#pragma auto_inline(off)
// Same-TU body of the STLport list::erase instantiation 0x006550E2: unlink the node, destroy it, return the next one.
__declspec(noinline) CThingListIterator CThingList::erase(CThingListIterator where)
{
    SThingListNode* next = where.m_pNode->m_pNext;
    SThingListNode* prev = where.m_pNode->m_pPrev;
    prev->m_pNext = next;
    next->m_pPrev = prev;
    ReleaseListNode_s7(where.m_pNode);
    return CThingListIterator(next);
}
#pragma auto_inline(on)

CThingListIterator __fastcall find(CThingListIterator first, CThingListIterator last, CThing* const& value);   // 0x00655CDC
CThing* __fastcall ListItem_s7(const unsigned char* pointer);   // stand-in: CIntelligentPointer<CThing>::GetPItem on the node's value
#pragma auto_inline(off)
// Same-TU body of the STLport find instantiation 0x00655CDC: walks the list comparing each item with the value read through
// its const reference.
__declspec(noinline) CThingListIterator __fastcall find(CThingListIterator first, CThingListIterator last, CThing* const& value)
{
    while (first != last && ListItem_s7(first.m_pNode->m_value) != value)
        first.m_pNode = first.m_pNode->m_pNext;
    return first;
}
#pragma auto_inline(on)

struct CPlayerGuiThingList_0064639c {
    unsigned char _pad00[0x3c];
    CThingList    m_things;                                 // +0x3c
    void RemoveThing(CThingCreatureBase* creature);
};

void CPlayerGuiThingList_0064639c::RemoveThing(CThingCreatureBase* creature)
{
    CThingListIterator it = find(m_things.begin(), m_things.end(), creature);
    if (it != m_things.end())
        m_things.erase(it);
}

// ---- stubs and driver ----
static int g_release = 0; static SThingListNode* g_released = 0;
CThing* __fastcall ListItem_s7(const unsigned char* pointer) { return *(CThing* const*)(pointer + 4); }   // the item word of the intelligent pointer
void __fastcall ReleaseListNode_s7(SThingListNode* node) { ++g_release; g_released = node; }
int main()
{
    static CThingCreatureBase a, b, c;
    static SThingListNode head, n1, n2;
    head.m_pNext = &n1; n1.m_pPrev = &head; n1.m_pNext = &n2; n2.m_pPrev = &n1; n2.m_pNext = &head; head.m_pPrev = &n2;
    *(CThing**)(n1.m_value + 4) = &a; *(CThing**)(n2.m_value + 4) = &b;
    static CPlayerGuiThingList_0064639c list; list.m_things.m_pHead = &head;
    list.RemoveThing(&c); CHECK(g_release == 0 && head.m_pNext == &n1 && n1.m_pNext == &n2);
    list.RemoveThing(&b); CHECK(g_release == 1 && g_released == &n2 && n1.m_pNext == &head && head.m_pPrev == &n1);
    list.RemoveThing(&a); CHECK(g_release == 2 && g_released == &n1 && head.m_pNext == &head && head.m_pPrev == &head);
    list.RemoveThing(&a); CHECK(g_release == 2);
    if (fails == 0) printf("0064639c_TEST PASS\n"); else printf("0064639c_TEST FAIL\n");
    return fails;
}