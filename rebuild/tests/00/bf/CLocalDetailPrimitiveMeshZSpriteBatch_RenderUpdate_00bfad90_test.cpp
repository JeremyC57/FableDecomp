// 00bfad90 CLocalDetailPrimitiveMeshZSpriteBatch::RenderUpdate
// Leaf: ?RenderUpdate@CLocalDetailPrimitiveMeshZSpriteBatch@@UAEXABVCEngineCamera@@_NKPBVCLocalDetailObjectCollectionType@@M@Z
// Flags: /O2 /Oy
// Inferred: retail clip-distance clamps, mesh fade and replacement flags are reconstructed from both loops.
// The individual flag enumerator names for 0x211, 0x311 and 0x1000 remain unresolved.
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_LOCAL_DETAIL_RENDER_PASS29
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_ADDSTATICMESH
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_ZSPRITE
#define FABLE_RETAIL_CENGINEFADEDISTANCE_INLINE_CTOR
#include <stdio.h>
#define FABLE_RETAIL_LOCAL_DETAIL_MESH_CTOR_PASS29
// Test-only access to the original input and output fields.
#define private public
#define protected public
#include "engine/C3DVector.h"
#include "engine/C3DMeshInfo.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "engine/CEngineFadeDistance.h"
#include "engine/CRGBColour.h"
#include "engine/CLocalDetailPrimitiveMeshZSpriteBatch.h"
#include "engine/GFFloatToLongNear.h"
#undef private
#undef protected
namespace NEngineInternal { extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; extern CEngineLocalDetailGenerator* GPLocalDetailGenerator; }
void CLocalDetailPrimitiveMeshZSpriteBatch::RenderUpdate(const CEngineCamera& camera, bool all_inside_zclip_region, unsigned long clip_flags,
                                                         const CLocalDetailObjectCollectionType* type, float interp)
{
    if (!ClipPrimitive(type, camera, all_inside_zclip_region, clip_flags)) {
        if (HasMeshPrimitiveHandles) {
            HasMeshPrimitiveHandles = false;
            for (long obj = 0; obj < ObjectCount; ++obj) {
                CObject& object = Objects[obj];
                float distance = (camera.PeekCameraPos() - object.BoundingSphere.Center).GetMagnitude();
                float zsprite_fade_end = type->PeekZSpriteFadeEnd();
                float min_zsprite_fade_end = NEngineInternal::GPPrimitiveRenderer->GetMinimumZSpriteAsMeshDistance();
                float max_zsprite_fade_end = NEngineInternal::GPPrimitiveRenderer->GetMaximumZSpriteAsMeshDistance();
                if (zsprite_fade_end < min_zsprite_fade_end) zsprite_fade_end = min_zsprite_fade_end;
                else if (zsprite_fade_end > max_zsprite_fade_end) zsprite_fade_end = max_zsprite_fade_end;
                if (distance > zsprite_fade_end)
                    object.PrimitiveHandle.Clear();
                else if (!object.PrimitiveHandle.IsNull())
                    HasMeshPrimitiveHandles = true;
            }
        }
        return;
    }
    for (long obj = 0; obj < ObjectCount; ++obj) {
        CObject& object = Objects[obj];
        C3DVector unrotated_bounding_sphere_center = type->PeekMesh()->GetBoundingSphere().Center;
        unrotated_bounding_sphere_center *= object.Scale;
        unrotated_bounding_sphere_center += object.Matrix.GetTranslation();
        float distance = (camera.PeekCameraPos() - unrotated_bounding_sphere_center).GetMagnitude();
        CEngineFadeDistance mesh_fade(type->PeekZSpriteFadeStart(), type->PeekZSpriteFadeEnd());
        float min_mesh_dist = NEngineInternal::GPPrimitiveRenderer->GetMinimumZSpriteAsMeshDistance();
        float max_mesh_dist = NEngineInternal::GPPrimitiveRenderer->GetMaximumZSpriteAsMeshDistance();
        float clamped_dist = mesh_fade.MaxDrawDistance < min_mesh_dist ? min_mesh_dist : (mesh_fade.MaxDrawDistance > max_mesh_dist ? max_mesh_dist : mesh_fade.MaxDrawDistance);
        mesh_fade.FadeBeginDistance -= mesh_fade.MaxDrawDistance - clamped_dist;
        mesh_fade.MaxDrawDistance = clamped_dist;
        long mesh_alpha = GFFloatToLongNear((distance - mesh_fade.MaxDrawDistance) * 255.0f / (mesh_fade.FadeBeginDistance - mesh_fade.MaxDrawDistance));
        mesh_alpha += NEngineInternal::GPLocalDetailGenerator->GetStippleAlphaMeshBias();
        if (mesh_alpha < 0)
            mesh_alpha = 0;
        else if (mesh_alpha > 255)
            mesh_alpha = 255;
        if (distance < mesh_fade.MaxDrawDistance) {
            if (object.PrimitiveHandle.IsNull() || NEngineInternal::GPPrimitiveRenderer->IsDrawDistanceInvalid() != 0.0f) {
                CEngineFadeDistance fade(type->PeekFadeStartBeforeVideoOptionModifier(), type->PeekFadeEndBeforeVideoOptionModifier());
                unsigned long flags = 0x11;
                flags |= 0x200;
                if (type->PeekAlphaIsBoolean())
                    flags |= 0x100;
                if ((float)type->PeekReceivesShadows() == 0.0f)
                    flags |= 0x1000;
                object.PrimitiveHandle = NEngineInternal::GPPrimitiveRenderer->AddStaticMesh(type->PeekMesh(), type->PeekShadowMesh(), object.Matrix, object.Scale, fade,
                                                                            mesh_alpha, CRGBColour(0xff, 0xff, 0xff, 0xff), type->PeekAlphaRef(),
                                                                            type->PeekAlphaMipBias(), true, flags);
                HasMeshPrimitiveHandles = true;
            } else
                NEngineInternal::GPPrimitiveRenderer->UpdateStaticMeshAlpha(object.PrimitiveHandle, mesh_alpha);
        } else
            object.PrimitiveHandle.Clear();
    }
}

namespace NEngineInternal { CEnginePrimitiveRenderer* GPPrimitiveRenderer; CEngineLocalDetailGenerator* GPLocalDetailGenerator; }
CBaseClass::CBaseClass() {} CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {} CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CCharString::CCharString():storage_(0) {} CCharString::~CCharString() {}
CEngineComponent::CEngineComponent() {} CEngineComponent::~CEngineComponent() {}
void CEngineComponent::FrameStart(const CEngineComponentParams&) {}
void CEngineComponent::FrameEnd(const CEngineComponentParams&) {}
bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&) {return true;}
void CEngineComponent::Render(const CEngineComponentRenderParams&) {}
void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::WorldFrameUpdate(bool) {} void CEngineComponent::PausedWorldFrameUpdate() {}
CCharString CEngineComponent::GetConsoleEnableFunctionName(){return CCharString();}
void CEngineComponent::OnReleaseDefaultPoolResources() {} bool CEngineComponent::OnRestoreDefaultPoolResources(){return true;}
CEngineCamera::CEngineCamera(bool) {} unsigned long CEngineCamera::PeekSceneFilterFlags()const{return 0;}
CEngineLocalDetailGenerator::CEngineLocalDetailGenerator(bool) {}
unsigned long CEngineLocalDetailGenerator::PeekSceneFilterFlags() const{return 0;}
CEnginePrimitiveRenderer::CEnginePrimitiveRenderer() {}
unsigned long CEnginePrimitiveRenderer::PeekSceneFilterFlags() const{return 0;}
CMeshDataBankEntry::CMeshDataBankEntry(unsigned long i,CMeshDataBank* b):Bank(b),Index(i) {}
CMeshDataBankEntry::~CMeshDataBankEntry(){} void CMeshDataBankEntry::FreeDeviceSpecificResources(){}
C3DMeshInfo::C3DMeshInfo(unsigned long, unsigned long i,CMeshDataBank* b):CMeshDataBankEntry(i,b){}
bool CMovableResource::CanRelocateData(long)const{return true;} unsigned long CMovableResource::Verify()const{return 123;}
void CLocalDetailPrimitiveBase::RenderUpdate(const CEngineCamera&,bool,unsigned long,const CLocalDetailObjectCollectionType*,float){}
bool CLocalDetailPrimitiveBase::NeedsRenderUpdate(){return true;} void CLocalDetailPrimitiveBase::OnPostRelocation(){}
bool CLocalDetailPrimitiveBase::ClipPrimitive(const CLocalDetailObjectCollectionType*,const CEngineCamera&,bool inside,unsigned long){return inside;}
class TestBatch:public CLocalDetailPrimitiveMeshZSpriteBatch {
public:
 void RelocateData(long,void*,void*,long){} void AddObjectsToPrimitiveRenderer(){}
 bool Load(CDataInputStream&){return true;} LOCAL_DETAIL_PRIMITIVE_TYPE GetType()const{return (LOCAL_DETAIL_PRIMITIVE_TYPE)0;}
};
static int releases,deleted,adds,updates,alpha;
static bool captured;
static unsigned long flags;
static const C3DMeshInfo *expectedMesh,*expectedShadow;
CEnginePersistentPrimitiveBase::CEnginePersistentPrimitiveBase(long t):Type(t){IVRefCount=0;}
CEnginePersistentPrimitiveBase::~CEnginePersistentPrimitiveBase(){}
void CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&){++releases;}
bool CEnginePersistentPrimitiveBase::WasRendered()const{return false;}
void CIVCountedPointeeBase::Delete()const{++deleted;}
class TestMeshPrimitive:public CEnginePersistentPrimitiveBase {
public: TestMeshPrimitive():CEnginePersistentPrimitiveBase(1){}
 void CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,
   EEnginePrimitiveStatIndex,unsigned long)const {}
};
static TestMeshPrimitive primitive;
void CEnginePrimitiveRenderer::UpdateStaticMeshAlpha(const CEnginePrimitiveHandle&,long a){++updates;alpha=a;}
CEnginePrimitiveHandle CEnginePrimitiveRenderer::AddStaticMesh(const C3DMeshInfo* mesh,const C3DMeshInfo* shadow,const CMatrix3x4& matrix,
  float scale,const CEngineFadeDistance& fade,long a,const CRGBColour& colour,long ref,float mip,bool persistent,unsigned long f){
 ++adds;alpha=a;flags=f;captured=mesh==expectedMesh&&shadow==expectedShadow&&scale==2.0f&&fade.FadeBeginDistance==10.0f&&fade.MaxDrawDistance==14.0f&&
  colour.R==255&&colour.G==255&&colour.B==255&&colour.A==255&&ref==7&&mip==0.75f&&persistent;
 CEnginePrimitiveHandle result;primitive.AddRef();result.SetData(&primitive);return result;
}
int main(){
 CEnginePrimitiveRenderer r; NEngineInternal::GPPrimitiveRenderer=&r;
 CEngineLocalDetailGenerator g(false); NEngineInternal::GPLocalDetailGenerator=&g;
 CEngineCamera camera(false); camera.CameraDesc.WorldPosition=C3DVector(0,0,0);
 C3DMeshInfo mesh(0,0,0),shadow(0,1,0); mesh.BoundingSphere.Center=C3DVector(1,0,0);expectedMesh=&mesh;expectedShadow=&shadow;
 CLocalDetailObjectCollectionType t;t.Mesh=&mesh;t.ShadowMesh=&shadow;t.PlainDataBlock.ZSpriteFadeStart=4;t.PlainDataBlock.ZSpriteFadeEnd=12;
 t.PlainDataBlock.FadeStart=10;t.PlainDataBlock.FadeEnd=14;t.PlainDataBlock.AlphaRef=7;t.PlainDataBlock.AlphaMipBias=.75f;
 for(int mask=0;mask<128;++mask)for(int band=0;band<4;++band)for(int step=0;step<21;++step){
   TestBatch batch;CLocalDetailPrimitiveMeshZSpriteBatch::CObject object;batch.Objects=&object;batch.ObjectCount=1;
   bool visible=(mask&1)!=0,live=(mask&2)!=0;batch.HasMeshPrimitiveHandles=(mask&64)!=0;
   r.DrawDistanceInvalid=(mask&4)!=0;t.PlainDataBlock.AlphaIsBoolean=(mask&8)!=0;t.PlainDataBlock.ReceivesShadows=(mask&16)!=0;
   g.StippleAlphaMeshBias=(mask&32)?130:-16;r.MinimumZSpriteAsMeshDistance=band==3?16.0f:2.0f;
   r.MaximumZSpriteAsMeshDistance=band==3?18.0f:float(6*(band+1));object.Scale=2.0f;
   object.Matrix.E41=float(step)-2;object.Matrix.E42=object.Matrix.E43=0;object.BoundingSphere.Center=C3DVector(float(step),0,0);
   if(live){primitive.AddRef();object.PrimitiveHandle.SetData(&primitive);}
   adds=updates=deleted=releases=0;alpha=-300;flags=0;captured=false;
   bool oldAny=batch.HasMeshPrimitiveHandles;batch.RenderUpdate(camera,visible,0,&t,0);
   int end=band==0?6:band==3?16:12;
   if(!visible){
     bool keep=live&&(!oldAny||step<=end);
     if((!object.PrimitiveHandle.IsNull())!=keep||batch.HasMeshPrimitiveHandles!=(oldAny&&keep)||adds||updates)return 1;
   } else if(step<end){
     if(object.PrimitiveHandle.IsNull())return 2;
     if(live&&!r.DrawDistanceInvalid){if(updates!=1||adds||batch.HasMeshPrimitiveHandles!=oldAny)return 3;}
     else{if(adds!=1||updates||!batch.HasMeshPrimitiveHandles||!captured)return 4;
       if(flags!=((t.PlainDataBlock.AlphaIsBoolean?0x311:0x211)|(t.PlainDataBlock.ReceivesShadows?0:0x1000)))return 5;}
     int numerator=255*(end-step),q=numerator/8,rem=numerator%8;
     if(rem>4||(rem==4&&(q&1)))++q;
     q+=g.StippleAlphaMeshBias;if(q<0)q=0;if(q>255)q=255;if(alpha!=q)return 6;
   }else if(!object.PrimitiveHandle.IsNull()||adds||updates)return 7;
   object.PrimitiveHandle.Clear();if(primitive.GetRefCount()!=0)return 8;
 }
 {
   TestBatch batch;
   CLocalDetailPrimitiveMeshZSpriteBatch::CObject objects[2];
   batch.Objects=objects;batch.ObjectCount=0;batch.HasMeshPrimitiveHandles=true;
   batch.RenderUpdate(camera,false,0,&t,0);
   if(batch.HasMeshPrimitiveHandles)return 9;
   batch.ObjectCount=2;batch.HasMeshPrimitiveHandles=true;r.MinimumZSpriteAsMeshDistance=2;r.MaximumZSpriteAsMeshDistance=18;
   for(int i=0;i<2;++i){objects[i].BoundingSphere.Center=C3DVector(20,0,0);primitive.AddRef();objects[i].PrimitiveHandle.SetData(&primitive);}
   releases=deleted=0;batch.RenderUpdate(camera,false,0,&t,0);
   if(!objects[0].PrimitiveHandle.IsNull()||!objects[1].PrimitiveHandle.IsNull()||batch.HasMeshPrimitiveHandles||
      releases!=1||deleted!=1||primitive.GetRefCount()!=0)return 10;
 }
 puts("00bfad90_TEST PASS");return 0;
}