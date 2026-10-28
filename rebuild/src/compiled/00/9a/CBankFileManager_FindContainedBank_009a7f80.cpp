// CBankFileManager::FindContainedBank(const CCharString& name, NBankFileManager::CContainedBank& contained_bank, CCountedPointer<NBankFileManager::CRetailBank>& retail_bank) -- resolve the name through the alias map, then search every open retail bank's ContainedBanks index for it; on a hit copy the CContainedBank and the bank pointer out and return true
// Leaf: ?FindContainedBank@CBankFileManager@@QAE_NABVCCharString@@AAVCContainedBank@NBankFileManager@@AAV?$CCountedPointer@VCRetailBank@NBankFileManager@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A7F80, length 270 bytes (`ret 0Ch`).
// Identity: Ego_r.pdb name set ?FindContainedBank@CBankFileManager@@QAE_NABVCCharString@@AAVCContainedBank@NBankFileManager@@
//   AAV?$CCountedPointer@VCRetailBank@NBankFileManager@@@@@Z (both outputs by reference). The alias step is the inline
//   `CCharString LookupBankAlias(const CCharString&) const` (Ego_r name set, QBE, returns by value).
// Callees: map<CCharString,CCharString>::_M_find @ 0x009AB5D0 (BankAliasMap), CCharString copy ctor @ 0x0099EC30,
//          map<CCharString,NBankFileManager::CContainedBank>::_M_find @ 0x009AB4F0 (CRetailBank::ContainedBanks),
//          ~CCharString @ 0x0099EAE0, operator delete @ 0x00BFE9BC (count blocks; the per-type deleter is called
//          through the block, ecx = object). Both _M_find calls bind those rows' own leaves (STLport 4.6 `_Rb_tree`).
// Layout (Ego_r, retail STLport sizes: map/set 12 bytes, list 4): CBankFileManager {vptr; Banks +0x04; RetailBanks
//   +0x10 (list<CCountedPointer<NBankFileManager::CRetailBank> >); UsingRetailBanks +0x14; BankAliasMap +0x18; BasePath
//   +0x24; CreatedRetailBanks +0x28; FileVersion +0x34}; NBankFileManager::CRetailBank {..., ContainedBanks +0x18};
//   NBankFileManager::CContainedBank {DataVersion, EntryCount, InfoOffset, InfoSize, Alignment} (0x14 bytes, copied out).
// Shape: the bank copy releases with the short DecRefToZero form (block in esi), the out-reference assignment with the
//          long re-reading form (CCountedPointer::operator= with the `Info != o.Info` guard). The alias resolution is an
//          inline by-value member (`if (alias != end) return alias->second; return name;`): each return copy-constructs
//          into the caller's local, so the local's address is formed before the branch and each arm pushes its own source
//          (`add eax,14h ; push eax ; jmp` / `push ebx`). A `?:` in the declaration, or a reference-returning resolver,
//          shares one `push eax` instead (1 byte longer).
void __cdecl operator delete(void* block);                 // 0x00BFE9BC

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                          // +0x00
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    bool operator<(const CCharString& other) const;
};
class CWideString {
public:
    void* PStringData;                                     // +0x00 (CWideStringData*)
};

struct SCountBlock {                                       // CCPPointerInfo: count, per-type deleter, object
    long  m_references;                                    // +0
    void (__fastcall* m_pfnDestroy)(void*);                // +4
    void* m_pObject;                                       // +8
};
__forceinline bool DecRefToZero(long& count) { return --count == 0; }

template <class T> class CCountedPointer {                 // {object, count block}
public:
    T*           Data;                                     // +0
    SCountBlock* Info;                                     // +4
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer()
    {
        if (Info && DecRefToZero(Info->m_references)) {
            Info->m_pfnDestroy(Info->m_pObject);
            delete Info;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        if (Info != o.Info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Data = o.Data;
            Info = o.Info;
            if (Info)
                ++Info->m_references;
        }
        return *this;
    }
    T* operator->() const { return Data; }
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
    private:
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;          // out of line
    public:
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
    const_iterator find(const _Key& __k) const { return const_iterator(_M_find(__k)); }
    iterator end() { return iterator(this->_M_header._M_data); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {                                             // STLport 4.6 (SGI): 12 bytes
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
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

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef typename _Traits::reference reference;
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {           // STLport 4.6: 4 bytes (the header node pointer)
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return this->_M_node._M_data; }
};

} // namespace std

class CBankFile;
namespace NBankFileManager {
class CContainedBank {                                     // 0x14 bytes (Ego_r)
public:
    unsigned long DataVersion;                             // +0x00
    unsigned long EntryCount;                              // +0x04
    unsigned long InfoOffset;                              // +0x08
    unsigned long InfoSize;                                // +0x0C
    unsigned long Alignment;                               // +0x10
};
class CRetailBank {
public:
    unsigned char _pad00[0x18];                            // vptr, DiskFile, ThreadedFile, UseDMA
    std::map<CCharString, CContainedBank> ContainedBanks;  // +0x18 (_M_find @ 0x009AB4F0)
    CCharString FilePath;                                  // +0x24
};
}

class CBankFileManager {
public:
    class CBankInfo {                                      // 0x14 bytes (Ego_r)
    public:
        CWideString DevBankFilePath;                       // +0x00
        CWideString RetailBankFilePath;                    // +0x04
        CWideString DevHeaderFilePath;                     // +0x08
        CWideString RetailHeaderFilePath;                  // +0x0C
        CBankFile*  RegisteredBank;                        // +0x10
    };
    virtual ~CBankFileManager();                           // vptr +0x00
    std::map<CCharString, CBankInfo> Banks;                // +0x04
    std::list<CCountedPointer<NBankFileManager::CRetailBank> > RetailBanks;   // +0x10
    bool UsingRetailBanks;                                 // +0x14
    std::map<CCharString, CCharString> BankAliasMap;       // +0x18 (_M_find @ 0x009AB5D0)
    CWideString BasePath;                                  // +0x24
    std::set<CWideString> CreatedRetailBanks;              // +0x28
    unsigned long FileVersion;                             // +0x34

    CCharString LookupBankAlias(const CCharString& alias) const
    {
        std::map<CCharString, CCharString>::const_iterator it = BankAliasMap.find(alias);
        if (it != BankAliasMap.end())
            return (*it).second;
        return alias;
    }
    bool FindContainedBank(const CCharString& name, NBankFileManager::CContainedBank& contained_bank,
                           CCountedPointer<NBankFileManager::CRetailBank>& retail_bank);
};

bool CBankFileManager::FindContainedBank(const CCharString& name, NBankFileManager::CContainedBank& contained_bank,
                                         CCountedPointer<NBankFileManager::CRetailBank>& retail_bank)
{
    CCharString bank_name = LookupBankAlias(name);
    for (std::list<CCountedPointer<NBankFileManager::CRetailBank> >::iterator it = RetailBanks.begin(); it != RetailBanks.end(); ++it) {
        CCountedPointer<NBankFileManager::CRetailBank> bank(*it);
        std::map<CCharString, NBankFileManager::CContainedBank>::iterator found = bank->ContainedBanks.find(bank_name);
        if (found != bank->ContainedBanks.end()) {
            contained_bank = (*found).second;
            retail_bank = bank;
            return true;
        }
    }
    return false;
}
