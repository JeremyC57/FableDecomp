// _$E2() -- `dynamic atexit destructor for 'desc'' (the function-local static CSectionDesc of CCodeSectionManager::GetSectionDesc): the implicit ~CSectionDesc of the static (~CCharString LongName, then ShortName)
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x0122A940, 20 bytes (padding to 0x0122A960), in the .text tail where VC7.1 places the
//   compiler-generated $E thunks: `mov ecx,0x013CAA58 ; call ~CCharString ; mov ecx,0x013CAA54 ; jmp ~CCharString`.
// Identity (proven, two sources): (1) Ego_r.pdb and FableWin.pdb both name the thunk "`dynamic atexit destructor for 'desc''" in
//   the scope of CCodeSectionManager::GetSectionDesc; the static at 0x013CAA54 is the 16-byte CSectionDesc {ShortName +0,
//   LongName +4, Loaded +8, Size +0xC} that GetSectionDesc 0x009F0240 returns when the handle is not in SectionDesc (it clears
//   Loaded and Size at 0x013CAA5C / 0x013CAA60, guard mask 1 of the $S1 guard 0x013CAA64, and registers this thunk with atexit);
//   (2) compiling GetSectionDesc with `static CSectionDesc desc;` makes VC7.1 emit exactly this thunk as _$E2 and the static as
//   ?desc@?4??GetSectionDesc@CCodeSectionManager@@AAEAAUCSectionDesc@2@K@Z@4U32@A (private AAE and the non-const reference
//   return per both PDBs). The catalog label "DynamicInit_013caa54_58" and the landed s_empty spelling are stand-ins (NAME_FIXES).
// Callees: CCharString::~CCharString() 0x0099EAE0 (twice, the second a tail jmp).
// Layout: CSectionDesc and CCodeSectionManager members per Ego_r.pdb: ShortNameToHandle +0x04, LongNameToHandle +0x10,
//   SectionDesc +0x1C (STLport 12-byte maps), SectionLoadingEnabled +0x28; the STLport _Rb_tree text needed by find / end.
// Shape: the TU is GetSectionDesc itself (the leaf is the compiler-generated thunk; GetSectionDesc's own COMDAT is a separate
//   landed row 0x009F0240): the PDB order -- find, compare with end(), the function-local static on the miss path.
class CCharStringData;
class CCharString {
public:
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
private:
    CCharStringData* PStringData;                       // +0x00
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                 // +0x00
    _Base_ptr _M_parent;                                // +0x04
    _Base_ptr _M_left;                                  // +0x08
    _Base_ptr _M_right;                                 // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    typename _Traits::pointer operator->() const { return &((_Link_type)_M_node)->_M_value_field; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    iterator begin() { return iterator((_Link_type)_M_header._M_data->_M_left); }
    iterator end() { return iterator(_M_header._M_data); }
    iterator find(const key_type& __x);                 // out of line (0x009F0D90)
protected:
    _STLP_alloc_proxy<_Link_type, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                         // +0x04
    _Compare     _M_key_compare;                        // +0x08
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    iterator end() { return _M_t.end(); }
    iterator find(const _Key& __x) { return _M_t.find(__x); }
private:
    _Rep_type _M_t;                                     // 12 bytes
};
}

template <class T> class CTBaseSingleton {             // RTTI base of CCodeSectionManager (+0), vptr only
public:
    virtual ~CTBaseSingleton();
};

class CCodeSectionManager : public CTBaseSingleton<CCodeSectionManager> {
public:
    struct CSectionDesc {                               // Ego_r.pdb, 16 bytes (implicit ctor / dtor)
        CCharString ShortName;                          // +0x00
        CCharString LongName;                           // +0x04
        bool        Loaded;                             // +0x08
        long        Size;                               // +0x0C
    };
    void LoadSection(unsigned long handle);             // 0x009F02C0
private:
    CSectionDesc& GetSectionDesc(unsigned long handle); // 0x009F0240
    std::map<CCharString, unsigned long>  ShortNameToHandle;     // +0x04
    std::map<CCharString, unsigned long>  LongNameToHandle;      // +0x10
    std::map<unsigned long, CSectionDesc> SectionDesc;           // +0x1C
    bool                                  SectionLoadingEnabled; // +0x28
};

CCodeSectionManager::CSectionDesc& CCodeSectionManager::GetSectionDesc(unsigned long handle)
{
    std::map<unsigned long, CSectionDesc>::iterator it = SectionDesc.find(handle);
    if (it == SectionDesc.end()) {
        static CSectionDesc desc;
        desc.Loaded = false;
        desc.Size = 0;
        return desc;
    }
    return it->second;
}