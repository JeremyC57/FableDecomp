// CBankFileManager::CBankFileManager() -- construct the bank manager: empty bank map, retail bank list, alias map, base path and created-bank set; not using retail banks, file version 100
// Leaf: ??0CBankFileManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A7AC0, 172 bytes, thiscall `ret`. PDB (Ego_r): `??0CBankFileManager@@QAE@XZ`. Catalog
// name "NHeroInformationScreens_CAbilityScreen_ctor" is propagated and wrong (the body stores CBankFileManager's vtable
// 0x0129B018; the callee-canon label file already says CBankFileManager::CBankFileManager). Caller: static init 0x01224015.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0, malloc @ 0x00BFEA0E (each container's header node: map 0x28, list 0x10,
// alias map 0x18, set 0x14), CWideString::CWideString() @ 0x0099AED0.
// Layout: Ego_r.pdb CBankFileManager (76 bytes there; the retail STLport containers make it 0x38): Banks +0x04,
// RetailBanks +0x10, UsingRetailBanks +0x14, BankAliasMap +0x18, BasePath +0x24, CreatedRetailBanks +0x28, FileVersion +0x34.
// Shape: STLport 4.6 container constructors inlined in member order (_Rb_tree_base: header = 0, header = allocate(1);
// _Rb_tree: count = 0, _M_empty_initialize = red header, null root, leftmost = rightmost = header; _List_base: node = 0,
// allocate(1), next = prev = itself), then the body `UsingRetailBanks = false; FileVersion = 100;`. No EH frame (the
// TU builds without /GX, as the landed destructor 0x009A7B70).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

class CWideString {
public:
    void* PStringData;                                      // +0x00
    CWideString();                                          // 0x0099AED0
};
class CCharString {
public:
    void* PStringData;                                      // +0x00
};
class CBankFile;
template <class T> class CCountedPointer {
public:
    T*    Object;                                           // +0x00
    void* Count;                                            // +0x04
};
namespace NBankFileManager { class CRetailBank; }

namespace _STL {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
};

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp> class _STLP_alloc_proxy : public allocator<_Tp> {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const allocator<_Tp>&, _Value __p) : _M_data(__p) {}
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Value> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, _Rb_tree_node<_Value> > _M_header;   // +0x00
    _Rb_tree_base(const allocator<_Value>&)
        : _M_header(allocator<_Rb_tree_node<_Value> >(), (_Rb_tree_node<_Value>*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};

template <class _Key, class _Value, class _Compare> class _Rb_tree : public _Rb_tree_base<_Value> {
public:
    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08
    _Rb_tree(const _Compare& __comp, const allocator<_Value>& __a)
        : _Rb_tree_base<_Value>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // _S_rb_tree_red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
};

template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Compare> _M_t;
    map() : _M_t(_Compare(), allocator<value_type>()) {}
};

template <class _Key, class _Compare = less<_Key> > class set {
public:
    _Rb_tree<_Key, _Key, _Compare> _M_t;
    set() : _M_t(_Compare(), allocator<_Key>()) {}
};

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x08
};
template <class _Tp> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp> > _M_node;   // +0x00
    _List_base(const allocator<_Tp>&)
        : _M_node(allocator<_List_node<_Tp> >(), (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};
template <class _Tp> class list : public _List_base<_Tp> {
public:
    list() : _List_base<_Tp>(allocator<_Tp>()) {}
};
}

class CBaseClass {
public:
    CBaseClass();                                           // 0x0099A2D0
    virtual void v00();
};

class CBankFileManager : public CBaseClass {                // vtable 0x0129B018
public:
    class CBankInfo {                                       // PDB: 20 bytes
    public:
        CWideString DevBankFilePath;                        // +0x00
        CWideString RetailBankFilePath;                     // +0x04
        CWideString DevHeaderFilePath;                      // +0x08
        CWideString RetailHeaderFilePath;                   // +0x0c
        CBankFile*  RegisteredBank;                         // +0x10
    };
    _STL::map<CCharString, CBankInfo>                           Banks;               // +0x04
    _STL::list<CCountedPointer<NBankFileManager::CRetailBank> > RetailBanks;         // +0x10
    bool                                                        UsingRetailBanks;    // +0x14
    _STL::map<CCharString, CCharString>                         BankAliasMap;        // +0x18
    CWideString                                                 BasePath;            // +0x24
    _STL::set<CWideString>                                      CreatedRetailBanks;  // +0x28
    unsigned long                                               FileVersion;         // +0x34
    CBankFileManager();
    virtual void v00();
};

CBankFileManager::CBankFileManager()
{
    UsingRetailBanks = false;
    FileVersion = 100;
}