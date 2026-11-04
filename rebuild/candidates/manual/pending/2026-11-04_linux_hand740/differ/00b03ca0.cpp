// NParticleEngine::CPSCRenderMesh::PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle) -- vtable slot 13: set the mesh-group / trail-group child primitives and one mesh (and trail line) primitive per live particle
// Grade: DIFFER(6601v6468) -- frame 0x44C v 0x3C0 (slot numbering throughout); the CArray<long>() temporary passed to PrimIDList.resize keeps its inline destructor (retail has none); CIVCountedPointeeBase::Release materialises the count (`mov/dec/mov`, retail `dec [ecx+4]`); colour arrays 4 bytes off
// Leaf: ?PreparePrimitives@CPSCRenderMesh@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B03CA0, 6468 bytes, thiscall, ebp frame (`lea ebp,[esp-64h]`), `ret 14h`.
// Identity: slot 13 of vtable 0x0129F96C = PreparePrimitives (chunk 737's proof for the CPSC vtables: FableWin.pdb
//   ?PreparePrimitives@CPSCRenderMesh@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z,
//   the landed slot-13 sibling CPSCUpdateNormal::PreparePrimitives 0x00B0C910, `ret 14h` = five stack arguments).
// Shape: a mesh bank entry of data type 4 is required (else slot 29 ClearAllPrimitives 0x00B01250). Unpack the packed parameters
//   (GFRoundFloatNear); the mesh-group child primitive (CEnginePrimitiveParticleMeshGroup 0x00AFEAD0 with GetParticleWorldTransform
//   0x00B05980) and the start / mid / end colours (alpha * FadeAlpha); the trail-group child (CEnginePrimitiveParticleLineGroup, inline,
//   type 0x1A) and its colours. Per particle: life, the colour from the 3-entry table (GFFloatToLongNear(life * 511): index >> 8,
//   fraction & 0xFF, GFGetInterpolatedColour 0x00A44250), orientation, size (RenderSizeParam or the lerp, times 100 / mesh radius),
//   CentredOnPos offset (TransformVector of the mesh sphere centre), fades (GFSmoothFadeValue inline), flicker
//   (CPerlinNoise::GetGaussian2DNoise 0x00F36540 on the manager's noise), then the mesh primitive (CEnginePrimitiveParticleMesh 0x00B058E0)
//   and the trail line (CEnginePrimitiveParticleLine 0x00AFE220) set or added through CParticle::GetPrimID / SetPrimitiveInfo (FableWin
//   names, inline here). Dead particles past MaxLife are cleared.
// Attempts: p1..p10 (LOG.tsv). p2 __forceinline GetPrimID / SetPrimitiveInfo, p3 GetInterpolatedPos (all called out of line otherwise);
//   p4 __forceinline CRGBColour copy ctor (the mid-colour GFGetInterpolatedColour arguments were an out-of-line copy: low site frequency);
//   p5 the noise reference before the C2DVector; p6/p7 TransformVector(src, dst) + `pos -= offset`, `pos =` assignment; p8 per-member
//   matrix negation (struct diff 387 -> 301); p9 the counted CIVCountedPointer / CEnginePrimitiveHandle assignment (overlay, FableWin
//   ??4 names; 301 -> 272); p10 resize(n) (no change).
// Next idea: find what removes the CArray<long>() temporary's destructor (resize taking the STLport `_Tp()` default inside an out-of-line
//   resize(n)?), then the Release shape (a Release written without the inline accessor), then the frame layout.
// Header overlay used: engine/CIVCountedPointer.h + engine/CEnginePrimitiveHandle.h behind FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN.
// Helpers: GFFloatToLongNear (GFRoundFloatNear, the colour index, GFSmoothFadeValue, GFFastArcCos / GFFastSin, the alpha byte) and
//   GFFloatToLongCeil (GetRecordedPos, 0x00B0503C..0x00B05048 / 0x00B051A4..0x00B051B0).
// Retail-asm: 0x00B03D26 `fistp dword ptr [ebp+4Ch]` (GFFloatToLongNear) and 0x00B05042 `fistp dword ptr [ebp+4Ch]` (GFFloatToLongCeil):
//   VC7.1 emits no bare dword fistp from C++ (proven-impossible list).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CIEngine.h"
#include "engine/CArray.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFloatToLongCeil.h"
extern float g_ArcCosTable[1026];       // 0x013CB530
extern float g_CosTable[1025];          // 0x013CD550
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
extern const float DELTA_NOTIONAL_ZERO;    // 0x0129BA3C
inline float __fastcall GFRoundFloatNear(float value, long numDecimalPlaces)
{
    float scale = 1.0f;
    for (long i = numDecimalPlaces; i != 0; --i)
        scale *= 10.0f;
    return (float)GFFloatToLongNear(value * scale) / scale;
}
static __forceinline float UnpackParam(unsigned long packed, float steps, float minValue, float maxValue)
{
    float unit = (float)packed * (1.0f / steps);
    return GFRoundFloatNear(unit * (maxValue - minValue) + minValue, 2);
}
__forceinline float __fastcall GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = GFFloatToLongNear(cosine - 0.5f);
    float frac = cosine - (float)i;
    return ((1.0f - frac) * g_ArcCosTable[i] + frac * g_ArcCosTable[i + 1]) * 0.15915494f;
}
__forceinline float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}
#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    __forceinline const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    __forceinline const C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    __forceinline const C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline const C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    __forceinline const C3DVector operator^(const C3DVector& o) const { return C3DVector(Y * o.Z - Z * o.Y, Z * o.X - X * o.Z, X * o.Y - Y * o.X); }
    __forceinline float GetSquaredMagnitude() const { return X * X + Z * Z + Y * Y; }
    __forceinline float GetMagnitude() const { return (float)sqrt(GetSquaredMagnitude()); }
    __forceinline const C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
    __forceinline const C3DVector& operator-=(const C3DVector& v) { X -= v.X; Y -= v.Y; Z -= v.Z; return *this; }
    __forceinline const C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};
#pragma pack(pop)
class C3DBoundingSphere { public:
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) { }
    C3DVector Center;
    float Radius;
    void AddToBounds(const C3DVector& point, float radius);     // 0x00ADDD80
};
class CMatrix3x4 { public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    __forceinline void Set(const C3DVector& x_axis,const C3DVector& y_axis,const C3DVector& z_axis,const C3DVector& translation) {
        E11=x_axis.X; E12=x_axis.Y; E13=x_axis.Z;
        E21=y_axis.X; E22=y_axis.Y; E23=y_axis.Z;
        E31=z_axis.X; E32=z_axis.Y; E33=z_axis.Z;
        E41=translation.X; E42=translation.Y; E43=translation.Z;
    }
    __forceinline void TransformPoint(C3DVector& point) const {
        const float xx=point.X;
        const float yy=point.Y;
        const float zz=point.Z;
        point.X=E11*xx+(E21*yy+E31*zz)+E41;
        point.Y=E12*xx+(E22*yy+E32*zz)+E42;
        point.Z=E13*xx+(E23*yy+E33*zz)+E43;
    }
    __forceinline void TransformVector(const C3DVector& source, C3DVector& destination) const {
        destination.X=E11*source.X+(E21*source.Y+E31*source.Z);
        destination.Y=E12*source.X+(E22*source.Y+E32*source.Z);
        destination.Z=E13*source.X+(E23*source.Y+E33*source.Z);
    }
    __forceinline void TransformVector(C3DVector& vector) const {
        const float xx=vector.X;
        const float yy=vector.Y;
        const float zz=vector.Z;
        vector.X=E11*xx+(E21*yy+E31*zz);
        vector.Y=E12*xx+(E22*yy+E32*zz);
        vector.Z=E13*xx+(E23*yy+E33*zz);
    }
    __forceinline void TransformPoint(const C3DVector& pSource,C3DVector& destination) const {
        destination.X=E11*pSource.X;
        destination.X+=E31*pSource.Z;
        destination.X+=E21*pSource.Y;
        destination.X+=E41;
        destination.Y=E12*pSource.X;
        destination.Y+=E32*pSource.Z;
        destination.Y+=E22*pSource.Y;
        destination.Y+=E42;
        destination.Z=E13*pSource.X;
        destination.Z+=E33*pSource.Z;
        destination.Z+=E23*pSource.Y;
        destination.Z+=E43;
    }
};
class CRightHandedSet { protected: C3DVector Up, Forward; public:
    const C3DVector& GetForward() const { return Forward; }
    const C3DVector& GetUp() const { return Up; }
    C3DVector GetRight() const;                         // 0x0049CE00 (out of line here)
    void Init(const C3DVector& up, const C3DVector& forward) { Up = up; Forward = forward; }
};
class CQuaternion {
public:
    float X, Y, Z, W;
    void SetSlerpFromIdentity(const CQuaternion& q, float t);
    CQuaternion operator*(const CQuaternion& q) const;          // 0x00A88B60
    __forceinline void ToRightHandedSet(CRightHandedSet& rhs) const
    {
        float norm = X * X + W * W + Z * Z + Y * Y;
        float s = (norm != 0.0f) ? 2.0f / norm : 1.0f;
        float xs = X * s, ys = Y * s, zs = Z * s;
        float wx = W * xs, wy = W * ys, wz = W * zs;
        float xx = X * xs, xy = X * ys, xz = X * zs;
        float yy = Y * ys, yz = Y * zs, zz = Z * zs;
        rhs.Init(C3DVector(xz - wy, yz + wx, 1.0f - (xx + yy)), C3DVector(xy + wz, 1.0f - (xx + zz), yz - wx));
    }
};
__forceinline void CQuaternion::SetSlerpFromIdentity(const CQuaternion& q, float t)
{
    float cosom = q.W;
    float sign = 1.0f;
    if (cosom < 0.0f) {
        sign = -1.0f;
        cosom = -cosom;
    }
    float scale0;
    float scale1;
    if (1.0f - cosom > 0.05f) {
        float omega = GFFastArcCos(cosom);
        float sinom = (float)sqrt(1.0f - cosom * cosom);
        scale0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
        scale1 = GFFastSin(omega * t) * (1.0f / sinom);
    }
    else {
        scale0 = 1.0f - t;
        scale1 = t;
    }
    scale1 *= sign;
    X = scale1 * q.X;
    Y = scale1 * q.Y;
    Z = scale1 * q.Z;
    W = scale1 * q.W + scale0;
}
class CRGBColour {
public:
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long IntValue;
    };
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    void Initialise(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& colour) { Initialise(colour.R, colour.G, colour.B, colour.A); }
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CBankFile { public:
    unsigned long GetEntryDataType(unsigned long entry) const;      // 0x009CC210
};
class C3DMeshInfo { public:
    void* _vfptr;
    CBankFile* BankFile;                                // +0x04 (PDB padding)
    unsigned long BankEntry;                            // +0x08 (PDB padding)
    unsigned char _pad0c[0x28 - 0xc];
    C3DBoundingSphere BoundingSphere;                   // +0x28
};
class CMeshDataBank { public:
    const C3DMeshInfo* PeekMeshInfo(unsigned long index) const;     // 0x00A24260
    const C3DMeshInfo* GetMeshInfo(long index) const;               // 0x00A24270
};
class CEnginePrimitivePrepareInfo { public:
    unsigned char Interpolate[0x34];
    CIEngine* Engine;                                   // +0x34
};
enum EEnginePrimitiveBlendMode { };
enum EEnginePrimitiveBlendOp { };
class CEnginePrimitiveParticleMeshGroup : public CEnginePrimitive { public:
    unsigned long BankIndex;
    CMatrix3x4 WorldMat;
    EEnginePrimitiveBlendMode BlendMode;
    EEnginePrimitiveBlendOp BlendOp;
    CEnginePrimitiveParticleMeshGroup(unsigned long bankIndex, const CMatrix3x4& worldMat, EEnginePrimitiveBlendMode blendMode, EEnginePrimitiveBlendOp blendOp);   // 0x00AFEAD0
};
class CEnginePrimitiveParticleLineGroup : public CEnginePrimitive { public:
    unsigned long BankIndex;                            // +0x0c
    unsigned short Flags;                               // +0x10
    EEnginePrimitiveBlendMode BlendMode;                // +0x14
    EEnginePrimitiveBlendOp BlendOp;                    // +0x18
    CEnginePrimitiveParticleLineGroup(unsigned long bankIndex, unsigned short flags, EEnginePrimitiveBlendMode blendMode, EEnginePrimitiveBlendOp blendOp)
        : CEnginePrimitive((EEnginePrimitiveType)0x1a, (EEngineInterpolationSet)0), BankIndex(bankIndex), Flags(flags), BlendMode(blendMode), BlendOp(blendOp) { }
};
class CEnginePrimitiveParticleMesh : public CEnginePrimitive { public:
    unsigned char _body[0x50];
    CEnginePrimitiveParticleMesh(unsigned __int64 handle, const C3DVector& pos, const CRightHandedSet& rhs, const C3DVector& scale, const CRGBColour& colour, EEngineInterpolationSet set);   // 0x00B058E0
};
class CEnginePrimitiveParticleLine : public CEnginePrimitive { public:
    C3DVector Start, End;
    CRGBColour Colour;
    float Width;
    CEnginePrimitiveParticleLine(unsigned __int64 handle, const C3DVector& start, const C3DVector& end, const CRGBColour& colour, float width, EEngineInterpolationSet set);   // 0x00AFE220
};
CRGBColour GFGetInterpolatedColour(CRGBColour start, CRGBColour end, unsigned long t);   // 0x00A44250
namespace NFractal {
class CPerlinNoise { public:
    float GetGaussian2DNoise(const C2DVector& pos) const;          // 0x00F36540
};
}
namespace NParticleEngine {
long __fastcall GetTickRate();                          // 0x00AEA9F0
float __fastcall GetOneOverTickRate();                  // 0x00AEAA00
const CMeshDataBank& __fastcall GetMeshBank();          // 0x00AEAAB0
static __forceinline float GFSmoothFadeValue(float t)
{
    t = t * 0.5f;
    t = t * 1024.0f;
    long i = GFFloatToLongNear(t - 0.5f);
    float frac = t - (float)i;
    i &= 0x3ff;
    return 0.5f - ((1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1]) * 0.5f;
}
class CParticleEmitterManager { public:
    unsigned char _pad00[8];
    NFractal::CPerlinNoise PerlinNoise;                 // +0x08
    const NFractal::CPerlinNoise& PeekPerlinNoise() const { return PerlinNoise; }
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager();   // 0x00AEAAF0
#pragma pack(push, 1)
class CParticle {
public:
    virtual void Delete();
    CQuaternion Orientation;                            // +0x04
    CQuaternion OrientationChange;                      // +0x14
    float Angle;                                        // +0x24
    signed char AngleChange;                            // +0x28
    C3DVector PositionBuffer[4];                        // +0x29
    C3DVector Velocity;                                 // +0x59
    C3DVector Acceleration;                             // +0x65
    unsigned char _pad71;
    short Lifetimer;                                    // +0x72
    unsigned char CreationTime;                         // +0x74
    unsigned char _pad75;
    short MaxLife;                                      // +0x76
    unsigned char FlickerIndex;                         // +0x78
    unsigned char FlickerTimeOffset;                    // +0x79
    bool Active;                                        // +0x7a
    unsigned char _pad7b;
    float ClosestAttractorDistance;                     // +0x7c
    unsigned char _pad80[4];
    CArray<CArray<long> > PrimIDList;                   // +0x84
    CArray<CEnginePrimitiveHandle> ParentHandleList;    // +0x90
    float GetCreationTime() const { return CreationTime * (1.0f / 255.0f); }
    bool IsActive() const { return Active; }
    float GetInterpolatedLifetimer(float time) const { return Lifetimer - GetCreationTime() + time; }
    float GetInterpolatedLifetimerInSeconds(float time) const;     // 0x00AF85E0
    long GetMaxLife() const { return MaxLife; }
    unsigned char GetFlickerIndex() const { return FlickerIndex; }
    unsigned char GetFlickerTimeOffset() const { return FlickerTimeOffset; }
    __forceinline C3DVector GetInterpolatedPos(float time) const { return (PositionBuffer[0] - PositionBuffer[1]) * time + PositionBuffer[1]; }
    __forceinline C3DVector GetRecordedPos(float time, float offset) const
    {
        float pos = time + 1.0f - offset;
        long index = GFFloatToLongCeil(pos);
        if (index < 1)
            index = 1;
        else if (index > 3)
            index = 3;
        float frac = (float)index - pos;
        const C3DVector& newer = PositionBuffer[index - 1];
        const C3DVector& older = PositionBuffer[index];
        return (newer - older) * frac + older;
    }
    __forceinline CQuaternion GetInterpolatedOrientation(float t) const
    {
        CQuaternion change;
        change.SetSlerpFromIdentity(OrientationChange, t);
        CQuaternion result = Orientation * change;
        return result;
    }
    __forceinline bool GetPrimID(long index, long subIndex, long& id) const
    {
        if (PrimIDList.size() > (unsigned long)index && PrimIDList[index].size() > (unsigned long)subIndex) {
            id = PrimIDList[index][subIndex];
            return id >= 0;
        }
        return false;
    }
    __forceinline void SetPrimitiveInfo(long index, long subIndex, const CEnginePrimitiveHandle& parent, long id)
    {
        if (PrimIDList.size() <= (unsigned long)index)
            PrimIDList.resize(index + 1, CArray<long>());
        if (PrimIDList[index].size() <= (unsigned long)subIndex)
            PrimIDList[index].resize(subIndex + 1, 0);
        PrimIDList[index][subIndex] = id;
        if (ParentHandleList.size() <= (unsigned long)index)
            ParentHandleList.resize(index + 1);
        ParentHandleList[index].Clear();
        ParentHandleList[index] = parent;
    }
};
#pragma pack(pop)
template <class A, class B> class CParticleUpdateParameters { public:
    bool GetVectorParam(C3DVector& value, const unsigned long& param) const;   // 0x00B059F0
};
class CParticleSystem { public:
    unsigned char _pad00[0x1c];
    std::list<CParticle*, std::allocator<CParticle*> > Particles;    // +0x1c
    unsigned char _pad20[0x50 - 0x20];
    bool Initialised : 1;                               // +0x50
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;
    bool Visible : 1;
    bool StayWithEmitter : 1;
    bool ScaleParticles : 1;
    float GlobalAlpha;                                  // +0x54
    float FadeAlpha;                                    // +0x58
    float NormalisedTime;                               // +0x5c
    C3DVector Scale;                                    // +0x60
    float GetGlobalAlpha() const { return GlobalAlpha; }
    float GetFadeAlpha() const { return FadeAlpha; }
    void GetSystemTransform(CMatrix3x4& matrix) const;          // 0x00AF2250
    void GetParticleWorldTransform(CMatrix3x4& matrix) const;   // 0x00B05980
    void ScaleParticleDimensions(float& value) const;           // 0x00AF8F80
    void ClearParticle(std::list<CParticle*, std::allocator<CParticle*> >::iterator& it);   // 0x00AF5090
};
class CPSCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle);
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26(); virtual void v27(); virtual void v28();
    virtual void ClearAllPrimitives(CIEngine* engine, CEnginePrimitiveHandle& handle);
    CParticleSystem* ParticleSystem;            // +0x04
    bool UpdateStarted : 1;                     // +0x08
    bool Initialised : 1;
};
class CPSCRenderMesh : public CPSCBase {
public:
    long BankIndex;                                     // +0x0c
    long TrailBankIndex;                                // +0x10
    CRGBColour TrailStartColour;                        // +0x14
    CRGBColour TrailMidColour;                          // +0x18
    CRGBColour TrailEndColour;                          // +0x1c
    CRGBColour StartColour;                             // +0x20
    CRGBColour MidColour;                               // +0x24
    CRGBColour EndColour;                               // +0x28
    unsigned long BlendMode : 3;                        // +0x2c
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;
    unsigned long TrailLengthInteger : 12;              // +0x30
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;
    unsigned long StartRenderSizeX : 11;                // +0x34
    unsigned long StartRenderSizeY : 11;
    unsigned long StartRenderSizeZ : 11;                // +0x38
    unsigned long CentredOnPos : 1;
    unsigned long AlphaFadeEnable : 1;
    unsigned long EndRenderSizeX : 11;
    unsigned long EndRenderSizeY : 11;                  // +0x3c
    unsigned long EndRenderSizeZ : 11;
    unsigned long SizeFadeEnable : 1;
    unsigned long FlickerEnable : 1;
    unsigned long TrailUseStartColour : 1;
    unsigned long TrailUseMidColour : 1;
    unsigned long TrailUseEndColour : 1;
    unsigned long UseRenderSizeParam : 1;
    unsigned long FlickerBias : 8;                      // +0x40
    unsigned long FlickerSpeed : 12;
    unsigned long AlphaFadeMinimum : 7;
    unsigned long UseStartColour : 1;
    unsigned long SizeFadeMinimum : 7;                  // +0x44
    unsigned long UseMidColour : 1;
    unsigned long TrailWidth : 10;
    unsigned long UseEndColour : 1;
    unsigned long RenderSizeParam;                      // +0x48
    CEnginePrimitiveHandle PrimHandle;                 // +0x4c
    long ChildPrimIndex;                                // +0x54
    CEnginePrimitiveHandle TrailPrimHandle;            // +0x58
    long ChildTrailPrimIndex;                           // +0x60
    virtual void PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle);
};
void CPSCRenderMesh::PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                       const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle)
{
    const C3DMeshInfo* checkInfo;
    if (BankIndex > 0 && (checkInfo = GetMeshBank().GetMeshInfo(BankIndex), checkInfo->BankFile->GetEntryDataType(checkInfo->BankEntry) == 4)) {
        float startSizeX = UnpackParam(StartRenderSizeX, 2047.0f, 0.0f, 20.0f);
        float startSizeY = UnpackParam(StartRenderSizeY, 2047.0f, 0.0f, 20.0f);
        float startSizeZ = UnpackParam(StartRenderSizeZ, 2047.0f, 0.0f, 20.0f);
        float endSizeX = UnpackParam(EndRenderSizeX, 2047.0f, 0.0f, 20.0f);
        float endSizeY = UnpackParam(EndRenderSizeY, 2047.0f, 0.0f, 20.0f);
        float endSizeZ = UnpackParam(EndRenderSizeZ, 2047.0f, 0.0f, 20.0f);
        float alphaFadeMinimum = UnpackParam(AlphaFadeMinimum, 127.0f, 0.0f, 1.0f);
        float sizeFadeMinimum = UnpackParam(SizeFadeMinimum, 127.0f, 0.0f, 1.0f);
        float flickerSpeed = UnpackParam(FlickerSpeed, 4095.0f, 0.0f, 30.0f);
        float flickerBias = UnpackParam(FlickerBias, 255.0f, -1.0f, 1.0f);
        float trailWidth = UnpackParam(TrailWidth, 1023.0f, 0.0f, 10.0f);
        float tickRate = (float)GetTickRate();
        float fadeInEnd = FadeInEndInteger * 0.001f;
        float fadeOutBegin = FadeOutBeginInteger * 0.001f;
        float fadeAlpha = system.GetFadeAlpha();
        float flickerRange = (float)fabs(flickerBias) + 0.5f;
        long blendMode = BlendMode;
        long blendOp = BlendOp;
        CRGBColour colours[3];
        long trailPrimIndex = 0;
        if (BankIndex) {
            CMatrix3x4 worldTransform;
            system.GetParticleWorldTransform(worldTransform);
            info.Engine->SetChildPrimitive(handle, ChildPrimIndex, PrimHandle,
                CEnginePrimitiveParticleMeshGroup(BankIndex, worldTransform, (EEnginePrimitiveBlendMode)blendMode, (EEnginePrimitiveBlendOp)blendOp), 0);
            trailPrimIndex = 1;
            colours[0] = UseStartColour ? StartColour : UseMidColour ? MidColour : UseEndColour ? EndColour : CRGBColour(255, 255, 255, 255);
            colours[2] = UseEndColour ? EndColour : UseMidColour ? MidColour : UseStartColour ? StartColour : CRGBColour(255, 255, 255, 255);
            colours[1] = UseMidColour ? MidColour : GFGetInterpolatedColour(colours[0], colours[2], 0x80);
            colours[0].A = (unsigned char)(colours[0].A * fadeAlpha);
            colours[1].A = (unsigned char)(colours[1].A * fadeAlpha);
            colours[2].A = (unsigned char)(colours[2].A * fadeAlpha);
        }
        long trailBlendMode = TrailBlendMode;
        long trailBlendOp = TrailBlendOp;
        CRGBColour trailColours[3];
        if (TrailBankIndex) {
            trailColours[0] = TrailUseStartColour ? TrailStartColour : TrailUseMidColour ? TrailMidColour : TrailUseEndColour ? TrailEndColour : CRGBColour(255, 255, 255, 255);
            trailColours[2] = TrailUseEndColour ? TrailEndColour : TrailUseMidColour ? TrailMidColour : TrailUseStartColour ? TrailStartColour : CRGBColour(255, 255, 255, 255);
            trailColours[1] = TrailUseMidColour ? TrailMidColour : GFGetInterpolatedColour(trailColours[0], trailColours[2], 0x80);
            info.Engine->SetChildPrimitive(handle, ChildTrailPrimIndex, TrailPrimHandle,
                CEnginePrimitiveParticleLineGroup(TrailBankIndex, 0, (EEnginePrimitiveBlendMode)trailBlendMode, (EEnginePrimitiveBlendOp)trailBlendOp), 0);
        }
        for (std::list<CParticle*, std::allocator<CParticle*> >::iterator it = system.Particles.begin(); it != system.Particles.end(); ++it) {
            CParticle* particle = *it;
            float lifetimer = particle->GetInterpolatedLifetimer(time);
            if (particle->IsActive() && particle->GetMaxLife() != 0) {
                float lifeSeconds = GetOneOverTickRate() * lifetimer;
                float life = lifetimer / particle->GetMaxLife();
                if (life < 0.0f)
                    life = 0.0f;
                else if (life > 1.0f)
                    life = 1.0f;
                float sizeLife = life;
                if (sizeLife < 0.0f)
                    sizeLife = 0.0f;
                else {
                    float limit = 1.0f - DELTA_NOTIONAL_ZERO;
                    if (sizeLife > limit)
                        sizeLife = limit;
                }
                unsigned long colourPos = GFFloatToLongNear(life * 511.0f);
                unsigned long colourIndex = colourPos >> 8;
                unsigned long colourFrac = colourPos & 0xff;
                CRGBColour colour = GFGetInterpolatedColour(colours[colourIndex], colours[colourIndex + 1], colourFrac);
                particle->GetInterpolatedLifetimerInSeconds(time);
                CQuaternion orientation = particle->GetInterpolatedOrientation(time);
                CRightHandedSet rhs;
                orientation.ToRightHandedSet(rhs);
                const C3DMeshInfo* meshInfo = GetMeshBank().PeekMeshInfo(BankIndex);
                float meshScale = 1.0f / (meshInfo->BoundingSphere.Radius * 0.01f);
                C3DVector meshCentre = meshInfo->BoundingSphere.Center;
                C3DVector size;
                if (!UseRenderSizeParam || !params.GetVectorParam(size, RenderSizeParam)) {
                    size.X = ((endSizeX - startSizeX) * sizeLife + startSizeX) * meshScale;
                    size.Y = ((endSizeY - startSizeY) * sizeLife + startSizeY) * meshScale;
                    size.Z = ((endSizeZ - startSizeZ) * sizeLife + startSizeZ) * meshScale;
                }
                C3DVector pos;
                pos = particle->GetInterpolatedPos(time);
                if (CentredOnPos) {
                    C3DVector forward = rhs.GetForward();
                    C3DVector up = rhs.GetUp();
                    C3DVector right = rhs.GetRight();
                    right *= size.X * system.Scale.X * 0.01f;
                    forward *= size.Y * system.Scale.Y * 0.01f;
                    up *= size.Z * system.Scale.Z * 0.01f;
                    CMatrix3x4 matrix;
                    matrix.E11 = -right.X; matrix.E12 = -right.Y; matrix.E13 = -right.Z;
                    matrix.E21 = -forward.X; matrix.E22 = -forward.Y; matrix.E23 = -forward.Z;
                    matrix.E31 = up.X; matrix.E32 = up.Y; matrix.E33 = up.Z;
                    C3DVector offset;
                    matrix.TransformVector(meshCentre, offset);
                    pos -= offset;
                }
                float alpha = system.GetGlobalAlpha();
                float sizeScale = 1.0f;
                if (life < fadeInEnd)
                    alpha *= GFSmoothFadeValue(life / fadeInEnd);
                if (life > fadeOutBegin)
                    alpha *= GFSmoothFadeValue((1.0f - life) / (1.0f - fadeOutBegin));
                float alphaScale = 1.0f;
                if (alpha < 1.0f) {
                    if (AlphaFadeEnable)
                        alphaScale = (1.0f - alphaFadeMinimum) * alpha + alphaFadeMinimum;
                    if (SizeFadeEnable)
                        sizeScale = (1.0f - sizeFadeMinimum) * alpha + sizeFadeMinimum;
                }
                if (FlickerEnable) {
                    const NFractal::CPerlinNoise& noise = GetParticleEmitterManager().PeekPerlinNoise();
                    float flicker = noise.GetGaussian2DNoise(
                        C2DVector(lifeSeconds * flickerSpeed + particle->GetFlickerTimeOffset(), particle->GetFlickerIndex()))
                        * flickerRange + flickerBias + 0.5f;
                    if (flicker < 0.0f)
                        flicker = 0.0f;
                    else if (flicker > 1.0f)
                        flicker = 1.0f;
                    float minAlpha = FlickerMinAlphaInteger * 0.001f;
                    alphaScale *= (1.0f - minAlpha) * flicker + minAlpha;
                    float minSize = FlickerMinSizeInteger * meshScale * 0.001f;
                    sizeScale *= (1.0f - minSize) * flicker + minSize;
                }
                size *= sizeScale;
                colour.A = (unsigned char)GFFloatToLongNear(colour.A * alphaScale);
                if (colour.A > 0) {
                    long id;
                    if (particle->GetPrimID(0, 0, id))
                        info.Engine->SetChildPrimitive(PrimHandle, id,
                            CEnginePrimitiveParticleMesh(0, pos, rhs, size, colour, (EEngineInterpolationSet)0), 0);
                    else
                        particle->SetPrimitiveInfo(0, 0, PrimHandle, info.Engine->AddChildPrimitive(PrimHandle,
                            CEnginePrimitiveParticleMesh(0, pos, rhs, size, colour, (EEngineInterpolationSet)0), 0));
                    if (TrailBankIndex) {
                        colour = GFGetInterpolatedColour(trailColours[colourIndex], trailColours[colourIndex + 1], colourFrac);
                        float trailLength = TrailLengthInteger * 0.001f;
                        C3DVector trailStart = pos;
                        C3DVector trailEnd;
                        if (trailLength > 2.0f) {
                            float trailScale = trailLength * 0.5f;
                            trailEnd = particle->GetRecordedPos(2.0f, time);
                            trailEnd = (trailEnd - pos) * trailScale + pos;
                        } else
                            trailEnd = particle->GetRecordedPos(trailLength, time);
                        if (system.StayWithEmitter) {
                            CMatrix3x4 transform;
                            system.GetSystemTransform(transform);
                            transform.TransformPoint(trailStart);
                        }
                        if (system.StayWithEmitter) {
                            CMatrix3x4 transform;
                            system.GetSystemTransform(transform);
                            transform.TransformPoint(trailEnd);
                        }
                        float width = sizeScale * meshScale * trailWidth;
                        system.ScaleParticleDimensions(width);
                        if (particle->GetPrimID(trailPrimIndex, 0, id))
                            info.Engine->SetChildPrimitive(TrailPrimHandle, id,
                                CEnginePrimitiveParticleLine(0, trailStart, trailEnd, colour, width, (EEngineInterpolationSet)0), 0);
                        else
                            particle->SetPrimitiveInfo(trailPrimIndex, 0, TrailPrimHandle, info.Engine->AddChildPrimitive(TrailPrimHandle,
                                CEnginePrimitiveParticleLine(0, trailStart, trailEnd, colour, width, (EEngineInterpolationSet)0), 0));
                    }
                }
            } else if (lifetimer > particle->GetMaxLife())
                system.ClearParticle(it);
        }
    } else
        ClearAllPrimitives(info.Engine, handle);
}
}
