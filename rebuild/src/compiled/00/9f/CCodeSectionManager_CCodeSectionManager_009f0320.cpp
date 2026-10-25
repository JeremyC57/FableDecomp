// CCodeSectionManager::CCodeSectionManager() -- empty short-name, long-name and section-descriptor maps; section loading off
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F0320, 131 bytes (144 with padding). PDB: ??0CCodeSectionManager@@QAE@XZ. Catalog
//   label "NQuestManager_InitializeMapsList" is wrong: vtable 0x0129C764 = CCodeSectionManager (RTTI; the landed
//   CCodeSectionManagerSingleton::Get 0x00414CE0 news 0x2C bytes and calls this).
// Callees: malloc 0x00BFEA0E three times (map headers: 0x18, 0x18 and 0x24 bytes = 0x10 + the value pair).
// Shape: STLport 4.x map() -> _Rb_tree(comp, alloc) -> _Rb_tree_base(alloc): the header proxy zeroed, then
//   allocate(1), node count 0, then _M_empty_initialize (red colour byte, root 0, leftmost/rightmost = header, each
//   re-read through the header member); SectionLoadingEnabled = false last. Only the derived vtable is stored (the
//   inline CTBaseSingleton<T> constructor's own vptr store is dead and dropped).
// Layout: Ego_r.pdb CCodeSectionManager (0x38) with retail 12-byte STLport maps: ShortNameToHandle +0x04,
//   LongNameToHandle +0x10, SectionDesc +0x1c, SectionLoadingEnabled +0x28 (0x2C bytes; landed dtor 0x009F1560).
// Leaf: ??0CCodeSectionManager@@QAE@XZ
extern "C" void* __cdecl malloc(unsigned int size);      // 0x00BFEA0E

class CCharString
{
public:
    void* PStringData;
};

namespace std {

template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };

template <class _Tp> class allocator
{
public:
    allocator() {}
    template <class _Tp1> allocator(const allocator<_Tp1>&) {}
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc
{
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

struct _Rb_tree_node_base
{
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base
{
    _Value _M_value_field;
};

template <class _Tp, class _Alloc> struct _Rb_tree_base
{
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const _Alloc& __a) : _M_header(allocator<_Node>(__a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc>
{
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;

    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // red: tells the header from the root
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    _Rb_tree(const _Compare& __comp = _Compare(), const _Alloc& __a = _Alloc())
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map
{
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Compare, _Alloc> _M_t;
    map() : _M_t(_Compare(), _Alloc()) {}
};

} // namespace std

template <class T> class CTBaseSingleton
{
protected:
    CTBaseSingleton() {}
public:
    virtual ~CTBaseSingleton() {}
};

class CCodeSectionManager : public CTBaseSingleton<CCodeSectionManager>    // vtable 0x0129C764
{
public:
    struct CSectionDesc                                     // 16 bytes
    {
        CCharString ShortName;
        CCharString LongName;
        bool        Loaded;
        long        Size;
    };

    std::map<CCharString, unsigned long> ShortNameToHandle;        // +0x04
    std::map<CCharString, unsigned long> LongNameToHandle;         // +0x10
    std::map<unsigned long, CSectionDesc> SectionDesc;             // +0x1c
    bool SectionLoadingEnabled;                                     // +0x28

    CCodeSectionManager();
    virtual ~CCodeSectionManager();
};

CCodeSectionManager::CCodeSectionManager()
    : SectionLoadingEnabled(false)
{
}