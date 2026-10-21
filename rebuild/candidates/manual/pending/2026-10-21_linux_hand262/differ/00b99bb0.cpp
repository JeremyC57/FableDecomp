// Grade: DIFFER(461v475) -- retail SSE distance block is an inlined inline-assembly block (box and camera-position pointers homed to [esp+8] and the dead camera arg slot, reloaded into ecx/edx, result via movss [esp+0xc] + fld, no aligned frame); intrinsics force an and-esp aligned ebp frame. Lane gap: inline-asm SSE.
// CEngineInternalPrimitiveRepeatedStaticMesh::IsInsideFrustum -- retail 0x00B99BB0, 480 bytes. Distance cull (box-to-camera distance vs
// fade scale * max draw distance; SSE or x87 clamp path), then the 4-plane n-vertex frustum test.
// Callees: none. Globals: g_pEngineGlobals 0x01436E80 (+0x88), g_bSSEAvailable 0x013D2880, 0.0f @0x0122DEDC.
// Layout: engine/CEngineInternalPrimitiveRepeatedStaticMesh.h (BoundingBox +0x58, +0x18 max draw distance), engine/CEngineCamera.h
// (CameraDesc +0x14, position at +0x54 of the camera, FrustrumPlanes +0x1c0). Shape: __fastcall(self, edx, camera, ignoreFrustum). Leaf: IsInsideFrustum
#include <math.h>
#pragma intrinsic(sqrt)
#include <xmmintrin.h>
#include "engine/CEngineInternalPrimitiveRepeatedStaticMesh.h"
#include "engine/CEngineCamera.h"

struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox {
    C3DVector Min;                                 // +0x00
    C3DVector Max;                                 // +0x0c
    float GetDistanceSquared(const C3DVector& p) const;
};
struct CPlane { C3DVector Normal; float D; };

extern unsigned char g_bSSEAvailable;              // 0x013D2880
struct CEngineGlobals {
    unsigned char _pad[0x88];
    float MeshFadeDistanceScale;                   // +0x88
};
extern CEngineGlobals* g_pEngineGlobals;           // 0x01436E80

static __forceinline __m128 LoadVector(const C3DVector& v)
{
    return _mm_loadh_pi(_mm_load_ss(&v.Z), (const __m64*)&v.X);
}

__forceinline float C3DBoundingBox::GetDistanceSquared(const C3DVector& p) const
{
    if (g_bSSEAvailable) {
        __m128 mn = LoadVector(Min);
        __m128 mx = LoadVector(Max);
        __m128 pos = LoadVector(p);
        __m128 d = _mm_sub_ps(_mm_min_ps(_mm_max_ps(pos, mn), mx), pos);
        d = _mm_mul_ps(d, d);
        __m128 t = _mm_add_ps(_mm_shuffle_ps(d, d, 0xb0), d);
        t = _mm_add_ss(_mm_shuffle_ps(t, t, 3), d);
        float result;
        _mm_store_ss(&result, t);
        return result;
    }
    C3DVector c = p;
    if (c.X < Min.X) c.X = Min.X; else if (c.X > Max.X) c.X = Max.X;
    if (c.Y < Min.Y) c.Y = Min.Y; else if (c.Y > Max.Y) c.Y = Max.Y;
    if (c.Z < Min.Z) c.Z = Min.Z; else if (c.Z > Max.Z) c.Z = Max.Z;
    float dx = p.X - c.X;
    float dy = p.Y - c.Y;
    float dz = p.Z - c.Z;
    return dz * dz + dy * dy + dx * dx;
}

static __forceinline const C3DBoundingBox& Box(const CEngineInternalPrimitiveRepeatedStaticMesh* self) { return *(const C3DBoundingBox*)self->BoundingBox; }
static __forceinline float MaxDrawDistance(const CEngineInternalPrimitiveRepeatedStaticMesh* self) { return *(const float*)self->_pad_0x18; }
static __forceinline const C3DVector& CameraPosition(const CEngineCamera& c) { return *(const C3DVector*)(c.CameraDesc + 0x40); }
static __forceinline const CPlane& FrustrumPlane(const CEngineCamera& c, int i) { return ((const CPlane*)c.FrustrumPlanes)[i]; }

bool __fastcall IsInsideFrustum(CEngineInternalPrimitiveRepeatedStaticMesh* self, int, const CEngineCamera& camera, bool ignoreFrustum)
{
    float maxDistance = g_pEngineGlobals->MeshFadeDistanceScale * MaxDrawDistance(self);
    const C3DBoundingBox& box = Box(self);
    if ((float)sqrt(box.GetDistanceSquared(CameraPosition(camera))) > maxDistance)
        return false;
    if (ignoreFrustum)
        return true;
    for (int i = 0; i < 4; ++i) {
        const CPlane& plane = FrustrumPlane(camera, i);
        float x = plane.Normal.X > 0.0f ? box.Min.X : box.Max.X;
        float y = plane.Normal.Y > 0.0f ? box.Min.Y : box.Max.Y;
        float z = plane.Normal.Z > 0.0f ? box.Min.Z : box.Max.Z;
        if (x * plane.Normal.X + y * plane.Normal.Y + z * plane.Normal.Z > plane.D)
            return false;
    }
    return true;
}
