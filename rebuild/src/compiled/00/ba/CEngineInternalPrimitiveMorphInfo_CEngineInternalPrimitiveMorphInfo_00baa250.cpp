// CEngineInternalPrimitiveMorphInfo::CEngineInternalPrimitiveMorphInfo() -- two empty vector maps, an empty set of hidden material names (header node allocated), an empty array
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAA250, length 82 bytes (catalogued as the propagated label
// "CTCOpinionSource::CTCOpinionSource"; its caller @ 0x00BAA362 is the CEngineInternalPrimitiveMorphedStaticMesh constructor
// (vtable 0x012A51B4) building its Morph member at +0xac, and the destructor @ 0x00BAA2B0 tears the same four members down).
// Leaf: ??0CEngineInternalPrimitiveMorphInfo
// Callees: malloc @ 0x00BFEA0E (the set's 0x14-byte header node).
// PDB CEngineInternalPrimitiveMorphInfo (retail 0x38 bytes): TextureDecals (CVectorMap: vector +0x00, comparator +0x0c,
// flag +0x0d), CompositeSurfaces (CVectorMap +0x10), HiddenMaterialNames (STLport set<CCharString>: header +0x20,
// node count +0x24, comparator +0x28), HiddenMaterials (CArray<unsigned char>, three words) +0x2c.
// Shape: STLport `_Rb_tree_base(a) : _M_header(a, 0) { _M_header._M_data = allocate(1); }`, `_M_node_count(0)`, then
// `_M_empty_initialize()` (red colour, null root, leftmost = rightmost = header) re-reading the header member after
// every store through it. The vectors (and the CArray, itself an STLport vector) are modelled with their real
// `_Vector_base` + `_STLP_alloc_proxy` structure: with three plain zeroing words the cdecl `add esp,4` of the allocation
// is scheduled before the CArray stores instead of before its last one.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct CCharString { void* PStringData; };

struct SAllocator { SAllocator() {} };          // empty allocator (user constructor: no zero fill of the temporary)

template<class T> struct SAllocProxy : SAllocator { // STLport _STLP_alloc_proxy: the end-of-storage pointer over the allocator
    T* m_data;
    SAllocProxy(const SAllocator& a, T* data) : SAllocator(a), m_data(data) {}
};

template<class T> struct CVectorBase {              // STLport _Vector_base
    T*             m_pFirst;
    T*             m_pLast;
    SAllocProxy<T> m_end;
    CVectorBase(const SAllocator& a) : m_pFirst(0), m_pLast(0), m_end(a, 0) {}
};

template<class T> struct CRetailVector : CVectorBase<T> {
    CRetailVector() : CVectorBase<T>(SAllocator()) {}
};

template<class T> struct CVectorMap {
    CRetailVector<T> m_entries;                     // +0x00
    unsigned char    m_compare;                     // +0x0c CKeyPairCompareLess (empty)
    bool             m_dirty;                       // +0x0d
    CVectorMap() : m_dirty(false) {}
};

struct SRbNode {                                    // _Rb_tree_node<CCharString>
    unsigned char m_color;                          // +0x00
    SRbNode*      m_parent;                         // +0x04
    SRbNode*      m_left;                           // +0x08
    SRbNode*      m_right;                          // +0x0c
    CCharString   m_value;                          // +0x10
};

struct CRbTreeBase {
    SRbNode* m_header;                              // _M_header._M_data
    CRbTreeBase() : m_header(0) { m_header = (SRbNode*)malloc(sizeof(SRbNode)); }
};

struct CStringSet : CRbTreeBase {                   // set<CCharString>
    unsigned long m_nodeCount;
    unsigned char m_compare;                        // less<CCharString> (empty)
    CStringSet() : m_nodeCount(0)
    {
        m_header->m_color = 0;                      // _S_rb_tree_red
        m_header->m_parent = 0;
        m_header->m_left = m_header;
        m_header->m_right = m_header;
    }
};

template<class T> struct CArray : CRetailVector<T> {   // retail CArray: an STLport vector (three words)
    CArray() {}
};

struct CEngineTextureDecalEntry { long key[2]; long value[4]; };
struct CCompositeSurfaceEntry { long key; long value[6]; };

struct CEngineInternalPrimitiveMorphInfo {
    CVectorMap<CEngineTextureDecalEntry> TextureDecals;     // +0x00
    CVectorMap<CCompositeSurfaceEntry>   CompositeSurfaces; // +0x10
    CStringSet                           HiddenMaterialNames;   // +0x20
    CArray<unsigned char>                HiddenMaterials;   // +0x2c
    CEngineInternalPrimitiveMorphInfo();
};

CEngineInternalPrimitiveMorphInfo::CEngineInternalPrimitiveMorphInfo()
{
}