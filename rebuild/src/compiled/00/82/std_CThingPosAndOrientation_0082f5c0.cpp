// 0082f5c0 std::__default_constructed(CScriptedMapBrush::CThingPosAndOrientation*) -- construct the brush's mapped movement value
// Leaf: ??$__default_constructed@VCThingPosAndOrientation@CScriptedMapBrush@@@std@@YI?AVCThingPosAndOrientation@CScriptedMapBrush@@PAV12@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags: /Gr
// Layout: imported raw layouts are implementation projections; true bases are restored below. The outer object is never instantiated here.
#define CScriptedMapBrush CScriptedMapBrush_Layout
#include "engine/CScriptedMapBrush.h"
#undef CScriptedMapBrush
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#pragma pack(push, 1)
#include "engine/C3DVector.h"
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