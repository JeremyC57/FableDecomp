#ifndef _MSC_VER
#define __fastcall
#endif
#include <string.h>
extern "C" int puts(const char*);
// 0082f5c0 std::__default_constructed(CScriptedMapBrush::CThingPosAndOrientation*) -- construct the brush's mapped movement value
// Leaf: ??$__default_constructed@VCThingPosAndOrientation@CScriptedMapBrush@@@std@@YI?AVCThingPosAndOrientation@CScriptedMapBrush@@PAV12@@Z
// Flags: /Gr
// Layout: imported raw layouts are implementation projections; true bases are restored below. The outer object is never instantiated here.
#define CBaseObject CBaseObject_Layout
#include "engine/CBaseObject.h"
#undef CBaseObject
#define CScriptedMapBrush CScriptedMapBrush_Layout
#include "engine/CScriptedMapBrush.h"
#undef CScriptedMapBrush
class CBaseClass { public: virtual ~CBaseClass(); };
class CBaseObject : public CBaseClass {
public:
    CBaseObjectPointer* IntelligentPointer;
protected:
    virtual ~CBaseObject();
};
#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class C3DAngles {
public:
    float AngleXY, AngleYZ, AngleXZ;
    C3DAngles() { }
};
class CScriptedMapBrush : public CBaseObject {
public:
    // Layout: no outer members are read by the selected template.
    unsigned char unobservedBrushState[sizeof(CScriptedMapBrush_Layout)-sizeof(CBaseObject)];
    class CThingPosAndOrientation {
    public:
        C3DVector Position;
        C3DAngles FacingAngles;
    };
};
#pragma pack(pop)
namespace std {
struct __false_type { };
template<class T> inline T __default_constructed_aux(T*, const __false_type&) {
    return T();
}
template<class T> T __fastcall __default_constructed(T* p) {
    return __default_constructed_aux(p, __false_type());
}
template CScriptedMapBrush::CThingPosAndOrientation __fastcall __default_constructed(CScriptedMapBrush::CThingPosAndOrientation*);
}

CBaseClass::~CBaseClass() { }
CBaseObject::~CBaseObject() { }
typedef CScriptedMapBrush::CThingPosAndOrientation FixtureMovement;
struct FixtureGuardedMovement { unsigned long before;FixtureMovement value;unsigned long after; };
static int fixture_constructions=0;
class FixtureDefinedValue {
public:
    long payload;
    FixtureDefinedValue():payload(73) { ++fixture_constructions; }
};
int main() {
    if(sizeof(FixtureMovement)!=24 || sizeof(FixtureGuardedMovement)!=32)return 1;
    FixtureMovement argument;
    argument.Position.X=1;argument.Position.Y=2;argument.Position.Z=3;
    argument.FacingAngles.AngleXY=4;argument.FacingAngles.AngleYZ=5;argument.FacingAngles.AngleXZ=6;
    unsigned char saved[24];memcpy(saved,&argument,24);
    // The selected original leaves floats unspecified. Exercise its actual return ABI and bounds without reading those values.
    for(unsigned long trial=0;trial<4096;++trial) {
        FixtureGuardedMovement guarded;
        guarded.before=0x12345678UL^trial;guarded.after=0xfedcba98UL^trial;
        guarded.value=std::__default_constructed(&argument);
        if(guarded.before!=(0x12345678UL^trial) || guarded.after!=(0xfedcba98UL^trial))return 2;
        if(memcmp(saved,&argument,24)!=0)return 3;
    }
    for(int trial=0;trial<4096;++trial) {
        argument.Position.X=(float)trial;argument.Position.Y=(float)-trial;argument.Position.Z=(float)(trial+5);
        argument.FacingAngles.AngleXY=(float)(trial-3);argument.FacingAngles.AngleYZ=(float)(trial+7);argument.FacingAngles.AngleXZ=(float)(trial*2);
        FixtureMovement copy(argument),assigned;assigned=copy;
        if(memcmp(&argument,&copy,24)!=0 || memcmp(&argument,&assigned,24)!=0)return 4;
    }
    FixtureDefinedValue seed;seed.payload=-9;fixture_constructions=0;
    FixtureDefinedValue value=std::__default_constructed(&seed);
    if(value.payload!=73 || seed.payload!=-9 || fixture_constructions!=1)return 5;
    puts("0082f5c0_TEST PASS");return 0;
}