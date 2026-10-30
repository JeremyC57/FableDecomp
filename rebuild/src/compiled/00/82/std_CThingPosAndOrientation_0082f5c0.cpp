// 0082f5c0 std::__default_constructed(CScriptedMapBrush::CThingPosAndOrientation*) -- construct the brush's mapped movement value
// Leaf: ??$__default_constructed@VCThingPosAndOrientation@CScriptedMapBrush@@@std@@YI?AVCThingPosAndOrientation@CScriptedMapBrush@@PAV12@@Z
// Flags: /Gr
// Layout: imported raw layouts are implementation projections; true bases are restored below. The outer object is never instantiated here.
#define CScriptedMapBrush CScriptedMapBrush_Layout
#include "engine/CScriptedMapBrush.h"
#undef CScriptedMapBrush
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
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