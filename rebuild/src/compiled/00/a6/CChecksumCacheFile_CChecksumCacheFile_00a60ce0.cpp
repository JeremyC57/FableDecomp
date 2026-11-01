// CChecksumCacheFile::~CChecksumCacheFile() -- close the cache file (save and clear), then destroy the Entries map (clear + free the header node) and the Filename string
// Leaf: ??1CChecksumCacheFile@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A60CE0, 82 bytes, thiscall, tail `jmp ~CWideString` on this+0.
// Identity (proven): Ego_r.pdb places ??1CChecksumCacheFile@@QAE@XZ (0x00C00700) right after ?Close@CChecksumCacheFile@@QAEXXZ
//   (0x00C006D0) and retail's dtor follows the landed Close 0x00A60C30 the same way; FableWin.pdb has the same symbol (0x033A6870).
//   Callers destroy CChecksumCacheFile objects: the landed CDefinitionManager::CheckDependencies 0x009B05F0 (a local) and
//   ~CBankFile 0x009CE180 (a member), both binding ??1CChecksumCacheFile@@QAE@XZ. The catalog label "~CPatchTesselationEdgeStrip"
//   is propagated; the binding ??1CChecksumFile@@QAE@XZ is invented.
// Callees: CChecksumCacheFile::Close 0x00A60C30, the map's _Rb_tree::_M_erase 0x00A60A00 (landed, private STLport member),
//   free 0x00BFEA14 (the header node), CWideString::~CWideString 0x0099B510 (tail jump).
// Shape: user destructor `{ Close(); }`; members in reverse order: Entries (STLport 4.6 ~_Rb_tree = clear(): `if (count)
//   { _M_erase(root); leftmost = header; root = 0; rightmost = header; count = 0; }`, then ~_Rb_tree_base's
//   `deallocate(header)`, a null-tested free), then Filename. BufferSize / NeedsSaving are trivial.
// Layout: Ego_r.pdb CChecksumCacheFile with the retail 12-byte STLport map (0x18): Filename +0, Entries +4 (header +4, node
//   count +8, compare +0xC), BufferSize +0x10, NeedsSaving +0x14. CEntry (Ego_r, 0x20): CDateAndTime TimeStamp +0, Checksum
//   +0x1C (trivially destructible). No engine header; STLport templates as the landed _M_erase 0x00A60A00 spells them.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CWideString {
public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};

class CDateAndTime {                                        // Ego_r 0x1C bytes, trivially destructible
public:
    long Year;
    unsigned long Month;
    unsigned long Day;
    unsigned long Hour;
    unsigned long Minute;
    unsigned long Second;
    unsigned long Millisecond;
};

namespace std {

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                   // +0x00
    _Base_ptr   _M_parent;                                  // +0x04
    _Base_ptr   _M_left;                                    // +0x08
    _Base_ptr   _M_right;                                   // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    unsigned int _M_node_count;                             // +0x04
    _Compare _M_key_compare;                                // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                          // 0x00A60A00 (this instantiation)
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
private:
    _Rep_type _M_t;
};

}   // namespace std

class CChecksumCacheFile {                                  // 0x18 bytes (retail)
public:
    struct CEntry {                                         // FableWin.pdb UCEntry; Ego_r 0x20
        CDateAndTime TimeStamp;                             // +0x00
        unsigned long Checksum;                             // +0x1C
    };
    CChecksumCacheFile(long bufferSize);
    ~CChecksumCacheFile();
    void Close();                                           // 0x00A60C30
    CWideString Filename;                                   // +0x00
    std::map<CWideString, CEntry> Entries;                  // +0x04
    long BufferSize;                                        // +0x10
    bool NeedsSaving;                                       // +0x14
};

CChecksumCacheFile::~CChecksumCacheFile()
{
    Close();
}