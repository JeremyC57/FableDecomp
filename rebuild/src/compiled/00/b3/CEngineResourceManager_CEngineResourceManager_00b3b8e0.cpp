// CEngineResourceManager::~CEngineResourceManager() -- unregister the async-loading callback, uninitialise the shaders, close the pixel shader bank and (outside retail-bank mode) the engine mesh bank, unregister the VS constant layouts, then destroy every member in reverse order
// Leaf: ??1CEngineResourceManager@@UAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

#include "engine/CBaseClassNonCopyable.h"              // ~CBaseClass 0x0099A2E0; ~CBaseClassNonCopyable 0x0099A300 (protected virtual, PDB MAE; naming wave 23)
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
class CTexture { public:                                // 8 bytes
    void*         Texture;
    unsigned long Flags;
    void Uninitialise();                                // 0x009F9F70
    ~CTexture() { Uninitialise(); }
};
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    ~CWideString();                                     // 0x0099B510
};
class CDataBankBase { public:                           // the bank vtable: Close is slot 5
    virtual ~CDataBankBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Close();
};
class CMeshDataBank : public CDataBankBase { public:
    unsigned char _pad04[0x45c];
    virtual ~CMeshDataBank();                           // 0x00A27350
};
class CShaderDataBank : public CDataBankBase { public:
    unsigned char _pad04[0x124];
    virtual ~CShaderDataBank();                         // 0x00A620D0
};
class CShaderResource { public:
    void CloseShaderBank();                             // 0x00A5D750
};
class CMovableResourceMemoryPool;
class CLoadingMemoryFailureHandler;
class CGraphicDataBank;
class CFontBank;
class CMemoryBlock;
class CArea { public:
    unsigned long Size;
    CArea*        NextFreeArea;
    long          Status;
};
class CSimpleMemoryPool : public CBaseClassNonCopyable { public:
    CMemoryBlock* FirstMemoryBlock;                     // +0x04
    unsigned long Capacity;                             // +0x08
    unsigned long GrowthGranularity;                    // +0x0c
    CArea         Root;                                 // +0x10
    virtual ~CSimpleMemoryPool() { ClearMemoryBlocks(); }
private:
    void ClearMemoryBlocks();                           // 0x00A5C600 (Ego_r.pdb private: ?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ; naming wave 25)
};
class C3DAnimationManager : public CBaseClassNonCopyable { public:
    CMeshDataBank*         MeshBank;                    // +0x04
    CArray<unsigned long>  BoneIndicesBuffer;           // +0x08
    CSimpleMemoryPool      MemoryPool;                  // +0x14
    CSimpleMemoryPool      MemoryPool2;                 // +0x30
};
class CShaderConstantRange : public CBaseClass { public:
    unsigned long Start;
    unsigned long Count;
};
class CVertexShaderConstantLayout { public:
    unsigned char _pad04[0x148];
    virtual ~CVertexShaderConstantLayout();             // 0x0098D710
};
class CEngineVSConstantLayoutBasic : public CVertexShaderConstantLayout { public: };
class CEngineVSConstantLayoutLights : public CVertexShaderConstantLayout { public: };
class CEngineVSConstantLayoutLightsBones : public CVertexShaderConstantLayout { public: };
class CEngineVSConstantLayoutParticles : public CVertexShaderConstantLayout { public: };
class CEngineVSConstantLayoutRepeatedMesh : public CVertexShaderConstantLayout { public:
    CShaderConstantRange LightingResults;               // +0x14c
    CShaderConstantRange MainLightLightingResults;      // +0x158
    CShaderConstantRange ObjectMatricies;               // +0x164
    CShaderConstantRange ObjectOffsets;                 // +0x170
};
class CGPUScratchBufferTextureManager { public:
    unsigned char _pad04[0x40];
    virtual ~CGPUScratchBufferTextureManager();         // 0x00F37570
};
struct SExternalTextureNode {                           // STLport 4.6 _Rb_tree_node
    long                  _M_color;
    SExternalTextureNode* _M_parent;
    SExternalTextureNode* _M_left;
    SExternalTextureNode* _M_right;
};
class CExternalTextureMap { public:                     // map<CTexture*, CExternalTextureAllocationInfo> (STLport 4.6 _Rb_tree)
    SExternalTextureNode* _M_header;
    unsigned long         _M_node_count;
    unsigned long         _M_key_compare;
    void _M_erase(SExternalTextureNode* x);             // 0x00B3E6E0
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
    ~CExternalTextureMap()
    {
        clear();
        if (_M_header)
            free(_M_header);
    }
};

class CEngineComponent { public:
    void**        GlobalPointerAddress;                 // +0x04
    bool          RenderEnabled;                        // +0x08
    unsigned char KeepAlive;                            // +0x09
    virtual ~CEngineComponent();                        // 0x00B59730
};
class CAsyncLoadingProcessCallback { public:
    virtual void ProcessLoading() = 0;
};
class CAsyncLoadingMemoryPool { public:
    static void __fastcall RemoveLoadingCallback(CAsyncLoadingProcessCallback* callback);   // 0x00A14930
};
class CShaderRenderManager { public:
    void UnregisterVertexShaderConstantLayouts();      // 0x0098B280
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14
class CBankFileManager { public:
    unsigned char _pad00[0x14];
    bool          m_retailMode;                         // +0x14
};
extern CBankFileManager g_BankFileManager;              // 0x013CA79C

class CEngineResourceManager : public CEngineComponent, public CAsyncLoadingProcessCallback {
public:
    CMeshDataBank*                                  MeshBanks[3];                   // +0x10
    CMeshDataBank*                                  MainMeshBank;                   // +0x1c
    CMeshDataBank                                   EditorMeshBank;                 // +0x20
    CMeshDataBank                                   EngineMeshBank;                 // +0x480
    CCountedPointer<CMovableResourceMemoryPool>     LoadingMemoryPool;              // +0x8e0
    CCountedPointer<CLoadingMemoryFailureHandler>   LoadingMemoryFailureHandler;    // +0x8e8
    CCountedPointer<CMovableResourceMemoryPool>     AnimationMemoryPool;            // +0x8f0
    CCountedPointer<CMovableResourceMemoryPool>     ClothMemoryPool;                // +0x8f8
    CCountedPointer<CMovableResourceMemoryPool>     MeshDataMemoryPool;             // +0x900
    C3DAnimationManager                             AnimationManager;               // +0x908
    CGraphicDataBank*                               GraphicBanks[1];                // +0x954
    CGraphicDataBank*                               MainGraphicBank;                // +0x958
    CFontBank*                                      EngineFont;                     // +0x95c
    CShaderDataBank                                 VShaderBanks[27];               // +0x960
    CShaderDataBank                                 ShaderBankPixel;                // +0x2898
    CCountedPointer<CShaderResource>                VShaderResources[27];           // +0x29c0
    CCountedPointer<CShaderResource>                ShaderResourcePixel;            // +0x2a98
    double                                          MaxLoadingTime;                 // +0x2aa0
    unsigned long                                   VertexBufferUsageFlags;         // +0x2aa8
    CTexture                                        StipleFadeTexture;              // +0x2aac
    CArray<unsigned long>                           HWInverseStipplePatterns;       // +0x2ab4
    CArray<CTexture>                                DitherTextureList;              // +0x2ac0
    CWideString                                     EngineCachePath;                // +0x2acc
    CWideString                                     WriteableEngineCachePath;       // +0x2ad0
    CTexture                                        ZeroRGBATexture;                // +0x2ad4
    CTexture                                        SatRGBATexture;                 // +0x2adc
    CEngineVSConstantLayoutBasic                    VSConstantLayoutBasic;          // +0x2ae4
    CEngineVSConstantLayoutLights                   VSConstantLayoutLights;         // +0x2c30
    CEngineVSConstantLayoutLightsBones              VSConstantLayoutLightsBones;    // +0x2d7c
    CEngineVSConstantLayoutRepeatedMesh             VSConstantLayoutRepeatedMesh;   // +0x2ec8
    CEngineVSConstantLayoutParticles                VSConstantLayoutParticles;      // +0x3044
    CGPUScratchBufferTextureManager                 GPUScratchBufferTextureManager; // +0x3190
    CExternalTextureMap                             ExternalTexturesAllocatedFromEnginePools; // +0x31d4

    virtual ~CEngineResourceManager();
    virtual void ProcessLoading();
private:
    void UninitialiseShaders();                         // 0x00B3B0C0 (private in FableWin.pdb: AAE)
};

CEngineResourceManager::~CEngineResourceManager()
{
    CAsyncLoadingMemoryPool::RemoveLoadingCallback(this);
    UninitialiseShaders();
    ShaderResourcePixel->CloseShaderBank();
    CShaderDataBank* pixelBank = &ShaderBankPixel;
    pixelBank->Close();
    if (!g_BankFileManager.m_retailMode) {
        CMeshDataBank* engineBank = &EngineMeshBank;
        engineBank->Close();
    }
    g_pShaderRenderManager->UnregisterVertexShaderConstantLayouts();
}