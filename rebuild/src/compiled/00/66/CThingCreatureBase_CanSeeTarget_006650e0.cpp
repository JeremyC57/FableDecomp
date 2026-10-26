// CThingCreatureBase::CanSeeTarget(const C3DVector& position, float fieldOfView, bool checkRange, bool checkObstruction) -- ask the look component (TC 0x43) when there is one, else test the angle from the head to the position against the facing: twice the angle difference within the field of view
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006650E0, length 171 bytes, `ret 10h`, bool in al.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0, CTCLook::IsPosInViewRange(const C3DVector&, float, bool, bool)
//   @ 0x007534B0, CTCPhysicsBase::GetFacingAngleXY (slot 63, +0xFC), CThingCreatureBase::GetHeadPos(bool) @ 0x00661B40
//   (C3DVector by hidden pointer), AngleBetweenPositions(from, to) @ 0x00661540 (`__fastcall`, float), AngleDifference(a, b)
//   @ 0x00A441E0 (`__stdcall`, `ret 8`, float).
// Layout: retail CThing interface bitset +0x20 (TC 0x43 = word +0x28 bit 3), interface map +0x44, PhysicsTC +0x60.
// Shape: HasTC is the raw bitset test and GetTC the inline const lookup forwarding all four arguments; the facing is a
//   named float local parked in the dead last-argument slot across the head-position call; `2*|d| <= fov` with
//   separate true/false exits (`test ah,41h ; jp`), the doubling as `fadd st,st`.
#include <math.h>
#pragma intrinsic(fabs)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};
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
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5)
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY();           // slot 63 (+0xFC)
};

class CTCLook { public:
    bool IsPosInViewRange(const C3DVector& position, float fieldOfView, bool checkRange, bool checkObstruction);   // 0x007534B0
};

float __fastcall AngleBetweenPositions(const C3DVector& from, const C3DVector& to);   // 0x00661540
float __stdcall AngleDifference(float a, float b);                                    // 0x00A441E0

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    template<class T> __forceinline T* GetTC(long interfaceType)
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
    C3DVector GetHeadPos(bool exact);           // 0x00661B40
    bool CanSeeTarget(const C3DVector& position, float fieldOfView, bool checkRange, bool checkObstruction);
};

bool CThingCreatureBase::CanSeeTarget(const C3DVector& position, float fieldOfView, bool checkRange, bool checkObstruction)
{
    if (UsingComponents[0x43 >> 5] & (1UL << (0x43 & 31)))
        return GetTC<CTCLook>(0x43)->IsPosInViewRange(position, fieldOfView, checkRange, checkObstruction);
    float facing = PhysicsTC->GetFacingAngleXY();
    if (fabs(AngleDifference(facing, AngleBetweenPositions(GetHeadPos(true), position))) * 2.0f <= fieldOfView)
        return true;
    return false;
}