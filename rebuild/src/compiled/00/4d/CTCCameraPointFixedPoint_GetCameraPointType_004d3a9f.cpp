// CTCCameraPointFixedPoint::GetCameraPointType() const -- returns (ECameraPointType)0
// Leaf: ?GetCameraPointType@CTCCameraPointFixedPoint@@UBE?AW4ECameraPointType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3A9F, thiscall, `ret`.
// Identity: slot 28 of CTCCameraPointFixedPoint's vtable 0x0123AF8C (defined here: the slot holds this body in 1 vtable(s), all of CTCCameraPointFixedPoint or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts GetCameraPointType(void) at this slot; Ego_r/FableWin name set: ?GetCameraPointType@CTCCameraPointFixedPoint@@UBE?AW4ECameraPointType@@XZ; `ret` matches the signature.
enum ECameraPointType { };
class CTCCameraPointFixedPoint {
public:
    virtual ECameraPointType GetCameraPointType() const;
};
ECameraPointType CTCCameraPointFixedPoint::GetCameraPointType() const
{
    return (ECameraPointType)0;
}
