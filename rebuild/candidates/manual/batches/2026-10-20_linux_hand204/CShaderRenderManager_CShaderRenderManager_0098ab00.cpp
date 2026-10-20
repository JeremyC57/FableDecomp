// CShaderRenderManager::CShaderRenderManager() -- construct the shader render manager: empty constant-layout list, null layout, light pool, empty light/resource/constant arrays and an empty constant-name index
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098AB00, 232 bytes (+ int3 padding), vtable 0x0129A080 (RTTI CShaderRenderManager).
// Leaf: ??0CShaderRenderManager
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0, CVertexShaderConstantLayout::CVertexShaderConstantLayout()
//   @ 0x0098D490, CSimpleMemoryPool::CSimpleMemoryPool(unsigned long, unsigned long) @ 0x00A5C570,
//   CPixelShader::CPixelShader() @ 0x00A5EC00 (stores vtable 0x0129A054 = RTTI CPixelShader; the manifest label
//   "CBaseIntelligentPointer" is propagated), malloc @ 0x00BFEA0E (the 0x18-byte map header node).
// Shape: only the members with constructors are touched; the raw pointers/scalars between them (layout index,
//   lights, colours, device, matrices, flags) are set later by Initialise. Every STLport container is 12 bytes in
//   retail (PDB 16), so the members sit below the Ego_r.pdb offsets: VertexShaderConstantLayouts +0x0C,
//   NullConstantLayout +0x18, LightPool +0x178, LightGlobals +0x194, LightAttenuations +0x1A0, CurrentPixelShader
//   +0x1E0, ResourceManagers +0x3A8, PSConstNames +0x3B4 (array +0x3B4, map +0x3C0), PSConstTable +0x3CC,
//   UpdateFlags +0x3D8 (left alone), BoneMatrices +0x3DC. The vectors are the STLport `_Vector_base` +
//   `_STLP_alloc_proxy` model (zero words in the ebx zero register, the next member's `lea ecx` scheduled
//   before the last zero store); the map is the STLport red-black tree: the base nulls the header, allocates it,
//   the tree zeroes its node count, then `_M_empty_initialize` colours the header red and links
//   root/leftmost/rightmost, re-reading the header member after every store through it. The malloc's
//   `add esp,4` is delayed to the end of the constructor.
// No engine header exists for CShaderRenderManager.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

struct CBaseClass { virtual ~CBaseClass(); };
struct CBaseClassNonCopyable : CBaseClass {
    CBaseClassNonCopyable();                                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                        // 0x0099A300
};

template <class T> struct CAllocator {                      // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {     // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                     // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CVector : CVectorBase<T> {        // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
};

struct SRbTreeNode {                                        // _Rb_tree_node<pair<const CCharString, long> >, 0x18 bytes
    bool         _M_color;                                  // +0x00 (false = red)
    SRbTreeNode* _M_parent;                                 // +0x04
    SRbTreeNode* _M_left;                                   // +0x08
    SRbTreeNode* _M_right;                                  // +0x0c
    void*        m_keyData;                                 // +0x10 CCharString (string block pointer)
    long         m_index;                                   // +0x14
};
struct CRbTreeBase {                                        // STLport _Rb_tree_base
    CAllocProxy<SRbTreeNode> _M_header;
    CRbTreeBase(const CAllocator<SRbTreeNode>& alloc) : _M_header(alloc, 0)
    {
        _M_header.m_data = (SRbTreeNode*)malloc(sizeof(SRbTreeNode));
    }
};
struct CLessCharString { CLessCharString() { } };           // std::less<CCharString>: empty
struct CRbTree : CRbTreeBase {                              // STLport _Rb_tree (12 bytes)
    unsigned long   _M_node_count;
    CLessCharString _M_key_compare;
    CRbTree() : CRbTreeBase(CAllocator<SRbTreeNode>()), _M_node_count(0)
    {
        _M_header.m_data->_M_color = false;
        _M_header.m_data->_M_parent = 0;
        _M_header.m_data->_M_left = _M_header.m_data;
        _M_header.m_data->_M_right = _M_header.m_data;
    }
};

struct CIndexedArrayCharString {                            // CIndexedArray<CCharString>: retail 0x18 bytes
    CVector<void*> Array;                                   // +0x00 CArray<CCharString>
    CRbTree        IndexMap;                                // +0x0c map<CCharString, long>
};

struct CVertexShaderConstantLayout {                        // 0x14C bytes in retail (PDB 0x150)
    CVertexShaderConstantLayout();                          // 0x0098D490
    unsigned char _body[0x14c];
};
struct CSimpleMemoryPool {                                  // 0x1C bytes
    CSimpleMemoryPool(unsigned long count, unsigned long size);   // 0x00A5C570
    unsigned char _body[0x1c];
};
struct CPixelShader {                                       // 8 bytes: vtable 0x0129A054 + Data
    CPixelShader();                                         // 0x00A5EC00
    unsigned char _body[8];
};
struct C4DVector { float X, Y, Z, W; };

struct CShaderRenderManager : CBaseClassNonCopyable {      // vtable 0x0129A080, retail 0x3E8 bytes
    void*                       CurrentVSConstantLayout;    // +0x04
    long                        CurrentVSConstantLayoutIndex; // +0x08
    CVector<void*>              VertexShaderConstantLayouts; // +0x0c
    CVertexShaderConstantLayout NullConstantLayout;         // +0x18
    void*                       Lights;                     // +0x164
    C4DVector*                  LightBuffer;                // +0x168
    unsigned long               LightCount;                 // +0x16c
    unsigned long               AllocatedLights;            // +0x170
    unsigned long               AllocatedLightSize;         // +0x174
    CSimpleMemoryPool           LightPool;                  // +0x178
    CVector<C4DVector>          LightGlobals;               // +0x194
    CVector<C4DVector>          LightAttenuations;          // +0x1a0
    unsigned char               _colours_device[0x1e0 - 0x1ac]; // +0x1ac AmbientColour .. CurrentVertexShaderVertexDeclairation
    CPixelShader                CurrentPixelShader;         // +0x1e0
    unsigned char               _matrices[0x3a8 - 0x1e8];   // +0x1e8 WorldMatrixIsIdentity .. CycleTextureStage
    CVector<void*>              ResourceManagers;           // +0x3a8 CArray<CCountedPointer<CShaderResource> >
    CIndexedArrayCharString     PSConstNames;               // +0x3b4
    CVector<C4DVector>          PSConstTable;               // +0x3cc
    unsigned long               UpdateFlags;                // +0x3d8
    CVector<void*>              BoneMatrices;               // +0x3dc CArray<CPreTransposedBoneMatrix>

    CShaderRenderManager();
};

CShaderRenderManager::CShaderRenderManager()
    : LightPool(0, 0x1000)
{
}
