// _global::FindThingInList(CThingListIterator first, CThingListIterator last, CThing* const& value) -- SGI/STLport `find` front end over a list of CIntelligentPointer<const CThing>: forwards to the `__find` loop with an iterator-category tag
// Leaf: FindThingInList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076D850, length 31 bytes.
// Callees: the `__find` loop @ 0x0076D760 (manifest "_Find<_List_unchecked_iterator<..CIntelligentPointer<CThing const>..>,CThing*>";
// `__fastcall`: hidden result in ecx, value reference in edx, first/last/tag on the stack, `ret 0Ch`).
// Label: the manifest's "CTCCameraPointScripted::GetLookDirection" is propagated and rejected.
// Shape: as 0x0076D6F0, but this iterator has only a default constructor (non-POD, so it still returns
// through the hidden pointer) and the implicit copy: both iterators are pushed straight from their
// slots, the value reference passes through edx, and the empty tag temporary (user constructor:
// no zero store) is homed in the dead `last` argument slot (`lea eax,[esp+0Ch]`).
struct CThing;
struct SThingListNode;

struct CThingListIterator {
    SThingListNode* m_pNode;
    CThingListIterator() { }
};

struct CIteratorTag { CIteratorTag() { } };

CThingListIterator __fastcall FindThingInListImpl(CThingListIterator first, CThingListIterator last, CThing* const& value, const CIteratorTag& tag);   // 0x0076D760

CThingListIterator __fastcall FindThingInList(CThingListIterator first, CThingListIterator last, CThing* const& value)
{
    return FindThingInListImpl(first, last, value, CIteratorTag());
}
