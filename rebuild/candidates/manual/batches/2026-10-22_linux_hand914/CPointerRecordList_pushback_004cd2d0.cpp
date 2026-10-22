// CPointerRecordList::push_back(const SPointerRecord&) -- STLport list push_back: insert(end(), x), discarding the returned iterator
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD2D0, 25 bytes. Callees: list::insert(iterator, const T&) @ 0x004CCC80
// (allocates a 0x14-byte node: next, prev, then a 12-byte value = a CIntelligentPointer {vtable
// 0x01238C6C, pointer} plus a long; returns the new node's iterator through a hidden pointer, `ret 0Ch`).
// Unreferenced in the image (recovered from a mis-cut catalog row). The value type is not identified.
// Shape: `insert(end(), x)` -- the end iterator (the sentinel pointer at +0) is built in its argument slot
// through the iterator's user copy constructor (`push ecx ; mov eax,esp ; mov [eax],edx`), and the
// discarded iterator result is homed in the dead `x` argument slot (`lea eax,[esp+0Ch] ; push eax`).
struct SListNodeBase {
    SListNodeBase* _M_next;           // +0
    SListNodeBase* _M_prev;           // +4
};
struct SListIterator {
    SListNodeBase* _M_node;
    SListIterator(SListNodeBase* node) : _M_node(node) {}
    SListIterator(const SListIterator& other) : _M_node(other._M_node) {}
};
struct SPointerRecord;                // 12 bytes: CIntelligentPointer<?> (8) + long
struct CPointerRecordList {
    SListNodeBase* _M_node;           // +0 sentinel node
    SListIterator end() { return SListIterator(_M_node); }
    SListIterator insert(SListIterator position, const SPointerRecord& x);   // 0x004CCC80
    void push_back(const SPointerRecord& x);
};

void CPointerRecordList::push_back(const SPointerRecord& x)
{
    insert(end(), x);
}
