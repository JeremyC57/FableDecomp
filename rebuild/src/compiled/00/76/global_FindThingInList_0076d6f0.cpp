// _global::FindThingInList(CThingListIterator first, CThingListIterator last, CThing* const& value) -- SGI/STLport `find` front end over a list of CIntelligentPointer<const CThing>: forwards to the `__find` loop with an iterator-category tag
// Leaf: FindThingInList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076D6F0, length 39 bytes.
// Callees: the `__find` loop @ 0x0076D5D0 (manifest "_Find<_List_unchecked_iterator<..CIntelligentPointer<CThing const>..>,CThing*>";
// `__fastcall`: hidden result in ecx, the value reference in edx, first/last/tag on the stack, `ret 0Ch`;
// it walks node->next comparing CBaseIntelligentPointer::GetPItem(node + 8) with *value).
// Label: the manifest's "CTCCameraPointScripted::GetLookDirection" is propagated and rejected.
// Shape: a free template function compiled `__fastcall` (the /Gr algorithm TUs): the iterator result
// comes back through the hidden pointer in ecx (kept in esi and returned), the value reference rides in
// edx untouched, and the two iterators are passed by value. This list iterator has a user copy
// constructor, so each copy is built in its own argument slot (`push ecx ; mov eax,esp ; mov [eax],ecx`);
// the empty category tag (user constructor, no zero store) is homed in the dead `last` slot
// (`lea eax,[esp+0Ch]`). Twin of 0x0076D850, whose iterator has no user copy constructor.
struct CThing;
struct SThingListNode;

struct CThingListIterator {
    SThingListNode* m_pNode;
    CThingListIterator() { }
    CThingListIterator(const CThingListIterator& other) : m_pNode(other.m_pNode) { }
};

struct CIteratorTag { CIteratorTag() { } };

CThingListIterator __fastcall FindThingInListImpl(CThingListIterator first, CThingListIterator last, CThing* const& value, const CIteratorTag& tag);   // 0x0076D5D0

CThingListIterator __fastcall FindThingInList(CThingListIterator first, CThingListIterator last, CThing* const& value)
{
    return FindThingInListImpl(first, last, value, CIteratorTag());
}