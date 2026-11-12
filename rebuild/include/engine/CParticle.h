#ifndef FABLE_ENGINE_CPARTICLE_H
#define FABLE_ENGINE_CPARTICLE_H
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CQuaternion.h"
#include "engine/CRandomAcceleration.h"
#include "engine/CArray.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "boost/scoped_ptr.h"
#include <string.h>
#ifdef FABLE_RETAIL_CPARTICLE_SPRITE
#include "engine/GFFastFloatToLong.h"
#include "engine/GFFloatToLongCeil.h"
#endif
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
#ifdef FABLE_RETAIL_CPARTICLE_SPRITE
// Donor02F2A9CA/02F2AA08 use the original one-argument STLport overloads.
// Their standard bodies delegate to resize(n,T()); the handle overload stays called.
namespace std {
template<> inline void vector<long,allocator<long> >::resize(size_type n){resize(n,long());}
template<> inline void vector<CArray<long>,allocator<CArray<long> > >::resize(size_type n){resize(n,CArray<long>());}
}
#endif
namespace NParticleEngine {
class CParticle:public CBaseClassNonCopyable {
public:
 virtual void Delete();
 virtual ~CParticle();
 bool IsActive()const{return Active;}
    const C3DVector& GetPosition() const { return PositionBuffer[0]; }
    void SetPosition(const C3DVector& position) { memmove(&PositionBuffer[1], &PositionBuffer[0], sizeof(C3DVector) * 3); PositionBuffer[0] = position; }
    const C3DVector& GetVelocity() const { return Velocity; }
    void SetVelocity(const C3DVector& velocity) { Velocity = velocity; }
    const C3DVector& GetAcceleration() const { return Acceleration; }
    void ClearAcceleration() {
#ifdef FABLE_RETAIL_C3DVECTOR_CLEAR
        Acceleration.Clear();
#else
        Acceleration.X = Acceleration.Y = Acceleration.Z = 0.0f;
#endif
    }
    // Original donor02F41070 returns Orientation at+4.
    const CQuaternion& GetOrientation()const{return Orientation;}
    void SetOrientation(const CQuaternion& orientation) { Orientation = orientation; }
    void SetOrientationChange(const CQuaternion& change) { OrientationChange = change; }
    void UpdateOrientation() { Orientation *= OrientationChange; }
    // Original getter, donor02F41100 and nativeAFD822 quantization.
    float GetAngleChange() const { return (AngleChange * (2.0f / 127.0f)); }
#ifdef FABLE_RETAIL_CPARTICLE_SPRITE
    void UpdateAngle() { Angle += AngleChange * (1.0f / 63.5f); }
#else
    void UpdateAngle() { Angle += GetAngleChange(); }
#endif
    void IncrementLifetimer() { ++Lifetimer; }
    short GetLifetimer() const { return Lifetimer; }
#ifdef FABLE_RETAIL_CPARTICLE_SPRITE
    float GetCreationTime() const { return CreationTime * (1.0f / 255.0f); }
#else
    float GetCreationTime() const { return CreationTime / 255.0f; }
#endif
    bool HaveRandomAccelerationData() const { return RandomAcceleration.get() != 0; }
    void CreateRandomAccelerationData(unsigned long& seed) {RandomAcceleration.reset(new CRandomAcceleration(seed));}
    unsigned char GetRandomAccelerationTimeOffsetX() const { return RandomAcceleration->TimeOffsetX; }
    unsigned char GetRandomAccelerationTimeOffsetY() const { return RandomAcceleration->TimeOffsetY; }
    unsigned char GetRandomAccelerationTimeOffsetZ() const { return RandomAcceleration->TimeOffsetZ; }
    unsigned char GetRandomAccelerationIndexX() const { return RandomAcceleration->IndexX; }
    unsigned char GetRandomAccelerationIndexY() const { return RandomAcceleration->IndexY; }
    unsigned char GetRandomAccelerationIndexZ() const { return RandomAcceleration->IndexZ; }
    C3DVector GetOldRandomOffset() const { return RandomAcceleration->OldOffset; }
    void SetOldRandomOffset(const C3DVector& offset) { RandomAcceleration->OldOffset = offset; }

#ifdef FABLE_RETAIL_CPARTICLE_SPRITE
    float GetAngle() const { return Angle; }
    // NativeAFD822..828: retain quantization product before caller interpolation.
    long GetMaxLife() const { return MaxLife; }
    unsigned char GetFlickerIndex() const { return FlickerIndex; }
    unsigned char GetFlickerTimeOffset() const { return FlickerTimeOffset; }
    float GetInterpolatedLifetimer(float fraction) const { return Lifetimer - GetCreationTime() + fraction; }
    bool IsActive(float time) const
    {
        if (Lifetimer > 0) return true;
        long creation = GFFastFloatToLong(time * 255.0f);
        return creation * (1.0f / 255.0f) >= GetCreationTime();
    }
    // Original donor02F27C00/02F27C40 call the genuine vector interpolation member.
    C3DVector GetInterpolatedPos(float interp)const{return PositionBuffer[1].Interpolate(PositionBuffer[0],interp);}
    C3DVector GetRecordedPos(float delay,float interp)const{
        float t=delay+1.0f-interp;
        long index=GFFloatToLongCeil(t);
        if(index<1)index=1;else if(index>3)index=3;
        return PositionBuffer[index].Interpolate(PositionBuffer[index-1],(float)index-t);
    }
    bool GetPrimID(long list, long index, long& id) const
    {
        if (PrimIDList.size() > (unsigned long)list && PrimIDList[list].size() > (unsigned long)index) {
            id = PrimIDList[list][index];
            return id >= 0;
        }
        return false;
    }
    void SetPrimitiveInfo(long list, long index, const CEnginePrimitiveHandle& parent, long id)
    {
        if (PrimIDList.size() <= (unsigned long)list)
            PrimIDList.resize(list + 1);
        if (PrimIDList[list].size() <= (unsigned long)index)
            PrimIDList[list].resize(index + 1);
        PrimIDList[list][index] = id;
        if (ParentHandleList.size() <= (unsigned long)list)
            ParentHandleList.resize(list + 1);
        ParentHandleList[list] = parent;
    }
#endif
#ifdef FABLE_RETAIL_CPARTICLE_CLEARPARTICLE
    // Opt-in (row 0x00AF5090 CParticleSystem::ClearParticle): FableWin.pdb accessors, inline in retail (debug COMDATs 0x02F18DA0..
    // 0x02F18ED0, same instruction counts as the CDecal twins of FABLE_RETAIL_CDECAL_CLEAR): ?GetParentHandle@CParticle@NParticleEngine@@
    // QBE_NJAAVCEnginePrimitiveHandle@@@Z, ?GetPrimID@...@QBE_NJJAAJ@Z (FABLE_RETAIL_CPARTICLE_SPRITE has the same body),
    // ?ClearParentHandle@...@QAEXJ@Z (FABLE_FORCEINLINE: VC7.1's inline budget otherwise calls it), ?ClearAll@...@QAEXXZ.
    bool GetParentHandle(long group, CEnginePrimitiveHandle& handle) const {
        if (ParentHandleList.size() > group) { handle = ParentHandleList[group]; return !handle.IsNull(); }
        return false;
    }
#ifndef FABLE_RETAIL_CPARTICLE_SPRITE
    bool GetPrimID(long list, long index, long& id) const
    {
        if (PrimIDList.size() > (unsigned long)list && PrimIDList[list].size() > (unsigned long)index) {
            id = PrimIDList[list][index];
            return id >= 0;
        }
        return false;
    }
#endif
    FABLE_FORCEINLINE void ClearParentHandle(long group) {
        if (ParentHandleList.size() > group) { ParentHandleList[group].Clear(); PrimIDList[group].clear(); }
    }
    void ClearAll() { ParentHandleList.clear(); PrimIDList.clear(); }
#endif
private:
 CQuaternion Orientation,OrientationChange;
 float Angle;signed char AngleChange;
 C3DVector PositionBuffer[4],Velocity,Acceleration;
 short Lifetimer;unsigned char CreationTime;short MaxLife;
 unsigned char FlickerIndex,FlickerTimeOffset;bool Active;float ClosestAttractorDistance;
 boost::scoped_ptr<CRandomAcceleration> RandomAcceleration;
 CArray<CArray<long> > PrimIDList;
 CArray<CEnginePrimitiveHandle> ParentHandleList;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticle)==0x9c);
#endif
