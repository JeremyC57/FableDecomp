// CTCDCameraPoint::CTCDCameraPoint(CThing& thing) -- thing-component constructor: the CTCDriverBase base, then the members zeroed
// Leaf: ??0CTCDCameraPoint@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D812A, 38 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): stores vtable 0x0123EC3C (RTTI .?AVCTCDCameraPoint@@); the zeroed members match Ego_r.pdb CTCDCameraPoint; its caller is the component allocator 0x004DAFCA (catalogued CTCDCameraPoint::Alloc); the base constructor 0x00771200 is bound ??0CTCDriverBase@@QAE@AAVCThing@@@Z.
// Callees: CTCDriverBase::CTCDriverBase(CThing&) 0x00771200.
// Shape (family M095, representative 0x004D43B9): `: CTCDriverBase(thing), CutInto(false), CutOutOf(false), TestAngleBeforeActivation(false), Type(CAMERA_POINT_TYPE_NONE) {}` (the landed CTCGold 0x007CBDA0 model);
//   `optimize("s")`: the thing reference pushed straight from its slot.
// Header debt: engine/CTCBase.h flattens the vptr, so the base is modelled locally (as the landed CTCGold).
#pragma optimize("s", on)
class CThing;
class CTCBase {                                          // retail 0x0C bytes (Ego_r.pdb)
public:
    CTCBase(CThing& thing);                              // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};
enum ECameraPointType { CAMERA_POINT_TYPE_NONE = 0 };
class CTCDriverBase : public CTCBase {
public:
    CTCDriverBase(CThing& thing);                        // 0x00771200
};
class CTCDCameraPoint : public CTCDriverBase {
public:
    CTCDCameraPoint(CThing& thing);
    bool CutInto;                                        // +0x0C
    bool CutOutOf;                                       // +0x0D
    bool TestAngleBeforeActivation;                      // +0x0E
    ECameraPointType Type;                               // +0x10
};
CTCDCameraPoint::CTCDCameraPoint(CThing& thing)
    : CTCDriverBase(thing), CutInto(false), CutOutOf(false), TestAngleBeforeActivation(false), Type(CAMERA_POINT_TYPE_NONE)
{
}