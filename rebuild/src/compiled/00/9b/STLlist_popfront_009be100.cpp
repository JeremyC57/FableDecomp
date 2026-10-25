// _STL::list::pop_front() -- STLport list<CSubTitle>: unlink the first subtitle node, destroy the subtitle (sound name, formatted text, counted text entry) and free the node
// Leaf: ?pop_front@?$list@UCSubTitle@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BE100, 66 bytes (+ int3 padding), __fastcall, plain `ret`.
// Identity: STLport 4.6 `void pop_front() { erase(begin()); }` with the inline single-node erase; the node payload at +8 is
//   Ego_r.pdb CSubTitle (28 bytes): TextEntry +0 (CIVCountedPointer<const NGameText::CTextEntry>), FormattedText +4
//   (CWideString), Sound +8 (CCharString), FrameStart, FrameEnd, StartTime, EndTime. Only caller 0x009BDAC5 (the subtitle
//   queue). The catalog's "list<CSubTitle>::clear" is propagated: the body removes exactly one node (no loop).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (Sound), CWideString::~CWideString @ 0x0099B510 (FormattedText), the
//   text entry's virtual slot 1 (the release when its count drops to 0), free @ 0x00BFEA14.
// Shape: CSubTitle declares no destructor: its implicit one runs Sound, FormattedText, then the inline CIVCountedPointer
//   destructor (`T* data = Data; if (data) { if (--data->RefCount == 0) data->Release(); Data = 0; }`: the
//   local keeps the pointer in ecx and the `dec` flags feed the branch; the clear is re-addressed through the node). erase
//   is `__forceinline`: as a plain inline VC7.1 keeps this payload's erase out of line (22-byte pop_front calling it).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

class CCharString {
public:
    void* PData;
    ~CCharString();                                     // 0x0099EAE0
};

class CWideString {
public:
    void* PData;
    ~CWideString();                                     // 0x0099B510
};

namespace NGameText {
class CTextEntry {                                      // counted text entry (vptr +0, count +4)
public:
    virtual ~CTextEntry();
    virtual void Release() const;                       // vtable slot 1
    mutable long RefCount;                              // +0x04
};
}

template <class T> class CIVCountedPointer {
public:
    T* Data;                                            // +0x00
    ~CIVCountedPointer()
    {
        T* data = Data;
        if (data) {
            if (--data->RefCount == 0)
                data->Release();
            Data = 0;
        }
    }
};

struct CSubTitle {                                      // 28 bytes (Ego_r.pdb; a struct in the PDB name sets)
public:
    CIVCountedPointer<const NGameText::CTextEntry> TextEntry;   // +0x00
    CWideString FormattedText;                          // +0x04
    CCharString Sound;                                  // +0x08
    long        FrameStart;                             // +0x0C
    long        FrameEnd;                               // +0x10
    float       StartTime;                              // +0x14
    float       EndTime;                                // +0x18
};

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x0
    _List_node_base* _M_prev;                           // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) {}
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // +0x00: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

    void _M_deallocate(_Node* __p) { if (__p != 0) free(__p); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }

    __forceinline iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        _M_deallocate(__n);
        return iterator(__next_node);
    }
    void pop_front() { erase(begin()); }
};

template void list<CSubTitle>::pop_front();

} // namespace _STL