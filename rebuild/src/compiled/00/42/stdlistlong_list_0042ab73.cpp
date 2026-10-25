// std::list<long>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x0042A168) then free the sentinel node
// Leaf: ??1?$list@JV?$allocator@J@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AB73, 23 bytes (`mov esi,ecx ; call clear ; if (sentinel) free(sentinel)`),
//   size-optimised TU (`pop ecx`).
// Identity (element type): retail does not fold identical COMDATs, so each of the 165 byte-twins of this body is one list<T>
//   instance. This one is list<long>: SExpSpendUndo::~SExpSpendUndo 0x005797FA destroys its four members through it
//   (struct_layouts_egor SExpSpendUndo: list<long> Generals / Strengths / Skills / Wills), and CTCInventoryTrade's
//   destructor 0x0059EC48 calls it for its only list member (struct_layouts_egor CTCInventoryTrade::NotInStock,
//   list<long>). Whether the out-of-line symbol is ~list (with ~_List_base inlined) or ~_List_base itself is not
//   recoverable from the bytes; the earlier landed name ~list is kept, now with T bound (was list_T_0042ab73).
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                              // +0x00 the sentinel node (STLport _List_base)
    void clear();                               // 0x0042A168 (_List_base::clear, out of line)
    ~list();
};

template <class _Tp, class _Alloc> list<_Tp, _Alloc>::~list()
{
    clear();
    if (_M_node)
        free(_M_node);
}

template list<long>::~list();
}
