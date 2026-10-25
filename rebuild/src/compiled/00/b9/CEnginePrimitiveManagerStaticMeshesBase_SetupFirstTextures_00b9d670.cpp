// CEnginePrimitiveManagerStaticMeshesBase::SetupFirstTextures(C3DPrimitive2& primitive, C3DTextureStages& textureStages) -- bind the first static block's diffuse and bump textures to the stages the material uses (a stage of -1 is skipped), through the render core's cached SetTexture
// Leaf: ?SetupFirstTextures@CEnginePrimitiveManagerStaticMeshesBase@@SIXAAVC3DPrimitive2@@AAVC3DTextureStages@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9D670, 179 bytes, static __fastcall (primitive ecx, textureStages edx), `ret`.
// Identity: Ego_r.pdb ?SetupFirstTextures@CEnginePrimitiveManagerStaticMeshesBase@@SIXAAVC3DPrimitive2@@AAVC3DTextureStages@@@Z;
//   callers CEngineBillboardGenerator::RenderBillboard 0x00B8C033, CEnginePrimitiveManagerStaticMeshesBase::
//   RenderMeshPrimitive 0x00BA5E8F and ::RenderPrimitiveToShadowBuffer 0x00BA6CA1. Catalog name agrees.
// Callees: IDirect3DDevice9::SetTexture (vtable slot 65, +0x104) through the inline CRenderManagerCore::SetTexture.
// Layout: C3DPrimitive2::StaticBlocks at retail +0x44 (Ego_r +0x48, less 4 for the 12-byte ClothPrimitives CArray);
//   CStaticBlock (Ego_r.pdb) DiffuseTexture +0x14, BumpTexture +0x18; CTexture::PD3DTexture +0; C3DTextureStages
//   {DiffuseTextureStage, BumpTextureStage, UseBlankTextures}; g_pRenderManagerCore 0x01436E18 (CurrentTextures +0x3D00,
//   NumTextureStagesUsed +0x3CB8, PD3DDevice +0x3CF0) as in the landed CEngine::Uninitialise 0x00B25D80.
// Shape: two inlined cached SetTexture calls (the core pointer re-read after each device call); each texture is
//   `texture ? texture->PD3DTexture : 0` inside a __forceinline CRenderManagerCore::SetTexture(long, const CTexture*)
//   overload (the landed 0x00BA7DB0 device) with the -1 stage tests outside it: that loads g_pRenderManagerCore at entry
//   and names block/stages ebx/ebp as retail; the select written in this body loads the core after it.
struct IDirect3DBaseTexture9 { };
struct IDirect3DTexture9 : IDirect3DBaseTexture9 { };
struct IDirect3DDevice9 {
    virtual void __stdcall Slot000(); virtual void __stdcall Slot001(); virtual void __stdcall Slot002(); virtual void __stdcall Slot003();
    virtual void __stdcall Slot004(); virtual void __stdcall Slot005(); virtual void __stdcall Slot006(); virtual void __stdcall Slot007();
    virtual void __stdcall Slot008(); virtual void __stdcall Slot009(); virtual void __stdcall Slot010(); virtual void __stdcall Slot011();
    virtual void __stdcall Slot012(); virtual void __stdcall Slot013(); virtual void __stdcall Slot014(); virtual void __stdcall Slot015();
    virtual void __stdcall Slot016(); virtual void __stdcall Slot017(); virtual void __stdcall Slot018(); virtual void __stdcall Slot019();
    virtual void __stdcall Slot020(); virtual void __stdcall Slot021(); virtual void __stdcall Slot022(); virtual void __stdcall Slot023();
    virtual void __stdcall Slot024(); virtual void __stdcall Slot025(); virtual void __stdcall Slot026(); virtual void __stdcall Slot027();
    virtual void __stdcall Slot028(); virtual void __stdcall Slot029(); virtual void __stdcall Slot030(); virtual void __stdcall Slot031();
    virtual void __stdcall Slot032(); virtual void __stdcall Slot033(); virtual void __stdcall Slot034(); virtual void __stdcall Slot035();
    virtual void __stdcall Slot036(); virtual void __stdcall Slot037(); virtual void __stdcall Slot038(); virtual void __stdcall Slot039();
    virtual void __stdcall Slot040(); virtual void __stdcall Slot041(); virtual void __stdcall Slot042(); virtual void __stdcall Slot043();
    virtual void __stdcall Slot044(); virtual void __stdcall Slot045(); virtual void __stdcall Slot046(); virtual void __stdcall Slot047();
    virtual void __stdcall Slot048(); virtual void __stdcall Slot049(); virtual void __stdcall Slot050(); virtual void __stdcall Slot051();
    virtual void __stdcall Slot052(); virtual void __stdcall Slot053(); virtual void __stdcall Slot054(); virtual void __stdcall Slot055();
    virtual void __stdcall Slot056(); virtual void __stdcall Slot057(); virtual void __stdcall Slot058(); virtual void __stdcall Slot059();
    virtual void __stdcall Slot060(); virtual void __stdcall Slot061(); virtual void __stdcall Slot062(); virtual void __stdcall Slot063();
    virtual void __stdcall Slot064();
    virtual long __stdcall SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture) = 0;   // slot 65 (+0x104)
};

class CRenderManagerCore {
public:
    unsigned char          _pad0[0x3cb8];
    long                   NumTextureStagesUsed;     // +0x3cb8
    unsigned char          _pad3cbc[0x3cf0 - 0x3cbc];
    IDirect3DDevice9*      PD3DDevice;               // +0x3cf0
    void*                  CurrentIndices;           // +0x3cf4
    unsigned char          _pad3cf8[8];
    IDirect3DBaseTexture9* CurrentTextures[4];       // +0x3d00
    void SetTexture(long stage, IDirect3DBaseTexture9* texture)
    {
        if (CurrentTextures[stage] != texture) {
            CurrentTextures[stage] = texture;
            PD3DDevice->SetTexture(stage, texture);
        }
        if (NumTextureStagesUsed < stage + 1)
            NumTextureStagesUsed = stage + 1;
    }
    void SetTexture(long stage, const class CTexture* texture);   // binds texture ? texture->PD3DTexture : 0
};
extern CRenderManagerCore* g_pRenderManagerCore;   // 0x01436E18

class CTexture {                                   // Ego_r.pdb, 8 bytes
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      Bits;                       // +0x04 ByteLength:28, AllocationSource:4
};

__forceinline void CRenderManagerCore::SetTexture(long stage, const CTexture* texture)
{
    SetTexture(stage, texture ? texture->PD3DTexture : 0);
}

class CStaticBlock {                               // Ego_r.pdb, 32 bytes
public:
    unsigned char _pad00[0xc];
    unsigned long DiffuseTextureFixupOffset;       // +0x0c
    unsigned long BumpTextureFixupOffset;          // +0x10
    CTexture*     DiffuseTexture;                  // +0x14
    CTexture*     BumpTexture;                     // +0x18
    long          MaterialIndex;                   // +0x1c
};

class C3DPrimitive2 {
public:
    unsigned char _pad00[0x44];
    CStaticBlock* StaticBlocks;                    // +0x44
};

class C3DTextureStages {                           // Ego_r.pdb, 12 bytes
public:
    long DiffuseTextureStage;                      // +0x00
    long BumpTextureStage;                         // +0x04
    bool UseBlankTextures;                         // +0x08
};

class CEnginePrimitiveManagerStaticMeshesBase {
public:
    static void __fastcall SetupFirstTextures(C3DPrimitive2& primitive, C3DTextureStages& textureStages);
};

void __fastcall CEnginePrimitiveManagerStaticMeshesBase::SetupFirstTextures(C3DPrimitive2& primitive, C3DTextureStages& textureStages)
{
    CStaticBlock* block = primitive.StaticBlocks;
    if (textureStages.DiffuseTextureStage != -1)
        g_pRenderManagerCore->SetTexture(textureStages.DiffuseTextureStage, block->DiffuseTexture);
    if (textureStages.BumpTextureStage != -1)
        g_pRenderManagerCore->SetTexture(textureStages.BumpTextureStage, block->BumpTexture);
}