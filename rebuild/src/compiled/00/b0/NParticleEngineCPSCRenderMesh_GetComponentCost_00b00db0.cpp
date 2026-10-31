// NParticleEngine::CPSCRenderMesh::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float particleCount, float) -- vtable slot 20: a mesh renderer adds the mesh's triangles per particle, the (system-scaled) bounding area of the larger of its start/end render sizes, and triangles x materials to the render cost; a trail adds two polygons per particle and its scaled width x length
// Leaf: ?GetComponentCost@CPSCRenderMesh@NParticleEngine@@UAEXAAVCParticleSystemCost@2@ABVCParticleSystem@2@MM@Z
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00B00E47 `fld dword ptr [esp+38h]` + 0x00B00E4B `fistp dword ptr [esp+34h]` -- the engine's inline-assembly float-to-long helper (the landed FloatToLong of CPSCEmitterGeneric::GetRandomRadius 0x00B102C0), 7 sites; VC7.1 `(long)x` calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B00DB0, 1177 bytes, `ret 10h` (no catalog length: extent from the retail bytes, BOUNDS_FIX.tsv).
// Name: Ego_r.pdb ?GetComponentCost@CPSCRenderMesh@NParticleEngine@@UAEXAAVCParticleSystemCost@2@ABVCParticleSystem@2@MM@Z
//   (the slot-20 spelling of the landed CPSCLight/CPSCOrbit/CPSCSpline GetComponentCost and of CPSCRenderSprite 0x00AF90C0)
//   + the manifest row (CPSCRenderMesh::GetComponentCost).
// Callees: NParticleEngine::GetMeshBank() 0x00AEAAB0 (Ego_r.pdb + FableWin.pdb ?GetMeshBank@NParticleEngine@@YIABVCMeshDataBank@@XZ:
//   retail `mov eax,[0x013D2E0C]; ret`, sitting 0x20 after its setter 0x00AEAA90 `mov [0x013D2E0C],ecx`, the PDB pair
//   SetMeshBank/GetMeshBank; manifest "GFGetPreMainMemoryUsed" and labels "GetGlobal_013d2e0c" are propagated/descriptive --
//   NAME_FIXES.tsv); CMeshDataBank::PeekMeshInfo 0x00A24260, C3DMeshInfo::GetMesh 0x00A25E70, C3DMesh2::GetTriangleCount
//   0x00A89350 (landed names; manifest labels for all three are propagated), CParticleSystem::ScaleParticleDimensions(float&)
//   const 0x00AF8F80 (same-TU noinline body, as 0x00AF90C0), operator delete 0x00BFE9BC (the CCountedPointer release).
// Layout: Ego_r.pdb CPSCRenderMesh (100 bytes; +0x34/+0x38 StartRenderSizeX/Y/Z, +0x38 bit 13 / +0x3c EndRenderSizeX/Y/Z,
//   +0x30 TrailLengthInteger, +0x44 TrailWidth bit 8); C3DMesh2 MaterialCount at retail +0x88 (PDB +0x90, retail sits 8 bytes
//   lower, as the landed GetTriangleCount); CCountedPointer from engine/CCountedPointer.h.
// Constants: 1/2047 0x0129F65C, 20.0f 0x0122F160, 1.0f 0x0122DED8, 10.0f 0x01237F0C, pi 0x0128F604, 1/1023 0x0129EF84,
//   0.001f 0x01266300, 2.0f 0x0122DCDC, 0.5f 0x0122F59C, 2^32 0x0122DCB4 (read from the binary).
// Shape: the larger render size is selected by reference (`lea eax`) and held as three scalars (a C3DVector copy takes three
//   frame words); the area is particleCount * (r*r*pi) (spelled r*r*pi*count VC7.1 moves the constant last and reorders the
//   render-cost sum).
#include <math.h>
#include "engine/CCountedPointer.h"
#define C3DMeshInfo C3DMeshInfoLayout
#include "engine/C3DMeshInfo.h"
#undef C3DMeshInfo
#define CMeshDataBank CMeshDataBankLayout
#include "engine/CMeshDataBank.h"
#undef CMeshDataBank

class C3DMesh2 { public:
    unsigned char m_pad00[0x88];
    unsigned long MaterialCount;                     // +0x88 (Ego_r.pdb +0x90; retail C3DMesh2 sits 8 bytes lower)
    unsigned long GetTriangleCount() const;          // 0x00A89350
};
class C3DMeshInfo : public C3DMeshInfoLayout { public:
    bool GetMesh(CCountedPointer<const C3DMesh2>& mesh, unsigned long lod, unsigned long flags) const;   // 0x00A25E70
};
class CMeshDataBank : public CMeshDataBankLayout { public:
    const C3DMeshInfo* PeekMeshInfo(unsigned long index) const;     // 0x00A24260
};

class C3DVector { public:
    float X, Y, Z;
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }   // FableWin.pdb ?GetSquaredMagnitude@C3DVector@@QBEMXZ (inline)
};

namespace NParticleEngine {

const CMeshDataBank& __fastcall GetMeshBank();       // 0x00AEAAB0

class CParticleSystemCost { public:
    float PolyCount;                        // +0x00
    float PixelCount;                       // +0x04
    float LightCount;                       // +0x08
    float RenderCost;                       // +0x0c
    float SetupCost;                        // +0x10
    float LightCost;                        // +0x14
};

class CParticleSystem { public:
    unsigned char m_pad00[0x50];
    bool          Initialised : 1;                   // +0x50 bit 0
    bool          Enabled : 1;
    bool          Dead : 1;
    bool          NormalisedTimeEnabled : 1;
    bool          UseAllAttractors : 1;
    bool          Visible : 1;
    bool          StayWithEmitter : 1;
    bool          ScaleParticles : 1;                // bit 7
    float         GlobalAlpha;                       // +0x54
    float         FadeAlpha;                         // +0x58
    float         NormalisedTime;                    // +0x5c
    C3DVector     Scale;                             // +0x60
    __declspec(noinline) void ScaleParticleDimensions(float& size) const   // 0x00AF8F80 (same-TU body, the landed one)
    {
        if (ScaleParticles) {
            float a = Scale.X > Scale.Y ? Scale.X : Scale.Y;
            size *= a > Scale.Z ? a : Scale.Z;
        }
    }
};

class CPSCBase { public:
    virtual ~CPSCBase();
    CParticleSystem* ParticleSystem;            // +0x04
    bool             UpdateStarted : 1;         // +0x08 bit 0
    bool             Initialised : 1;           // +0x08 bit 1
};

class CPSCRenderMesh : public CPSCBase { public:
    long          BankIndex;                    // +0x0c
    long          TrailBankIndex;               // +0x10
    unsigned long TrailStartColour;             // +0x14 CRGBColour
    unsigned long TrailMidColour;               // +0x18
    unsigned long TrailEndColour;               // +0x1c
    unsigned long StartColour;                  // +0x20
    unsigned long MidColour;                    // +0x24
    unsigned long EndColour;                    // +0x28
    unsigned long BlendMode : 3;                // +0x2c
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;
    unsigned long TrailLengthInteger : 12;      // +0x30
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;
    unsigned long StartRenderSizeX : 11;        // +0x34
    unsigned long StartRenderSizeY : 11;
    unsigned long StartRenderSizeZ : 11;        // +0x38
    unsigned long CentredOnPos : 1;
    unsigned long AlphaFadeEnable : 1;
    unsigned long EndRenderSizeX : 11;          // bit 13
    unsigned long EndRenderSizeY : 11;          // +0x3c
    unsigned long EndRenderSizeZ : 11;
    unsigned long SizeFadeEnable : 1;
    unsigned long FlickerEnable : 1;
    unsigned long TrailUseStartColour : 1;
    unsigned long TrailUseMidColour : 1;
    unsigned long TrailUseEndColour : 1;
    unsigned long UseRenderSizeParam : 1;
    unsigned long FlickerBias : 8;              // +0x40
    unsigned long FlickerSpeed : 12;
    unsigned long AlphaFadeMinimum : 7;
    unsigned long UseStartColour : 1;
    unsigned long SizeFadeMinimum : 7;          // +0x44
    unsigned long UseMidColour : 1;
    unsigned long TrailWidth : 10;              // bit 8
    unsigned long UseEndColour : 1;
    unsigned long RenderSizeParam;              // +0x48
    unsigned long PrimHandle[2];                // +0x4c CEnginePrimitiveHandle
    long          ChildPrimIndex;               // +0x54
    unsigned long TrailPrimHandle[2];           // +0x58
    long          ChildTrailPrimIndex;          // +0x60
    virtual void GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float particleCount, float);
};

// the engine's inline-assembly float-to-long helper: VC7.1 expands a __forceinline function that contains __asm in place
static __forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    long i = decimals;
    do
        scale *= 10.0f;
    while (--i);
    float scaled = value * scale;
    return FloatToLong(scaled) / scale;
}

static __forceinline float QuantiseRenderSize(unsigned long bits)
{
    float unit = (float)bits * (1.0f / 2047.0f);
    return RoundToDecimals(unit * 20.0f, 2);
}

void CPSCRenderMesh::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float particleCount, float)
{
    if (BankIndex) {
        CCountedPointer<const C3DMesh2> mesh;
        if (GetMeshBank().PeekMeshInfo(BankIndex)->GetMesh(mesh, 0, 0)) {
            C3DVector start;
            start.X = QuantiseRenderSize(StartRenderSizeX);
            start.Y = QuantiseRenderSize(StartRenderSizeY);
            start.Z = QuantiseRenderSize(StartRenderSizeZ);
            C3DVector end;
            end.X = QuantiseRenderSize(EndRenderSizeX);
            end.Y = QuantiseRenderSize(EndRenderSizeY);
            end.Z = QuantiseRenderSize(EndRenderSizeZ);
            const C3DVector& size = start.GetSquaredMagnitude() > end.GetSquaredMagnitude() ? start : end;
            float x = size.X, y = size.Y, z = size.Z;
            if (system.ScaleParticles) {
                x *= system.Scale.X;
                y *= system.Scale.Y;
                z *= system.Scale.Z;
            }
            float radius = sqrtf(x * x + y * y + z * z);
            float polys = (float)mesh.Data->GetTriangleCount() * particleCount;
            float area = particleCount * (radius * radius * 3.14159265f);
            cost.PolyCount += polys;
            cost.PixelCount += area;
            cost.RenderCost += (float)mesh.Data->MaterialCount * polys + area;
        }
    }
    if (TrailBankIndex) {
        float unit = (float)TrailWidth * (1.0f / 1023.0f);
        float width = RoundToDecimals(unit * 10.0f, 2);
        float polys = particleCount + particleCount;
        float length = (float)TrailLengthInteger * 0.001f;
        if (length > 2.0f)
            length = 2.0f;
        length *= 0.5f;
        system.ScaleParticleDimensions(width);
        system.ScaleParticleDimensions(length);
        float area = length * width * particleCount;
        cost.PolyCount += polys;
        cost.PixelCount += area;
        cost.RenderCost += area + polys;
    }
}

} // namespace NParticleEngine