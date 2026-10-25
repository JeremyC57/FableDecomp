// std::list::push_back(const CFileInstaller::CPendingWrite& x) -- STLport list push_back of a 12-byte pending-write record (insert(end(), x) inlined)
// Leaf: ?push_back@?$list@VCPendingWrite@CFileInstaller@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C6AF0, 59 bytes, thiscall `ret 4` (this = the list).
// Name: FableWin.pdb has `list<CFileInstaller::CPendingWrite>::push_back`; the only caller,
// CFileInstaller::ProcessActiveFileForDecompression (0x009C3C20), builds a 12-byte local {chunk, write
// position [+0x9C], length} and calls 0x009C6AF0 on the list at +0x90. Label correction: catalogued as
// `CFileInstaller_AppendBufferChunk`.
// Element: Ego_r.pdb CPendingWrite {CBufferChunk* PBufferChunk; ulong FilePos; ulong WriteLength} (12 bytes; the
// FableWin ctor CPendingWrite(ulong, ulong, CBufferChunk*) confirms the three words). Node = 0x14 bytes.
// Callee: malloc @ 0x00BFEA0E (STLport node allocator).
// Shape: STLport `push_back(x) { insert(end(), x); }` with insert and `_M_create_node` inlined: the sentinel is
// read first (`mov esi,[ecx]`), placement new null-tests &node->_M_data and copies the trivially-copyable record
// as three dword moves through edi, then next/prev/prev->next/sentinel->prev.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CBufferChunk;

class CFileInstaller {
public:
    class CPendingWrite {                               // 12 bytes
    public:
        CBufferChunk* PBufferChunk;                     // +0x00
        unsigned long FilePos;                          // +0x04
        unsigned long WriteLength;                      // +0x08
    };
};

namespace std {

template <class _Tp> class allocator { };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;

    _List_node_base* _M_node;

    iterator end() { return iterator(_M_node); }

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }

    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }

    void push_back(const _Tp& __x);
};

template <class _Tp, class _Alloc> void list<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    insert(end(), __x);
}

template class list<CFileInstaller::CPendingWrite>;

}