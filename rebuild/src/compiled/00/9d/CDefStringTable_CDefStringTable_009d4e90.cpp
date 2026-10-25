// CDefStringTable::~CDefStringTable() -- clear the CCharString -> id map, destroy the filename and the string stream, then the map itself (clear again, free the header node)
// Leaf: ??1CDefStringTable@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D4E90, 114 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: catalogued as "LTextTreeWalkThrough_Destructor" (propagated from the shared _M_erase label). The only caller is
// the atexit destructor stub at 0x0122A780 (`mov ecx, 0x013CA828 ; jmp`), i.e. the global g_DefStringTable @ 0x013CA828
// whose ctor is the landed CDefStringTable::CDefStringTable 0x009D4DB0 (layout from Ego_r.pdb CDefStringTable).
// Callees: STLport _Rb_tree<CCharString, pair<const CCharString, long>, ...>::_M_erase 0x0043020A (shared body; it destroys
// the CCharString key at node +0x10 with 0x0099EAE0), CWideString::~CWideString 0x0099B510 (TableFilename +0x30),
// CMemoryDataOutputStream::~CMemoryDataOutputStream 0x00996B30 (StringStream +0x0C; vtable 0x0129A788), MSVCR71 free 0x00BFEA14.
// Shape: the destructor body clears the map (inline STLport clear: `if (count) { _M_erase(root); leftmost = header;
// root = 0; rightmost = header; count = 0; }`, the header re-read for every store); the members then die in reverse order
// (+0x30, +0x0C, +0x00), and the map's own ~_Rb_tree repeats clear() before ~_Rb_tree_base deallocates the header
// (STLport allocator::deallocate tests the pointer: `if (p) free(p)`).
extern "C" void __cdecl free(void* block);   // 0x00BFEA14

class CCharString
{
public:
    void* PData;                                       // +0x00 shared string data
};

class CWideString
{
public:
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};

class CMemoryDataOutputStream
{
public:
    unsigned char _body[0x18];                         // vptr + CCharString name + buffer pointer + ...
    ~CMemoryDataOutputStream();                        // 0x00996B30
};

namespace std
{
    template <class _T1, class _T2>
    struct pair
    {
        _T1 first;
        _T2 second;
    };

    template <class _Tp>
    struct less { };

    template <class _Pair>
    struct _Select1st { };

    template <class _Tp>
    class allocator
    {
    public:
        void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
    };

    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc
    {
    public:
        _Value _M_data;
    };

    typedef bool _Rb_tree_Color_type;

    struct _Rb_tree_node_base
    {
        typedef _Rb_tree_node_base* _Base_ptr;
        _Rb_tree_Color_type _M_color;                  // +0x00
        _Base_ptr           _M_parent;                 // +0x04
        _Base_ptr           _M_left;                   // +0x08
        _Base_ptr           _M_right;                  // +0x0C
    };

    template <class _Value>
    struct _Rb_tree_node : public _Rb_tree_node_base
    {
        _Value _M_value_field;                         // +0x10
    };

    template <class _Tp, class _Alloc>
    class _Rb_tree_base
    {
    public:
        typedef _Rb_tree_node<_Tp> _Node;
        ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
        _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;   // +0x00
    };

    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
    class _Rb_tree : public _Rb_tree_base<_Value, _Alloc>
    {
    public:
        typedef _Rb_tree_node_base* _Base_ptr;
        typedef _Rb_tree_node<_Value>* _Link_type;

        unsigned int _M_node_count;                    // +0x04
        _Compare     _M_key_compare;                   // +0x08

        _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
        _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
        _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }

        void _M_erase(_Link_type __x);                 // 0x0043020A

        ~_Rb_tree() { clear(); }

        void clear()
        {
            if (_M_node_count != 0) {
                _M_erase((_Link_type)_M_root());
                _M_leftmost() = this->_M_header._M_data;
                _M_root() = 0;
                _M_rightmost() = this->_M_header._M_data;
                _M_node_count = 0;
            }
        }
    };

    template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
    class map
    {
    public:
        typedef pair<const _Key, _Tp> value_type;
        typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
        _Rep_type _M_t;
        void clear() { _M_t.clear(); }
    };
}

class CDefStringTable
{
public:
    std::map<CCharString, long> StringMap;            // +0x00
    CMemoryDataOutputStream StringStream;              // +0x0C
    unsigned long StringCount;                         // +0x24
    unsigned long SharedCount;                         // +0x28
    unsigned long RandomID;                            // +0x2C
    CWideString TableFilename;                         // +0x30
    bool Locked;                                       // +0x34
    bool ReadOnly;                                     // +0x35
    ~CDefStringTable();
};

CDefStringTable::~CDefStringTable()
{
    StringMap.clear();
}