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