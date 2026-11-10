#include <assert.h>
#include <stdio.h>
#include <vector>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
class CTexture {public:int ID;};
class CGraphicDataBank {};
class CCompositeSurfaceInfo {};
template<class K,class V>class CKeyPairCompareLess {};
template<class K,class V,class C>class CVectorMap {};
class C3DMeshMaterial {public:int ID;};
class C3DTextureStages {public:long DiffuseTextureStage,BumpTextureStage;bool UseBlankTextures;};
namespace N3DPrimitive {
class CBaseBlock {public:unsigned long PrimitiveCount,StartIndex;bool IsStrip,DegenerateTriangles;unsigned char ChangeFlags; unsigned long GetChangeFlags()const{return ChangeFlags;} };
class CStaticBlock:public CBaseBlock {public:unsigned long DiffuseTextureFixupOffset,BumpTextureFixupOffset;const CTexture* DiffuseTexture;const CTexture* BumpTexture;long MaterialIndex;long GetMaterialIndex()const{return MaterialIndex;}};
}
class C3DPrimitive2 {
public:unsigned long StaticBlockCount;N3DPrimitive::CStaticBlock* StaticBlocks;long RecordedFrame;
    N3DPrimitive::CStaticBlock* GetStaticBlock(unsigned long i){return &StaticBlocks[i];}
    unsigned long GetStaticBlockCount()const{return StaticBlockCount;}
    void SetRecordedFrame(long frame){RecordedFrame=frame;}
};
class C3DMesh2 {public:C3DMeshMaterial* Materials;const C3DMeshMaterial* GetMaterial(unsigned long index)const{return &Materials[index];}};
class CEnginePrimitiveManagerMeshBase {
public:
    static const CTexture* FABLE_FASTCALL GetDiffuseTexture(const C3DMeshMaterial*,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >*,unsigned long,bool&,const CGraphicDataBank*,bool);
    static const CTexture* FABLE_FASTCALL GetBumpTexture(const C3DMeshMaterial*,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >*,unsigned long,bool&,const CGraphicDataBank*,bool);
};
class CEnginePrimitiveManagerStaticMeshesBase:public CEnginePrimitiveManagerMeshBase {
public:static bool FABLE_FASTCALL SetupStaticBlocksNoMorph(C3DPrimitive2&,const C3DMesh2&,C3DTextureStages&,long,const CGraphicDataBank*,bool);
};
class Call {public:char Kind;int Material;Call(char kind,int material):Kind(kind),Material(material){}};
static std::vector<Call> calls;
static CTexture textures[32],unchanged;
static CGraphicDataBank bank;
static unsigned int failureMask;
static bool expectedLoad,mutate;
static C3DPrimitive2* activePrimitive;
static C3DMesh2* activeMesh;
static N3DPrimitive::CStaticBlock* bBlocks;
static N3DPrimitive::CStaticBlock* cBlocks;
static N3DPrimitive::CStaticBlock* dBlocks;
static C3DMeshMaterial* bMaterials;
static C3DMeshMaterial* cMaterials;
static const CTexture* Texture(char kind,const C3DMeshMaterial* material,unsigned long level,bool& failed,const CGraphicDataBank* dataBank,bool load){
    assert(level==7&&dataBank==&bank&&load==expectedLoad);
    unsigned int index=(unsigned int)calls.size();assert(index<32);calls.push_back(Call(kind,material->ID));
    failed=failed||((failureMask&(1u<<index))!=0);
    if(mutate){
        if(index==0){activePrimitive->StaticBlocks=bBlocks;activePrimitive->StaticBlockCount=4;activeMesh->Materials=bMaterials;}
        if(index==1){activePrimitive->StaticBlocks=cBlocks;activePrimitive->StaticBlockCount=3;}
        if(index==2){activePrimitive->StaticBlocks=dBlocks;activePrimitive->StaticBlockCount=1;activeMesh->Materials=cMaterials;cBlocks[1].ChangeFlags=0;}
    }
    return &textures[index];
}
const CTexture* FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(const C3DMeshMaterial* m,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* surfaces,unsigned long level,bool& failed,const CGraphicDataBank* b,bool load){assert(!surfaces);return Texture('D',m,level,failed,b,load);}
const CTexture* FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetBumpTexture(const C3DMeshMaterial* m,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* surfaces,unsigned long level,bool& failed,const CGraphicDataBank* b,bool load){assert(!surfaces);return Texture('B',m,level,failed,b,load);}
bool FABLE_FASTCALL CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph(C3DPrimitive2& mesh_prim, const C3DMesh2& mesh,
    C3DTextureStages& stages, long mipmap_level, const CGraphicDataBank* graphic_bank, bool block_on_load)
{
    N3DPrimitive::CStaticBlock& first_block=*mesh_prim.GetStaticBlock(0);
    const C3DMeshMaterial* material=mesh.GetMaterial(first_block.GetMaterialIndex());
    bool failed=false;
    if(stages.DiffuseTextureStage!=-1)
        first_block.DiffuseTexture=CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(material,0,mipmap_level,failed,graphic_bank,block_on_load);
    if(stages.BumpTextureStage!=-1)
        first_block.BumpTexture=CEnginePrimitiveManagerMeshBase::GetBumpTexture(material,0,mipmap_level,failed,graphic_bank,block_on_load);
    unsigned long no_blocks=mesh_prim.GetStaticBlockCount();
    for(unsigned long c_block=1;c_block<no_blocks;++c_block) {
        N3DPrimitive::CStaticBlock& block_info=*mesh_prim.GetStaticBlock(c_block);
        const C3DMeshMaterial* current_material=mesh.GetMaterial(block_info.GetMaterialIndex());
        if(block_info.GetChangeFlags()&1) {
            if(stages.DiffuseTextureStage!=-1)
                block_info.DiffuseTexture=CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(current_material,0,mipmap_level,failed,graphic_bank,block_on_load);
            else failed=true;
        }
        if((block_info.GetChangeFlags()&2)&&stages.BumpTextureStage!=-1)
            block_info.BumpTexture=CEnginePrimitiveManagerMeshBase::GetBumpTexture(current_material,0,mipmap_level,failed,graphic_bank,block_on_load);
    }
    mesh_prim.SetRecordedFrame(-1);
    return !failed;
}

#ifndef FABLE_FORCEINLINE
#define FABLE_FORCEINLINE inline
#endif
namespace TextureHelperTest {
class CTexture {public:int ID;explicit CTexture(int id=0):ID(id){}};
class CCompositeSurfaceInfo {};
template<class K,class V>class CKeyPairCompareLess {};
template<class K,class V,class C>class CVectorMap {};
class CResourceList {public:unsigned long CurrentFrame;unsigned long GetCurrentFrame()const{return CurrentFrame;}};
class CResource {public:CResourceList* ResourceList;unsigned long LastUsedFrame;int touches;CResource():ResourceList(0),LastUsedFrame(0),touches(0){} void SetAsMRU(){assert(ResourceList);++touches;}};
static int live_managed,live_frames,ctor_calls,frame_destroyed;
class CManagedTextureData:public CResource {public:unsigned long refs;CManagedTextureData():refs(1){++live_managed;}~CManagedTextureData(){--live_managed;}};
template<class T>class CIVCountedPointer {T* p;CIVCountedPointer(const CIVCountedPointer&);CIVCountedPointer&operator=(const CIVCountedPointer&);public:CIVCountedPointer():p(0){}~CIVCountedPointer(){Release();}bool IsNull()const{return p==0;}T*operator->()const{return p;}void Release(){if(p){if(--p->refs==0)delete p;p=0;}}void Reset(T* value){Release();p=value;}};
class CManagedTexture {public:CIVCountedPointer<CManagedTextureData> Data;void TouchTexture();};
class CGraphicFrameData {public:struct Handle{CTexture* Data;}PTexture;CTexture*GetTexture()const{return PTexture.Data;}};
class CGraphicFrame {public:CGraphicFrameData* FrameData;CGraphicFrame():FrameData(0){}};
class CGraphicDataBank;class CGraphicInfo;
class CGraphicInfoFrame {
public:
    class CFrameData;
    CGraphicInfoFrame():FrameNum(0),SmallMipmapLevel(0),GraphicInfo(0){}
    bool GetTexture(const CTexture**,unsigned long,unsigned long)const;
    const CTexture*GetBlankTexture()const;
    void TouchFrame(unsigned long)const;
    bool InternalGetFrameForcedLoading(unsigned long)const;
    unsigned short FrameNum;signed char SmallMipmapLevel;
    mutable CIVCountedPointer<CFrameData> Data;CGraphicInfo*GraphicInfo;
};
class CGraphicInfoFrame::CFrameData:public CResource {
public:
    unsigned long refs;
    CFrameData(const CGraphicInfoFrame*);
    ~CFrameData(){--live_frames;++frame_destroyed;}
    void BeginLoading(unsigned long,unsigned long);
    CGraphicFrame Frame;CManagedTexture ManagedTexture,LoadingManagedTexture;
    struct Handle{void*Data;Handle():Data(0){}}FileData;
    signed char CurrentMipmapLevel,LoadingMipmapLevel,TouchedMipmapLevel;
    unsigned long TouchedFrame;const CGraphicInfoFrame*GraphicInfoFrame;
};
class CGraphicInfo {public:CGraphicDataBank*Bank;CGraphicInfoFrame*Frames;CGraphicInfo():Bank(0),Frames(0){}const CGraphicInfoFrame*PeekInfoFrame(unsigned long i)const{return Frames+i;}CGraphicDataBank*GetBank()const{return Bank;}};
class CGraphicDataBank {
public:
    enum ETextureType{TEXTURE_TYPE_OPAQUE=0,TEXTURE_TYPE_BUMP=4};
    std::vector<CGraphicInfo>GraphicList;CResourceList clock;
    const CGraphicInfo*PeekGraphic(unsigned long)const;
    const CGraphicInfoFrame*PeekGraphicInfoFrame(unsigned long,unsigned long)const;
    const CTexture*GetBlankTexture(ETextureType)const;
    unsigned long GetCurrentFrame()const{return clock.GetCurrentFrame();}
};
class C3DMeshMaterial {public:unsigned long DiffuseMapBankIndex,BumpMapBankIndex;unsigned long GetDiffuseMapBankIndex()const{return DiffuseMapBankIndex;}unsigned long GetBumpMapBankIndex()const{return BumpMapBankIndex;}};
class CEnginePrimitiveRenderer {public:bool BlockOnLoad;mutable int queries;CEnginePrimitiveRenderer():BlockOnLoad(false),queries(0){}bool GetBlockOnLoad()const{++queries;return BlockOnLoad;}};
class CEnginePrimitiveManagerMeshBase {public:static const CTexture*FABLE_FASTCALL GetDiffuseTexture(const C3DMeshMaterial*,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >*,unsigned long,bool&,const CGraphicDataBank*,bool);static const CTexture*FABLE_FASTCALL GetBumpTexture(const C3DMeshMaterial*,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >*,unsigned long,bool&,const CGraphicDataBank*,bool);private:static const CTexture*FABLE_FASTCALL GetComposedTexture(long&,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >&,unsigned long,bool,const CGraphicDataBank*);};
static CEnginePrimitiveRenderer renderer;
CEnginePrimitiveRenderer*g_pEnginePrimitiveRenderer=&renderer;
static CTexture opaque_blank(10),bump_blank(11),frame_blank(12),loaded_texture(20),forced_texture(21),composed_texture(22),sentinel_texture(23);
static CGraphicFrameData loaded_frame,forced_frame;
static bool forced_success,outstanding_file;
static int load_calls,forced_calls,blank_type,frame_blank_calls,composition_calls;
static unsigned long last_level,last_flags,composition_level;
static bool composition_bump;
static const CTexture*composition_result;
static long composition_index;
static CGraphicInfoFrame::CFrameData*ctor_replacement;
CGraphicInfoFrame::CFrameData::CFrameData(const CGraphicInfoFrame* owner):refs(1),CurrentMipmapLevel(-1),LoadingMipmapLevel(-1),TouchedMipmapLevel(7),TouchedFrame(0),GraphicInfoFrame(owner){++live_frames;++ctor_calls;if(ctor_replacement){CFrameData*old=ctor_replacement;ctor_replacement=0;owner->Data.Reset(old);}}
void CGraphicInfoFrame::CFrameData::BeginLoading(unsigned long level,unsigned long flags){++load_calls;last_level=level;last_flags=flags;FileData.Data=outstanding_file?this:0;}
bool CGraphicInfoFrame::InternalGetFrameForcedLoading(unsigned long flags)const{++forced_calls;last_flags=flags;if(forced_success){Data->Frame.FrameData=&forced_frame;Data->CurrentMipmapLevel=0;}return forced_success;}
const CTexture*CGraphicInfoFrame::GetBlankTexture()const{++frame_blank_calls;return &frame_blank;}
const CTexture*CGraphicDataBank::GetBlankTexture(ETextureType type)const{blank_type=type;return type==TEXTURE_TYPE_OPAQUE?&opaque_blank:&bump_blank;}
const CTexture*FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetComposedTexture(long&index,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >&,unsigned long level,bool bump,const CGraphicDataBank*){++composition_calls;composition_level=level;composition_bump=bump;if(composition_index>=0)index=composition_index;return composition_result;}

extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;

inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index<GraphicList.size() ? &GraphicList[index] : 0;
}
inline const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic,unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}
inline void CManagedTexture::TouchTexture()
{
    if(!Data.IsNull() && Data->ResourceList && Data->LastUsedFrame!=Data->ResourceList->GetCurrentFrame()) {
        Data->SetAsMRU();
        Data->LastUsedFrame=Data->ResourceList->GetCurrentFrame();
    }
}
inline void CGraphicInfoFrame::TouchFrame(unsigned long mipmap_level) const
{
    const unsigned long frame=GraphicInfo->GetBank()->GetCurrentFrame();
    if(Data->TouchedFrame!=frame || mipmap_level<=Data->TouchedMipmapLevel) {
        Data->TouchedFrame=frame;
        Data->ManagedTexture.TouchTexture();
        if(mipmap_level<=Data->CurrentMipmapLevel && Data->ResourceList && Data->LastUsedFrame!=Data->ResourceList->GetCurrentFrame()) {
            Data->SetAsMRU();
            Data->LastUsedFrame=Data->ResourceList->GetCurrentFrame();
        }
        if(Data->TouchedMipmapLevel>mipmap_level)
            Data->TouchedMipmapLevel=(signed char)mipmap_level;
    }
}
FABLE_FORCEINLINE bool CGraphicInfoFrame::GetTexture(const CTexture** texture,unsigned long mipmap_level,unsigned long flags) const
{
    if(!Data.IsNull() && Data->Frame.FrameData) {
        if(!(flags&8)) TouchFrame(mipmap_level);
        if(mipmap_level<Data->CurrentMipmapLevel) {
            Data->BeginLoading(mipmap_level,flags);
            if((flags&16) && Data->FileData.Data) goto load_frame;
        }
        *texture=Data->Frame.FrameData->GetTexture();
        return true;
    }
load_frame:
    if(Data.IsNull()) {
        if(flags&4) return false;
        Data.Reset(new CFrameData(this));
    }
    Data->BeginLoading(mipmap_level,flags);
    if((flags&1) && InternalGetFrameForcedLoading(flags)) {
        *texture=Data->Frame.FrameData->GetTexture();
        return true;
    }
    return false;
}

const CTexture* FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(const C3DMeshMaterial* material,
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces,
    unsigned long mipmap_level,bool& failed,const CGraphicDataBank* graphic_bank,const bool block_on_load)
{
    long graphic_index=material->GetDiffuseMapBankIndex();
    if(graphic_index==0) return graphic_bank->GetBlankTexture(CGraphicDataBank::TEXTURE_TYPE_OPAQUE);
    if(composite_surfaces) {
        const CTexture* texture=GetComposedTexture(graphic_index,*composite_surfaces,mipmap_level,false,graphic_bank);
        if(texture) return texture;
    }
    const CGraphicInfoFrame* frame_info=graphic_bank->PeekGraphicInfoFrame(graphic_index,0);
    unsigned long get_flags=0;
    if(g_pEnginePrimitiveRenderer->GetBlockOnLoad() || block_on_load) get_flags=17;
    if(frame_info) {
        const CTexture* texture;
        if(frame_info->GetTexture(&texture,mipmap_level,get_flags)) return texture;
    }
    failed=true;
    return 0;
}
const CTexture* FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetBumpTexture(const C3DMeshMaterial* material,
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces,
    unsigned long mipmap_level,bool& failed,const CGraphicDataBank* graphic_bank,const bool block_on_load)
{
    long graphic_index=material->GetBumpMapBankIndex();
    if(graphic_index==0) return graphic_bank->GetBlankTexture(CGraphicDataBank::TEXTURE_TYPE_BUMP);
    if(composite_surfaces) {
        const CTexture* texture=GetComposedTexture(graphic_index,*composite_surfaces,mipmap_level,true,graphic_bank);
        if(texture) return texture;
    }
    const CGraphicInfoFrame* frame_info=graphic_bank->PeekGraphicInfoFrame(graphic_index,0);
    unsigned long get_flags=0;
    if(g_pEnginePrimitiveRenderer->GetBlockOnLoad() || block_on_load) get_flags=17;
    const CTexture* texture;
    if(frame_info->GetTexture(&texture,mipmap_level,get_flags)) return texture;
    return frame_info->GetBlankTexture();
}

static void ResetControls(){renderer.BlockOnLoad=false;renderer.queries=0;forced_success=false;outstanding_file=false;load_calls=forced_calls=frame_blank_calls=composition_calls=0;blank_type=-1;last_level=last_flags=composition_level=0;composition_bump=false;composition_result=0;composition_index=-1;ctor_replacement=0;loaded_frame.PTexture.Data=&loaded_texture;forced_frame.PTexture.Data=&forced_texture;}
class Fixture {
public:CGraphicDataBank bank;CGraphicInfoFrame frames[3];C3DMeshMaterial material;
    Fixture(){bank.clock.CurrentFrame=9;bank.GraphicList.resize(3);for(int i=0;i<3;++i){bank.GraphicList[i].Bank=&bank;bank.GraphicList[i].Frames=&frames[i];frames[i].GraphicInfo=&bank.GraphicList[i];}material.DiffuseMapBankIndex=material.BumpMapBankIndex=1;}
    CGraphicInfoFrame::CFrameData* Loaded(int current=0){CGraphicInfoFrame::CFrameData*p=new CGraphicInfoFrame::CFrameData(&frames[1]);frames[1].Data.Reset(p);p->Frame.FrameData=&loaded_frame;p->CurrentMipmapLevel=(signed char)current;p->ResourceList=&bank.clock;p->ManagedTexture.Data.Reset(new CManagedTextureData);p->ManagedTexture.Data->ResourceList=&bank.clock;return p;}
    const CTexture*Diffuse(bool&failed,bool block=false,unsigned long level=1){return CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(&material,0,level,failed,&bank,block);}
    const CTexture*Bump(bool&failed,bool block=false,unsigned long level=1){return CEnginePrimitiveManagerMeshBase::GetBumpTexture(&material,0,level,failed,&bank,block);}
};
static void Run(){
    ResetControls();{Fixture f;bool failed=false;f.material.DiffuseMapBankIndex=f.material.BumpMapBankIndex=0;assert(f.Diffuse(failed)==&opaque_blank&&blank_type==0&&!failed);assert(f.Bump(failed)==&bump_blank&&blank_type==4&&!failed);assert(renderer.queries==0&&load_calls==0);}
    ResetControls();{Fixture f;bool failed=false;f.material.DiffuseMapBankIndex=99;assert(f.Diffuse(failed)==0&&failed&&renderer.queries==1);}
    ResetControls();{Fixture f;bool failed=false;CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >surfaces;composition_result=&composed_texture;assert(CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(&f.material,&surfaces,2,failed,&f.bank,false)==&composed_texture&&!failed);assert(composition_calls==1&&!composition_bump&&composition_level==2&&renderer.queries==0);assert(CEnginePrimitiveManagerMeshBase::GetBumpTexture(&f.material,&surfaces,3,failed,&f.bank,false)==&composed_texture&&!failed&&composition_bump);}
    ResetControls();{Fixture f;bool failed=false;CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >surfaces;composition_index=2;f.frames[2].Data.Reset(new CGraphicInfoFrame::CFrameData(&f.frames[2]));f.frames[2].Data->Frame.FrameData=&loaded_frame;f.frames[2].Data->CurrentMipmapLevel=0;assert(CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(&f.material,&surfaces,1,failed,&f.bank,false)==&loaded_texture&&!failed&&composition_calls==1);}
    ResetControls();{Fixture f;bool failed=false;CGraphicInfoFrame::CFrameData*p=f.Loaded();assert(f.Diffuse(failed)==&loaded_texture&&!failed&&load_calls==0);assert(p->TouchedFrame==9&&p->TouchedMipmapLevel==1&&p->ManagedTexture.Data->touches==1&&p->ManagedTexture.Data->LastUsedFrame==9&&p->touches==0);}
    ResetControls();{Fixture f;bool failed=false;f.Loaded(3);assert(f.Diffuse(failed,true)==&loaded_texture&&!failed&&load_calls==1&&forced_calls==0&&last_flags==17);}
    ResetControls();{Fixture f;bool failed=false;CGraphicInfoFrame::CFrameData*p=f.Loaded(3);outstanding_file=true;assert(f.Diffuse(failed)==&loaded_texture&&!failed&&load_calls==1&&forced_calls==0&&last_flags==0);assert(p->touches==1);}
    ResetControls();{Fixture f;bool failed=false;f.Loaded(3);outstanding_file=true;forced_success=true;renderer.BlockOnLoad=true;assert(f.Diffuse(failed)==&forced_texture&&!failed&&load_calls==2&&forced_calls==1&&last_flags==17&&last_level==1);}
    ResetControls();{Fixture f;bool failed=false;f.Loaded(3);outstanding_file=true;assert(f.Diffuse(failed,true)==0&&failed&&load_calls==2&&forced_calls==1&&last_flags==17);}
    ResetControls();{Fixture f;bool failed=true;f.Loaded(3);outstanding_file=true;assert(f.Bump(failed,true)==&frame_blank&&failed&&frame_blank_calls==1&&load_calls==2&&forced_calls==1);}
    ResetControls();{Fixture f;bool failed=false;int before=ctor_calls;assert(f.Diffuse(failed)==0&&failed&&ctor_calls==before+1&&load_calls==1&&forced_calls==0);}
    ResetControls();{Fixture f;bool failed=false;forced_success=true;assert(f.Diffuse(failed,true)==&forced_texture&&!failed&&load_calls==1&&forced_calls==1);}
    ResetControls();{Fixture f;bool failed=false;assert(f.Bump(failed)==&frame_blank&&!failed&&frame_blank_calls==1);}
    ResetControls();{Fixture f;const CTexture*out=&sentinel_texture;int before=ctor_calls;assert(!f.frames[1].GetTexture(&out,1,4)&&out==&sentinel_texture&&ctor_calls==before&&load_calls==0);}
    ResetControls();{Fixture f;CGraphicInfoFrame::CFrameData*p=f.Loaded();const CTexture*out=0;assert(f.frames[1].GetTexture(&out,1,8)&&out==&loaded_texture&&p->TouchedFrame==0&&p->ManagedTexture.Data->touches==0&&load_calls==0);}
    ResetControls();{Fixture f;CGraphicInfoFrame::CFrameData*p=f.Loaded(3);p->TouchedFrame=9;p->TouchedMipmapLevel=0;f.frames[1].TouchFrame(1);assert(p->ManagedTexture.Data->touches==0&&p->touches==0&&p->TouchedMipmapLevel==0);p->TouchedMipmapLevel=1;f.frames[1].TouchFrame(1);assert(p->ManagedTexture.Data->touches==1&&p->touches==1&&p->TouchedMipmapLevel==1);f.frames[1].TouchFrame(1);assert(p->ManagedTexture.Data->touches==1&&p->touches==1);}
    ResetControls();{Fixture f;ctor_replacement=new CGraphicInfoFrame::CFrameData(&f.frames[1]);int before=frame_destroyed;const CTexture*out=&sentinel_texture;assert(!f.frames[1].GetTexture(&out,1,0)&&out==&sentinel_texture&&frame_destroyed==before+1&&ctor_replacement==0);}
    ResetControls();{Fixture f;CResourceList managed_clock,frame_clock;managed_clock.CurrentFrame=11;frame_clock.CurrentFrame=13;CGraphicInfoFrame::CFrameData*p=f.Loaded(3);p->ResourceList=&frame_clock;p->ManagedTexture.Data->ResourceList=&managed_clock;p->TouchedMipmapLevel=-1;f.frames[1].TouchFrame(1);assert(p->TouchedFrame==9&&p->TouchedMipmapLevel==1&&p->LastUsedFrame==13&&p->ManagedTexture.Data->LastUsedFrame==11&&p->touches==1&&p->ManagedTexture.Data->touches==1);}
    assert(live_frames==0&&live_managed==0);
}
}

static void Initialize(N3DPrimitive::CStaticBlock*blocks,C3DMeshMaterial*materials,int materialBase){
    for(int i=0;i<4;++i){blocks[i].MaterialIndex=i;blocks[i].ChangeFlags=3;blocks[i].DiffuseTexture=&unchanged;blocks[i].BumpTexture=&unchanged;materials[i].ID=materialBase+i;}
}
int main(){
    int cases=0;mutate=false;
    for(int count=0;count<=4;++count)for(int diffuse=0;diffuse<2;++diffuse)for(int bump=0;bump<2;++bump)for(int pattern=0;pattern<64;++pattern)for(int failure=0;failure<4;++failure){
        N3DPrimitive::CStaticBlock blocks[4];C3DMeshMaterial materials[4];Initialize(blocks,materials,10);
        for(int i=1;i<4;++i)blocks[i].ChangeFlags=(unsigned char)((pattern>>(2*(i-1)))&3);
        C3DPrimitive2 primitive;primitive.StaticBlocks=blocks;primitive.StaticBlockCount=count;primitive.RecordedFrame=123;
        C3DMesh2 mesh;mesh.Materials=materials;C3DTextureStages stages;stages.DiffuseTextureStage=diffuse?0:-1;stages.BumpTextureStage=bump?2:-1;stages.UseBlankTextures=false;
        calls.clear();failureMask=failure==0?0:failure==1?1:failure==2?2:0xffffffffu;expectedLoad=(pattern&1)!=0;
        bool actual=CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph(primitive,mesh,stages,7,&bank,expectedLoad);
        std::vector<Call> expected;CTexture* expectedDiffuse[4];CTexture* expectedBump[4];bool failed=false;
        for(int i=0;i<4;++i){expectedDiffuse[i]=&unchanged;expectedBump[i]=&unchanged;}
        if(diffuse){expectedDiffuse[0]=&textures[expected.size()];expected.push_back(Call('D',10));}
        if(bump){expectedBump[0]=&textures[expected.size()];expected.push_back(Call('B',10));}
        for(int i=1;i<count;++i){
            if(blocks[i].ChangeFlags&1){if(diffuse){expectedDiffuse[i]=&textures[expected.size()];expected.push_back(Call('D',10+i));}else failed=true;}
            if((blocks[i].ChangeFlags&2)&&bump){expectedBump[i]=&textures[expected.size()];expected.push_back(Call('B',10+i));}
        }
        assert(calls.size()==expected.size());
        for(size_t i=0;i<expected.size();++i){assert(calls[i].Kind==expected[i].Kind&&calls[i].Material==expected[i].Material);if(failureMask&(1u<<i))failed=true;}
        assert(actual==!failed&&primitive.RecordedFrame==-1);
        for(int i=0;i<4;++i)assert(blocks[i].DiffuseTexture==expectedDiffuse[i]&&blocks[i].BumpTexture==expectedBump[i]);
        ++cases;
    }
    N3DPrimitive::CStaticBlock a[4],b[4],c[4],d[4];C3DMeshMaterial ma[4],mb[4],mc[4],unused[4];Initialize(a,ma,10);Initialize(b,mb,20);Initialize(c,mc,30);Initialize(d,unused,40);
    C3DPrimitive2 primitive;primitive.StaticBlocks=a;primitive.StaticBlockCount=1;primitive.RecordedFrame=123;
    C3DMesh2 mesh;mesh.Materials=ma;C3DTextureStages stages;stages.DiffuseTextureStage=0;stages.BumpTextureStage=0;stages.UseBlankTextures=true;
    c[1].ChangeFlags=3;d[2].ChangeFlags=2;
    bBlocks=b;cBlocks=c;dBlocks=d;bMaterials=mb;cMaterials=mc;activePrimitive=&primitive;activeMesh=&mesh;mutate=true;calls.clear();failureMask=0;expectedLoad=true;
    bool actual=CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph(primitive,mesh,stages,7,&bank,true);
    assert(actual&&primitive.RecordedFrame==-1&&calls.size()==4);
    assert(calls[0].Kind=='D'&&calls[0].Material==10&&calls[1].Kind=='B'&&calls[1].Material==10);
    assert(calls[2].Kind=='D'&&calls[2].Material==21&&calls[3].Kind=='B'&&calls[3].Material==32);
    assert(a[0].DiffuseTexture==&textures[0]&&a[0].BumpTexture==&textures[1]);
    assert(b[0].DiffuseTexture==&unchanged&&b[0].BumpTexture==&unchanged&&c[0].BumpTexture==&unchanged);
    assert(c[1].DiffuseTexture==&textures[2]&&c[1].BumpTexture==&unchanged&&d[2].BumpTexture==&textures[3]);
    assert(cases==5120);
    TextureHelperTest::Run();
    puts("00ba48f0_TEST PASS");return 0;
}