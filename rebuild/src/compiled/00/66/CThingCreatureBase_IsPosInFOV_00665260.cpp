// CThingCreatureBase::IsPosInFOV(const C3DVector& position, bool useLookOffset) const -- with the field-of-view TC (0x34): the wrapped XY angle to the position is within half the field of view of the creature's facing (plus the look TC's head offset when asked)
// Leaf: ?IsPosInFOV@CThingCreatureBase@@QBE_NABVC3DVector@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00665260, `ret 8`, bool in al. Catalogued as CVertexBufferWin32::ComputeAngleBetweenPositions
//   (wrong: `this` is a creature -- interface bitset +0x20, interface map +0x44, PhysicsTC +0x60, CThingCreatureBase::GetPosition
//   0x006A4D00); FableWin.pdb names it ?IsPosInFOV@CThingCreatureBase@@QBE_NABVC3DVector@@_N@Z.
// Callees: CInterfaceMap::LowerBound const 0x004365B0, TC 0x34's float query 0x007445D0 (catalogued GetAirResistance),
//   CThingCreatureBase::GetPosition 0x006A4D00, PhysicsTC slot 70 (+0x118, facing direction by hidden pointer),
//   CTCLook float query 0x00751A80 (catalogued DrawGetSeaLevel), _CIfmod 0x00BFEB28. Constants: 1/(2pi) 0x01237F08,
//   0.0f 0x0122DEDC, 1.0f 0x0122DED8, double 1.0 0x01237F00, 0.5f 0x0122F59C.
// Layout: retail CThing interface bitset +0x20 (TC 0x34 = word +0x24 bit 20, TC 0x43 = word +0x28 bit 3), interface map +0x44,
//   PhysicsTC +0x60 (the landed 0x006650E0 CanSeeTarget model).
#include <math.h>
#pragma intrinsic(atan2, fmod, fabs)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();

class CTCPhysicsBase { public:
    FILL10(0) FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5) FILL10(6)
    virtual C3DVector GetFacingDirection();     // slot 70 (+0x118)
};
struct CTCFieldOfView {
    float GetFieldOfView();                     // 0x007445D0
};
class CTCLook { public:
    float GetHeadAngleOffset();                 // 0x00751A80
};

__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

__forceinline float AngleDistance(float a, float b)
{
    a = WrapAngle(a);
    float d = (float)fabs(a - WrapAngle(b));
    if (d > 0.5f)
        d = 1.0f - d;
    return d;
}

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

class CThingCreatureBase : public CThing {
public:
    const C3DVector& GetPosition() const;       // 0x006A4D00
    bool IsPosInFOV(const C3DVector& position, bool useLookOffset) const;
};

bool CThingCreatureBase::IsPosInFOV(const C3DVector& position, bool useLookOffset) const
{
    if (UsingComponents[0x34 >> 5] & (1UL << (0x34 & 31))) {
        float fieldOfView = GetTC<CTCFieldOfView>(0x34)->GetFieldOfView();
        C3DVector delta = position - GetPosition();
        C3DVector dir = PhysicsTC->GetFacingDirection();
        float angle = WrapAngle((float)atan2(dir.X, dir.Y) * 0.15915494f);
        float facing = angle;
        if (UsingComponents[0x43 >> 5] & (1UL << (0x43 & 31))) {
            CTCLook* look = GetTC<CTCLook>(0x43);
            if (useLookOffset)
                facing = WrapAngle(look->GetHeadAngleOffset() + angle);
        }
        float target = WrapAngle((float)atan2(delta.X, delta.Y) * 0.15915494f);
        if (AngleDistance(target, facing) <= fieldOfView * 0.5f)
            return true;
    }
    return false;
}