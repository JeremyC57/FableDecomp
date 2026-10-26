// behaviour test for CBankFileManager::RegisterBank (0x009A8090): a registered name gets the bank pointer, an unknown name changes nothing
#include <cstdio>
class CCharStringData;
class CCharString {                                     // 4 bytes: the shared string block pointer
public:
    CCharStringData* PStringData;
    bool operator<(const CCharString& other) const;
};
namespace std {

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> struct _Const_traits {
    typedef _Tp value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    reference operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;
    unsigned int _M_node_count;                         // +0x04
    _Compare _M_key_compare;                            // +0x08 (empty)
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;          // out of line
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
    const_iterator find(const _Key& __k) const { return const_iterator(_M_find(__k)); }
    iterator end() { return iterator(this->_M_header._M_data); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {                                             // STLport 4.6 (SGI): 12 bytes
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __x) const { return _M_t.find(__x); }
    iterator end() const { return _M_t.end(); }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                             // STLport 4.6 (SGI): 12 bytes
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __x) { return _M_t.find(__x); }
    const_iterator find(const _Key& __x) const { return _M_t.find(__x); }
    iterator end() { return _M_t.end(); }
    const_iterator end() const { return _M_t.end(); }
};

} // namespace std

class CWideString {                                     // 4 bytes
public:
    void* PStringData;
};
class CBankFile;

class CBankFileManager {                                // Ego_r: vptr, Banks +0x04, ...
public:
    class CBankInfo {                                   // Ego_r: 20 bytes
    public:
        CWideString DevBankFilePath;                    // +0x00
        CWideString RetailBankFilePath;                 // +0x04
        CWideString DevHeaderFilePath;                  // +0x08
        CWideString RetailHeaderFilePath;               // +0x0C
        CBankFile*  RegisteredBank;                     // +0x10
    };
    virtual ~CBankFileManager() { }
    std::map<CCharString, CBankInfo> Banks;             // +0x04 (STLport, 12 bytes)
    void RegisterBank(const CCharString& name, CBankFile* bank);
};

void CBankFileManager::RegisterBank(const CCharString& name, CBankFile* bank)
{
    std::map<CCharString, CBankInfo>::iterator it = Banks.find(name);
    if (it != Banks.end())
        (*it).second.RegisteredBank = bank;
}

typedef std::_Rb_tree_node<std::pair<const CCharString, CBankFileManager::CBankInfo> > TNode;
static std::_Rb_tree_node_base gHeader;
static unsigned long gNodeBuf[(sizeof(TNode) + 3) / 4];
static TNode& gNode = *(TNode*)gNodeBuf;
static bool gFound = false;
static const CCharString* gAsked = 0;
namespace std {
template <> TNode* _Rb_tree<CCharString, pair<const CCharString, CBankFileManager::CBankInfo>, _Select1st<pair<const CCharString, CBankFileManager::CBankInfo> >, less<CCharString>, allocator<pair<const CCharString, CBankFileManager::CBankInfo> > >::_M_find(const CCharString& __k) const
{
    gAsked = &__k;
    return gFound ? &gNode : (TNode*)this->_M_header._M_data;
}
}
int main()
{
    CBankFileManager m;
    m.Banks._M_t._M_header._M_data = (TNode*)&gHeader;
    CCharString name;
    CBankFile* bank = (CBankFile*)&gHeader;
    gNode._M_value_field.second.RegisteredBank = 0;
    gFound = false;
    m.RegisterBank(name, bank);
    bool ok = gAsked == &name && gNode._M_value_field.second.RegisteredBank == 0;
    gFound = true;
    m.RegisterBank(name, bank);
    ok = ok && gNode._M_value_field.second.RegisteredBank == bank;
    if (!ok) { std::printf("009a8090_TEST FAIL\n"); return 1; }
    std::printf("009a8090_TEST PASS\n");
    return 0;
}
