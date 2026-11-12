// Behaviour test for 0x00BA4A40 CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksBlankTextures: a copy of the model
// (texture bank with a graphic list and per-type blank textures, primitive with static blocks and the recorded frame / stages).
#include <cstdio>

enum ETextureType { TEXTURE_TYPE_OPAQUE=0, TEXTURE_TYPE_ALPHA=1, TEXTURE_TYPE_BUMP=4, TEXTURE_TYPE_BUMP_ALPHA=5, TEXTURE_TYPE_COUNT=11 };
class CTexture { public: int id; };
class CGraphicInfo;
class CGraphicInfoFrame { public: CGraphicInfo* GraphicInfo; ETextureType GetTextureType() const; };
class CGraphicInfo { public:
    ETextureType TextureType; CGraphicInfoFrame* Frames;
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long index) const { return Frames+index; }
    ETextureType GetTextureType() const { return TextureType; }
};
ETextureType CGraphicInfoFrame::GetTextureType() const { return GraphicInfo->GetTextureType(); }
class CGraphicDataBank { public:
    unsigned long CurrentFrame; CGraphicInfo* First; CGraphicInfo* Last; CTexture Blank[TEXTURE_TYPE_COUNT]; mutable int Calls;
    unsigned long GetCurrentFrame() const { return CurrentFrame; }
    const CGraphicInfo* PeekGraphic(unsigned long index) const { return index<(unsigned long)(Last-First) ? &First[index] : 0; }
    const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long graphic,unsigned long frame) const
    { return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0; }
    const CTexture* GetBlankTexture(ETextureType type) const { ++Calls; return &Blank[type]; }
};
class C3DTextureStages { public:
    long DiffuseTextureStage,BumpTextureStage; bool UseBlankTextures;
    bool operator==(const C3DTextureStages& o) const
    { return DiffuseTextureStage==o.DiffuseTextureStage && BumpTextureStage==o.BumpTextureStage && UseBlankTextures==o.UseBlankTextures; }
};
struct CStaticBlock { const CTexture* DiffuseTexture; const CTexture* BumpTexture; };
class C3DPrimitive2 { public:
    unsigned long StaticBlockCount; CStaticBlock* StaticBlocks; C3DTextureStages RecordedTextureStages; long RecordedFrame;
    CStaticBlock* GetStaticBlock(unsigned long i) { return &StaticBlocks[i]; }
    unsigned long GetStaticBlockCount() const { return StaticBlockCount; }
    long GetRecordedFrame() const { return RecordedFrame; }
    const C3DTextureStages& PeekRecordedTextureStages() const { return RecordedTextureStages; }
    void SetRecordedFrame(long f) { RecordedFrame=f; }
    void SetRecordedTextureStages(const C3DTextureStages& s) { RecordedTextureStages=s; }
};
class C3DMeshMaterial { public:
    unsigned long DiffuseMapBankIndex,BumpMapBankIndex;
    unsigned long GetDiffuseMapBankIndex() const { return DiffuseMapBankIndex; }
    unsigned long GetBumpMapBankIndex() const { return BumpMapBankIndex; }
};
struct CMesh { const CGraphicDataBank* TextureBank; const CGraphicDataBank* PeekTextureBank() const { return TextureBank; } };

static bool SetupStaticBlocksBlankTextures(C3DPrimitive2& mesh_prim, CMesh& prim, C3DTextureStages& stages,
    const C3DMeshMaterial* material, bool* already_set_up)
{
    const CGraphicDataBank* graphic_bank=prim.PeekTextureBank();
    long frame=graphic_bank->GetCurrentFrame();
    if(frame==mesh_prim.GetRecordedFrame() && stages==mesh_prim.PeekRecordedTextureStages()) {
        *already_set_up=true;
        return true;
    }
    *already_set_up=false;
    CStaticBlock& first_block=*mesh_prim.GetStaticBlock(0);
    if(stages.DiffuseTextureStage!=-1) {
        ETextureType texture_type=TEXTURE_TYPE_OPAQUE;
        unsigned long graphic_index=material->GetDiffuseMapBankIndex();
        if(graphic_index!=0) texture_type=graphic_bank->PeekGraphicInfoFrame(graphic_index,0)->GetTextureType();
        first_block.DiffuseTexture=graphic_bank->GetBlankTexture(texture_type);
    }
    if(stages.BumpTextureStage!=-1) {
        ETextureType texture_type=TEXTURE_TYPE_BUMP;
        unsigned long graphic_index=material->GetBumpMapBankIndex();
        if(graphic_index!=0) texture_type=graphic_bank->PeekGraphicInfoFrame(graphic_index,0)->GetTextureType();
        first_block.BumpTexture=graphic_bank->GetBlankTexture(texture_type);
    }
    unsigned long no_blocks=mesh_prim.GetStaticBlockCount();
    for(unsigned long c_block=1;c_block<no_blocks;++c_block) {
        CStaticBlock& block_info=*mesh_prim.GetStaticBlock(c_block);
        block_info.DiffuseTexture=first_block.DiffuseTexture;
        block_info.BumpTexture=first_block.BumpTexture;
    }
    mesh_prim.SetRecordedFrame(frame);
    mesh_prim.SetRecordedTextureStages(stages);
    return true;
}

int main()
{
    int bad=0;
    CGraphicInfo graphics[3]; CGraphicInfoFrame frames[3];
    ETextureType types[3]={TEXTURE_TYPE_OPAQUE,TEXTURE_TYPE_ALPHA,TEXTURE_TYPE_BUMP_ALPHA};
    int i;
    for(i=0;i<3;++i) { graphics[i].TextureType=types[i]; graphics[i].Frames=&frames[i]; frames[i].GraphicInfo=&graphics[i]; }
    CGraphicDataBank bank; bank.CurrentFrame=7; bank.First=graphics; bank.Last=graphics+3; bank.Calls=0;
    for(i=0;i<TEXTURE_TYPE_COUNT;++i) bank.Blank[i].id=i;
    CMesh mesh; mesh.TextureBank=&bank;
    CTexture sentinel; sentinel.id=99;
    CStaticBlock blocks[3];
    for(i=0;i<3;++i) { blocks[i].DiffuseTexture=&sentinel; blocks[i].BumpTexture=&sentinel; }
    C3DPrimitive2 prim; prim.StaticBlockCount=3; prim.StaticBlocks=blocks; prim.RecordedFrame=-1;
    prim.RecordedTextureStages.DiffuseTextureStage=-1; prim.RecordedTextureStages.BumpTextureStage=-1; prim.RecordedTextureStages.UseBlankTextures=false;
    C3DTextureStages stages; stages.DiffuseTextureStage=0; stages.BumpTextureStage=1; stages.UseBlankTextures=true;
    C3DMeshMaterial material; material.DiffuseMapBankIndex=1; material.BumpMapBankIndex=2;
    bool already=true;

    // 1. Fresh primitive: both stages active, types come from the material's graphics, every block gets block 0's textures.
    if(!SetupStaticBlocksBlankTextures(prim,mesh,stages,&material,&already)) bad=1;
    if(already) bad=1;
    for(i=0;i<3;++i) if(blocks[i].DiffuseTexture!=&bank.Blank[TEXTURE_TYPE_ALPHA] || blocks[i].BumpTexture!=&bank.Blank[TEXTURE_TYPE_BUMP_ALPHA]) bad=1;
    if(prim.RecordedFrame!=7 || !(prim.RecordedTextureStages==stages) || bank.Calls!=2) bad=1;

    // 2. Same frame, same stages: nothing is looked up again.
    already=false;
    if(!SetupStaticBlocksBlankTextures(prim,mesh,stages,&material,&already) || !already || bank.Calls!=2) bad=1;

    // 3. Same frame, one stage field differs (the bool): set up again.
    stages.UseBlankTextures=false; already=true;
    if(!SetupStaticBlocksBlankTextures(prim,mesh,stages,&material,&already) || already || bank.Calls!=4) bad=1;
    if(prim.RecordedTextureStages.UseBlankTextures) bad=1;

    // 4. New frame, no graphic indices: the default types (opaque / bump).
    bank.CurrentFrame=8; material.DiffuseMapBankIndex=0; material.BumpMapBankIndex=0; already=true;
    if(!SetupStaticBlocksBlankTextures(prim,mesh,stages,&material,&already) || already) bad=1;
    for(i=0;i<3;++i) if(blocks[i].DiffuseTexture!=&bank.Blank[TEXTURE_TYPE_OPAQUE] || blocks[i].BumpTexture!=&bank.Blank[TEXTURE_TYPE_BUMP]) bad=1;
    if(prim.RecordedFrame!=8 || bank.Calls!=6) bad=1;

    // 5. New frame, diffuse stage off: only the bump texture changes; the old diffuse stays and is still copied down.
    bank.CurrentFrame=9; stages.DiffuseTextureStage=-1; material.BumpMapBankIndex=1;
    blocks[0].DiffuseTexture=&sentinel; already=true;
    if(!SetupStaticBlocksBlankTextures(prim,mesh,stages,&material,&already) || already || bank.Calls!=7) bad=1;
    for(i=0;i<3;++i) if(blocks[i].DiffuseTexture!=&sentinel || blocks[i].BumpTexture!=&bank.Blank[TEXTURE_TYPE_ALPHA]) bad=1;
    if(prim.RecordedFrame!=9 || prim.RecordedTextureStages.DiffuseTextureStage!=-1) bad=1;

    // 6. One block only: the copy loop does not run; both stages off: no lookups, but the state is still recorded.
    bank.CurrentFrame=10; stages.BumpTextureStage=-1; prim.StaticBlockCount=1;
    blocks[1].BumpTexture=&sentinel; already=true;
    if(!SetupStaticBlocksBlankTextures(prim,mesh,stages,&material,&already) || already || bank.Calls!=7) bad=1;
    if(blocks[1].BumpTexture!=&sentinel || prim.RecordedFrame!=10 || prim.RecordedTextureStages.BumpTextureStage!=-1) bad=1;

    if(bad) { std::printf("00ba4a40_TEST FAIL\n"); return 1; }
    std::printf("00ba4a40_TEST PASS\n");
    return 0;
}