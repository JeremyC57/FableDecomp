// Inferred: reconstruct the control flow and original ABI from the retail body and donor local/type evidence.
// Leaf: ?BuildDefaultSection@CLandscapeBackgroundPatch@@QAEXXZ
// Flags: /GS
// Retail-asm-helper: GFFloatToLongNear
// Retail-asm: 0x00BE6ACA `fistp dword ptr [esp+28h]` (vertex U).
// Retail-asm: 0x00BE6AEA `fistp dword ptr [esp+20h]` (vertex V).
// Retail-asm: 0x00BE6AF9 `fistp dword ptr [esp+30h]` (normal Z).
// Retail-asm: 0x00BE6B01 `fistp dword ptr [esp+18h]` (normal Y).
// Retail-asm: 0x00BE6B09 `fistp dword ptr [esp+2Ch]` (normal X).
// Retail-asm: 0x00BE6C54 `fistp dword ptr [esp+44h]` (fan normal Z).
// Retail-asm: 0x00BE6C5C `fistp dword ptr [esp+78h]` (fan normal Y).
// Retail-asm: 0x00BE6C64 `fistp dword ptr [esp+6Ch]` (fan normal X).
// Retail-asm: 0x00BE6C91 `fistp dword ptr [esp+70h]` (fan U).
// Retail-asm: 0x00BE6CA0 `fistp dword ptr [esp+7Ch]` (fan V).
// Retail-asm: 0x00BE6D8F `fistp dword ptr [esp+40h]` (edge normal Z).
// Retail-asm: 0x00BE6D97 `fistp dword ptr [esp+3Ch]` (edge normal Y).
// Retail-asm: 0x00BE6D9F `fistp dword ptr [esp+48h]` (edge normal X).
// Retail-asm: 0x00BE6DDA `fistp dword ptr [esp+50h]` (edge U).
// Retail-asm: 0x00BE6DF7 `fistp dword ptr [esp+58h]` (edge V).
#define FABLE_RETAIL_BACKGROUND_VERTEX_HELPERS_PASS28
#define FABLE_RETAIL_CGENERICVERTEX_INLINE_SETTERS
#define FABLE_RETAIL_LANDSCAPE_BUILD_DEFAULT_SECTION_PASS28
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_SUB_NODE_BASE_PASS22_ISPATCH
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_TREE_NODE_PASS22
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_ACCESSORS
#define FABLE_RETAIL_CEDGESTIPVERTEXARRAY_ACCESSORS
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#include <string.h>
#include "stlport/fable_containers.h"
#include "engine/EVertexType.h"
#include "engine/EIndexBufferFormat.h"
#include "engine/EPrimitiveType.h"
#include "engine/CEngineMap.h"
#include "engine/_quarantine/CEngineLandscapeMap.h"
#include "engine/CEngineDef.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CLandscapeBackgroundTreeNode.h"
#include "engine/CLandscapePatchTesselator.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/GFFloatToLongNear.h"

#include "engine/C3DPackedNormal.h"

inline long CLandscapeBackgroundPatch::PackUVForVertexBuffer(float uv)
{
    return GFFloatToLongNear(static_cast<float>(uv * 255.0f));
}

inline void C3DPackedNormal::Initialise(const C3DVector& normal)
{
    VectorBits = ((static_cast<unsigned long>(GFFloatToLongNear(static_cast<float>(normal.Z * 511.0f))) & 0x3ff) << 22)
               | ((static_cast<unsigned long>(GFFloatToLongNear(static_cast<float>(normal.Y * 1023.0f))) & 0x7ff) << 11)
               | (static_cast<unsigned long>(GFFloatToLongNear(static_cast<float>(normal.X * 1023.0f))) & 0x7ff);
}

inline unsigned long CLandscapeBackgroundPatch::PackVertexNormal(const C3DVector& normal)
{
    C3DPackedNormal packed_normal(normal);
    return packed_normal.GetAsPackedInt();
}

// FableWin 02CB2210 returns this vector by value. The inner CEngineMap getter returns a reference.
inline C3DVector CEngineLandscapeMap::PeekWorldPosition() const
{
    return Map->PeekWorldPosition();
}
inline long CLandscapeBackgroundPatch::PeekWorldX() const
{
    return MapX + Parent->PeekMap()->Map->PeekWorldX();
}
inline long CLandscapeBackgroundPatch::PeekWorldY() const
{
    return MapY + Parent->PeekMap()->Map->PeekWorldY();
}

void FABLE_FASTCALL CLandscapeBackgroundPatch::SetTargetVertexNormal(CTVertexLandscapeBackground& vertex, unsigned long packedNormal)
{
    long x = packedNormal & 0x7ff;
    long y = (packedNormal >> 11) & 0x7ff;
    long z = packedNormal >> 22;
    x |= (x & 0x400) ? ~0x7ffL : 0;
    y |= (y & 0x400) ? ~0x7ffL : 0;
    z |= (z & 0x200) ? ~0x3ffL : 0;
    C3DVector normal(x * (1.0f / 1023.0f), y * (1.0f / 1023.0f), z * (1.0f / 511.0f));
    vertex.NormalX = normal.X;
    vertex.NormalY = normal.Y;
    vertex.NormalZ = normal.Z;
}

namespace NEngineInternal {
    // FableWin public at 04AC8000. Retail loads the same engine definition at 01436E20.
    extern const CEngineDef* GPEngineDef;
    extern CEngineResourceManager* GPResourceManager;
    extern CEngineLandscapeRenderer* GPLandscapeRenderer;
}

// Inferred: the default section is a sea-level fan with four corners and a common center.
void CLandscapeBackgroundPatch::BuildDefaultSection()
{
    PolyCountIncDegenerates = 4;
    VertexCount = 5;
    CEngineLandscapeMemoryManager* mem_manager = NEngineInternal::GPLandscapeRenderer->MemoryManager;
    VertexBuffer = mem_manager->AllocateVertexBuffer(this, VERTEX_NULL, VertexCount,
        sizeof(CTVertexLandscapeBackground), NEngineInternal::GPResourceManager->GetVertexBufferUsage());
    CTVertexLandscapeBackground* verts = reinterpret_cast<CTVertexLandscapeBackground*>(
        VertexBuffer->Lock(0, 0, VertexBuffer->GetSize()));
    const float sea_height = NEngineInternal::GPEngineDef->SeaHeight;
    const C3DVector vt[5] = {
        C3DVector(MapX, MapY, sea_height),
        C3DVector(MapX + Width, MapY, sea_height),
        C3DVector(MapX + Width, MapY + Height, sea_height),
        C3DVector(MapX, MapY + Height, sea_height),
        C3DVector(MapX + (Width / 2), MapY + (Height / 2), sea_height)
    };
    const float vert_uvs_u[5] = {0.0f, 1.0f, 1.0f, 0.0f, 0.5f};
    const float vert_uvs_v[5] = {0.0f, 0.0f, 1.0f, 1.0f, 0.5f};
    for (long n = 0; n < 5; ++n) {
        verts[n].SetPosition(vt[n] + Parent->PeekMap()->PeekWorldPosition());
        verts[n].SetU1(PackUVForVertexBuffer(vert_uvs_u[n]));
        verts[n].SetV1(PackUVForVertexBuffer(vert_uvs_v[n]));
        SetTargetVertexNormal(verts[n],
            PackVertexNormal(C3DVector(0.0f, 0.0f, 1.0f)));
    }
    VertexBuffer->Unlock();

    const long world_x = PeekWorldX();
    const long world_y = PeekWorldY();
    EdgeStrips[2].Reset(world_x, Width, false, this);
    EdgeStrips[3].Reset(world_x, Width, false, this);
    EdgeStrips[0].Reset(world_y, Height, true, this);
    EdgeStrips[1].Reset(world_y, Height, true, this);
    const long corner_vertex_table[2][2] = {{0, 3}, {1, 2}};
    for (long side = 0; side < 4; ++side) {
        CPatchTesselationEdgeStrip::CFanBaseVertex& fan_vert = EdgeStrips[side].GetFanBaseVert(0);
        fan_vert.Normal = PackVertexNormal(C3DVector(0.0f, 0.0f, 1.0f));
        fan_vert.U = static_cast<unsigned char>(PackUVForVertexBuffer(0.5f));
        fan_vert.V = static_cast<unsigned char>(PackUVForVertexBuffer(0.5f));
        fan_vert.WorldX = static_cast<unsigned short>(world_x + (Width / 2));
        fan_vert.WorldY = static_cast<unsigned short>(world_y + (Height / 2));
        fan_vert.WorldZ = Parent->PeekMap()->PeekWorldPosition().Z + sea_height;
        fan_vert.VertexBufferIndex = 4;
        for (long vt = 0; vt < 2; ++vt) {
            CPatchTesselationEdgeStrip::CVertex& tvert = EdgeStrips[side].GetVert(vt);
            long grid_x = 0;
            long grid_y = 0;
            if (side == 2) {
                grid_x += vt;
            } else if (side == 3) {
                grid_x += vt;
                ++grid_y;
            } else if (side == 0) {
                grid_y += vt;
            } else {
                ++grid_x;
                grid_y += vt;
            }
            const long map_pos_x = MapX + grid_x * Width;
            const long map_pos_y = MapY + grid_y * Height;
            tvert.WorldX = static_cast<unsigned short>(world_x + grid_x * Width);
            tvert.WorldY = static_cast<unsigned short>(world_y + grid_y * Height);
            tvert.WorldZ = Parent->PeekMap()->PeekWorldPosition().Z + sea_height;
            tvert.Normal = PackVertexNormal(C3DVector(0.0f, 0.0f, 1.0f));
            tvert.U = static_cast<unsigned char>(PackUVForVertexBuffer(static_cast<float>(grid_x)));
            tvert.V = static_cast<unsigned char>(PackUVForVertexBuffer(static_cast<float>(grid_y)));
            tvert.FanBaseIndex = 0;
            tvert.IndexBufferIndex = static_cast<unsigned short>(side * 3);
            tvert.VertexBufferIndex = static_cast<unsigned short>(corner_vertex_table[grid_x][grid_y]);
        }
        EdgeStrips[side].ResizeVertexCount(2, 1, 0, 0, this);
    }

    const unsigned short id_table[12] = {0, 3, 4, 1, 4, 2, 0, 4, 1, 3, 2, 4};
    IndexBuffer = mem_manager->AllocateIndexBuffer(this, PolyCountIncDegenerates * 6,
        NEngineInternal::GPResourceManager->GetIndexBufferUsage(), IB_FORMAT_16, PT_TRIANGLE_LIST);
    SharedIndexBuffer = false;
    unsigned short* ids = reinterpret_cast<unsigned short*>(IndexBuffer->Lock(0, 0, 0));
    memcpy(ids, id_table, sizeof(id_table));
    IndexBuffer->Unlock();
    IsDefaultSection = true;
}