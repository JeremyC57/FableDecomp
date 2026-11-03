// CEngineLightingManager::UpdateShadowedSpotLights() -- pick the shadowed spot light nearest the camera (and the runner-up); the nearest one becomes the current shadowed spot light (its colour faded out by how close the runner-up is), and every shadowed spot light is also queued as an ordinary local light (the nearest one scaled by the fade)
// Grade: DIFFER(1028v1028) -- same length and shape; register allocation only: retail keeps `nearest` in edi (mirrored at [esp+20h]) and `second` in memory [esp+1Ch], ours puts nearest in ebp and second in edi, which re-colours the parameter-copy temporaries (edi v edx) after loop 2 (17 probes: declaration orders, combined declarator, nested if, array, if/else orientation).
// Leaf: ?UpdateShadowedSpotLights@CEngineLightingManager@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B47BC0, 1028 bytes, esp frame 0x2C, plain `ret`.
// Name: Ego_r.pdb + FableWin.pdb ?UpdateShadowedSpotLights@CEngineLightingManager@@AAEXXZ (private).
// Callees: CDynamicObjectHeap<72,16>::CreateMoreBlocks 0x00B48F90, CEngineLocalLightElement ctor 0x00B44E30,
//   vector<CManagerLocalLight*>::_M_insert_overflow 0x00B49DF0 (declared as an explicit specialisation so VC calls it out of line, as retail).
// Globals: g_pEngineCamera 0x01436EA0, LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight 0x01436FB0; 1e30f 0x012A22A8, 0.25 0x01238010, 4.0 0x012724C8 (literals).
// Needs hdr/stlport/fable_containers.h (FABLE_RETAIL_STLP_TAG_CTORS: the push_back tag temporary is not zero-filled).
#define CEngineLightingManager CEngineLightingManagerLayout
#include "engine/CEngineLightingManager.h"
#undef CEngineLightingManager
#include "engine/C3DVector.h"
#include "engine/CEngineCamera.h"
#define FABLE_RETAIL_STLP_TAG_CTORS
#include "engine/CArray.h"
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float s) const { return CRGBFloatColour(R * s, G * s, B * s, A * s); }
    CRGBFloatColour& operator*=(float s) { R *= s; G *= s; B *= s; A *= s; return *this; }
};
class CEngineLocalLightElement { public:
    C3DVector WorldPos;
    float Radius;
    float RadiusSquared;
    float SpotRadius;
    CRGBFloatColour Colour;
    float AttenuationDistSquared;
    float AttenuationConstant;
    unsigned long ChannelMask;
    CEngineLocalLightElement(const C3DVector& pos, float radius, float spotRadius, const CRGBFloatColour& colour, unsigned long channelMask);
};
#pragma pack(push, 1)
class CEngineLightingManagerSpotlightParams { public:
    bool Active;
    C3DVector Pos;
    C3DVector Vector;
    unsigned char _pad_0x19[3];
    float Radius;
    float SpotRadius;
    float ConeWidth;
    CRGBFloatColour Colour;
};
#pragma pack(pop)
class CEngineCameraDesc { public:
    float Window[4];
    float RotationMatrix[12];
    C3DVector WorldPosition;
};
extern CEngineCamera* g_pEngineCamera;   // 0x01436EA0

class CDynamicObjectHeapBase { public:
    virtual ~CDynamicObjectHeapBase();
    long AllocCount;
    long TotalAllocatorBlocks;
    bool RefCountReleaseEnabled;
    CDynamicObjectHeapBase* HeapNext;
private:
    virtual void Release() = 0;
};
template <long ObjectSize, long BlockCount> class CDynamicObjectHeap : public CDynamicObjectHeapBase { public:
    struct CObjectEntry { CObjectEntry* NextEntry; unsigned char Pad[ObjectSize - 4]; };
    struct CObjectAllocatorBlockEntry { CObjectEntry Blocks[BlockCount]; CObjectAllocatorBlockEntry* NextEntry; };
    CObjectEntry* FreeList;
    CObjectAllocatorBlockEntry StartBlock;
    bool AllowHeapToGrow;
    void* Alloc(long)
    {
        CObjectEntry* entry = FreeList;
        if (!entry) {
            if (AllowHeapToGrow) {
                CreateMoreBlocks();
                entry = FreeList;
            } else
                return 0;
        }
        ++AllocCount;
        FreeList = entry->NextEntry;
        return entry;
    }
private:
    virtual void Release();
    void CreateMoreBlocks();
};
extern CDynamicObjectHeap<72, 16> LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight;   // 0x01436FB0

class CEngineLightingManager : public CEngineLightingManagerLayout {
public:
    struct CManagerLocalLight {
        virtual void Delete();
        CEngineLocalLightElement LightElement;
        CManagerLocalLight* NextLocalLight;
        float ClippingScore;
        float ThemeWeight;
        bool AlreadyFoundFlag;
        CManagerLocalLight(const C3DVector& pos, float radius, float spotRadius, const CRGBFloatColour& colour, unsigned long channelMask)
            : LightElement(pos, radius, spotRadius, colour, channelMask), AlreadyFoundFlag(false) { }
        static void* __fastcall operator new(unsigned int size)
        {
            return LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight.Alloc((long)size);
        }
    };
    struct CManagerShadowedSpotLight {
        virtual void Delete();
        C3DVector Position;
        C3DVector Direction;
        float Radius;
        float SpotRadius;
        float ConeRadius;
        CRGBFloatColour Colour;
    };
private:
    void UpdateShadowedSpotLights();
};

// the vector growth member is the out-of-line one retail calls (0x00B49DF0)
namespace std {
template <> void vector<CEngineLightingManager::CManagerLocalLight*, allocator<CEngineLightingManager::CManagerLocalLight*> >::_M_insert_overflow(
    CEngineLightingManager::CManagerLocalLight** position, CEngineLightingManager::CManagerLocalLight* const& x,
    const __true_type&, size_type fillLength, bool atEnd);
}

void CEngineLightingManager::UpdateShadowedSpotLights()
{
    CEngineLightingManagerSpotlightParams& params = reinterpret_cast<CEngineLightingManagerSpotlightParams&>(CurrentShadowedSpotLightParams);
    if (!ShadowedSpotLightsEnabled) {
        params.Active = false;
        return;
    }
    C3DVector cameraPos = reinterpret_cast<CEngineCameraDesc&>(g_pEngineCamera->CameraDesc).WorldPosition;
    float nearestDistance = 1e30f;
    float secondDistance = 1e30f;
    long nearest = -1;
    long second = -1;
    for (unsigned long i = 0; i < reinterpret_cast<CArray<CManagerShadowedSpotLight*>&>(ShadowedSpotLightList).size(); ++i) {
        C3DVector offset = cameraPos - reinterpret_cast<CArray<CManagerShadowedSpotLight*>&>(ShadowedSpotLightList)[i]->Position;
        float distance = offset.X * offset.X + offset.Y * offset.Y + offset.Z * offset.Z;
        if (distance < nearestDistance) {
            nearestDistance = distance;
            nearest = i;
        }
    }
    const long nearestIndex = nearest;
    for (unsigned long i = 0; i < reinterpret_cast<CArray<CManagerShadowedSpotLight*>&>(ShadowedSpotLightList).size(); ++i) {
        C3DVector offset = cameraPos - reinterpret_cast<CArray<CManagerShadowedSpotLight*>&>(ShadowedSpotLightList)[i]->Position;
        float distance = offset.X * offset.X + offset.Y * offset.Y + offset.Z * offset.Z;
        if (i != nearestIndex && distance < secondDistance) {
            secondDistance = distance;
            second = i;
        }
    }
    if (nearestIndex >= 0) {
        CManagerShadowedSpotLight* light = reinterpret_cast<CArray<CManagerShadowedSpotLight*>&>(ShadowedSpotLightList)[nearestIndex];
        params.Active = true;
        params.Colour = light->Colour;
        params.ConeWidth = light->ConeRadius * 2.0f;
        float fade = 0.0f;
        params.Radius = light->Radius;
        params.SpotRadius = light->SpotRadius;
        params.Vector = light->Direction;
        params.Pos = light->Position;
        if (second >= 0) {
            C3DVector o1 = cameraPos - light->Position;
            float d1 = (float)sqrt(o1.X * o1.X + o1.Y * o1.Y + o1.Z * o1.Z);
            C3DVector o2 = cameraPos - reinterpret_cast<CArray<CManagerShadowedSpotLight*>&>(ShadowedSpotLightList)[second]->Position;
            float d2 = (float)sqrt(o2.X * o2.X + o2.Y * o2.Y + o2.Z * o2.Z);
            fade = (d1 / (d1 + d2) - 0.25) * 4.0;
            if (fade < 0.0f)
                fade = 0.0f;
            else if (fade > 1.0f)
                fade = 1.0f;
            params.Colour *= 1.0f - fade;
        }
        for (unsigned long i = 0; i < reinterpret_cast<CArray<CManagerShadowedSpotLight*>&>(ShadowedSpotLightList).size(); ++i) {
            float scale = (i == nearestIndex) ? fade : 1.0f;
            CManagerShadowedSpotLight* spot = reinterpret_cast<CArray<CManagerShadowedSpotLight*>&>(ShadowedSpotLightList)[i];
            reinterpret_cast<CArray<CManagerLocalLight*>&>(LocalLightList).push_back(
                new CManagerLocalLight(spot->Position, spot->Radius, spot->SpotRadius, spot->Colour * scale, 0xFFFFFFFF));
        }
    } else
        params.Active = false;
}
