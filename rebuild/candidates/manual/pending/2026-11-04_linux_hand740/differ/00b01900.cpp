// NParticleEngine::CPSCRenderMesh::AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params) -- vtable slot 19: grow the emitter's bounding sphere over every live mesh particle and its trail
// Grade: DIFFER(3755v3777) -- frame 0x298 v 0x2B0 (slot numbering throughout), loop-head alignment pad, the Z row order of the two trail-end TransformPoint expansions (retail E23*yy, E13*xx, E33*zz), the trail magnitude's load order (retail Z, Y, X) and the !CentredOnPos matrix block (retail scales right / forward / up in order and keeps E11..E13 in memory)
// Leaf: ?AddToBoundingSphere@CPSCRenderMesh@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B01900, 3777 bytes, thiscall, `ret 10h`, returns bool.
// Identity: FableWin.pdb ?AddToBoundingSphere@CPSCRenderMesh@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z;
//   slot 19 of vtable 0x0129F96C (CPSCBase's slot 19 is AddToBoundingSphere 0x00AF2A30; the CPSCLight twin 0x00B18AF0 is landed); `ret 10h`
//   matches the four stack arguments.
// Shape: unpack the start / end sizes and the trail width (GFRoundFloatNear), trail length = TrailLengthInteger * 0.001 (recorded positions
//   capped at 2, the rest scaled); the mesh's bounding sphere from GetMeshBank().PeekMeshInfo(BankIndex). Per active particle: the
//   interpolated position (system transform when the particles stay with the emitter); the trail end from GetRecordedPos and the trail
//   segment added as a sphere; the mesh size (RenderSizeParam via GetVectorParam, else the life lerp; ScaleParticles), its orientation
//   (GetInterpolatedOrientation -> ToRightHandedSet) placing the mesh sphere centre unless CentredOnPos; dead particles past MaxLife are
//   cleared. Nothing added: both primitive handles are cleared.
// Callees: C3DBoundingSphere::AddToBounds 0x00ADDD80, GetMeshBank 0x00AEAAB0, CMeshDataBank::PeekMeshInfo 0x00A24260,
//   CParticleSystem::GetSystemTransform 0x00AF2250 / ClearParticle 0x00AF5090, CParticleUpdateParameters::GetVectorParam 0x00B059F0,
//   CQuaternion::operator* 0x00A88B60.
// Attempts: b1..b19 (LOG.tsv); best b8/b16/b19 family. Tried: __forceinline particle helpers (needed), sequential C3DVector member
//   stores, a user C3DVector copy ctor (dl 704 -> 468), Z-row order x,y,z (fixes the first TransformPoint), `trail = trailEnd - pos`
//   assignment (fixes the temp copy), all six magnitude orders (load order unchanged: X first), axis statements before Set (b16),
//   CMatrix3x4::operator* with a zero translation and `pos +=` (b17/b18, worse).
// Next idea: model the mesh offset as CMatrix3x4::TransformVector + `pos +=` with per-member negation (the shape that fixed the matrix
//   block of PreparePrimitives 0x00B03CA0, p8), then retune the trail-end transforms.
// Helpers: GFFloatToLongNear (fistp in GFRoundFloatNear / GFFastArcCos / GFFastSin), GFFastFloatToLong (CParticle::IsActive,
//   0x00B01C4D..0x00B01C65), GFFloatToLongCeil (GetRecordedPos, 0x00B01DAA..0x00B01DD0 / 0x00B01F9D..0x00B01FBD).
// Retail-asm: 0x00B01956 `fistp dword ptr [ebp+54h]` (GFFloatToLongNear) and the bare dword fistp of GFFastFloatToLong /
//   GFFloatToLongCeil above: VC7.1 emits no bare dword fistp from C++ (proven-impossible list).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFastFloatToLong.h"
#include "engine/GFFloatToLongCeil.h"
extern float g_ArcCosTable[1026];       // 0x013CB530
extern float g_CosTable[1025];          // 0x013CD550
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
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
        point.Y=E12*xx;
        point.Y+=E32*zz;
        point.Y+=E22*yy;
        point.Y+=E42;
        point.Z=E13*xx;
        point.Z+=E23*yy;
        point.Z+=E33*zz;
        point.Z+=E43;
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
    C3DVector GetRight() const { return Forward ^ Up; }
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
class CMeshDataBank;
class C3DMeshInfo { public:
    unsigned char _pad[0x28];
    C3DBoundingSphere BoundingSphere;                   // +0x28
    C3DBoundingSphere GetBoundingSphere() const { return BoundingSphere; }
};
class CMeshDataBank { public:
    const C3DMeshInfo* PeekMeshInfo(unsigned long index) const;     // 0x00A24260
};
namespace std {
template <class _Tp> class allocator { };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
template <class _Tp, class _Alloc> class list {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_node<_Tp>* _M_node;
    iterator begin() { return iterator((_List_node<_Tp>*)_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
};
}
namespace NParticleEngine {
const CMeshDataBank& __fastcall GetMeshBank();       // 0x00AEAAB0
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
    float GetCreationTime() const { return CreationTime * (1.0f / 255.0f); }
    __forceinline bool IsActive(float time) const
    {
        if (!Active)
            return false;
        if (Lifetimer > 0)
            return true;
        long quantised = GFFastFloatToLong(time * 255.0f);
        return quantised * (1.0f / 255.0f) >= GetCreationTime();
    }
    float GetInterpolatedLifetimer(float time) const { return Lifetimer - GetCreationTime() + time; }
    long GetMaxLife() const { return MaxLife; }
    C3DVector GetInterpolatedPos(float time) const { return (PositionBuffer[0] - PositionBuffer[1]) * time + PositionBuffer[1]; }
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
    void GetSystemTransform(CMatrix3x4& matrix) const;          // 0x00AF2250
    void ClearParticle(std::list<CParticle*, std::allocator<CParticle*> >::iterator& it);   // 0x00AF5090
};
class CEnginePersistentPrimitiveBaseX;
class CPSCBase { public:
    virtual void v00();
    CParticleSystem* ParticleSystem;            // +0x04
    bool UpdateStarted : 1;                     // +0x08
    bool Initialised : 1;
};
class CEnginePrimitiveHandleX { public: void* vt; void* Data; };
class CPSCRenderMesh : public CPSCBase {
public:
    long BankIndex;                                     // +0x0c
    long TrailBankIndex;                                // +0x10
    unsigned long Colours[6];                           // +0x14
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
    virtual bool AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params);
};
bool CPSCRenderMesh::AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    C3DVector startSize;
    startSize.X = UnpackParam(StartRenderSizeX, 2047.0f, 0.0f, 20.0f);
    startSize.Y = UnpackParam(StartRenderSizeY, 2047.0f, 0.0f, 20.0f);
    startSize.Z = UnpackParam(StartRenderSizeZ, 2047.0f, 0.0f, 20.0f);
    C3DVector endSize;
    endSize.X = UnpackParam(EndRenderSizeX, 2047.0f, 0.0f, 20.0f);
    endSize.Y = UnpackParam(EndRenderSizeY, 2047.0f, 0.0f, 20.0f);
    endSize.Z = UnpackParam(EndRenderSizeZ, 2047.0f, 0.0f, 20.0f);
    float trailWidth = UnpackParam(TrailWidth, 1023.0f, 0.0f, 10.0f);
    float trailLength = (float)TrailLengthInteger * 0.001f;
    bool longTrail;
    float recordedLength;
    if (trailLength > 2.0f) {
        recordedLength = 2.0f;
        longTrail = true;
    } else {
        recordedLength = trailLength;
        longTrail = false;
    }
    float trailScale = recordedLength * 0.5f;
    float meshScale = 0.0f;
    C3DBoundingSphere meshSphere;
    if (BankIndex) {
        meshSphere = GetMeshBank().PeekMeshInfo(BankIndex)->GetBoundingSphere();
        meshScale = 1.0f / meshSphere.Radius;
    }
    bool added = false;
    for (std::list<CParticle*, std::allocator<CParticle*> >::iterator it = system.Particles.begin(); it != system.Particles.end(); ++it) {
        CParticle* particle = *it;
        if (particle->IsActive(timeOffset)) {
            C3DVector pos = particle->GetInterpolatedPos(timeOffset);
            if (system.StayWithEmitter) {
                CMatrix3x4 transform;
                system.GetSystemTransform(transform);
                transform.TransformPoint(pos);
            }
            if (TrailBankIndex) {
                C3DVector trailEnd;
                if (longTrail) {
                    trailEnd = particle->GetRecordedPos(2.0f, timeOffset);
                    if (system.StayWithEmitter) {
                        CMatrix3x4 transform;
                        system.GetSystemTransform(transform);
                        transform.TransformPoint(trailEnd);
                    }
                    trailEnd = (trailEnd - pos) * trailScale + pos;
                } else {
                    trailEnd = particle->GetRecordedPos(trailLength, timeOffset);
                    if (system.StayWithEmitter) {
                        CMatrix3x4 transform;
                        system.GetSystemTransform(transform);
                        transform.TransformPoint(trailEnd);
                    }
                }
                C3DVector trail;
                trail = trailEnd - pos;
                float length = trail.GetMagnitude();
                if (trailWidth > length)
                    length = trailWidth;
                sphere.AddToBounds(trail * 0.5f + pos, length * 0.5f);
                added = true;
            }
            if (BankIndex) {
                C3DVector size;
                if (!UseRenderSizeParam || !params.GetVectorParam(size, RenderSizeParam)) {
                    float life = particle->GetInterpolatedLifetimer(timeOffset) / particle->GetMaxLife();
                    if (life < 0.0f)
                        life = 0.0f;
                    else {
                        float limit = 1.0f - DELTA_NOTIONAL_ZERO;
                        if (life > limit)
                            life = limit;
                    }
                    size.X = (endSize.X - startSize.X) * life + startSize.X;
                    size.Y = (endSize.Y - startSize.Y) * life + startSize.Y;
                    size.Z = (endSize.Z - startSize.Z) * life + startSize.Z;
                }
                if (system.ScaleParticles) {
                    size.X *= system.Scale.X;
                    size.Y *= system.Scale.Y;
                    size.Z *= system.Scale.Z;
                }
                if (!CentredOnPos) {
                    CQuaternion orientation = particle->GetInterpolatedOrientation(timeOffset);
                    CRightHandedSet rhs;
                    orientation.ToRightHandedSet(rhs);
                    C3DVector right = rhs.GetRight();
                    CMatrix3x4 matrix;
                    C3DVector xAxis = right * (size.X * meshScale);
                    C3DVector yAxis = rhs.GetForward() * (size.Y * meshScale);
                    C3DVector zAxis = rhs.GetUp() * (size.Z * meshScale);
                    matrix.Set(-xAxis, -yAxis, zAxis, pos);
                    matrix.TransformPoint(meshSphere.Center, pos);
                }
                sphere.AddToBounds(pos, size.GetMagnitude() * 0.5f);
                added = true;
            }
        } else if (particle->GetInterpolatedLifetimer(timeOffset) > particle->GetMaxLife()) {
            system.ClearParticle(it);
        }
    }
    if (!added) {
        PrimHandle.Clear();
        TrailPrimHandle.Clear();
    }
    return added;
}
}
