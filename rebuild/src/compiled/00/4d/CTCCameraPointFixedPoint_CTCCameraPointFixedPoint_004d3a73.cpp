// CTCCameraPointFixedPoint::CTCCameraPointFixedPoint(CThing& thing) -- CTCCameraPointShapeActivationBase(thing), then this class's vtable and TrackThing = true
// Leaf: ??0CTCCameraPointFixedPoint@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3A73, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123AF8C.
// Identity: CTCCameraPointFixedPoint::CTCCameraPointFixedPoint(CThing&) -- proven; evidence: it stores vtable 0x0123AF8C whose RTTI is .?AVCTCCameraPointFixedPoint@@ : CTCCameraPointShapeActivationBase : CTCCameraPointDefinitionBase : CTCBase;
//   FableWin.pdb name set holds ??0CTCCameraPointFixedPoint@@QAE@AAVCThing@@@Z; its only caller is the landed type factory 0x004D81E6 (CTCCameraPointFixedPoint_Alloc); the base 0x004D3A03 stores vtable 0x0123AEDC (.?AVCTCCameraPointShapeActivationBase@@) after CTCCameraPointDefinitionBase 0x0075C180. Family G056 (seven such constructors; the propagated label
//   "CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun" is right only for 0x004DCA62).
// Callees: CTCCameraPointShapeActivationBase::CTCCameraPointShapeActivationBase(CThing&) 0x004D3A03 (landed callers bind it as ??0CTCCameraPointShapeActivationBase@@QAE@PAUCThing@@@Z; the PDB takes CThing&).
// Layout: Ego_r.pdb CTCCameraPointFixedPoint::TrackThing (bool) at retail +0x40; LookVector (C3DVector) after it has a trivial constructor.
#pragma optimize("s", on)
class CThing;
class CTCCameraPointShapeActivationBase {                                     // retail 0x40 bytes
public:
    CTCCameraPointShapeActivationBase(CThing& thing);                       // 0x004D3A03
    virtual ~CTCCameraPointShapeActivationBase();
    unsigned char _body[0x40 - 4];
};
class CTCCameraPointFixedPoint : public CTCCameraPointShapeActivationBase {
public:
    CTCCameraPointFixedPoint(CThing& thing);
    bool TrackThing;                    // +0x40
};
CTCCameraPointFixedPoint::CTCCameraPointFixedPoint(CThing& thing)
    : CTCCameraPointShapeActivationBase(thing), TrackThing(true)
{
}