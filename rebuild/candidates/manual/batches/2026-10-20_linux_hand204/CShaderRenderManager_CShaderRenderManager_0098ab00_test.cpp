// Behaviour test for CShaderRenderManager::CShaderRenderManager() (0x0098AB00): member constructors run in order, vectors are empty, the pool gets (0, 0x1000) and the index map has an empty red header.
#include <cstdio>
#include <cstdlib>
#include <new>
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
static int g_order[8]; static int g_n = 0; static unsigned long g_poolArgs[2];
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { g_order[g_n++] = 1; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CVertexShaderConstantLayout::CVertexShaderConstantLayout() { g_order[g_n++] = 2; }
CSimpleMemoryPool::CSimpleMemoryPool(unsigned long count, unsigned long size) { g_order[g_n++] = 3; g_poolArgs[0] = count; g_poolArgs[1] = size; }
CPixelShader::CPixelShader() { g_order[g_n++] = 4; }
int main()
{
    void* raw = std::malloc(sizeof(CShaderRenderManager));
    unsigned char* b = (unsigned char*)raw;
    for (unsigned i = 0; i < sizeof(CShaderRenderManager); ++i) b[i] = 0xcd;
    CShaderRenderManager* m = new (raw) CShaderRenderManager;
    bool ok = g_n == 4 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3 && g_order[3] == 4;
    ok = ok && g_poolArgs[0] == 0 && g_poolArgs[1] == 0x1000;
    ok = ok && m->VertexShaderConstantLayouts._M_start == 0 && m->VertexShaderConstantLayouts._M_finish == 0 && m->VertexShaderConstantLayouts._M_end_of_storage.m_data == 0;
    ok = ok && m->LightGlobals._M_start == 0 && m->LightAttenuations._M_end_of_storage.m_data == 0;
    ok = ok && m->ResourceManagers._M_finish == 0 && m->PSConstNames.Array._M_start == 0 && m->PSConstTable._M_start == 0 && m->BoneMatrices._M_end_of_storage.m_data == 0;
    SRbTreeNode* h = m->PSConstNames.IndexMap._M_header.m_data;
    ok = ok && h != 0 && h->_M_color == false && h->_M_parent == 0 && h->_M_left == h && h->_M_right == h && m->PSConstNames.IndexMap._M_node_count == 0;
    unsigned long untouched; for (unsigned k = 0; k < sizeof untouched; ++k) ((unsigned char*)&untouched)[k] = 0xcd;
    ok = ok && m->UpdateFlags == untouched;     /* left for Initialise */
    std::free(h);
    std::free(raw);
    if (!ok) { std::printf("0098ab00_TEST FAIL\n"); return 1; }
    std::printf("0098ab00_TEST PASS\n");
    return 0;
}
