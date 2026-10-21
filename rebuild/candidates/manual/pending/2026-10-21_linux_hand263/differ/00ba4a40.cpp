// Grade: DIFFER(466v463) -- register allocation: retail keeps the primitive in ebp and the block pointer in ebx, so the bank array base is reloaded after the size imul; ours swaps them (ebx/ebp) and keeps the base live (tried compare orders, operator== on the stages, three operator[] forms, a block reference)
// CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksBlankTextures(C3DPrimitive2& primitive, CEngineInternalPrimitiveStaticMesh& mesh, C3DTextureStages& stages, const C3DMeshMaterial* material, bool* alreadySetUp) -- give every static block of the primitive the bank's blank diffuse/bump textures for the material's texture types, unless the primitive already recorded this bank frame and these texture stages
// Leaf: SetupStaticBlocksBlankTextures
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA4A40, length 464 bytes (body 0x1cf), `ret 14h` (`this` unused).
// Callees: CGraphicDataBank::GetBlankTexture(ETextureType) 0x009FD150.
// Layout: C3DPrimitive2 retail (PDB less 4 past the 12-byte CArray): StaticBlockCount +0x40, StaticBlocks +0x44 (0x20-byte
//   CStaticBlock: DiffuseTexture +0x14, BumpTexture +0x18), RecordedTextureStages +0x6c (C3DTextureStages, 12 bytes),
//   RecordedFrame +0x7c. C3DMeshMaterial (PDB): DiffuseMapBankIndex +0x10, BumpMapBankIndex +0x14. The mesh reaches the
//   graphic bank through +0x50 -> +4 -> +0x3d4; the bank's frame stamp is +0x194 and its texture entries (0x2c bytes,
//   def holder at +0x28, def at holder +0xc, texture type at def +0x1c) are a CArray at +0x1e0.
// Shape: the bank entry lookup is the CIndexedArray pair `GetPtr(i)` (branchy bounds check) then `operator[](i)` (the
//   `cmp/sbb/and` select); an index of 0 keeps the default type (0 diffuse, 4 bump).
struct CTexture;

struct CTextureDef {
    unsigned char _pad00[0x1c];
    long          TextureType;                     // +0x1c ETextureType
};
struct CTextureDefHolder {
    unsigned char _pad00[0xc];
    CTextureDef*  Def;                             // +0x0c
};
struct CBankTextureEntry {                         // 0x2c bytes
    unsigned char      _pad00[0x28];
    CTextureDefHolder* Holder;                     // +0x28
};

template <class T> struct CIndexedArray {
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    T* GetPtr(unsigned long index) { return index < Size() ? &First[index] : 0; }
    T& operator[](unsigned long index) { T* p = &First[index]; return *(index < Size() ? p : 0); }
};

struct CGraphicDataBank {
    unsigned char                    _pad000[0x194];
    long                             FrameStamp;   // +0x194
    unsigned char                    _pad198[0x1e0 - 0x198];
    CIndexedArray<CBankTextureEntry> Textures;     // +0x1e0
    CTexture* GetBlankTexture(long type);          // 0x009FD150

    __forceinline long GetTextureType(unsigned long index, long defaultType)
    {
        long type = defaultType;
        if (index)
            type = (Textures.GetPtr(index) ? Textures[index].Holder : 0)->Def->TextureType;
        return type;
    }
};

struct CGraphicsOwner { unsigned char _pad[0x3d4]; CGraphicDataBank* Bank; };   // +0x3d4
struct CMeshResource { unsigned char _pad[4]; CGraphicsOwner* Owner; };          // +0x04

struct C3DTextureStages {
    long DiffuseTextureStage;                      // +0x00
    long BumpTextureStage;                         // +0x04
    bool UseBlankTextures;                         // +0x08
    bool operator==(const C3DTextureStages& other) const
    {
        return DiffuseTextureStage == other.DiffuseTextureStage && BumpTextureStage == other.BumpTextureStage
            && UseBlankTextures == other.UseBlankTextures;
    }
};

struct CStaticBlock {                              // 0x20 bytes
    unsigned char _pad00[0x14];
    CTexture*     DiffuseTexture;                  // +0x14
    CTexture*     BumpTexture;                     // +0x18
    long          MaterialIndex;                   // +0x1c
};

struct C3DPrimitive2 {
    unsigned char    _pad00[0x40];
    unsigned long    StaticBlockCount;             // +0x40
    CStaticBlock*    StaticBlocks;                 // +0x44
    unsigned char    _pad48[0x6c - 0x48];
    C3DTextureStages RecordedTextureStages;        // +0x6c
    unsigned char    _pad78[4];
    long             RecordedFrame;                // +0x7c
};

struct CEngineInternalPrimitiveStaticMesh {
    unsigned char  _pad00[0x50];
    CMeshResource* Mesh;                           // +0x50
};

struct C3DMeshMaterial {
    unsigned char _pad00[0x10];
    unsigned long DiffuseMapBankIndex;             // +0x10
    unsigned long BumpMapBankIndex;                // +0x14
};

struct CEnginePrimitiveManagerStaticMeshesBase {
    bool SetupStaticBlocksBlankTextures(C3DPrimitive2& primitive, CEngineInternalPrimitiveStaticMesh& mesh, C3DTextureStages& stages,
                                        const C3DMeshMaterial* material, bool* alreadySetUp);
};

bool CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksBlankTextures(C3DPrimitive2& primitive, CEngineInternalPrimitiveStaticMesh& mesh,
                                                                             C3DTextureStages& stages, const C3DMeshMaterial* material,
                                                                             bool* alreadySetUp)
{
    CGraphicDataBank* bank = mesh.Mesh->Owner->Bank;
    long frame = bank->FrameStamp;
    if (frame == primitive.RecordedFrame && stages == primitive.RecordedTextureStages) {
        *alreadySetUp = true;
        return true;
    }
    *alreadySetUp = false;
    CStaticBlock* blocks = primitive.StaticBlocks;
    if (stages.DiffuseTextureStage != -1)
        blocks[0].DiffuseTexture = bank->GetBlankTexture(bank->GetTextureType(material->DiffuseMapBankIndex, 0));
    if (stages.BumpTextureStage != -1)
        blocks[0].BumpTexture = bank->GetBlankTexture(bank->GetTextureType(material->BumpMapBankIndex, 4));
    for (unsigned long i = 1; i < primitive.StaticBlockCount; ++i) {
        primitive.StaticBlocks[i].DiffuseTexture = blocks[0].DiffuseTexture;
        primitive.StaticBlocks[i].BumpTexture = blocks[0].BumpTexture;
    }
    primitive.RecordedFrame = frame;
    primitive.RecordedTextureStages = stages;
    return true;
}
