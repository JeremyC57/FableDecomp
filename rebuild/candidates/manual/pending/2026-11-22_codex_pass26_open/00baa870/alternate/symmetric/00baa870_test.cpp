#define FABLE_RETAIL_ORIGINAL_PRIMITIVE_TO_D3D_PASS26
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION_SQUARE_PAREN_PASS26
#define FABLE_RETAIL_C3DVECTOR_DIV_ZSPRITE_OWNER_GROUP_PASS26
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_CMATRIX3X4_DATA_UNION
#define FABLE_RETAIL_CMATRIX3X4_PARTICLE_POINT
#define FABLE_RETAIL_CMATRIX3X4_ZSPRITE_SOURCE_GEOMETRY_PASS26
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_VIEWPORT_FLOAT_TEMPORARY_PASS26
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_GETDIMENTIONS
#define FABLE_RETAIL_C3DBOUNDINGBOX_CENTER_MIN_FIRST
#define FABLE_RETAIL_C3DBOUNDINGBOX_SETMINMAX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_CMATRIX3X4_PRETRANSLATE
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY_XYZ
#define FABLE_RETAIL_CMATRIX3X4_POSTSCALE_XYZ
#define FABLE_RETAIL_CENGINEFADEDISTANCE_INLINE_CTOR
#define FABLE_RETAIL_CPRETRANSPOSEDBONEMATRIX_SET
#define FABLE_RETAIL_CPRETRANSPOSEDBONEMATRIX_ZSPRITE_CONSTRUCTOR
#include "fable_compiler.h"
#include "engine/C3DVector.h"
#include "engine/C3DBoundingBox.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C2DBoxF.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineFadeDistance.h"
#include "engine/CPreTransposedBoneMatrix.h"
#include "engine/EPrimitiveType.h"
#include "engine/GFCos.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int Scenario;
#define CHECK(expr) do { if (!(expr)) { printf("scenario %u line %d: %s\n", Scenario, __LINE__, #expr); exit(1); } } while (0)

enum Stage {
    ST_LOAD, ST_LEVEL, ST_GET, ST_BILLBOARD_ENABLED, ST_SHADOW, ST_LIGHTING, ST_BILLBOARD,
    ST_RELEASE, ST_SHADER_PUSH, ST_LAYOUT_PUSH, ST_CAPTURE, ST_VERTEX, ST_PIXEL, ST_PREPARE, ST_QUERY,
    ST_QUERY_START, ST_STREAM, ST_TEXTURE, ST_INDEX, ST_REALISE, ST_DRAW, ST_QUERY_END,
    ST_RESTORE, ST_LAYOUT_POP, ST_SHADER_POP, ST_COUNT
};
static Stage events[64];
static int event_count;
static int stage_count[ST_COUNT];
static int depth;
static unsigned long expected_query;
static void Visit(Stage stage)
{
    CHECK(event_count < sizeof(events) / sizeof(events[0]));
    events[event_count++] = stage;
    ++stage_count[stage];
}

enum EAlphaTestFunction { ALPHA_TEST_GREATER };
enum ETextureUVMode { TEXTURE_UV_WRAP };
enum ETextureFilterMode { TEXTURE_FILTER_POINT };
enum ERenderPassType { RENDER_PASS_COMBINED };
class C3DMesh2 {};
class CTexture {};
class CEngineBillboard { public: CTexture* Texture; };
class CVertexShader {};
class CPixelShader {};
class CVertexBufferWin32 {
public:
    void* PVertexBuffer;
    unsigned long Stride;
    void* GetD3DVertexBuffer() { return PVertexBuffer; }
    long GetStride() const { return Stride; }
};
class CIndexBuffer {
public:
    void* PD3DIndexBuffer;
    EPrimitiveType PrimitiveType;
    EPrimitiveType GetPrimitiveType() const { return PrimitiveType; }
    void* GetD3DIndexBuffer() { return PD3DIndexBuffer; }
};

static C3DMesh2 test_mesh;
static CTexture billboard_texture, old_texture, fade_texture;
static CEngineBillboard* expected_billboard;
static const CTexture* bound_textures[2];
static int render_modes;
static unsigned long alpha_ref, expected_objects;
static bool alpha_flag, occlusion_in_progress;
static CVertexBufferWin32* expected_vertices;
static CIndexBuffer* expected_indices;
static long last_billboard_alpha;
static float last_billboard_distance, last_billboard_scale;

template<class T> class CCountedPointer {
public:
    CCountedPointer() : Pointer(0) {}
    ~CCountedPointer() { Visit(ST_RELEASE); }
    T* Get() const { return Pointer; }
    T* Pointer;
};

class C3DMeshInfo {
public:
    bool Loaded, Available;
    unsigned long BestLevel;
    unsigned long LastFlags;
    C3DBoundingSphere Sphere;
    C3DBoundingBox Box;

    bool IsLoaded() const { Visit(ST_LOAD); return Loaded; }
    unsigned long GetBestCurrentDetailLevel() const { Visit(ST_LEVEL); return BestLevel; }
    void BeginLoading(unsigned long lod, bool wait, bool recursive) const
    {
        CHECK(lod == 0 && !wait && !recursive);
        ++requests;
    }
    bool GetMesh(CCountedPointer<const C3DMesh2>& mesh, unsigned long lod, unsigned long flags)
    {
        Visit(ST_GET);
        CHECK(lod == 0);
        LastFlags = flags;
        mesh.Pointer = Available ? &test_mesh : 0;
        return Available;
    }
    C3DBoundingSphere GetBoundingSphere() const { return Sphere; }
    C3DBoundingBox GetBoundingBox() const { return Box; }
    static int requests;
};
int C3DMeshInfo::requests;

class CEnginePrimitiveRenderer {
public:
    bool Blocking;
    float DrawDistanceFactor, Minimum, Maximum;
    bool GetBlockOnLoad() const { return Blocking; }
    float GetZSpriteDrawDistanceFactor() { return DrawDistanceFactor; }
    float GetMinimumZSpriteAsMeshDistance() { return Minimum; }
    float GetMaximumZSpriteAsMeshDistance() { return Maximum; }
};

class CEngineCamera {
public:
    C3DVector Position;
    CMatrix3x4 View, Inverse;
    C2DBoxF Window;

    const CMatrix3x4& PeekNonHomogenousViewTransform() const { return View; }
    const CMatrix3x4& PeekInverseNonHomogenousViewTransform() const { return Inverse; }
    float PeekHFOV() const { return 1.2f; }
    const C3DVector& PeekCameraPos() const { return Position; }
    float PeekNonHomogenousViewToViewScaleX() const { return 2.0f; }
    float PeekNonHomogenousViewToViewScaleY() const { return 3.0f; }
    float PeekInvZToZBufferFactor() const { return 4.0f; }
    float PeekInvZToZBufferOffset() const { return 5.0f; }
    const C2DBoxF& PeekWindow() const { return Window; }
};

void CPreTransposedBoneMatrix::Set(const CMatrix3x4& matrix)
{
    Data[0][0] = matrix.E11; Data[0][1] = matrix.E21; Data[0][2] = matrix.E31; Data[0][3] = matrix.E41;
    Data[1][0] = matrix.E12; Data[1][1] = matrix.E22; Data[1][2] = matrix.E32; Data[1][3] = matrix.E42;
    Data[2][0] = matrix.E13; Data[2][1] = matrix.E23; Data[2][2] = matrix.E33; Data[2][3] = matrix.E43;
}

class CScopedShadersEnable {
public:
    CScopedShadersEnable(bool vertex, bool pixel)
    {
        CHECK(depth == 0 && vertex && pixel);
        ++depth;
        Visit(ST_SHADER_PUSH);
    }
    ~CScopedShadersEnable() { CHECK(depth == 1); --depth; Visit(ST_SHADER_POP); }
};
class CScopedVertexShaderConstantLayout {
public:
    CScopedVertexShaderConstantLayout(long layout)
    {
        CHECK(depth == 1 && layout == 2);
        ++depth;
        Visit(ST_LAYOUT_PUSH);
    }
    ~CScopedVertexShaderConstantLayout() { CHECK(depth == 2); --depth; Visit(ST_LAYOUT_POP); }
};
class CRenderStateScopedCaptureBlock {
public:
    CRenderStateScopedCaptureBlock() { CHECK(depth == 2); ++depth; Visit(ST_CAPTURE); }
    ~CRenderStateScopedCaptureBlock() { CHECK(depth == 3); --depth; render_modes = 0; Visit(ST_RESTORE); }
};

class CEngineLightingManager {
public:
    void SetEnableFadedShadowFactor(bool enable) { CHECK(enable); Visit(ST_SHADOW); }
    void UpdateGlobalLightingStates() { CHECK(events[event_count - 1] == ST_SHADOW); Visit(ST_LIGHTING); }
};
class CEngineLocalDetailGenerator {
public:
    bool Enabled;
    bool PeekZSpriteGenerationEnabled() const { Visit(ST_BILLBOARD_ENABLED); return Enabled; }
};
class CEngineBillboardGenerator {
public:
    void RenderBillboard(const C3DMesh2* mesh, CEngineBillboard* billboard, float distance, float scale, long alpha)
    {
        CHECK(mesh == &test_mesh && billboard == expected_billboard);
        CHECK(events[event_count - 1] == ST_LIGHTING);
        billboard->Texture = &billboard_texture;
        last_billboard_distance = distance;
        last_billboard_scale = scale;
        last_billboard_alpha = alpha;
        Visit(ST_BILLBOARD);
    }
};
class CEngineOcclusionManager {
public:
    void StartOcclusionTest()
    {
        CHECK(expected_query != 0xffffffff && !occlusion_in_progress);
        occlusion_in_progress = true;
        Visit(ST_QUERY_START);
    }
    void EndOcclusionTest(unsigned long query)
    {
        CHECK(occlusion_in_progress && query == expected_query && events[event_count - 1] == ST_DRAW);
        occlusion_in_progress = false;
        Visit(ST_QUERY_END);
    }
};
class CEngineResourceManager {
public:
    CTexture& GetStipleFadeTexture() { return fade_texture; }
};
class CRenderManager {
public:
    void SetAlphaTestEnable(bool value) { alpha_flag = value; render_modes |= 1; }
    void SetAlphaRef(unsigned long value) { alpha_ref = value; render_modes |= 2; }
    void SetAlphaTestFunc(EAlphaTestFunction value) { CHECK(value == ALPHA_TEST_GREATER); render_modes |= 4; }
    void SetSpecularEnable(bool value) { CHECK(value); render_modes |= 8; }
    void SetFogEnable(bool value) { CHECK(value); render_modes |= 16; }
    void SetTextureUVModeU(long stage, ETextureUVMode mode) { CHECK(stage == 1 && mode == TEXTURE_UV_WRAP); render_modes |= 32; }
    void SetTextureUVModeV(long stage, ETextureUVMode mode) { CHECK(stage == 1 && mode == TEXTURE_UV_WRAP); render_modes |= 64; }
    void SetTextureFilterModeMag(long stage, ETextureFilterMode mode) { CHECK(stage == 1 && mode == TEXTURE_FILTER_POINT); render_modes |= 128; }
    void SetTextureFilterModeMin(long stage, ETextureFilterMode mode) { CHECK(stage == 1 && mode == TEXTURE_FILTER_POINT); render_modes |= 256; }
    void AttachTextureToStage(long stage, const CTexture* texture)
    {
        CHECK(stage == stage_count[ST_TEXTURE] && stage < 2);
        bound_textures[stage] = texture;
        Visit(ST_TEXTURE);
    }
    void RealiseRenderState() { CHECK(render_modes == 511); Visit(ST_REALISE); }
};
class CTestDevice {
public:
    void SetStreamSource(unsigned long stream, const void* buffer, unsigned long offset, unsigned long stride)
    {
        CHECK(depth == 3 && stream == 0 && offset == 0);
        CHECK(buffer == expected_vertices->PVertexBuffer && stride == expected_vertices->Stride);
        Visit(ST_STREAM);
    }
    void SetIndices(const void* buffer) { CHECK(buffer == expected_indices->PD3DIndexBuffer); Visit(ST_INDEX); }
    void DrawIndexedPrimitive(D3DPRIMITIVETYPE type, long base, unsigned long startVertex, unsigned long vertices,
                              unsigned long startIndex, unsigned long triangles)
    {
        CHECK(depth == 3 && type == D3DPT_TRIANGLELIST && base == 0 && startVertex == 0 && startIndex == 0);
        CHECK(occlusion_in_progress == (expected_query != 0xffffffff));
        CHECK(vertices == expected_objects * 4 && triangles == expected_objects * 2);
        CHECK(events[event_count - 1] == ST_REALISE);
        Visit(ST_DRAW);
    }
};
typedef CTestDevice IDirect3DDevice9;

class CShaderRenderManager {
public:
    IDirect3DDevice9* GetD3DDevice();
    CRenderManager* RenderManager;
    CTestDevice* D3DDevice;
    float constants[16][4];
    unsigned long constant_mask;
    long object_count;
    const float* objects;
    const CVertexShader* vertex;
    const CPixelShader* pixel;

    void ApplyVertexShader(const CVertexShader& shader, bool enabled)
    {
        CHECK(!enabled && depth == 3);
        vertex = &shader;
        Visit(ST_VERTEX);
    }
    void ApplyPixelShader(const CPixelShader& shader)
    {
        CHECK(depth == 3 && events[event_count - 1] == ST_VERTEX);
        pixel = &shader;
        Visit(ST_PIXEL);
    }
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w)
    {
        CHECK(depth == 3 && index < 16 && !(constant_mask & (1U << index)));
        constants[index][0] = x; constants[index][1] = y; constants[index][2] = z; constants[index][3] = w;
        constant_mask |= 1U << index;
    }
    void SetUserVertexShaderConstant(unsigned long index, const C3DVector& value, float w)
    {
        SetUserVertexShaderConstant(index, value.X, value.Y, value.Z, w);
    }
    void SetUserVertexShaderConstants(unsigned long index, long count, float* entries)
    {
        if (index == 16) {
            object_count = count;
            objects = entries;
            constant_mask |= 1U << 16;
        } else {
            CHECK(index == 8 && count == 3);
            for (long i = 0; i < count; ++i)
                SetUserVertexShaderConstant(index + i, entries[4 * i], entries[4 * i + 1], entries[4 * i + 2], entries[4 * i + 3]);
        }
    }
    void PrepareForRender(unsigned long flags)
    {
        CHECK(flags == 0x200 && constant_mask == 0x11fff && render_modes == 511);
        Visit(ST_PREPARE);
    }
    void SelectVertexBuffer(CVertexBufferWin32&);
    void DrawBuffersIndexed(unsigned long, unsigned long, CIndexBuffer&, unsigned long, unsigned long, EPrimitiveType);
    void DrawBuffersIndexed(const CTexture*[], unsigned long, CVertexBufferWin32&, unsigned long, unsigned long,
                            CIndexBuffer&, unsigned long, unsigned long, EPrimitiveType);
};

class CEngineInternalPrimitiveRepeatedZSprite {
public:
    C3DMeshInfo* Mesh;
    float FadeInStart, FadeInEnd, MinDistance, MaxScale;
    CEngineFadeDistance FadeDistance;
    unsigned long AlphaRef;
    long ObjectCount;
    float* ObjectsEntries;
    CEngineFadeDistance GetTransitionFade() const;
};
class CEnginePrimitiveManager {
public:
    static bool __fastcall GetBlockOnLoad();
};
class CEnginePrimitiveManagerRepeatedZSprites : public CEnginePrimitiveManager {
public:
    C3DMeshInfo* CurrentRenderedMesh;
    CEngineBillboard BillboardTexture;
    CVertexShader VertexShader;
    CPixelShader PixelShader;
    CVertexBufferWin32 VertexBuffer;
    CIndexBuffer IndexBuffer;
    float VerticalControlPointWeightingFactor, ControlPointWeightingPower, NonStippleAlphaFactor;
    bool EnableZSpriteAlpha;
    void PrepareOcclusionTest(CEngineInternalPrimitiveRepeatedZSprite& primitive, ERenderPassType pass, unsigned long& query)
    {
        CHECK(pass == RENDER_PASS_COMBINED && primitive.ObjectCount == expected_objects && events[event_count - 1] == ST_PREPARE);
        query = expected_query;
        Visit(ST_QUERY);
    }
    void RenderFromSortList(CEngineInternalPrimitiveRepeatedZSprite&);
};

namespace NEngineInternal {
    CRenderManager* GPRenderManager;
    CShaderRenderManager* GPShaderRenderManager;
    CEngineLightingManager* GPLightingManager;
    CEngineResourceManager* GPResourceManager;
    CEnginePrimitiveRenderer* GPPrimitiveRenderer;
    CEngineCamera* GPCamera;
    CEngineOcclusionManager* GPOcclusionManager;
}
CEngineLocalDetailGenerator* g_pLocalDetailGenerator;
CEngineBillboardGenerator* g_pEngineBillboardGenerator;

static const double TWO_PI = 6.2831854820251465;

inline IDirect3DDevice9* CShaderRenderManager::GetD3DDevice()
{
    return D3DDevice;
}

inline bool FABLE_FASTCALL CEnginePrimitiveManager::GetBlockOnLoad()
{
    return NEngineInternal::GPPrimitiveRenderer->GetBlockOnLoad();
}

// Inline-budget: the original three draw helpers expand through slots 0x190, 0x1A0 and 0x148.
FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertex_buffer)
{
    GetD3DDevice()->SetStreamSource(0, vertex_buffer.GetD3DVertexBuffer(), 0, vertex_buffer.GetStride());
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersIndexed(unsigned long min_vertex_index, unsigned long no_vertices, CIndexBuffer& index_buffer,
                                                            unsigned long first_index, unsigned long no_primitives, EPrimitiveType prim_type)
{
    D3DDevice->SetIndices(index_buffer.GetD3DIndexBuffer());
    if (prim_type == PT_NULL)
        prim_type = index_buffer.GetPrimitiveType();
    RenderManager->RealiseRenderState();
    D3DDevice->DrawIndexedPrimitive(GFPrimitiveTypeToD3DPT(prim_type), 0, min_vertex_index, no_vertices, first_index, no_primitives);
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersIndexed(const CTexture* textures[], unsigned long no_textures, CVertexBufferWin32& vertex_buffer,
                                                            unsigned long min_vertex_index, unsigned long no_vertices, CIndexBuffer& index_buffer,
                                                            unsigned long first_index, unsigned long no_primitives, EPrimitiveType prim_type)
{
    SelectVertexBuffer(vertex_buffer);
    for (long c_stage = 0; static_cast<unsigned long>(c_stage) < no_textures; ++c_stage)
        RenderManager->AttachTextureToStage(c_stage, textures[c_stage]);
    DrawBuffersIndexed(min_vertex_index, no_vertices, index_buffer, first_index, no_primitives, prim_type);
}

// Same-TU callee (landed 0x00BAA460, defined earlier in the retail source file and called out of line there): its real body, kept
// out of line, so c2 knows the hidden result's address does not escape.
#ifdef _MSC_VER
__declspec(noinline)
#endif
CEngineFadeDistance CEngineInternalPrimitiveRepeatedZSprite::GetTransitionFade() const
{
    CEngineFadeDistance fade(FadeInStart, FadeInEnd);
    float min_mesh_dist = NEngineInternal::GPPrimitiveRenderer->GetMinimumZSpriteAsMeshDistance();
    float max_mesh_dist = NEngineInternal::GPPrimitiveRenderer->GetMaximumZSpriteAsMeshDistance();
    float clamped_dist = fade.MaxDrawDistance < min_mesh_dist ? min_mesh_dist :
        (fade.MaxDrawDistance > max_mesh_dist ? max_mesh_dist : fade.MaxDrawDistance);
    fade.FadeBeginDistance -= fade.MaxDrawDistance - clamped_dist;
    fade.MaxDrawDistance = clamped_dist;
    return fade;
}

void CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList(CEngineInternalPrimitiveRepeatedZSprite& primitive)
{
    if (!primitive.Mesh->IsLoaded()) {
        primitive.Mesh->BeginLoading(0, false, false);
        return;
    }
    if (primitive.Mesh->GetBestCurrentDetailLevel() != 0)
        primitive.Mesh->BeginLoading(0, false, false);
    if (CurrentRenderedMesh != primitive.Mesh) {
        CCountedPointer<const C3DMesh2> mesh;
        if (!primitive.Mesh->GetMesh(mesh, 0, GetBlockOnLoad() ? 0x11 : 0))
            return;
        if (g_pLocalDetailGenerator->PeekZSpriteGenerationEnabled()) {
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
            g_pEngineBillboardGenerator->RenderBillboard(mesh.Get(), &BillboardTexture, primitive.MinDistance, primitive.MaxScale, primitive.AlphaRef);
        }
        CurrentRenderedMesh = primitive.Mesh;
    }
    CScopedShadersEnable enable_shaders(true, true);
    CScopedVertexShaderConstantLayout layout(2);
    CRenderStateScopedCaptureBlock block;
    NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
    NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShader);

    C3DBoundingSphere mesh_sphere = primitive.Mesh->GetBoundingSphere();
    CMatrix3x4 object_to_texture_matrix = NEngineInternal::GPCamera->PeekNonHomogenousViewTransform();
    object_to_texture_matrix.SetTranslateOnly(0.0f, 0.0f, 0.0f);
    object_to_texture_matrix.PreTranslate(-mesh_sphere.Center);
    object_to_texture_matrix.PostScale((1.0f / mesh_sphere.Radius) * 0.5f,
                                       (1.0f / mesh_sphere.Radius) * -0.5f, 0.0f);
    object_to_texture_matrix.PostTranslate(0.5f, 0.5f, 0.0f);

    CMatrix3x4 inv_view_mat = NEngineInternal::GPCamera->PeekInverseNonHomogenousViewTransform();
    inv_view_mat.SetTranslateOnly(0.0f, 0.0f, 0.0f);
    C3DVector back_vec_os(0.0f, 1.0f, 0.0f);
    inv_view_mat.TransformPoint(back_vec_os);
    back_vec_os.Z = 0.0f;
    back_vec_os.Normalise();

    C3DVector base_cp_object_space(0.0f, 0.0f, 0.0f);
    C3DVector top_cp_object_space(0.0f, 0.0f, primitive.Mesh->GetBoundingBox().GetCenter().Z +
                                             primitive.Mesh->GetBoundingBox().GetDiagonal().Z);
    C3DVector back_cp_object_space = back_vec_os * mesh_sphere.Radius;
    C3DVector front_cp_object_space = -back_vec_os * mesh_sphere.Radius;
    C3DVector sphere_center_object_space = mesh_sphere.GetCenter();

    C3DVector base_cp_texture_space;
    C3DVector top_cp_texture_space;
    C3DVector back_cp_texture_space;
    C3DVector front_cp_texture_space;
    object_to_texture_matrix.TransformPoint(base_cp_object_space, base_cp_texture_space);
    object_to_texture_matrix.TransformPoint(top_cp_object_space, top_cp_texture_space);
    object_to_texture_matrix.TransformPoint(back_cp_object_space, back_cp_texture_space);
    object_to_texture_matrix.TransformPoint(front_cp_object_space, front_cp_texture_space);

    float cp_pair_weight_v = fabs(base_cp_texture_space.Y - top_cp_texture_space.Y) * VerticalControlPointWeightingFactor;
    float cp_pair_weight_h = fabs(back_cp_texture_space.Y - front_cp_texture_space.Y);
    cp_pair_weight_v = pow(cp_pair_weight_v, ControlPointWeightingPower);
    cp_pair_weight_h = pow(cp_pair_weight_h, ControlPointWeightingPower);
    float total = cp_pair_weight_v + cp_pair_weight_h;
    cp_pair_weight_h /= total;
    cp_pair_weight_v /= total;
    C3DVector control_point1_object_space = top_cp_object_space * cp_pair_weight_v + back_cp_object_space * cp_pair_weight_h;
    C3DVector control_point2_object_space = base_cp_object_space * cp_pair_weight_v + front_cp_object_space * cp_pair_weight_h;
    C3DVector control_point1_texture_space = top_cp_texture_space * cp_pair_weight_v + back_cp_texture_space * cp_pair_weight_h;
    C3DVector control_point2_texture_space = base_cp_texture_space * cp_pair_weight_v + front_cp_texture_space * cp_pair_weight_h;

    float u_radius = primitive.Mesh->GetBoundingBox().GetDiagonal().GetMagnitude() * (1.0f / mesh_sphere.Radius) * 0.5f;
    float u_left = 0.0f > base_cp_texture_space.X - u_radius ? 0.0f : base_cp_texture_space.X - u_radius;
    float u_right = 1.0f < base_cp_texture_space.X + u_radius ? 1.0f : base_cp_texture_space.X + u_radius;

    CEngineFadeDistance near_fade = primitive.GetTransitionFade();
    float world_space_radius = mesh_sphere.Radius * 0.01f;
    float half_fov = NEngineInternal::GPCamera->PeekHFOV() * (float)(0.5 / TWO_PI);
    float clip_z = GFCos(half_fov) * (near_fade.FadeBeginDistance - world_space_radius);

    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, NEngineInternal::GPCamera->PeekCameraPos(), clip_z);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, NEngineInternal::GPCamera->PeekNonHomogenousViewToViewScaleX(),
        NEngineInternal::GPCamera->PeekNonHomogenousViewToViewScaleY(), NEngineInternal::GPCamera->PeekInvZToZBufferFactor(), NEngineInternal::GPCamera->PeekInvZToZBufferOffset());
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, cp_pair_weight_v, cp_pair_weight_h, 1.0f / 256.0f, NonStippleAlphaFactor * 0.5f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, control_point1_object_space / mesh_sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, control_point2_object_space / mesh_sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(5, sphere_center_object_space / mesh_sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(6, control_point1_texture_space, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(7, u_left, u_right - u_left, 1.0f / (control_point2_texture_space.Y - control_point1_texture_space.Y), 0.0f);
    CPreTransposedBoneMatrix view_matrix(NEngineInternal::GPCamera->PeekNonHomogenousViewTransform());
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(8, 3, &view_matrix.Data[0][0]);

    float sts = 32.0f;
    C2DBoxF window = NEngineInternal::GPCamera->PeekWindow();
    // Inferred: retail keeps float extents and the double viewport offset (0x00BAB33C, 0x00BAB355).
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(
        0xb, window.GetWidth() / 2.0f / sts, -window.GetHeight() / 2.0f / sts,
        (window.TLX + window.BRX) / 2.0f / sts + 0.5 / sts,
        (window.TLY + window.BRY) / 2.0f / sts + 0.5 / sts);

    float draw_dist_factor = NEngineInternal::GPPrimitiveRenderer->GetZSpriteDrawDistanceFactor();
    CEngineFadeDistance far_fade;
    far_fade.FadeBeginDistance = draw_dist_factor * primitive.FadeDistance.FadeBeginDistance;
    far_fade.MaxDrawDistance = draw_dist_factor * primitive.FadeDistance.MaxDrawDistance;

    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(
        0xc, 1.0f / (near_fade.MaxDrawDistance - near_fade.FadeBeginDistance),
        -(near_fade.FadeBeginDistance * (1.0f / (near_fade.MaxDrawDistance - near_fade.FadeBeginDistance))),
        (1.0f / (far_fade.MaxDrawDistance - far_fade.FadeBeginDistance)) * -1.0f,
        far_fade.FadeBeginDistance * (1.0f / (far_fade.MaxDrawDistance - far_fade.FadeBeginDistance)) + 1.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(0x10, primitive.ObjectCount, (float*)primitive.ObjectsEntries);
    const CTexture* texture[2] = { BillboardTexture.Texture, &NEngineInternal::GPResourceManager->GetStipleFadeTexture() };
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(EnableZSpriteAlpha);
    NEngineInternal::GPRenderManager->SetAlphaRef(primitive.AlphaRef);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    NEngineInternal::GPRenderManager->SetSpecularEnable(true);
    NEngineInternal::GPRenderManager->SetFogEnable(true);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x200);
    unsigned long occlusion_handle;
    PrepareOcclusionTest(primitive, RENDER_PASS_COMBINED, occlusion_handle);
    if (occlusion_handle != 0xffffffff)
        NEngineInternal::GPOcclusionManager->StartOcclusionTest();
    NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(texture, 2, VertexBuffer, 0, primitive.ObjectCount * 4, IndexBuffer, 0,
                                                               primitive.ObjectCount * 2, PT_NULL);
    if (occlusion_handle != 0xffffffff)
        NEngineInternal::GPOcclusionManager->EndOcclusionTest(occlusion_handle);
}

static void Expect(double actual, double expected)
{
    CHECK(fabs(actual - expected) < 0.004 * (1.0 + fabs(expected)));
}
static void VerifyConstants(const CShaderRenderManager& shader, const CEngineCamera& camera,
                            const CEnginePrimitiveRenderer& renderer, const CEnginePrimitiveManagerRepeatedZSprites& manager,
                            const CEngineInternalPrimitiveRepeatedZSprite& primitive, double half_width, double half_height)
{
    const C3DBoundingSphere& sphere = primitive.Mesh->Sphere;
    double scale = 0.5 / sphere.Radius;
    const CMatrix3x4& view = camera.View;
    const double back_length = sqrt(view.E12 * view.E12 + view.E22 * view.E22);
    const double back_x = view.E12 / back_length;
    const double back_y = view.E22 / back_length;
    const double base_x = 0.5 - scale * (view.E11 * sphere.Center.X + view.E21 * sphere.Center.Y + view.E31 * sphere.Center.Z);
    const double base_y = 0.5 + scale * (view.E12 * sphere.Center.X + view.E22 * sphere.Center.Y + view.E32 * sphere.Center.Z);
    const double top_x = base_x + 4.0 * view.E31 * scale;
    const double top_y = base_y - 4.0 * view.E32 * scale;
    const double horiz_x = 0.5 * (view.E11 * back_x + view.E21 * back_y);
    const double horiz_y = 0.5 * (view.E12 * back_x + view.E22 * back_y);
    const double back_texture_y = base_y - horiz_y;
    const double front_texture_y = base_y + horiz_y;
    const double wv = pow(fabs(top_y - base_y) * manager.VerticalControlPointWeightingFactor, double(manager.ControlPointWeightingPower));
    const double wh = pow(fabs(front_texture_y - back_texture_y), double(manager.ControlPointWeightingPower));
    const double vertical = wv / (wv + wh);
    const double horizontal = wh / (wv + wh);
    const double point1_x = top_x * vertical + (base_x + horiz_x) * horizontal;
    const double point1_y = top_y * vertical + back_texture_y * horizontal;
    const double point2_y = base_y * vertical + front_texture_y * horizontal;
    const double u_radius = sqrt(half_width * half_width + half_height * half_height + 4.0) * scale;
    double left = base_x - u_radius;
    double right = base_x + u_radius;
    if (left < 0.0) left = 0.0;
    if (right > 1.0) right = 1.0;
    double near_begin = primitive.FadeInStart;
    if (primitive.FadeInEnd < renderer.Minimum)
        near_begin += renderer.Minimum - primitive.FadeInEnd;
    if (primitive.FadeInEnd > renderer.Maximum)
        near_begin += renderer.Maximum - primitive.FadeInEnd;
    double near_slope = 1.0 / (primitive.FadeInEnd - primitive.FadeInStart);
    double far_begin = renderer.DrawDistanceFactor * primitive.FadeDistance.FadeBeginDistance;
    double far_slope = 1.0 / (renderer.DrawDistanceFactor * primitive.FadeDistance.MaxDrawDistance - far_begin);
    const double reference[13][4] = {
        { camera.Position.X, camera.Position.Y, camera.Position.Z, (near_begin - 0.01 * sphere.Radius) * cos(0.6) },
        { 2, 3, 4, 5 },
        { vertical, horizontal, 1.0 / 256.0, manager.NonStippleAlphaFactor / 2.0 },
        { back_x * horizontal, back_y * horizontal, 4.0 * vertical / sphere.Radius, 0 },
        { -back_x * horizontal, -back_y * horizontal, 0, 0 },
        { sphere.Center.X / sphere.Radius, sphere.Center.Y / sphere.Radius, sphere.Center.Z / sphere.Radius, 0 },
        { point1_x, point1_y, 0, 0 },
        { left, right - left, 1.0 / (point2_y - point1_y), 0 },
        { view.E11, view.E21, view.E31, 7 },
        { view.E12, view.E22, view.E32, 8 },
        { view.E13, view.E23, view.E33, 9 },
        { 10.0 / 64, -17.0 / 64, 15.0 / 64, 24.0 / 64 },
        { near_slope, -near_begin * near_slope, -far_slope, 1.0 + far_begin * far_slope },
    };
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 4; ++j)
            Expect(shader.constants[i][j], reference[i][j]);
}
static void Run(unsigned int conditions, int fade_case, int clamp_case, bool tilt_and_yaw)
{
    bool loaded = (conditions & 1) != 0;
    bool best = (conditions & 2) != 0;
    bool cached = (conditions & 4) != 0;
    bool available = (conditions & 8) != 0;
    bool generate = (conditions & 16) != 0;
    bool blocking = (conditions & 32) != 0;
    bool query = (conditions & 64) != 0;
    bool alpha_enabled = (conditions & 128) != 0;

    depth = event_count = C3DMeshInfo::requests = render_modes = 0;
    memset(stage_count, 0, sizeof(stage_count));
    expected_query = query ? 0x123 : 0xffffffff;
    expected_objects = (fade_case + clamp_case) % 6;

    C3DMeshInfo mesh, previous_mesh;
    mesh.Loaded = loaded;
    mesh.Available = available;
    mesh.BestLevel = best ? 0 : 2;
    mesh.LastFlags = 0x1234;
    float x = clamp_case == 0 || clamp_case == 3 ? 0.0f : (clamp_case == 1 ? 2.4f : -2.4f);
    mesh.Sphere = C3DBoundingSphere(C3DVector(x, 0.7f, 1.2f), 4.0f);
    float half_extent = clamp_case == 3 ? 4.0f : 0.5f;
    mesh.Box.SetMinMax(C3DVector(-half_extent, -half_extent, 0.0f), C3DVector(half_extent, half_extent, 4.0f));
    CEnginePrimitiveRenderer renderer;
    renderer.Blocking = blocking;
    renderer.DrawDistanceFactor = 1.25f + 0.5f * clamp_case;
    renderer.Minimum = 15.0f;
    renderer.Maximum = 45.0f;

    CEngineCamera camera;
    camera.Position = C3DVector(2.0f, 7.0f, 9.0f);
    camera.View.SetIdentity();
    camera.View.E22 = 0.8f;
    camera.View.E23 = 0.6f;
    camera.View.E32 = -0.6f;
    camera.View.E33 = 0.8f;
    camera.View.SetTranslateOnly(7.0f, 8.0f, 9.0f);
    camera.Inverse.SetIdentity();
    camera.Inverse.E22 = 0.8f;
    camera.Inverse.E23 = -0.6f;
    camera.Inverse.E32 = 0.6f;
    camera.Inverse.E33 = 0.8f;
    if (tilt_and_yaw) {
        camera.View.E11 = 0.6f;
        camera.View.E12 = 0.64f;
        camera.View.E13 = 0.48f;
        camera.View.E21 = -0.8f;
        camera.View.E22 = 0.48f;
        camera.View.E23 = 0.36f;
        camera.Inverse.E11 = 0.6f;
        camera.Inverse.E12 = -0.8f;
        camera.Inverse.E21 = 0.64f;
        camera.Inverse.E22 = 0.48f;
        camera.Inverse.E31 = 0.48f;
        camera.Inverse.E32 = 0.36f;
    }
    camera.Inverse.SetTranslateOnly(4.0f, 6.0f, 7.0f);
    camera.Window.TLX = 2;
    camera.Window.TLY = 3;
    camera.Window.BRX = 12;
    camera.Window.BRY = 20;

    CRenderManager render;
    CTestDevice device;
    CShaderRenderManager shader;
    shader.RenderManager = &render;
    shader.D3DDevice = &device;
    shader.constant_mask = 0;
    CEngineLightingManager lighting;
    CEngineLocalDetailGenerator local_detail;
    local_detail.Enabled = generate;
    CEngineBillboardGenerator billboard;
    CEngineOcclusionManager occlusion;
    CEngineResourceManager resources;
    NEngineInternal::GPRenderManager = &render;
    NEngineInternal::GPShaderRenderManager = &shader;
    NEngineInternal::GPLightingManager = &lighting;
    NEngineInternal::GPResourceManager = &resources;
    NEngineInternal::GPPrimitiveRenderer = &renderer;
    NEngineInternal::GPCamera = &camera;
    NEngineInternal::GPOcclusionManager = &occlusion;
    g_pLocalDetailGenerator = &local_detail;
    g_pEngineBillboardGenerator = &billboard;

    CEnginePrimitiveManagerRepeatedZSprites manager;
    manager.CurrentRenderedMesh = cached ? &mesh : &previous_mesh;
    manager.BillboardTexture.Texture = &old_texture;
    expected_billboard = &manager.BillboardTexture;
    manager.VertexBuffer.PVertexBuffer = &mesh;
    manager.VertexBuffer.Stride = 28;
    manager.IndexBuffer.PD3DIndexBuffer = &renderer;
    manager.IndexBuffer.PrimitiveType = PT_TRIANGLE_LIST;
    expected_vertices = &manager.VertexBuffer;
    expected_indices = &manager.IndexBuffer;
    manager.VerticalControlPointWeightingFactor = 0.75f + 0.25f * clamp_case;
    manager.ControlPointWeightingPower = 1.0f + 0.5f * fade_case;
    manager.NonStippleAlphaFactor = 0.52f;
    manager.EnableZSpriteAlpha = alpha_enabled;

    float entries[20] = { 1, 2, 3, 4 };
    CEngineInternalPrimitiveRepeatedZSprite primitive;
    primitive.Mesh = &mesh;
    primitive.AlphaRef = 127;
    primitive.MinDistance = 12.0f;
    primitive.MaxScale = 1.75f;
    primitive.FadeInEnd = fade_case == 0 ? 10.0f : (fade_case == 1 ? 30.0f : 50.0f);
    primitive.FadeInStart = primitive.FadeInEnd - 4.0f;
    primitive.FadeDistance = CEngineFadeDistance(90.0f, 110.0f);
    primitive.ObjectCount = expected_objects;
    primitive.ObjectsEntries = entries;

    manager.RenderFromSortList(primitive);
    bool replaced = loaded && !cached && available;
    bool drawn = loaded && (cached || available);
    bool regenerated = replaced && generate;
    CHECK(stage_count[ST_LOAD] == 1 && events[0] == ST_LOAD);
    CHECK(stage_count[ST_LEVEL] == int(loaded) && C3DMeshInfo::requests == int(!loaded || !best));
    CHECK(stage_count[ST_GET] == int(loaded && !cached));
    CHECK(mesh.LastFlags == (loaded && !cached ? (blocking ? 0x11 : 0) : 0x1234));
    CHECK(stage_count[ST_RELEASE] == stage_count[ST_GET]);
    CHECK(stage_count[ST_BILLBOARD_ENABLED] == int(replaced));
    CHECK(manager.CurrentRenderedMesh == (replaced || cached ? &mesh : &previous_mesh));
    CHECK(stage_count[ST_BILLBOARD] == int(regenerated));
    CHECK(stage_count[ST_LIGHTING] == regenerated + drawn && stage_count[ST_SHADOW] == stage_count[ST_LIGHTING]);
    CHECK(stage_count[ST_SHADER_PUSH] == int(drawn) && stage_count[ST_SHADER_POP] == int(drawn));
    CHECK(stage_count[ST_LAYOUT_PUSH] == int(drawn) && stage_count[ST_LAYOUT_POP] == int(drawn));
    CHECK(stage_count[ST_CAPTURE] == int(drawn) && stage_count[ST_RESTORE] == int(drawn));
    CHECK(stage_count[ST_VERTEX] == int(drawn) && stage_count[ST_PIXEL] == int(drawn));
    CHECK(stage_count[ST_DRAW] == int(drawn) && stage_count[ST_QUERY] == int(drawn) && stage_count[ST_PREPARE] == int(drawn));
    CHECK(stage_count[ST_QUERY_START] == int(drawn && query) && stage_count[ST_QUERY_END] == int(drawn && query));
    CHECK(stage_count[ST_TEXTURE] == 2 * drawn && stage_count[ST_STREAM] == int(drawn) && stage_count[ST_INDEX] == int(drawn));
    CHECK(depth == 0 && render_modes == 0 && !occlusion_in_progress);
    CHECK(manager.BillboardTexture.Texture == (regenerated ? &billboard_texture : &old_texture));
    if (regenerated) {
        CHECK(last_billboard_distance == primitive.MinDistance && last_billboard_scale == primitive.MaxScale);
        CHECK(last_billboard_alpha == long(primitive.AlphaRef));
    }
    if (drawn) {
        CHECK(bound_textures[0] == manager.BillboardTexture.Texture && bound_textures[1] == &fade_texture);
        CHECK(shader.object_count == long(expected_objects) && shader.objects == entries);
        CHECK(shader.pixel == &manager.PixelShader && shader.vertex == &manager.VertexShader);
        CHECK(alpha_ref == primitive.AlphaRef && alpha_flag == alpha_enabled);
        CHECK(events[event_count - 3] == ST_RESTORE && events[event_count - 2] == ST_LAYOUT_POP &&
              events[event_count - 1] == ST_SHADER_POP);
        VerifyConstants(shader, camera, renderer, manager, primitive, half_extent, half_extent);
    } else {
        CHECK(shader.constant_mask == 0);
    }
}
unsigned long GFastOneOverSqrtTable[128];
int main()
{
    CHECK(GFPrimitiveTypeToD3DPT(PT_POINT_LIST) == D3DPT_POINTLIST);
    CHECK(GFPrimitiveTypeToD3DPT(PT_LINE_LIST) == D3DPT_LINELIST);
    CHECK(GFPrimitiveTypeToD3DPT(PT_LINE_STRIP) == D3DPT_LINESTRIP);
    CHECK(GFPrimitiveTypeToD3DPT(PT_TRIANGLE_LIST) == D3DPT_TRIANGLELIST);
    CHECK(GFPrimitiveTypeToD3DPT(PT_TRIANGLE_STRIP) == D3DPT_TRIANGLESTRIP);
    CHECK(GFPrimitiveTypeToD3DPT(PT_TRIANGLE_FAN) == D3DPT_TRIANGLEFAN);
    for (unsigned int i = 0; i < 128; ++i) {
        float number = (i < 64 ? 2.0f : 1.0f) * (1.0f + (i & 63) / 64.0f);
        float reciprocal_root = 1.0f / sqrt(number);
        unsigned int bits;
        memcpy(&bits, &reciprocal_root, sizeof(bits));
        GFastOneOverSqrtTable[i] = (bits & 0x00ffffffUL) >> 15;
    }
    for (unsigned int conditions = 0; conditions < 256; ++conditions)
        for (int fade = 0; fade < 3; ++fade)
            for (int clamp = 0; clamp < 4; ++clamp)
                for (int orientation = 0; orientation < 2; ++orientation) {
                    ++Scenario;
                    Run(conditions, fade, clamp, orientation != 0);
                }
    CHECK(Scenario == 6144);
    puts("00baa870_TEST PASS");
    return 0;
}
