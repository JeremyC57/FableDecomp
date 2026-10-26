// NBankFileManager::CRetailBank::CRetailBank() -- an unopened retail bank: no disk or threaded file, DMA off, an empty contained-bank map and an empty file path
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AC4D0, 84 bytes, plain `ret`.
// Leaf: ??0CRetailBank
// Callees: the root base constructor @ 0x0099A2F0 (stores the CBaseClassNonCopyable vtable 0x0129A7C4; declared ??0CBase@@QAE@XZ as
//          the landed rows do), malloc @ 0x00BFEA0E (the 0x28-byte map header node), CCharString::CCharString() @ 0x0099E4B0.
//          Vtable 0x0129B010 (NBankFileManager::CRetailBank; its deleting dtor 0x009AC7A0 is landed).
// Layout: Ego_r.pdb CRetailBank: DiskFile (CCountedPointer<CDiskFileWin32>) +0x04, ThreadedFile (CCountedPointer<CThreadedFile>) +0x0c,
//         UseDMA +0x14, ContainedBanks (map<CCharString, CContainedBank>) +0x18, FilePath +0x24 -- the retail map is the 12-byte
//         STLport tree {header, node count, empty compare}, so FilePath follows at +0x24 (rebuild/include/fable_retail_banks.h).
// Shape: the counted pointers' inline default ctors (Data, Info = 0) and the flag through one zero register (ebx); the STLport
//        _Rb_tree_base ctor (header zeroed by the alloc proxy, then allocate(1): a 0x28-byte node, 0x10 links + the 24-byte
//        pair) runs before the derived _Rb_tree's node count (a flat ctor stores the count before the malloc), then
//        _M_empty_initialize (red colour byte, null root, leftmost = rightmost = header,
//        each through a re-read of the header pointer).
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
struct CBase {                                       // root base (CBaseClassNonCopyable)
    virtual ~CBase();
    CBase();                                         // 0x0099A2F0
};
class CCharString {
public:
    void* m_pStringData;
    CCharString();                                   // 0x0099E4B0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CDiskFileWin32;
class CThreadedFile;
namespace NBankFileManager {
class CContainedBank { public:
    unsigned long DataVersion;                       // +0x00
    unsigned long EntryCount;                        // +0x04
    unsigned long InfoOffset;                        // +0x08
    unsigned long InfoSize;                          // +0x0c
    unsigned long Alignment;                         // +0x10
};
}
namespace std {
template <class K, class V> struct pair { K first; V second; };
template <class T> struct less { };
template <class V> struct _Rb_tree_node {
    bool              _M_color;                      // +0x00 (red = false)
    _Rb_tree_node*    _M_parent;                     // +0x04
    _Rb_tree_node*    _M_left;                       // +0x08
    _Rb_tree_node*    _M_right;                      // +0x0c
    V                 _M_value_field;                // +0x10
};
template <class V> struct _Rb_tree_base {
    typedef _Rb_tree_node<V> _Node;
    _Node* _M_header;                                // +0x00 (alloc proxy data)
    static _Node* allocate(unsigned int n) { return (_Node*)malloc(n * sizeof(_Node)); }
    _Rb_tree_base() : _M_header(0) { _M_header = allocate(1); }
};
template <class K, class V, class C> struct _Rb_tree : _Rb_tree_base<V> {
    unsigned int _M_node_count;                      // +0x04
    C            _M_key_compare;                     // +0x08
    _Rb_tree() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = false;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
template <class K, class T, class C = less<K> > struct map {
    _Rb_tree<K, pair<K, T>, C> _M_t;
};
}
namespace NBankFileManager {
class CRetailBank : public CBase {                   // vtable 0x0129B010
public:
    CCountedPointer<CDiskFileWin32>                       DiskFile;        // +0x04
    CCountedPointer<CThreadedFile>                        ThreadedFile;    // +0x0c
    bool                                                  UseDMA;          // +0x14
    std::map<CCharString, NBankFileManager::CContainedBank> ContainedBanks;  // +0x18
    CCharString                                           FilePath;        // +0x24
    CRetailBank();
    virtual ~CRetailBank();
};
CRetailBank::CRetailBank()
    : UseDMA(false)
{
}
}