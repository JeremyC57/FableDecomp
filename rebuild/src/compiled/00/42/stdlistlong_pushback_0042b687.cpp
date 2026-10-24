// std::list<long>::push_back(const long& value) -- STLport push_back: insert(end(), value) through the out-of-line insert @ 0x0042AB33
// Leaf: ?push_back@?$list@JU?$allocator@J@std@@@std@@QAEXABJ@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042B687, 26 bytes (`ret 4`). Label correction (chunk 345 report): the
// manifest's "LinkedList_PushFront" is wrong -- insert(end(), x) links the new node BEFORE the sentinel, i.e. at the END.
// Twins (immediates masked): 0x0042B6B8 "Std_List_PushFront", 0x0042B646, 0x00493D16, 0x0053CAE8 .. (other element types).
// Callees: list<long>::insert(iterator position, const long& value) @ 0x0042AB33 (`ret 0Ch`: hidden iterator result slot,
// the by-value iterator and the value reference; it mallocs the 12-byte node via 0x00429FE3 and links it before position;
// the manifest's "LinkedList_Insert_Before" describes it).
// Layout: the list is STLport's one word, the sentinel-node pointer (_M_node); a node is {next, prev, long data}.
// Shape: frame-pointer TU (`push ebp ; mov ebp,esp`), the end() iterator is built straight into its push slot
// (`push ecx ; mov eax,esp ; mov [eax],edx`, STLport's user copy constructor) and the dead `value` argument slot receives
// the discarded hidden iterator result (`lea eax,[ebp+8] ; push eax`); the reference itself is forwarded (`push [ebp+8]`).
#pragma optimize("s", on)
#pragma optimize("y", off)
namespace std {
struct _List_node_long {
    _List_node_long* _M_next;
    _List_node_long* _M_prev;
    long             _M_data;
};
template <class T> struct allocator { };
template <class T, class A> class list {
public:
    struct iterator {
        _List_node_long* _M_node;
        iterator(_List_node_long* node) : _M_node(node) { }
        iterator(const iterator& other) : _M_node(other._M_node) { }
    };
    _List_node_long* _M_node;                    // the sentinel
    iterator end() { return iterator(_M_node); }
    iterator insert(iterator position, const T& value);   // 0x0042AB33
    void push_back(const T& value);
};
template <class T, class A> void list<T, A>::push_back(const T& value)
{
    insert(end(), value);
}
template class list<long, allocator<long> >;
}
