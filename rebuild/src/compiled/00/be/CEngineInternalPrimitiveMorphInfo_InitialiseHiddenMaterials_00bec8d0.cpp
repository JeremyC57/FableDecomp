// CEngineInternalPrimitiveMorphInfo::InitialiseHiddenMaterials(const C3DMesh2& mesh) -- size the per-material hidden flags to the mesh's material count and set each flag when the material's name is in HiddenMaterialNames
// Leaf: ?InitialiseHiddenMaterials@CEngineInternalPrimitiveMorphInfo@@QAEXABVC3DMesh2@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEC8D0, 127 bytes, `__fastcall`, `ret 4`. Ego_r.pdb:
// ?InitialiseHiddenMaterials@CEngineInternalPrimitiveMorphInfo@@QAEXABVC3DMesh2@@@Z (the manifest label
// "CEngineInternalPrimitive_UpdateVisibilityVector" is descriptive). Callers: nine render paths (static/animated/2D managers).
// Callees: CArray<unsigned char>::resize(n, const unsigned char&) @ 0x00486189 (STLport vector<unsigned char>::resize), the set's
// out-of-line _M_find @ 0x004B9370 (returns the node; compared with the header = end()).
// Layout (Ego_r.pdb CEngineInternalPrimitiveMorphInfo, retail sizes: CVectorMap 0x10, STLport set 0xc, CArray 0xc): TextureDecals
// +0, CompositeSurfaces +0x10, HiddenMaterialNames set<CCharString> +0x20, HiddenMaterials CArray<unsigned char> +0x2c.
// C3DMesh2 (Ego_r.pdb names, retail offsets 8 below the PDB's): MaterialCount +0x88, Materials +0x8c; C3DMeshMaterial is 0x30
// bytes with its CCharString Name at +8.
// Shape: the count is read once into a local (ebp: the loop limit and the resize argument), the resize's `false` value is a byte
// temporary in the frame; per material the flag is a bool local cleared before the find (`xor bl,bl`) and set on a hit, stored
// through the re-read vector start; the &set and &HiddenMaterials pointers live in frame words (register pressure). The count
// in ebp and &set in a frame word need the same-TU stand-in body of the out-of-line _M_find (a read-only walk; only its
// visibility matters); declared only, VC7.1 swaps them.
class CCharString {
public:
    void* m_pData;                                 // 4 bytes in retail
};

class C3DMeshMaterial {                            // 0x30 bytes
public:
    void*         __vftable;                       // +0x00
    long          MaterialIndex;                   // +0x04
    CCharString   Name;                            // +0x08
    unsigned char _pad0c[0x24];
};

class C3DMesh2 {
public:
    unsigned char    _pad00[0x88];
    unsigned long    MaterialCount;                // +0x88
    C3DMeshMaterial* Materials;                    // +0x8c
};

namespace _STL {
template <class T> struct less { };
template <class T> class allocator { };
template <class T> struct _Identity { };
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
inline bool operator!=(const _Rb_tree_base_iterator& x, const _Rb_tree_base_iterator& y) { return x._M_node != y._M_node; }
template <class V> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V> iterator;
    _Rb_tree_node_base* _M_header;                 // +0 (end node)
    unsigned int        _M_node_count;             // +4
    C                   _M_key_compare;            // +8
    _Rb_tree_node_base* _M_find(const K& key) const;   // 0x004B9370
    iterator find(const K& key) { return iterator(_M_find(key)); }
    iterator end() { return iterator(_M_header); }
};
// Same-TU stand-in body of the out-of-line lookup 0x004B9370 (read-only lower-bound walk; only its visibility matters).
#pragma auto_inline(off)
template <class K, class V, class KoV, class C, class A>
__declspec(noinline) _Rb_tree_node_base* _Rb_tree<K, V, KoV, C, A>::_M_find(const K& key) const
{
    _Rb_tree_node_base* y = _M_header;
    _Rb_tree_node_base* x = _M_header->_M_parent;
    while (x != 0) {
        if (!(((_Rb_tree_node<V>*)x)->_M_value_field.m_pData < key.m_pData)) {
            y = x;
            x = x->_M_left;
        } else
            x = x->_M_right;
    }
    if (y == _M_header || key.m_pData < ((_Rb_tree_node<V>*)y)->_M_value_field.m_pData)
        y = _M_header;
    return y;
}
#pragma auto_inline(on)
template <class K, class C, class A> class set {
public:
    typedef _Rb_tree<K, K, _Identity<K>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const K& key) { return _M_t.find(key); }
    iterator end() { return _M_t.end(); }
};
}

template <class T> class CArray {                  // retail CArray<T>: the 12-byte STLport vector
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void resize(unsigned int newSize, const T& x);     // 0x00486189
    T& operator[](unsigned int n) { return *(_M_start + n); }
};

class CEngineInternalPrimitiveMorphInfo {
public:
    unsigned char TextureDecals[0x10];             // +0x00 CVectorMap<pair<long,long>, CEngineTextureDecal>
    unsigned char CompositeSurfaces[0x10];         // +0x10 CVectorMap<long, CCompositeSurfaceInfo>
    typedef _STL::set<CCharString, _STL::less<CCharString>, _STL::allocator<CCharString> > HiddenMaterialSet;
    HiddenMaterialSet HiddenMaterialNames;         // +0x20
    CArray<unsigned char> HiddenMaterials;         // +0x2c
    void InitialiseHiddenMaterials(const C3DMesh2& mesh);
};

void CEngineInternalPrimitiveMorphInfo::InitialiseHiddenMaterials(const C3DMesh2& mesh)
{
    unsigned long count = mesh.MaterialCount;
    HiddenMaterials.resize(count, false);
    for (unsigned long i = 0; i < count; ++i) {
        bool hidden = false;
        HiddenMaterialSet::iterator it = HiddenMaterialNames.find(mesh.Materials[i].Name);
        if (it != HiddenMaterialNames.end())
            hidden = true;
        HiddenMaterials[i] = hidden;
    }
}