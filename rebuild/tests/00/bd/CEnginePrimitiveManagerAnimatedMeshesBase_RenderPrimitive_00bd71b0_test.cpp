// Complete production parent body, with observed external-call fixtures. No native layout claim.
// Exact representable geometry/fade values; integer wire state uses int, not host-sized long.
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
static bool bad=false;
static int transformCalls,debugCalls;static bool boneView;
static int setupCalls,meshCalls,textureCalls,generatedCalls,simulationCalls,lightCalls,hiddenCalls,clipCalls,clothCalls,runningCalls;
static bool allowMesh=true,wantOccluded=false,occlusion=true,persistent=true,clothEnabled=true,running=true,shadow=true;
static float effectValue=1.0f;
static int baseAlpha=255;
static unsigned int hiddenMask;
static unsigned long loadFlags;
enum ERendered {RENDERED_NOTHING=0,RENDERED_BOUNDING_VOLUME=1,RENDERED_OBJECT=2};
enum EEngineRenderLayer { TEST_LAYER=0 };
enum EEngineOcclusionLayer { OCCLUSION_LAYER_NONE=-1,OCCLUSION_LAYER_MAIN=0 };
enum EEnginePrimitiveType {ENGINE_PRIMITIVE_ANIMATING_MESH=9,ENGINE_PRIMITIVE_MORPHED_ANIMATING_MESH=11,ENGINE_PRIMITIVE_2D_MORPHED_ANIMATING_MESH=38};
enum EEngineMeshEffect {MESH_EFFECT_OUTLINE_SMOOTH=0,MESH_EFFECT_OUTLINE=1,MESH_EFFECT_DISTANCE_FADE=2};
enum ERenderMode {RENDER_MODE_UNTEXTURED=1,RENDER_MODE_TEXTURE_FOG=4,RENDER_MODE_TEXTURE_ENV_FOG=7,RENDER_MODE_TEXTURE_DISABLED_BUMP_FOG=8,RENDER_MODE_TEXTURE_FOG_SHADOW=9,RENDER_MODE_TEXTURE_ENV_FOG_SHADOW=10,RENDER_MODE_TEXTURE_DP3_BUMP_FOG_SHADOW=11,RENDER_MODE_TEXTURE_FOG_SPOT_SHADOW=12,RENDER_MODE_TEXTURE_ENV_FOG_SPOT_SHADOW=13};
enum ESubPrimitiveType {SUBPRIM_TYPE_BOUNDING_VOLUME=7,SUBPRIM_TYPE_OUTLINE_STENCIL_SMOOTH=8,SUBPRIM_TYPE_OUTLINE_SMOOTH_FILL=13,SUBPRIM_TYPE_OUTLINE_STENCIL=10,SUBPRIM_TYPE_OUTLINE_ALPHA_STENCIL_MASK_SMOOTH=9,SUBPRIM_TYPE_OUTLINE_FILL=14};
enum EBankLoadPriority {BANK_LOAD_PRIORITY_NORMAL=1};
enum ELightingManagerShadowMode {TEST_SHADOW=2};
class C3DVector {public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} float GetMagnitude()const{return(float)sqrt(X*X+Y*Y+Z*Z);} };
class CMatrix3x4 {public:void TransformPoint(C3DVector& v)const{v.X+=2;v.Y+=4;v.Z+=8;}};
class C3DBoundingSphere {public:C3DVector Center;float Radius;C3DBoundingSphere():Center(1,2,2),Radius(2){}float GetRadius()const{return Radius;}C3DVector GetCenter()const{return Center;}};
template<class T>class CCountedPointer {T*p;public:CCountedPointer():p(0){}void Set(T*q){p=q;}T*Get()const{return p;}T*operator->()const{return p;}T&operator*()const{return*p;}};
template<class T>class CIVCountedPointer {T*p;public:CIVCountedPointer():p(0){}void Set(T*q){p=q;}T*Get()const{return p;}T&operator*()const{return*p;}};
class CEngineMeshEffect {public:EEngineMeshEffect MeshEffect;EEngineMeshEffect GetEffect()const{return MeshEffect;}};
class CEngineMeshEffectOutline:public CEngineMeshEffect{};
class CEngineMeshEffectDistanceFade:public CEngineMeshEffect {public:float GetAlphaAtDistance(float v)const{if(v!=8.0f)bad=true;return effectValue;}};
class C3DMeshMaterial {public:bool transparent,booleanAlpha;unsigned long maps;bool IsTransparent()const{return transparent;}bool IsBooleanAlpha()const{return booleanAlpha;}unsigned long GetMapFlags()const{return maps;}};
class C3DPrimitive2 {public:int material;bool cloth;long GetMaterialIndex()const{return material;}bool HasCloth()const{return cloth;}};
class C3DMesh2 {public:C3DPrimitive2 prims[3];C3DMeshMaterial materials[3];bool containsCloth;bool ContainsCloth()const{return containsCloth;}unsigned long GetPrimitiveCount()const{return 3;}C3DPrimitive2*GetPrimitive(unsigned long i)const{if(i>=3){bad=true;i=0;}return const_cast<C3DPrimitive2*>(&prims[i]);}const C3DMeshMaterial*GetMaterial(long i)const{if(i<0||i>=3){bad=true;i=0;}return &materials[i];}};
static C3DMesh2*testMesh;
class C3DMeshInfo {public:C3DBoundingSphere GetBoundingSphere()const{return C3DBoundingSphere();}bool GetMesh(CCountedPointer<const C3DMesh2>&out,unsigned long lod,unsigned long flags)const{++meshCalls;loadFlags=flags;if(lod!=2)bad=true;if(allowMesh)out.Set(testMesh);return allowMesh;}void BeginLoadingTextures(EBankLoadPriority p)const{++textureCalls;if(p!=BANK_LOAD_PRIORITY_NORMAL)bad=true;}};
class CEngineLocalLightArray{};
static CEngineLocalLightArray testLights;
class CEngineRenderInfoMeshBase {public:CEngineLocalLightArray*Lights;const C3DMesh2*Mesh;unsigned long LOD;void SetMesh(const C3DMesh2*mesh,unsigned long lod){Mesh=mesh;LOD=lod;}};
class CEngineRenderInfoAnimatedMesh:public CEngineRenderInfoMeshBase{public:CMatrix3x4 ObjectTransform;float Distance;};
class CEngineInternalPrimitiveBase {public:virtual~CEngineInternalPrimitiveBase(){}unsigned long EnginePrimitiveFlags;};
class CEngineInternalPrimitiveAnimatedMesh:public CEngineInternalPrimitiveBase {public:C3DMeshInfo*MeshInfo;float Scale;CMatrix3x4 ObjectTransform;int ThemeChannelDesc;CIVCountedPointer<CEngineMeshEffect>MeshEffect;EEnginePrimitiveType type;CEngineRenderInfoAnimatedMesh info;
 float GetCurrentInterpolate()const{return .5f;}void CalcInterpolatedObjectTransform(CMatrix3x4&m,float v,bool cache)const{++transformCalls;if(&m!=&info.ObjectTransform||v!=.5f||!cache)bad=true;}bool SetupMesh(float d,float r){++meshCalls;if(d!=16||r!=10.5f)bad=true;if(allowMesh){info.Mesh=testMesh;info.LOD=2;}return allowMesh;}
 bool IsPersistent()const{return persistent;}void SetupOcclusionInfo(EEngineOcclusionLayer l){++setupCalls;if(l!=0)bad=true;}bool IsOccluded(EEngineOcclusionLayer l){if(l!=0)bad=true;return wantOccluded;}CEngineRenderInfoAnimatedMesh*GetRenderInfo(){return &info;}void InitialiseSimulation(const C3DMeshInfo*p,const C3DMesh2&m){++simulationCalls;if(p!=MeshInfo||&m!=testMesh)bad=true;}EEnginePrimitiveType GetType()const{return type;}unsigned char GetAlphaIncludingDistanceFade(float d){if(d!=16)bad=true;return(unsigned char)baseAlpha;}bool IsSimulationRunning(unsigned long i)const{++runningCalls;if(i>=3)bad=true;return running;}};
class CEngineInternalPrimitiveMorphInfo {unsigned char hidden[3];public:const unsigned char*GetHiddenMaterials(const C3DMesh2&m){++hiddenCalls;if(&m!=testMesh)bad=true;for(int i=0;i<3;++i)hidden[i]=(hiddenMask>>i)&1;return hidden;}};
class CEngineInternalPrimitiveMorphedAnimatedMesh:public CEngineInternalPrimitiveAnimatedMesh {public:CEngineInternalPrimitiveMorphInfo Morph;};
class CEngineInternalPrimitive2DMorphedAnimatedMesh:public CEngineInternalPrimitiveAnimatedMesh {public:CEngineInternalPrimitiveMorphInfo Morph;};
class CShadowClipVolume {public:bool ClipSphere(const C3DVector&v,float r)const{++clipCalls;if(v.X!=3||v.Y!=6||v.Z!=10||r!=10.5f)bad=true;return shadow;}};
class CShadowRendererSceneData {public:CShadowClipVolume RecieverClipVolume;};
class CEngineShadowRenderer {public:CShadowRendererSceneData PeekSceneData()const{return CShadowRendererSceneData();}bool PeekRenderEnabled()const{return true;}};
class CEnginePrimitiveRendererSharedData {public:float GetDrawDistanceFactor()const{return 2;}};
class CEngineClothRenderer {public:bool GetEnable()const{++clothCalls;return clothEnabled;}};
class CEnginePrimitiveRenderer {CEngineClothRenderer cloth;public:float BoundingRadiusFix;float GetBoundingRadiusAnimated(const C3DMeshInfo&,float)const;CEngineClothRenderer&GetClothRenderer(){return cloth;}};
class CEngineLightingManager {public:ELightingManagerShadowMode PeekGlobalShadowMode()const{return TEST_SHADOW;}};
class CEngineSubPrimitiveChain{};
class CEnginePrimitiveManager;
class CEngineSubPrimitiveBase {public:virtual~CEngineSubPrimitiveBase(){} };
class CEngineSubPrimitiveBoundingBox:public CEngineSubPrimitiveBase {public:CEngineInternalPrimitiveAnimatedMesh*primitive;explicit CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveAnimatedMesh*p):primitive(p){}};
class CEngineSubPrimitiveAnimatedMesh:public CEngineSubPrimitiveBase {public:CEngineInternalPrimitiveAnimatedMesh*primitive;C3DPrimitive2*mesh;ERenderMode mode;bool cloth;CEngineSubPrimitiveAnimatedMesh(CEngineInternalPrimitiveAnimatedMesh*p,C3DPrimitive2*m,ERenderMode r,bool c):primitive(p),mesh(m),mode(r),cloth(c){}};
struct Recorded {int type,material,alpha;unsigned long flags;ERenderMode mode;bool cloth;};
static std::vector<Recorded>events;
static CEngineInternalPrimitiveAnimatedMesh*testPrimitive;
static CEnginePrimitiveManager*testManager;
static CEngineSubPrimitiveChain*testChain;
class CEngineSubPrimitiveRenderer {public:void AddSubPrimitive(ESubPrimitiveType type,CEngineSubPrimitiveBase*p,float distance,CEnginePrimitiveManager*manager){if(distance!=16||manager!=testManager)bad=true;Recorded e;e.type=type;e.material=-1;e.alpha=-1;e.flags=0;e.mode=RENDER_MODE_UNTEXTURED;e.cloth=false;if(type==SUBPRIM_TYPE_BOUNDING_VOLUME){if(static_cast<CEngineSubPrimitiveBoundingBox*>(p)->primitive!=testPrimitive)bad=true;}else{CEngineSubPrimitiveAnimatedMesh*s=static_cast<CEngineSubPrimitiveAnimatedMesh*>(p);if(s->primitive!=testPrimitive)bad=true;e.material=(int)s->mesh->material;e.mode=s->mode;e.cloth=s->cloth;}events.push_back(e);delete p;}};
namespace NEngineInternal {static CEngineSubPrimitiveRenderer*GPSubPrimitiveRenderer;static CEnginePrimitiveRendererSharedData*GPPrimitiveRendererSharedData;static CEnginePrimitiveRenderer*GPPrimitiveRenderer;static CEngineShadowRenderer*GPShadowRenderer;static CEngineLightingManager*GPLightingManager;}
static long GFFloatToLongNear(float n){long k=(long)n;float f=n-k;if(f>0.5f||(f==0.5f&&(k&1)))++k;return k;}
class CEnginePrimitiveRenderUtil {public:unsigned long CalculateSubPrimitiveRenderFlags(bool,bool,bool,unsigned long,bool);};
class CEnginePrimitiveManager:public CEnginePrimitiveRenderUtil {public:bool enabled,block;bool GetEnable()const{return enabled;}bool GetBlockOnLoad()const{return block;}void AddBoundingVolume(CEngineSubPrimitiveBase*p,float d){NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_BOUNDING_VOLUME,p,d,this);}void AddSubPrimitive(CEngineSubPrimitiveAnimatedMesh*p,float d,unsigned char alpha,unsigned long flags,CEngineSubPrimitiveChain&chain){if(d!=16||this!=testManager||&chain!=testChain||p->primitive!=testPrimitive)bad=true;Recorded e;e.type=100;e.material=p->mesh->material;e.alpha=alpha;e.flags=flags;e.mode=p->mode;e.cloth=p->cloth;events.push_back(e);delete p;}};
class CEnginePrimitiveManagerMeshBase:public CEnginePrimitiveManager {public:static ERenderMode GetAppropriateRenderMode(const C3DMeshMaterial*,unsigned long,unsigned long);};
class CEnginePrimitiveManagerAnimatedMeshesBase:public CEnginePrimitiveManagerMeshBase {public:ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);EEngineOcclusionLayer GetCurrentOcclusionLayer(){return OCCLUSION_LAYER_MAIN;}float CalcDistanceFromCamera(const C3DVector&v){if(v.X!=3||v.Y!=6||v.Z!=10)bad=true;return 16;}bool GetOcclusionEnable(){return occlusion;}float GetLODErrorTolerance(){return .25f;}unsigned long GetAppropriateLOD(const C3DMeshInfo*p,float d,float r,float tolerance){if(p!=testPrimitive->MeshInfo||d!=16||r!=10.5f||tolerance!=.25f)bad=true;return 2;}bool GetBoneViewEnabled()const{return boneView;}void RenderDebugBoneHierarchy(CEngineInternalPrimitiveAnimatedMesh&p){++debugCalls;if(&p!=testPrimitive)bad=true;}void AddGeneratedObjects(const CEngineInternalPrimitiveAnimatedMesh&p,const CMatrix3x4&m){++generatedCalls;if(&p!=testPrimitive||&m!=&p.info.ObjectTransform)bad=true;}CEngineLocalLightArray*FindLocalLights(const C3DVector&v,float r,int ch){++lightCalls;if(v.X!=3||v.Y!=6||v.Z!=10||r!=10.5f||ch!=3)bad=true;return &testLights;}};
inline float CEnginePrimitiveRenderer::GetBoundingRadiusAnimated(const C3DMeshInfo& mesh_info,float scale)const
{ return (mesh_info.GetBoundingSphere().Radius+mesh_info.GetBoundingSphere().GetCenter().GetMagnitude())*scale+BoundingRadiusFix; }
inline ERenderMode CEnginePrimitiveManagerMeshBase::GetAppropriateRenderMode(
    const C3DMeshMaterial* material,unsigned long modifier_flags,unsigned long subprim_render_flags)
{
    if (material->GetMapFlags() & 1) {
        unsigned long additional_maps=material->GetMapFlags() & ~1UL;
        if (subprim_render_flags & 0x200) {
            if (subprim_render_flags & 0x10) {
                if (additional_maps & 4) return RENDER_MODE_TEXTURE_ENV_FOG_SPOT_SHADOW;
                return RENDER_MODE_TEXTURE_FOG_SPOT_SHADOW;
            }
            switch (additional_maps) {
            case 2: case 6: return RENDER_MODE_TEXTURE_DP3_BUMP_FOG_SHADOW;
            case 4: return RENDER_MODE_TEXTURE_ENV_FOG_SHADOW;
            default: return RENDER_MODE_TEXTURE_FOG_SHADOW;
            }
        }
        switch (additional_maps) {
        case 2: case 6: return RENDER_MODE_TEXTURE_DISABLED_BUMP_FOG;
        case 4: return RENDER_MODE_TEXTURE_ENV_FOG;
        default: return RENDER_MODE_TEXTURE_FOG;
        }
    }
    return RENDER_MODE_UNTEXTURED;
}
inline unsigned long CEnginePrimitiveRenderUtil::CalculateSubPrimitiveRenderFlags(bool primitive_alphaed,
    bool texture_has_alpha,bool texture_has_boolean_alpha,unsigned long primitive_mod_flags,bool in_shadow_range)
{
    if((primitive_mod_flags & 0x100) && texture_has_alpha) texture_has_boolean_alpha=true;
    bool use_stipple_alpha=(primitive_mod_flags & 0x200)!=0;
    unsigned long flags=0;
    if(primitive_alphaed) {
        if(use_stipple_alpha) flags |= 0xa;
        else flags |= 5;
    }
    if(texture_has_boolean_alpha) flags |= 2;
    else if(texture_has_alpha) flags |= 5;
    if(in_shadow_range && NEngineInternal::GPShadowRenderer->PeekRenderEnabled()) {
        flags |= 0x200;
        if(NEngineInternal::GPLightingManager->PeekGlobalShadowMode()==(ELightingManagerShadowMode)2) flags |= 0x10;
    }
    if(primitive_mod_flags & 0x800) flags |= 0xc0;
    if(primitive_mod_flags & 8) flags |= 0x180;
    return flags;
}

ERendered CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitive(
    CEngineInternalPrimitiveBase& primitive_base, EEngineRenderLayer layer,
    CEngineSubPrimitiveChain& subprim_chain)
{
    if (!GetEnable()) return RENDERED_NOTHING;
    CEngineInternalPrimitiveAnimatedMesh& primitive = static_cast<CEngineInternalPrimitiveAnimatedMesh&>(primitive_base);
    CEngineRenderInfoAnimatedMesh* render_info=primitive.GetRenderInfo();
    primitive.CalcInterpolatedObjectTransform(render_info->ObjectTransform,primitive.GetCurrentInterpolate(),true);
    const CMatrix3x4& obj_transform=render_info->ObjectTransform;
    float bounding_radius=NEngineInternal::GPPrimitiveRenderer->GetBoundingRadiusAnimated(*primitive.MeshInfo,primitive.Scale);
    C3DVector center=primitive.MeshInfo->GetBoundingSphere().GetCenter();
    obj_transform.TransformPoint(center);
    EEngineOcclusionLayer occlusion_layer = GetCurrentOcclusionLayer();
    float distance = CalcDistanceFromCamera(center);
    render_info->Distance=distance;
    if (GetOcclusionEnable()) {
        if (primitive.IsPersistent()) primitive.SetupOcclusionInfo(occlusion_layer);
        if (primitive.IsOccluded(occlusion_layer)) {
            // Inline-budget: native00BD7373 calls the real ctor; other site expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            CEngineSubPrimitiveBoundingBox* subprim = new CEngineSubPrimitiveBoundingBox(&primitive);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            AddBoundingVolume(subprim, distance);
            return RENDERED_BOUNDING_VOLUME;
        }
    }
    if (!primitive.SetupMesh(distance,bounding_radius)) {
        AddBoundingVolume(new CEngineSubPrimitiveBoundingBox(&primitive),distance);
        primitive.MeshInfo->BeginLoadingTextures(BANK_LOAD_PRIORITY_NORMAL);
        return RENDERED_BOUNDING_VOLUME;
    }
    const C3DMesh2* mesh=primitive.GetRenderInfo()->Mesh;
    AddGeneratedObjects(primitive,obj_transform);
    if (mesh->ContainsCloth()) primitive.InitialiseSimulation(primitive.MeshInfo, *mesh);
    render_info->Lights = FindLocalLights(center, bounding_radius, primitive.ThemeChannelDesc);
    const unsigned char* hidden_materials = 0;
    if (primitive.GetType() == ENGINE_PRIMITIVE_MORPHED_ANIMATING_MESH) {
        CEngineInternalPrimitiveMorphedAnimatedMesh& morph_primitive = static_cast<CEngineInternalPrimitiveMorphedAnimatedMesh&>(primitive);
        hidden_materials = morph_primitive.Morph.GetHiddenMaterials(*mesh);
    }
    else if(primitive.GetType()==ENGINE_PRIMITIVE_2D_MORPHED_ANIMATING_MESH) {
        CEngineInternalPrimitive2DMorphedAnimatedMesh& morph_primitive=static_cast<CEngineInternalPrimitive2DMorphedAnimatedMesh&>(primitive);
        hidden_materials=morph_primitive.Morph.GetHiddenMaterials(*mesh);
    }
    // Inline-budget: native00BD7522..75C7 expands the actual ClipSphere helper.
    bool in_shadow_range = !(primitive.EnginePrimitiveFlags & 0x1000)
        && NEngineInternal::GPShadowRenderer->PeekSceneData().RecieverClipVolume.ClipSphere(center, bounding_radius);
    unsigned long no_primitives=mesh->GetPrimitiveCount();
    for (unsigned long c_prim = 0; c_prim < no_primitives; ++c_prim) {
        C3DPrimitive2* mesh_prim = mesh->GetPrimitive(c_prim);
        long material_index = mesh_prim->GetMaterialIndex();
        if (hidden_materials && hidden_materials[material_index]) continue;
        const C3DMeshMaterial* material = mesh->GetMaterial(material_index);
        long prim_alpha = primitive.GetAlphaIncludingDistanceFade(distance);
        if (!prim_alpha) continue;
        unsigned long render_flags = CalculateSubPrimitiveRenderFlags(prim_alpha < 255,
            material->IsTransparent(), material->IsBooleanAlpha(), primitive.EnginePrimitiveFlags, in_shadow_range);
        ERenderMode render_mode = GetAppropriateRenderMode(material, primitive.EnginePrimitiveFlags, render_flags);
        bool use_cloth = mesh_prim->HasCloth() && NEngineInternal::GPPrimitiveRenderer->GetClothRenderer().GetEnable()
            && primitive.IsSimulationRunning(c_prim);
        if (primitive.MeshEffect.Get()) {
            const CEngineMeshEffect& effect = *primitive.MeshEffect;
            switch (effect.GetEffect()) {
            case MESH_EFFECT_OUTLINE_SMOOTH: {
                const CEngineMeshEffectOutline& outline_effect = static_cast<const CEngineMeshEffectOutline&>(effect);
                NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_OUTLINE_STENCIL_SMOOTH,
                    new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth), distance, this);
                if (material->IsTransparent())
                    NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_OUTLINE_ALPHA_STENCIL_MASK_SMOOTH,
                        new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth), distance, this);
                NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_OUTLINE_SMOOTH_FILL,
                    new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth), distance, this);
                render_flags |= 0xa0;
                break;
            }
            case MESH_EFFECT_OUTLINE:
                NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_OUTLINE_STENCIL,
                    new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth), distance, this);
                NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_OUTLINE_FILL,
                    new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth), distance, this);
                render_flags |= 0xa0;
                break;
            }
        }
        CEngineSubPrimitiveAnimatedMesh* subprim = new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth);
        AddSubPrimitive(subprim, distance, static_cast<unsigned char>(prim_alpha), render_flags, subprim_chain);
    }
    if(GetBoneViewEnabled()) RenderDebugBoneHierarchy(primitive);
    return RENDERED_OBJECT;
}


static unsigned long ExpectedFlags(int alpha,const C3DMeshMaterial&m,unsigned long mod,bool inRange){unsigned long f=(alpha<255)?((mod&512)?10:5):0;if(m.booleanAlpha||((mod&256)&&m.transparent))f|=2;else if(m.transparent)f|=5;if(inRange){f|=512;f|=16;}if(mod&2048)f|=192;if(mod&8)f|=384;return f;}
static ERenderMode ExpectedMode(unsigned long maps,unsigned long flags){if(!(maps&1))return RENDER_MODE_UNTEXTURED;unsigned long rest=maps&~1UL;int table[8]={4,4,8,4,7,4,8,4};if(!(flags&512))return(ERenderMode)table[rest&7];if(flags&16)return(ERenderMode)((rest&4)?13:12);return(ERenderMode)((rest==2||rest==6)?11:(rest==4?10:9));}
int main(){
 CEngineSubPrimitiveRenderer submit;CEnginePrimitiveRendererSharedData data;CEnginePrimitiveRenderer renderer;CEngineShadowRenderer shadows;CEngineLightingManager lighting;
 NEngineInternal::GPSubPrimitiveRenderer=&submit;NEngineInternal::GPPrimitiveRendererSharedData=&data;NEngineInternal::GPPrimitiveRenderer=&renderer;NEngineInternal::GPShadowRenderer=&shadows;NEngineInternal::GPLightingManager=&lighting;
 CEnginePrimitiveManagerAnimatedMeshesBase manager;CEngineSubPrimitiveChain chain;C3DMeshInfo meshInfo;C3DMesh2 mesh;CEngineInternalPrimitiveMorphedAnimatedMesh morphA;CEngineInternalPrimitive2DMorphedAnimatedMesh morphB;CEngineMeshEffectOutline outline;CEngineMeshEffectDistanceFade fade;
 renderer.BoundingRadiusFix=.5f;testMesh=&mesh;testManager=&manager;testChain=&chain;
 unsigned int cases=0;
 for(unsigned int scenario=0;scenario<8192;++scenario){
  CEngineInternalPrimitiveAnimatedMesh& primitive=(scenario%3==2)?static_cast<CEngineInternalPrimitiveAnimatedMesh&>(morphB):static_cast<CEngineInternalPrimitiveAnimatedMesh&>(morphA);primitive.MeshInfo=&meshInfo;primitive.Scale=2;primitive.ThemeChannelDesc=3;testPrimitive=&primitive;transformCalls=debugCalls=0;boneView=(scenario&1)!=0;
  setupCalls=meshCalls=textureCalls=generatedCalls=simulationCalls=lightCalls=hiddenCalls=clipCalls=clothCalls=runningCalls=0;events.clear();bad=false;
  manager.enabled=(scenario%17)!=0;manager.block=(scenario&1)!=0;occlusion=(scenario&2)!=0;wantOccluded=(scenario%13)==0;persistent=(scenario&4)!=0;allowMesh=(scenario%19)!=0;shadow=(scenario&8)!=0;clothEnabled=(scenario&16)!=0;running=(scenario&32)!=0;
  primitive.type=(scenario%3==2)?ENGINE_PRIMITIVE_2D_MORPHED_ANIMATING_MESH:(scenario%3==1)?ENGINE_PRIMITIVE_MORPHED_ANIMATING_MESH:ENGINE_PRIMITIVE_ANIMATING_MESH;hiddenMask=(scenario>>7)&7;primitive.EnginePrimitiveFlags=((scenario&1024)?0x1000:0)|((scenario&2048)?0x100:0)|((scenario&4096)?0x200:0)|((scenario%7)?0x800:8);
  baseAlpha=(scenario%3==0)?0:((scenario%3==1)?128:255);int effect=(scenario/3)%4;effectValue=((scenario/12)%3)*.5f;
  primitive.MeshEffect.Set(0);if(effect==1||effect==2){outline.MeshEffect=(effect==1)?MESH_EFFECT_OUTLINE_SMOOTH:MESH_EFFECT_OUTLINE;primitive.MeshEffect.Set(&outline);}else if(effect==3){fade.MeshEffect=MESH_EFFECT_DISTANCE_FADE;primitive.MeshEffect.Set(&fade);}
  mesh.containsCloth=(scenario&128)!=0;
  for(int i=0;i<3;++i){mesh.prims[i].material=(i+1)%3;mesh.prims[i].cloth=((scenario>>i)&1)!=0;mesh.materials[i].transparent=((scenario>>(i+3))&1)!=0;mesh.materials[i].booleanAlpha=((scenario>>(i+6))&1)!=0;mesh.materials[i].maps=(scenario+i)&7;}
  primitive.info.Mesh=0;primitive.info.LOD=99;primitive.info.Lights=0;loadFlags=99;
  ERendered result=manager.RenderPrimitive(primitive,TEST_LAYER,chain);
  bool earlyOcclusion=manager.enabled&&occlusion&&wantOccluded;bool loaded=manager.enabled&&!earlyOcclusion&&allowMesh;
  int expectedResult=!manager.enabled?0:earlyOcclusion||!allowMesh?1:2;
  if((int)result!=expectedResult)bad=true;
  if(setupCalls!=(manager.enabled&&occlusion&&persistent?1:0)||meshCalls!=(manager.enabled&&!earlyOcclusion?1:0)||textureCalls!=(manager.enabled&&!earlyOcclusion&&!allowMesh?1:0))bad=true;
  if(generatedCalls!=(loaded?1:0)||simulationCalls!=(loaded&&mesh.containsCloth?1:0)||lightCalls!=(loaded?1:0)||hiddenCalls!=(loaded&&primitive.type!=ENGINE_PRIMITIVE_ANIMATING_MESH?1:0)||clipCalls!=(loaded&&!(primitive.EnginePrimitiveFlags&4096)?1:0))bad=true;
  if(transformCalls!=(manager.enabled?1:0)||debugCalls!=(loaded&&boneView?1:0))bad=true;
  if(manager.enabled&&primitive.info.Distance!=16)bad=true;
  if(loaded&&(primitive.info.Mesh!=&mesh||primitive.info.LOD!=2||primitive.info.Lights!=&testLights))bad=true;
  unsigned int cursor=0;int expectedClothCalls=0,expectedRunningCalls=0;
  if(expectedResult==1){if(events.size()!=1||events[0].type!=7)bad=true;cursor=1;}
  else if(expectedResult==2){
   int alpha=baseAlpha;
   for(int i=0;i<3;++i){int mat=mesh.prims[i].material;if((primitive.type!=ENGINE_PRIMITIVE_ANIMATING_MESH&&((hiddenMask>>mat)&1))||alpha==0)continue;
    bool cloth=mesh.prims[i].cloth&&clothEnabled&&running;if(mesh.prims[i].cloth){++expectedClothCalls;if(clothEnabled)++expectedRunningCalls;}
    unsigned long flags=ExpectedFlags(alpha,mesh.materials[mat],primitive.EnginePrimitiveFlags,shadow&&!(primitive.EnginePrimitiveFlags&4096));ERenderMode mode=ExpectedMode(mesh.materials[mat].maps,flags);
    int types[4],n=0;if(effect==1){types[n++]=8;if(mesh.materials[mat].transparent)types[n++]=9;types[n++]=13;flags|=160;}else if(effect==2){types[n++]=10;types[n++]=14;flags|=160;}types[n++]=100;
    for(int j=0;j<n;++j){if(cursor>=events.size()){bad=true;break;}const Recorded&e=events[cursor++];if(e.type!=types[j]||e.material!=mat||e.mode!=mode||e.cloth!=cloth)bad=true;if(e.type==100&&(e.alpha!=alpha||e.flags!=flags))bad=true;}
   }
  }
  if(cursor!=events.size()||clothCalls!=expectedClothCalls||runningCalls!=expectedRunningCalls)bad=true;
  if(bad){fprintf(stderr,"case %u failed result%d expected%d events%lu cursor%u\n",scenario,(int)result,expectedResult,(unsigned long)events.size(),cursor);return 1;}++cases;
 }
 fprintf(stderr,"%u complete-parent cases checked\n",cases);puts("00bd71cc_TEST PASS");return 0;
}