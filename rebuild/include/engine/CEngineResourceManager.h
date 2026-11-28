#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_EXTERNALTEXTURES
class CXboxBufferMemoryPool;
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER
#ifndef FABLE_ENGINE_CENGINERESOURCEMANAGER_H
#define FABLE_ENGINE_CENGINERESOURCEMANAGER_H
// Retail branch (pass 12 header wave, 0x00B3BE10): RTTI CEngineResourceManager : CEngineComponent, CAsyncLoadingProcessCallback
// (the dtor 0x00B3B8E0 stores 0x012A1B3C at +0 and 0x012A1B34 at +0x0C). Members in Ego_r TPI order; retail sizes from the
// dtor: VShaderBanks stride 0x128 (not the donor's 0x1A8 or the default's 0x13C), ExternalTexturesAllocatedFromEnginePools
// +0x31D4 (the map the dtor clears), sizeof 0x31E0 (the generated default below says 0x3410). Unused composites stay opaque.
// BuildAlphaStippleTexture: Ego_r / FableWin ?BuildAlphaStippleTexture@CEngineResourceManager@@AAEXJAAV?$CArray@E@@@Z (private).
#include "rebuild_abi.h"
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "engine/CEngineComponent.h"
#include "engine/CAsyncLoadingProcessCallback.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_STIPPLE
// Retail opt-in (chunk 2001, row 0x00B3C5D0 SetupAlphaStippleTexture): the stipple members typed (StipleFadeTexture +0x2AAC CTexture,
// HWInverseStipplePatterns +0x2AB4, DitherTextureList +0x2AC0 CArray<CTexture> (8-byte elements: `sar ecx,3` at 0x00B3C66F),
// EngineCachePath +0x2ACC CWideString (operator+ at 0x00B3C746)); same offsets and sizes as the opaque default members.
#ifndef FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE
#endif
#include "engine/CTexture.h"
#include "fable_string.h"
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_CTOR
// Retail opt-in (wfE7, row 0x00B3CB30 CEngineResourceManager::CEngineResourceManager): every member typed (Ego_r.pdb member types,
// retail offsets / sizes from the ctor and the landed dtor 0x00B3B8E0) so the ctor's member-construction sequence is compiler-emitted:
// CMeshDataBank() 0x00A27030 x2, C3DAnimationManager() 0x00AA0F60, CShaderDataBank() 0x00A62020 x28, CWideString() 0x0099AED0 x2,
// the five VS constant layout ctors, CGPUScratchBufferTextureManager() 0x00F37520, the inline STLport map ctor. Needs
// FABLE_RETAIL_CENGINERESOURCEMANAGER_STIPPLE / _EXTERNALTEXTURES, FABLE_RETAIL_CMESHDATABANK_BASES / _CTOR,
// FABLE_RETAIL_CSHADERDATABANK_CTOR, FABLE_RETAIL_CENGINEVSCONSTANTLAYOUT{BASIC,LIGHTS,LIGHTSBONES,REPEATEDMESH,PARTICLES},
// FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_CTOR, FABLE_RETAIL_STLP_MAP.
#include "engine/CMeshDataBank.h"
#include "engine/CShaderDataBank.h"
#include "engine/C3DAnimationManager.h"
#include "engine/CEngineVSConstantLayoutBasic.h"
#include "engine/CEngineVSConstantLayoutLights.h"
#include "engine/CEngineVSConstantLayoutLightsBones.h"
#include "engine/CEngineVSConstantLayoutRepeatedMesh.h"
#include "engine/CEngineVSConstantLayoutParticles.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "stlport/fable_map.h"
class CMovableResourceMemoryPool; class CLoadingMemoryFailureHandler; class CVertexShaderInput;
struct _D3DVERTEXELEMENT9;
enum EVertexType;
#endif
class CMeshDataBank; class CGraphicDataBank; class CFontBank;
class CShaderResource; class CGPUScratchBufferTextureManager;
enum EShaderBankNo;                                          // MS forward enums (chunk 723; no enumerator name recovered)
enum EIndexBufferPool;
enum EVertexBufferPool;
enum EGraphicBankNo;                                         // MS forward enum (no enumerator name recovered)
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKMESHBANK
#ifdef FABLE_RETAIL_EMESHBANKNO_ENUMERATORS
// Opt-in (egod e03, row 0x00B81820): Ego_d.pdb LF_ENUM 0x66775 EMeshBankNo enumerators (MAIN 0, EDITOR 1, ENGINE 2, MAX_MESH_BANKS 3).
enum EMeshBankNo { MAIN_MESH_BANK_NO = 0, EDITOR_MESH_BANK_NO = 1, ENGINE_MESH_BANK_NO = 2, MAX_MESH_BANKS = 3 };
#else
enum EMeshBankNo;                                            // MS forward enum (no enumerator name recovered)
#endif
#endif
class CEngineInit;
class CEngineMemoryInit;
class CEngineResourceManager : public CEngineComponent, public CAsyncLoadingProcessCallback {
public:
#ifdef FABLE_RETAIL_ENGINE_LOADSCENE_PASS30
    void ChangeScene();
#endif

#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_TERRAIN_ACCESSORS_PASS30
    const class CGraphicInfoFrame* GetTerrainTexture(long index);
    const CGraphicDataBank* PeekTerrainBank() const;
#elif defined(FABLE_RETAIL_CENGINERESOURCEMANAGER_SEAMIDGROUND)
    // Original PDB member, donor02C85C70; disabled original water texture path.
    const class CGraphicInfoFrame* GetTerrainTexture(long index);
    const CGraphicDataBank* PeekTerrainBank() const;
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_EXTERNALTEXTURES
    // Nested opt-in (agentNF24, Part 2 rows 0x00B3E6E0 / 0x00B3E750 / 0x00B3E8D0 / 0x00B3E990 / 0x00B3E9D0, the
    // map<CTexture*, CExternalTextureAllocationInfo> ExternalTexturesAllocatedFromEnginePools members): Ego_r.pdb struct (U key, FableWin
    // ??0?$map@PAVCTexture@@UCExternalTextureAllocationInfo@CEngineResourceManager@@...) CExternalTextureAllocationInfo, 8 bytes:
    // Handle +0 (long), Pool +4 (CXboxBufferMemoryPool*). A type only: no layout change.
    struct CExternalTextureAllocationInfo {
        long Handle;                                                     // +0x00
        CXboxBufferMemoryPool* Pool;                                     // +0x04
    };
#endif
    // Chunk 731: ??0CEngineResourceManager@@QAE@ABVCEngineInit@@ABVCEngineMemoryInit@@@Z 0x00B3CB30 (Ego_r.pdb / FableWin.pdb;
    // CEngine::Initialise 0x00B26360 news 0x31E0 bytes and passes the init and its CDefaultEngineMemoryInit local).
    CEngineResourceManager(const CEngineInit& init, const CEngineMemoryInit& memory_init);
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_SETUP_EDITOR_ANIMATION
    // FableWin SetupEditorAnimation and the retail mesh-bank slot +0x14.
    unsigned long SetupEditorAnimation(const CWideString& filename);
    static CWideString __fastcall ConvertArtResourcePathToFullPath(const CWideString& filename);
#endif

    virtual ~CEngineResourceManager();
    virtual void ProcessLoading();
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_PROCESSLOADING_UNTIL
    // Opt-in (NF53, round 7, row 0x00B3B7F0): the non-virtual overload ?ProcessLoading@CEngineResourceManager@@QAEX_NN@Z (Ego_r.pdb 0x009D9E50 /
    // FableWin.pdb 0x02DD3C30; retail 0x00B3B7F0, `ret 0Ch`). Declaration only: no layout change.
    void ProcessLoading(bool flag, double until);                       // 0x00B3B7F0
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_FRAMEEND
    // NF56 opt-in (row 0x00B3B870): the non-virtual ?FrameEnd@CEngineResourceManager@@QAEXXZ (Ego_r.pdb 0x009D9ED0 / FableWin.pdb 0x02DD3B60;
    // the virtual CEngineComponent override is the other overload). Declaration only: no layout change.
    void FrameEnd();                                                     // 0x00B3B870
#endif
    virtual unsigned long PeekSceneFilterFlags() const;
    // Chunk 723 (rows 0x00BC14E0 / 0x00B71880; folded into this branch at landing): FableWin.pdb inlines
    // ?GetVShaderResource@CEngineResourceManager@@QBEPBVCShaderResource@@W4EShaderBankNo@@@Z (0x00BC14E0 reads bank 21 at +0x2A68),
    // ?GetPShaderResource@CEngineResourceManager@@QBEPBVCShaderResource@@XZ (+0x2A98) and the scratch-manager member read
    // (+0x3190: 0x00B71880 / 0x00B6D630 / 0x00B23F60); Ego_r.pdb / FableWin.pdb out-of-line GetIndexBufferMemPool 0x00B3B0A0,
    // GetIndexBufferUsage 0x00B3B060, GetVertexBufferUsage 0x00B3B030.
    EIndexBufferPool GetIndexBufferMemPool();                            // 0x00B3B0A0
    unsigned long GetIndexBufferUsage();                                 // 0x00B3B060
    unsigned long GetVertexBufferUsage();                                // 0x00B3B030
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_ALLOCATEEXTERNALTEXTUREFROMRESOURCEPOOLS
    // NF152 opt-in (q4595, row 0x00B3B1F0): Ego_r.pdb 0x009D9860 / FableWin.pdb 0x02DD6090
    // ?AllocateExternalTextureFromResourcePools@CEngineResourceManager@@QAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z (public; the target
    // of CEngine::AllocateTextureFromEngineResourcePools 0x00B23F40, `ret 10h`). Declaration only: no layout change.
    class CTexture* AllocateExternalTextureFromResourcePools(unsigned long id, const class C2DExtentsI& size, long levels,
                                                             const class CPixelFormat& format);   // 0x00B3B1F0
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_FREEEXTERNALTEXTUREFROMRESOURCEPOOLS
    // NF152 opt-in (q4596, row 0x00B3B5B0): Ego_r.pdb 0x009D9C10 / FableWin.pdb 0x02DD6150
    // ?FreeExternalTextureFromResourcePools@CEngineResourceManager@@QAEXPAVCTexture@@@Z (public; the target of
    // CEngine::FreeTextureFromEngineResourcePools 0x00B23F50, `ret 4`). Declaration only: no layout change.
    void FreeExternalTextureFromResourcePools(class CTexture* texture);                          // 0x00B3B5B0
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_VBPOOL
    // Retail opt-in (chunk 2005, row 0x00B51570): Ego_r.pdb ?GetVertexBufferMemPool@CEngineResourceManager@@QAE?AW4EVertexBufferPool@@XZ,
    // out of line at 0x00B3B090 (no arguments; `mov ecx,[0x01436E98] ; call` ahead of GetVertexBufferUsage at 0x00B51583).
    EVertexBufferPool GetVertexBufferMemPool();                          // 0x00B3B090
#endif
    const CShaderResource* GetVShaderResource(EShaderBankNo bank) const { return VShaderResources[bank].Get(); }
    const CShaderResource* GetPShaderResource() const { return ShaderResourcePixel.Get(); }
    // Chunk 742: ?PeekGraphicBank@CEngineResourceManager@@QBEPBVCGraphicDataBank@@W4EGraphicBankNo@@@Z (FableWin.pdb 0x02C85CC0;
    // inline in retail: the particle line / sprite-trail render passes 0x00BC3F30 / 0x00BBE090 read GraphicBanks[0] at +0x954).
    const CGraphicDataBank* PeekGraphicBank(EGraphicBankNo bank) const { return GraphicBanks[bank]; }
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKMESHBANK
    // Overlay opt-in (chunk 2011, rows 0x00BF12A0 / 0x00BF18B0): FableWin.pdb ?PeekMeshBank@CEngineResourceManager@@QBEPBVCMeshDataBank@@W4EMeshBankNo@@@Z
    // (0x02D05E30; no Ego_r.pdb or Fable.exe copy: inline, MeshBanks[bank] at +0x10 read in place, 0x00BF12ED `mov ecx,[ecx+10h]`).
    const CMeshDataBank* PeekMeshBank(EMeshBankNo bank) const { return MeshBanks[bank]; }
#endif
#if defined(FABLE_RETAIL_CENGINERESOURCEMANAGER_GETMESHBANK) && defined(FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKMESHBANK)
    // Retail opt-in (wfE7, row 0x00B33B50): Ego_r.pdb ?GetMeshBank@CEngineResourceManager@@QAEPAVCMeshDataBank@@W4EMeshBankNo@@@Z, inline in
    // retail (the CEnginePrimitiveRenderer ctor reads MeshBanks[0] at +0x10 in place and calls SetDP3BumpEnable on it, 0x00B346FB).
    CMeshDataBank* GetMeshBank(EMeshBankNo bank) { return MeshBanks[bank]; }
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
    // Retail opt-in (chunk 848, row 0x00B2D9F0): Ego_r.pdb ?GetGraphicBank@CEngineResourceManager@@QAEPAVCGraphicDataBank@@W4EGraphicBankNo@@@Z,
    // inline in retail (UpdateDiskCache 0x00B2DA07 / 0x00B2DBA2 read GraphicBanks[0] at +0x954 and write through it).
    CGraphicDataBank* GetGraphicBank(EGraphicBankNo bank) { return GraphicBanks[bank]; }
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKVSCONSTANTLAYOUTREPEATEDMESH
    // Opt-in (wfA5, row 0x00B9AC30): FableWin.pdb ?PeekVSConstantLayoutRepeatedMesh@CEngineResourceManager@@QAEABVCEngineVSConstantLayoutRepeatedMesh@@XZ,
    // inline in retail (ObjectMatricies.Count read in place at +0x3034 = VSConstantLayoutRepeatedMesh +0x2EC8 + 0x16C, 0x00B9B969). The member
    // stays opaque, as in the LIGHTSBONES opt-in; the includer supplies engine/CEngineVSConstantLayoutRepeatedMesh.h (retail branch).
    const class CEngineVSConstantLayoutRepeatedMesh& PeekVSConstantLayoutRepeatedMesh()
    {
        return *reinterpret_cast<const class CEngineVSConstantLayoutRepeatedMesh*>(VSConstantLayoutRepeatedMesh);
    }
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKVSCONSTANTLAYOUTLIGHTSBONES
    // Retail opt-in (chunk F2, row 0x00BCFB00): FableWin.pdb ?PeekVSConstantLayoutLightsBones@CEngineResourceManager@@QAEABVCEngineVSConstantLayoutLightsBones@@XZ,
    // inline in retail (the row reads the layout's BoneMatrices at +0x2E50 = VSConstantLayoutLightsBones +0x2D7C + 0xD4 in place).
    // The member stays opaque; the includer supplies engine/CEngineVSConstantLayoutLightsBones.h (retail branch).
    const class CEngineVSConstantLayoutLightsBones& PeekVSConstantLayoutLightsBones()
    {
        return *reinterpret_cast<const class CEngineVSConstantLayoutLightsBones*>(VSConstantLayoutLightsBones);
    }
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_GETSTIPLEFADETEXTURE
    // Retail opt-in (row 0x00BBCC70): FableWin.pdb ?GetStipleFadeTexture@CEngineResourceManager@@QAEAAVCTexture@@XZ, inline in
    // retail (SetupPixelShader takes the member's address: `mov ecx,[GPResourceManager] ; add ecx,2AACh`). The member stays opaque.
    class CTexture& GetStipleFadeTexture() { return *(class CTexture*)StipleFadeTexture; }
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_CACHEPATHS
    // Retail opt-in (chunk 2018, row 0x00B2CB40): Ego_r.pdb / FableWin.pdb ?PeekEngineCachePath@CEngineResourceManager@@QAEABVCWideString@@XZ /
    // ?PeekWriteableEngineCachePath@..@@QAEABVCWideString@@XZ, inline in retail (0x00B2CD98 `add edx,2AD0h` = WriteableEngineCachePath,
    // passed to the CWideString operator+ 0x0099BF30); the two members typed CWideString (same offsets +0x2ACC / +0x2AD0).
    const CWideString& PeekEngineCachePath() { return EngineCachePath; }
    const CWideString& PeekWriteableEngineCachePath() { return WriteableEngineCachePath; }
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_CTOR
    CGPUScratchBufferTextureManager& GetGPUScratchBufferTextureManager() { return GPUScratchBufferTextureManager; }   // (typed member)
#else
    CGPUScratchBufferTextureManager& GetGPUScratchBufferTextureManager()
    {
        return *(CGPUScratchBufferTextureManager*)GPUScratchBufferTextureManager;
    }
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_GETLOADINGMEMORYPOOL
    // Opt-in (NF46 round 7, rows 0x00B25C80 / 0x00B42750 ..): FableWin.pdb ?GetLoadingMemoryPool@CEngineResourceManager@@QAE?AV?$CCountedPointer@VCMovableResourceMemoryPool@@@@XZ
    // (0x02CA1F60; inline in retail: the +0x8E0 counted pointer copied by value, add-ref then release). The member stays opaque outside _CTOR.
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_CTOR
    CCountedPointer<CMovableResourceMemoryPool> GetLoadingMemoryPool() { return LoadingMemoryPool; }
#else
    CCountedPointer<class CMovableResourceMemoryPool> GetLoadingMemoryPool()
    {
        return *reinterpret_cast<CCountedPointer<class CMovableResourceMemoryPool>*>(LoadingMemoryPool);
    }
#endif
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKANIMATIONMEMORYPOOL
    // Opt-in (NF46 round 7, row 0x00B25CC0): FableWin.pdb ?PeekAnimationMemoryPool@CEngineResourceManager@@QBE?AV?$CCountedPointer@VCMovableResourceMemoryPool@@@@XZ
    // (0x02CA1FF0; inline in retail: the +0x8F0 counted pointer copied by value). The member stays opaque outside _CTOR.
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_CTOR
    CCountedPointer<CMovableResourceMemoryPool> PeekAnimationMemoryPool() const { return AnimationMemoryPool; }
#else
    CCountedPointer<class CMovableResourceMemoryPool> PeekAnimationMemoryPool() const
    {
        return *reinterpret_cast<const CCountedPointer<class CMovableResourceMemoryPool>*>(AnimationMemoryPool);
    }
#endif
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_ZERORGBATEXTURE
    // Retail opt-in (chunk 2048, row 0x00B64FA0): FableWin.pdb ?GetZeroRGBATexture@CEngineResourceManager@@QAEPAVCTexture@@XZ (its COMDAT is
    // listed right after RenderClouds) on the Ego_r.pdb member ZeroRGBATexture (+0x2AD4, the landed ~CEngineResourceManager 0x00B3B8E0);
    // inline in retail (RenderClouds 0x00B659C9..0x00B659DA: `add edx,2AD4h`).
    class CTexture* GetZeroRGBATexture() { return (class CTexture*)ZeroRGBATexture; }
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_SATRGBATEXTURE
    // Retail opt-in (wfB3, rows 0x00BA2350 / 0x00BA9A70): FableWin.pdb ?GetSatRGBATexture@CEngineResourceManager@@QAEPAVCTexture@@XZ
    // (0x02E73E10, listed right after Get2DAlphaMaskTexture 0x02E73D60) on the Ego_r.pdb member SatRGBATexture (retail +0x2ADC, next to
    // ZeroRGBATexture +0x2AD4; the landed Get2DAlphaMaskTexture 0x00BA9A70 returns it). Inline in retail.
    class CTexture* GetSatRGBATexture() { return (class CTexture*)SatRGBATexture; }
#endif
private:
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_ONRELEASE
    // Chunk 767 (row 0x00B3AFE0, vtable 0x012A1B3C slot 11): Ego_r.pdb ?OnReleaseDefaultPoolResources@CEngineResourceManager@@EAEXXZ
    // (a private override of the CEngineComponent virtual), `add ecx,3190h ; jmp 0x00F37170` = the scratch manager's BeginShutdown.
    virtual void OnReleaseDefaultPoolResources();                        // 0x00B3AFE0
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_ONRESTORE
    // NF172 opt-in (row 0x00B3AFF0, vtable 0x012A1B3C slot 12): Ego_r.pdb 0x009D9640 / FableWin.pdb 0x02DD2A20
    // ?OnRestoreDefaultPoolResources@CEngineResourceManager@@EAE_NXZ (private override of CEngineComponent::OnRestoreDefaultPoolResources),
    // `add ecx,3190h ; jmp 0x00F36D30` = the scratch manager's EndShutdown. Declaration only.
    virtual bool OnRestoreDefaultPoolResources();                        // 0x00B3AFF0
#endif
    void BuildAlphaStippleTexture(long,CArray<unsigned char>&);
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_STIPPLE
    // Ego_r.pdb / FableWin.pdb ?BuildHWStipplePatterns@CEngineResourceManager@@AAEXABV?$CArray@E@@@Z (0x00B3C4B0, landed) and
    // ?SetupAlphaStippleTexture@CEngineResourceManager@@AAEX_N@Z (0x00B3C5D0), both private; the PDBs list them in this order.
    void BuildHWStipplePatterns(const CArray<unsigned char>&);
    void SetupAlphaStippleTexture(bool);
#endif
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_CTOR
    // Ego_r.pdb / FableWin.pdb ?AllocatePools@CEngineResourceManager@@AAEX_NABVCEngineMemoryInit@@@Z (0x00B3B240) and the two
    // ?SetupVertexShaderResource@CEngineResourceManager@@AAEXW4EShaderBankNo@@ABVCCharString@@... overloads (0x00B3B5D0 / 0x00B3B6D0),
    // all private, in the PDB order.
    void AllocatePools(bool, const CEngineMemoryInit&);                  // 0x00B3B240
    void SetupVertexShaderResource(EShaderBankNo, const CCharString&, const _D3DVERTEXELEMENT9*, EVertexType);   // 0x00B3B5D0
    void SetupVertexShaderResource(EShaderBankNo, const CCharString&, const CVertexShaderInput&);              // 0x00B3B6D0
    CMeshDataBank* MeshBanks[3];                             // +0x10
    CMeshDataBank* MainMeshBank;                             // +0x1C
    CMeshDataBank EditorMeshBank;                            // +0x20
    CMeshDataBank EngineMeshBank;                            // +0x480
    CCountedPointer<CMovableResourceMemoryPool> LoadingMemoryPool;          // +0x8E0
    CCountedPointer<CLoadingMemoryFailureHandler> LoadingMemoryFailureHandler;   // +0x8E8
    CCountedPointer<CMovableResourceMemoryPool> AnimationMemoryPool;        // +0x8F0
    CCountedPointer<CMovableResourceMemoryPool> ClothMemoryPool;            // +0x8F8
    CCountedPointer<CMovableResourceMemoryPool> MeshDataMemoryPool;         // +0x900
    C3DAnimationManager AnimationManager;                    // +0x908
    CGraphicDataBank* GraphicBanks[1];                       // +0x954
    CGraphicDataBank* MainGraphicBank;                       // +0x958
    const CFontBank* EngineFont;                             // +0x95C
    CShaderDataBank VShaderBanks[27];                        // +0x960
    CShaderDataBank ShaderBankPixel;                         // +0x2898
    CCountedPointer<CShaderResource> VShaderResources[27];  // +0x29C0
    CCountedPointer<CShaderResource> ShaderResourcePixel;    // +0x2A98
    double MaxLoadingTime;                                   // +0x2AA0
    unsigned long VertexBufferUsageFlags;                    // +0x2AA8
    CTexture StipleFadeTexture;                              // +0x2AAC
    CArray<unsigned long> HWInverseStipplePatterns;          // +0x2AB4
    CArray<CTexture> DitherTextureList;                      // +0x2AC0
    CWideString EngineCachePath;                             // +0x2ACC
    CWideString WriteableEngineCachePath;                    // +0x2AD0
    CTexture ZeroRGBATexture;                                // +0x2AD4
    CTexture SatRGBATexture;                                 // +0x2ADC
    CEngineVSConstantLayoutBasic VSConstantLayoutBasic;      // +0x2AE4
    CEngineVSConstantLayoutLights VSConstantLayoutLights;    // +0x2C30
    CEngineVSConstantLayoutLightsBones VSConstantLayoutLightsBones;     // +0x2D7C
    CEngineVSConstantLayoutRepeatedMesh VSConstantLayoutRepeatedMesh;   // +0x2EC8
    CEngineVSConstantLayoutParticles VSConstantLayoutParticles;         // +0x3044
    CGPUScratchBufferTextureManager GPUScratchBufferTextureManager;     // +0x3190
    std::map<CTexture*, CExternalTextureAllocationInfo> ExternalTexturesAllocatedFromEnginePools;   // +0x31D4
#else
    CMeshDataBank* MeshBanks[3];                             // +0x10
    CMeshDataBank* MainMeshBank;                             // +0x1C
    unsigned char EditorMeshBank[0x460];                     // +0x20 CMeshDataBank
    unsigned char EngineMeshBank[0x460];                     // +0x480 CMeshDataBank
    unsigned char LoadingMemoryPool[8];                      // CCountedPointer<CMovableResourceMemoryPool>
    unsigned char LoadingMemoryFailureHandler[8];            // CCountedPointer<CLoadingMemoryFailureHandler>
    unsigned char AnimationMemoryPool[8],ClothMemoryPool[8],MeshDataMemoryPool[8];   // CCountedPointer<CMovableResourceMemoryPool>
    unsigned char AnimationManager[0x4c];                    // C3DAnimationManager
    CGraphicDataBank* GraphicBanks[1];
    CGraphicDataBank* MainGraphicBank;
    const CFontBank* EngineFont;
    unsigned char VShaderBanks[27*0x128],ShaderBankPixel[0x128];   // CShaderDataBank
    CCountedPointer<CShaderResource> VShaderResources[27];  // +0x29C0 (chunk 723: 0x00BC14E0 bank 21 at +0x2A68)
    CCountedPointer<CShaderResource> ShaderResourcePixel;    // +0x2A98
    double MaxLoadingTime;
    unsigned long VertexBufferUsageFlags;
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_STIPPLE
    CTexture StipleFadeTexture;                              // +0x2AAC
    CArray<unsigned long> HWInverseStipplePatterns;          // +0x2AB4
    CArray<CTexture> DitherTextureList;                      // +0x2AC0
    CWideString EngineCachePath;                             // +0x2ACC
    unsigned char WriteableEngineCachePath[4];               // CWideString
#else
    unsigned char StipleFadeTexture[8];                      // CTexture
    CArray<unsigned long> HWInverseStipplePatterns;
    unsigned char DitherTextureList[12];                     // CArray<CTexture>
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_CACHEPATHS
    CWideString EngineCachePath,WriteableEngineCachePath;    // +0x2ACC / +0x2AD0
#else
    unsigned char EngineCachePath[4],WriteableEngineCachePath[4];  // CWideString
#endif
#endif
    unsigned char ZeroRGBATexture[8],SatRGBATexture[8];      // CTexture
    unsigned char VSConstantLayoutBasic[0x14c];              // CEngineVSConstantLayoutBasic
    unsigned char VSConstantLayoutLights[0x14c];             // CEngineVSConstantLayoutLights
    unsigned char VSConstantLayoutLightsBones[0x14c];        // CEngineVSConstantLayoutLightsBones
    unsigned char VSConstantLayoutRepeatedMesh[0x17c];       // CEngineVSConstantLayoutRepeatedMesh
    unsigned char VSConstantLayoutParticles[0x14c];          // CEngineVSConstantLayoutParticles
    unsigned char GPUScratchBufferTextureManager[0x44];      // CGPUScratchBufferTextureManager
    unsigned char ExternalTexturesAllocatedFromEnginePools[12];   // +0x31D4 map<CTexture*,CExternalTextureAllocationInfo>
#endif
};
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_TERRAIN_ACCESSORS_PASS30
// FableWin 0x02C85C70 and 0x02C85CA0 are the two typed forwarders.
#include "engine/EGraphicBankNo.h"
#include "engine/CGraphicInfo.h"
FABLE_FORCEINLINE const CGraphicDataBank* CEngineResourceManager::PeekTerrainBank() const
{
    return PeekGraphicBank(MAIN_GRAPHIC_BANK_NO);
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CEngineResourceManager::GetTerrainTexture(long index)
{
    return PeekTerrainBank()->PeekGraphicInfoFrame(index, 0);
}
#endif
FABLE_STATIC_ASSERT_AT(172, sizeof(CEngineResourceManager)==0x31e0);
#endif // FABLE_ENGINE_CENGINERESOURCEMANAGER_H
#else // generated flattened default (unchanged)
#ifndef FABLE_ENGINE_CENGINERESOURCEMANAGER_H
#define FABLE_ENGINE_CENGINERESOURCEMANAGER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineResourceManager = 0x4150.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineResourceManager = 0x3410.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCPPointerInfo;
class CFontBank;
class CGraphicDataBank;
class CLoadingMemoryFailureHandler;
class CMeshDataBank;
class CMovableResourceMemoryPool;
class CWideStringData;
struct IDirect3DTexture9;

#pragma pack(push, 1)
class CEngineResourceManager {
public:
#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER_EXTERNALTEXTURES
    // Nested opt-in (agentNF24, Part 2 rows 0x00B3E6E0 / 0x00B3E750 / 0x00B3E8D0 / 0x00B3E990 / 0x00B3E9D0, the
    // map<CTexture*, CExternalTextureAllocationInfo> ExternalTexturesAllocatedFromEnginePools members): Ego_r.pdb struct (U key, FableWin
    // ??0?$map@PAVCTexture@@UCExternalTextureAllocationInfo@CEngineResourceManager@@...) CExternalTextureAllocationInfo, 8 bytes:
    // Handle +0 (long), Pool +4 (CXboxBufferMemoryPool*). A type only: no layout change.
    struct CExternalTextureAllocationInfo {
        long Handle;                                                     // +0x00
        CXboxBufferMemoryPool* Pool;                                     // +0x04
    };
#endif
    void*                         __vftable;                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char                 _pad_0x04[0x4];            // +0x04
    unsigned char                 _pad_0x08[0x4];            // +0x08
    unsigned char                 _pad_0x0c[0x4];            // +0x0c
    CMeshDataBank*                MeshBanks[3];              // +0x10
    CMeshDataBank*                MainMeshBank;              // +0x1c
    unsigned char                 EditorMeshBank[0x460];     // +0x20 CMeshDataBank
    unsigned char                 EngineMeshBank[0x460];     // +0x480 CMeshDataBank
    CMovableResourceMemoryPool*   LoadingMemoryPool_Data;    // +0x8e0 CCountedPointer<CMovableResourceMemoryPool>::Data
    CCPPointerInfo*               LoadingMemoryPool_Info;    // +0x8e4 CCountedPointer<CMovableResourceMemoryPool>::Info
    CLoadingMemoryFailureHandler* LoadingMemoryFailureHandler_Data; // +0x8e8 CCountedPointer<CLoadingMemoryFailureHandler>::Data
    CCPPointerInfo*               LoadingMemoryFailureHandler_Info; // +0x8ec CCountedPointer<CLoadingMemoryFailureHandler>::Info
    CMovableResourceMemoryPool*   AnimationMemoryPool_Data;  // +0x8f0 CCountedPointer<CMovableResourceMemoryPool>::Data
    CCPPointerInfo*               AnimationMemoryPool_Info;  // +0x8f4 CCountedPointer<CMovableResourceMemoryPool>::Info
    CMovableResourceMemoryPool*   ClothMemoryPool_Data;      // +0x8f8 CCountedPointer<CMovableResourceMemoryPool>::Data
    CCPPointerInfo*               ClothMemoryPool_Info;      // +0x8fc CCountedPointer<CMovableResourceMemoryPool>::Info
    CMovableResourceMemoryPool*   MeshDataMemoryPool_Data;   // +0x900 CCountedPointer<CMovableResourceMemoryPool>::Data
    CCPPointerInfo*               MeshDataMemoryPool_Info;   // +0x904 CCountedPointer<CMovableResourceMemoryPool>::Info
    unsigned char                 AnimationManager[0x4c];    // +0x908 C3DAnimationManager
    CGraphicDataBank*             GraphicBanks[1];           // +0x954
    CGraphicDataBank*             MainGraphicBank;           // +0x958
    CFontBank*                    EngineFont;                // +0x95c
    unsigned char                 VShaderBanks[0x2154];      // +0x960 CShaderDataBank[27]
    unsigned char                 ShaderBankPixel[0x13c];    // +0x2ab4 CShaderDataBank
    unsigned char                 VShaderResources[0xd8];    // +0x2bf0 CCountedPointer<CShaderResource>[27]
    unsigned char                 ShaderResourcePixel[0x8];  // +0x2cc8 CCountedPointer<CShaderResource>
    double                        MaxLoadingTime;            // +0x2cd0
    unsigned long                 VertexBufferUsageFlags;    // +0x2cd8
    unsigned char                 StipleFadeTexture[0x8];    // +0x2cdc CTexture
    unsigned char                 HWInverseStipplePatterns[0xc];  // +0x2ce4 CArray<unsigned_long>
    unsigned char                 DitherTextureList[0xc];    // +0x2cf0 CArray<CTexture>
    CWideStringData*              EngineCachePath_PStringData; // +0x2cfc CWideString::PStringData
    CWideStringData*              WriteableEngineCachePath_PStringData; // +0x2d00 CWideString::PStringData
    unsigned char                 ZeroRGBATexture[0x8];      // +0x2d04 CTexture
    unsigned char                 SatRGBATexture[0x8];       // +0x2d0c CTexture
    unsigned char                 VSConstantLayoutBasic[0x14c]; // +0x2d14 CEngineVSConstantLayoutBasic
    unsigned char                 VSConstantLayoutLights[0x14c]; // +0x2e60 CEngineVSConstantLayoutLights
    unsigned char                 VSConstantLayoutLightsBones[0x14c]; // +0x2fac CEngineVSConstantLayoutLightsBones
    unsigned char                 VSConstantLayoutRepeatedMesh[0x17c]; // +0x30f8 CEngineVSConstantLayoutRepeatedMesh
    unsigned char                 VSConstantLayoutParticles[0x14c]; // +0x3274 CEngineVSConstantLayoutParticles
    unsigned char                 GPUScratchBufferTextureManager[0x44]; // +0x33c0 CGPUScratchBufferTextureManager
    unsigned char                 ExternalTexturesAllocatedFromEnginePools[0xc];  // +0x3404 map<CTexture_*,CEngineResourceManager::CExternalTextureAllocationInfo,std::less<CTexture_*>,std::allocator<std::pair<CTexture_*_const,CEngineResourceManager::CExternalTextureAllocationInfo>_>_>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(253, sizeof(CEngineResourceManager) == 0x3410);
FABLE_STATIC_ASSERT_AT(254, offsetof(CEngineResourceManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(255, offsetof(CEngineResourceManager, MeshBanks) == 0x10);
FABLE_STATIC_ASSERT_AT(256, offsetof(CEngineResourceManager, MainMeshBank) == 0x1c);
FABLE_STATIC_ASSERT_AT(257, offsetof(CEngineResourceManager, EditorMeshBank) == 0x20);
FABLE_STATIC_ASSERT_AT(258, offsetof(CEngineResourceManager, EngineMeshBank) == 0x480);
FABLE_STATIC_ASSERT_AT(259, offsetof(CEngineResourceManager, LoadingMemoryPool_Data) == 0x8e0);
FABLE_STATIC_ASSERT_AT(260, offsetof(CEngineResourceManager, LoadingMemoryPool_Info) == 0x8e4);
FABLE_STATIC_ASSERT_AT(261, offsetof(CEngineResourceManager, LoadingMemoryFailureHandler_Data) == 0x8e8);
FABLE_STATIC_ASSERT_AT(262, offsetof(CEngineResourceManager, LoadingMemoryFailureHandler_Info) == 0x8ec);
FABLE_STATIC_ASSERT_AT(263, offsetof(CEngineResourceManager, AnimationMemoryPool_Data) == 0x8f0);
FABLE_STATIC_ASSERT_AT(264, offsetof(CEngineResourceManager, AnimationMemoryPool_Info) == 0x8f4);
FABLE_STATIC_ASSERT_AT(265, offsetof(CEngineResourceManager, ClothMemoryPool_Data) == 0x8f8);
FABLE_STATIC_ASSERT_AT(266, offsetof(CEngineResourceManager, ClothMemoryPool_Info) == 0x8fc);
FABLE_STATIC_ASSERT_AT(267, offsetof(CEngineResourceManager, MeshDataMemoryPool_Data) == 0x900);
FABLE_STATIC_ASSERT_AT(268, offsetof(CEngineResourceManager, MeshDataMemoryPool_Info) == 0x904);
FABLE_STATIC_ASSERT_AT(269, offsetof(CEngineResourceManager, AnimationManager) == 0x908);
FABLE_STATIC_ASSERT_AT(270, offsetof(CEngineResourceManager, GraphicBanks) == 0x954);
FABLE_STATIC_ASSERT_AT(271, offsetof(CEngineResourceManager, MainGraphicBank) == 0x958);
FABLE_STATIC_ASSERT_AT(272, offsetof(CEngineResourceManager, EngineFont) == 0x95c);
FABLE_STATIC_ASSERT_AT(273, offsetof(CEngineResourceManager, VShaderBanks) == 0x960);
FABLE_STATIC_ASSERT_AT(274, offsetof(CEngineResourceManager, ShaderBankPixel) == 0x2ab4);
FABLE_STATIC_ASSERT_AT(275, offsetof(CEngineResourceManager, VShaderResources) == 0x2bf0);
FABLE_STATIC_ASSERT_AT(276, offsetof(CEngineResourceManager, ShaderResourcePixel) == 0x2cc8);
FABLE_STATIC_ASSERT_AT(277, offsetof(CEngineResourceManager, MaxLoadingTime) == 0x2cd0);
FABLE_STATIC_ASSERT_AT(278, offsetof(CEngineResourceManager, VertexBufferUsageFlags) == 0x2cd8);
FABLE_STATIC_ASSERT_AT(279, offsetof(CEngineResourceManager, StipleFadeTexture) == 0x2cdc);
FABLE_STATIC_ASSERT_AT(280, offsetof(CEngineResourceManager, HWInverseStipplePatterns) == 0x2ce4);
FABLE_STATIC_ASSERT_AT(281, offsetof(CEngineResourceManager, DitherTextureList) == 0x2cf0);
FABLE_STATIC_ASSERT_AT(282, offsetof(CEngineResourceManager, EngineCachePath_PStringData) == 0x2cfc);
FABLE_STATIC_ASSERT_AT(283, offsetof(CEngineResourceManager, WriteableEngineCachePath_PStringData) == 0x2d00);
FABLE_STATIC_ASSERT_AT(284, offsetof(CEngineResourceManager, ZeroRGBATexture) == 0x2d04);
FABLE_STATIC_ASSERT_AT(285, offsetof(CEngineResourceManager, SatRGBATexture) == 0x2d0c);
FABLE_STATIC_ASSERT_AT(286, offsetof(CEngineResourceManager, VSConstantLayoutBasic) == 0x2d14);
FABLE_STATIC_ASSERT_AT(287, offsetof(CEngineResourceManager, VSConstantLayoutLights) == 0x2e60);
FABLE_STATIC_ASSERT_AT(288, offsetof(CEngineResourceManager, VSConstantLayoutLightsBones) == 0x2fac);
FABLE_STATIC_ASSERT_AT(289, offsetof(CEngineResourceManager, VSConstantLayoutRepeatedMesh) == 0x30f8);
FABLE_STATIC_ASSERT_AT(290, offsetof(CEngineResourceManager, VSConstantLayoutParticles) == 0x3274);
FABLE_STATIC_ASSERT_AT(291, offsetof(CEngineResourceManager, GPUScratchBufferTextureManager) == 0x33c0);
FABLE_STATIC_ASSERT_AT(292, offsetof(CEngineResourceManager, ExternalTexturesAllocatedFromEnginePools) == 0x3404);

#endif // FABLE_ENGINE_CENGINERESOURCEMANAGER_H
#endif // FABLE_RETAIL_CENGINERESOURCEMANAGER
