// CWorldMap::ApplyScriptedMapBrush(const CScriptedMapBrush& brush, bool apply) -- look the brush's name up in the loaded-brush map and, when it is registered, forward the apply to the map it lives in
// Leaf: ?ApplyScriptedMapBrush@CWorldMap@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FE550, 61 bytes, `ret 8` (thiscall: brush, apply). PDB name
//   ?ApplyScriptedMapBrush@CWorldMap@@QAEXABVCScriptedMapBrush@@_N@Z (Ego_r.pdb name set; the CCharString overload is
//   inlined into it). No direct caller in retail (reached through the script layer). Catalog label
//   "CreatureMode_ApplyAnimatedBrush" is descriptive.
// Callees: CScriptedMapBrush::GetName 0x0082E250, map<CCharString, long>::_M_find 0x0042B467 (STLport `find` = the
//   out-of-line node search wrapped in an iterator), CMap::ApplyScriptedMapBrush(const CCharString&, bool) 0x00820B80.
// Layout: CWorldMap (Ego_r.pdb names) LoadedScriptedMapBrushes +0x14 (STLport map: header, count, comparator = 12 bytes;
//   the engine header carries the donor 16), Maps (CArray<CMapInfo>, 0x48-byte records, PMap +0x10) +0x20; the node
//   value pair<const CCharString, long> sits at +0x10, so the map number is node +0x14.
// Shape: the name reference lives in edi for both calls, the map's address in ebx (`find` then `!= end()`).
extern "C" void __cdecl free(void* p);             // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                // 0x0099EAE0
};

class CMap {
public:
    void ApplyScriptedMapBrush(const CCharString& name, bool apply);   // 0x00820B80
};

class CScriptedMapBrush {                           // Ego_r.pdb (BrushName at +0x48 retail: its three sets/map are 12 bytes)
public:
    unsigned char _pad00[0x48];
    CCharString BrushName;                         // +0x48
    const CCharString& GetName() const;            // 0x0082E250 (`lea eax,[ecx+48h]`; manifest "CModeBlock::GetAnimFilter" is folded)
};

template <class T> class CCountedPointer {
public:
    T* Object;                                     // +0x00
    void* Info;                                    // +0x04
    T* operator->() const { return Object; }
};

class CMapInfo {                                    // Ego_r.pdb, 0x48 bytes
public:
    unsigned char Area[0x10];                      // +0x00 C2DBoxI
    CCountedPointer<CMap> PMap;                    // +0x10
    unsigned char _rest[0x30];                     // +0x18 LevelName .. IsSea
};

namespace std {

template <class T> class allocator {
public:
    allocator() { }
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const;
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

struct _Rb_tree_node_base {
    bool _M_color;                                 // +0x00
    _Rb_tree_node_base* _M_parent;                 // +0x04
    _Rb_tree_node_base* _M_left;                   // +0x08
    _Rb_tree_node_base* _M_right;                  // +0x0C
};

template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                              // +0x10
};

template <class Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
        _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E (_STLP_CALL)
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class T> __forceinline void _Destroy(T* p) { p->~T(); }

template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
    V* operator->() const { return &operator*(); }
    bool operator==(const _Rb_tree_iterator& y) const { return _M_node == y._M_node; }
    bool operator!=(const _Rb_tree_iterator& y) const { return _M_node != y._M_node; }
};

template <class K, class V, class Compare, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<V> _Node;
    typedef _Rb_tree_iterator<V> iterator;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                    // +0x04
    Compare _M_key_compare;                        // +0x08
    _Node* _M_find(const K& k) const;              // 0x0042B467 (manifest "StdMap_FindLE" is descriptive)
    iterator end() { return iterator(_M_header._M_data); }
    iterator find(const K& k) { return iterator(_M_find(k)); }
    static __forceinline void deallocate(_Node* p, unsigned int) { if (p != 0) free(p); }
    void erase(iterator position)
    {
        _Node* y = (_Node*)_Rb_global_inst::_Rebalance_for_erase(position._M_node, _M_header._M_data->_M_parent,
            _M_header._M_data->_M_left, _M_header._M_data->_M_right);
        _Destroy(&y->_M_value_field);
        deallocate(y, 1);
        --_M_node_count;
    }
};

template <class K, class T, class Compare = less<K>, class Alloc = allocator<pair<const K, T> > > class map {
public:
    typedef _Rb_tree<K, pair<const K, T>, Compare, Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    iterator find(const K& k) { return _M_t.find(k); }
    void erase(iterator position) { _M_t.erase(position); }
};

template <class T> class vector {
public:
    T* _M_start;                                   // +0x00
    T* _M_finish;                                  // +0x04
    T* _M_end_of_storage;                          // +0x08
    T& operator[](unsigned int n) { return _M_start[n]; }
};

}

template <class T> class CArray : public std::vector<T> {
};

class CWorldMap {                                   // local model: retail LoadedScriptedMapBrushes is 12 bytes (donor 16)
public:
    unsigned char _pad00[0x14];                    // +0x00 vtable, World, Component, PThingMapManager
    std::map<CCharString, long> LoadedScriptedMapBrushes;   // +0x14 brush name -> map number
    CArray<CMapInfo> Maps;                         // +0x20
    void ApplyScriptedMapBrush(const CScriptedMapBrush& brush, bool apply);
};

void CWorldMap::ApplyScriptedMapBrush(const CScriptedMapBrush& brush, bool apply)
{
    const CCharString& name = brush.GetName();
    std::map<CCharString, long>::iterator it = LoadedScriptedMapBrushes.find(name);
    if (it != LoadedScriptedMapBrushes.end())
        Maps[it->second].PMap->ApplyScriptedMapBrush(name, apply);
}