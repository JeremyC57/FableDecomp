// CLevelLoader::CObject::~CObject() -- destroy the region level-file vector (each CLevelFiles, then the block) and the loadable-section set (clear the tree, free the header)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005195B0, length 103 bytes (padded to 112).
// Leaf: ??1CObject@CLevelLoader
// Label correction: the catalog "CParticleEmitter::DeleteAllParticles" is propagated and wrong. The element destructor is
// ~CLevelFiles @ 0x0050F980 (28-byte records), the neighbouring push_back @ 0x0051AE60 is vector<CLevelLoader::CObject::CLevelFiles>,
// and the callers are the deleting destructor 0x0051A3D0 and the delete helper 0x0051AF00 -- the implicit ~CObject().
// Callees: ~CLevelFiles @ 0x0050F980, set<CCharString>::_M_erase(node) @ 0x004ABA70, free @ 0x00BFEA14.
// Layout: Ego_r.pdb CLevelLoader::CObject is 40 bytes; retail is 0x20 (12-byte STLport set and vector): LoadableSections +0,
// PLevelLoader +0xc, RegionLevelFiles +0x10, RegionIndex +0x1c.
// Shape: an empty user destructor; the members die in reverse order: STLport ~vector (_Destroy over by-value bounds so the end
// sits in ebx, then the block freed under a null test) and ~set (inline clear(): erase from the root, leftmost/root/rightmost
// reset in STLport order, count zeroed; then the header freed).
extern "C" void __cdecl free(void* p);      // 0x00BFEA14
struct CCharString { void* m_pData; };
struct CAFile;
struct CCountedPointerAFile { CAFile* Object; void* Block; };
struct CLevelFiles {                        // CLevelLoader::CObject::CLevelFiles (28 bytes)
    long LevelID;                           // +0x00
    CCountedPointerAFile PTopologyFile;     // +0x04
    CCountedPointerAFile PNavigationFile;   // +0x0c
    CCountedPointerAFile PThingFile;        // +0x14
    ~CLevelFiles();                         // 0x0050F980
};
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct vector {          // retail STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
struct _Rb_tree_node_base {
    char _M_color;                          // +0x00
    _Rb_tree_node_base* _M_parent;          // +0x04
    _Rb_tree_node_base* _M_left;            // +0x08
    _Rb_tree_node_base* _M_right;           // +0x0c
};
struct less_CCharString { };
struct CCharStringSet {                     // retail STLport set<CCharString> (12 bytes)
    _Rb_tree_node_base* _M_header;          // +0x00
    unsigned long       _M_node_count;      // +0x04
    less_CCharString    _M_key_compare;     // +0x08
    void _M_erase(_Rb_tree_node_base* x);   // 0x004ABA70
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    ~CCharStringSet()
    {
        clear();
        if (_M_header)
            free(_M_header);
    }
};
struct CLevelLoader {
struct CObject {
    CCharStringSet LoadableSections;        // +0x00
    CLevelLoader* PLevelLoader;             // +0x0c
    vector<CLevelFiles> RegionLevelFiles;   // +0x10
    long RegionIndex;                       // +0x1c
    ~CObject();
};
};
CLevelLoader::CObject::~CObject()
{
}
