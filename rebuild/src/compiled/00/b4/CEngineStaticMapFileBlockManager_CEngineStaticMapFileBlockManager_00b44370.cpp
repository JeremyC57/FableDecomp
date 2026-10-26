// CEngineStaticMapFileBlockManager::CEngineStaticMapFileBlockManager() -- default constructor: a null async bank entry pointer and an empty block-index -> file-block map
// Leaf: ??0CEngineStaticMapFileBlockManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B44370, 55 bytes. __fastcall, returns this.
// Identity: FableWin.pdb / Ego_r.pdb `??0CEngineStaticMapFileBlockManager@@QAE@XZ`; Ego_r's 24-byte class is
//   {CCountedPointer<CBankFileAsyncEntry> AsyncBankEntry +0x00, map<long, CFileBlock> Blocks +0x08} (retail STLport map
//   12 bytes, so 20 bytes in retail). It sits between the map's own ctor/dtor COMDATs 0x00B442E0 / 0x00B44290 and the
//   class destructor 0x00B443B0 (map clear through _M_erase 0x00B441F0, free the header, then the counted pointer's
//   release), which CEngineMap's destructor 0x00B42A90 calls on its FileBlockManager member (+0x44; Ego_r CEngineMap +68).
//   No direct caller (CEngineMap's constructor inlines it). The 36-byte node is the 16-byte node base plus
//   pair<const long, CFileBlock> (CFileBlock {long RefCount; bool LowPriority; CCountedPointer<CBankFileAsyncData>
//   LoadingData}). Catalog label "List_InitializeNode_36Bytes_Complex" is descriptive; the catalog's
//   "~CBankFileEntryUpdateData" at 0x00B443B0 / 0x00B42A90 is propagated (those are this class's and CEngineMap's dtors).
// Callee: malloc @ 0x00BFEA0E (the map's header node).
// Shape: the counted pointer's `Data(0), Info(0)` then STLport `_Rb_tree(_Compare(), allocator_type())` (the landed
//   CVertexGroupBlock::CVertexGroupBlock 0x00AD9030 model: header proxy zeroed, allocated, count zeroed, red colour,
//   no parent, left/right at the header); one shared zero register.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    template <class U> allocator(const allocator<U>&) { }
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
};
template <class T> class less {
public:
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const A& a) : _M_header(allocator<_Node>(a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};
template <class T> struct _Identity { };
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    _Rb_tree(const Compare& comp, const A& a) : _Rb_tree_base<V, A>(a), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
};
template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    typedef _Rb_tree<K, K, _Identity<K>, C, A> _Rep_type;
    _Rep_type _M_t;
    set() : _M_t(C(), A()) { }
};
template <class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
};
template <class P> struct _Select1st { };
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    _Rep_type _M_t;
    map() : _M_t(C(), A()) { }
};
} // namespace std

class CBankFileAsyncEntry;
class CBankFileAsyncData;
class CCPPointerInfo;

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer() : Data(0), Info(0) { }
};

class CEngineStaticMapFileBlockManager {
public:
    struct CFileBlock {                                  // 16 bytes
    public:
        long                                RefCount;     // +0x00
        bool                                LowPriority;  // +0x04
        CCountedPointer<CBankFileAsyncData> LoadingData;  // +0x08
    };
    CCountedPointer<CBankFileAsyncEntry>  AsyncBankEntry; // +0x00
    std::map<long, CFileBlock>           Blocks;         // +0x08
    CEngineStaticMapFileBlockManager();
    ~CEngineStaticMapFileBlockManager();                // 0x00B443B0
};

CEngineStaticMapFileBlockManager::CEngineStaticMapFileBlockManager()
{
}