// CShaderRenderManager::~CShaderRenderManager() -- uninitialise, then destroy the members in reverse: bone matrices, pixel-shader constant table and names, the shader resource managers, the current pixel shader (releasing its data), the light attenuation / global arrays, the light pool, the null constant layout and the constant-layout list, then the CBaseClassNonCopyable base
// Leaf: ??1CShaderRenderManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098ABF0, 246 bytes, `ret`. Called from the deleting destructor 0x0098DE90 (CShaderRenderManager
//   vtable 0x0129A080 slot 0, rtti-vtables.tsv).
// Label correction: catalogued as ~CPatchTesselationEdgeStrip (propagated); the vtable store and the deleting destructor pin it.
// Callees: CShaderRenderManager::Uninitialise 0x0098A7A0 (PDB name; catalogued "CPlayerManager::PrepareForLoad"; also called
//   from 0x009C01D8), the CIndexedArray<CCharString> destructor 0x0098D9F0 (catalogued "FinishWithDynamicTextures",
//   propagated), the `__fastcall` range destroy of the resource managers 0x0098CEE0 (first ecx, last edx, the allocator by
//   const reference), CSimpleMemoryPool::ClearMemoryBlocks 0x00A5C600, the CVertexShaderConstantLayout destructor 0x0098D710,
//   ~CBaseClassNonCopyable 0x0099A300, free 0x00BFEA14. Vtables: CPixelShader 0x0129A054, CShaderBase 0x0129A030,
//   CSimpleMemoryPool 0x0129A064.
// Layout: Ego_r.pdb CShaderRenderManager (0x410) with 12-byte retail CArrays and a 0x14C-byte CVertexShaderConstantLayout:
//   VertexShaderConstantLayouts +0x0C, NullConstantLayout +0x18, LightPool +0x178, LightGlobals +0x194, LightAttenuations
//   +0x1A0, CurrentPixelShader +0x1E0 (Data +0x1E4), ResourceManagers +0x3A8, PSConstNames +0x3B4, PSConstTable +0x3CC,
//   BoneMatrices +0x3DC. engine/CSimpleMemoryPool.h models the vptr as a plain pointer, so the pool is modelled locally as a
//   CBaseClassNonCopyable with an inline destructor.
// Shape: POD arrays free their storage only; the resource-manager array destroys its range with an allocator temporary (top
//   byte of the `push ecx` word); the pixel shader's inline destructor releases its data through a local copy (`dec [ecx+8] ;
//   jne ; push 1 ; call [vtable]`) between its own and the CShaderBase vtable stores.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable();                   // 0x0099A300
};

template <class T> struct CPODArray {                   // CArray<T> of a trivial T, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CPODArray() { if (_M_start) free(_M_start); }
};

template <class T> struct allocator { allocator() {} };
template <class T> void __fastcall _Destroy_range(T* first, T* last, const allocator<T>& alloc);   // 0x0098CEE0 for this T

template <class T> struct CArray {                      // CArray<T> of a non-trivial T, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CArray()
    {
        _Destroy_range(_M_start, _M_finish, allocator<T>());
        if (_M_start)
            free(_M_start);
    }
};

class CShaderResource;
template <class T> struct CCountedPointer { T* Object; void* Info; };

class CVertexShaderConstantLayout {                     // 0x14C bytes in retail
public:
    ~CVertexShaderConstantLayout();                     // 0x0098D710
    unsigned char _body[0x14c];
};

class CMemoryBlock;
class CSimpleMemoryPool : public CBaseClassNonCopyable {   // 0x1C bytes, vtable 0x0129A064
public:
    CMemoryBlock*  FirstMemoryBlock;                    // +0x04
    unsigned long  Capacity;                            // +0x08
    unsigned long  GrowthGranularity;                   // +0x0c
    unsigned char  Root[0xc];                           // +0x10 CArea
    void ClearMemoryBlocks();                           // 0x00A5C600
    ~CSimpleMemoryPool() { ClearMemoryBlocks(); }
};

class CPixelShaderData {
public:
    virtual ~CPixelShaderData();
    long _04;
    long RefCount;                                      // +0x08
};
class CShaderBase {                                     // vtable 0x0129A030
public:
    virtual ~CShaderBase() {}
};
class CPixelShader : public CShaderBase {               // 8 bytes, vtable 0x0129A054
public:
    CPixelShaderData* Data;                             // +0x04
    ~CPixelShader()
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->RefCount == 0)
                delete data;
        }
    }
};

class CIndexedArray_CCharString {                       // CIndexedArray<CCharString>, 0x18 bytes in retail
public:
    ~CIndexedArray_CCharString();                       // 0x0098D9F0
    unsigned char _body[0x18];
};

class CShaderRenderManager : public CBaseClassNonCopyable {   // vtable 0x0129A080
public:
    void*                                   CurrentVSConstantLayout;      // +0x04
    long                                    CurrentVSConstantLayoutIndex; // +0x08
    CPODArray<const CVertexShaderConstantLayout*> VertexShaderConstantLayouts;   // +0x0c
    CVertexShaderConstantLayout             NullConstantLayout;           // +0x18
    void*                                   Lights;                       // +0x164
    void*                                   LightBuffer;                  // +0x168
    unsigned long                           LightCount;                   // +0x16c
    unsigned long                           AllocatedLights;              // +0x170
    unsigned long                           AllocatedLightSize;           // +0x174
    CSimpleMemoryPool                       LightPool;                    // +0x178
    CPODArray<float>                        LightGlobals;                 // +0x194 CArray<C4DVector>
    CPODArray<float>                        LightAttenuations;            // +0x1a0 CArray<C4DVector>
    unsigned char                           _pad1ac[0x34];                // +0x1ac colours, render manager, device, shaders
    CPixelShader                            CurrentPixelShader;           // +0x1e0
    unsigned char                           _pad1e8[0x1c0];               // +0x1e8 matrices, flags, cycle textures
    CArray<CCountedPointer<CShaderResource> > ResourceManagers;           // +0x3a8
    CIndexedArray_CCharString               PSConstNames;                 // +0x3b4
    CPODArray<float>                        PSConstTable;                 // +0x3cc CArray<C4DVector>
    unsigned long                           UpdateFlags;                  // +0x3d8
    CPODArray<float>                        BoneMatrices;                 // +0x3dc CArray<CPreTransposedBoneMatrix>
    void Uninitialise();                                // 0x0098A7A0
    virtual ~CShaderRenderManager();
};

CShaderRenderManager::~CShaderRenderManager()
{
    Uninitialise();
}