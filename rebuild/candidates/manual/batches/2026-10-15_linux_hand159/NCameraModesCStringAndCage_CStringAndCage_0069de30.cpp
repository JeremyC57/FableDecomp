// NCameraModes::CStringAndCage::CStringAndCage(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&, float stringLength, float cageRadius, float heightOffset, bool autoGoBehind, float autoGoBehindTime, float defaultFOV) -- the camera-mode root, the string-and-cage targets, both expansion timers at -1, the auto-go-behind settings and the default FOV
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DE30, 104 bytes; `ret 20h`. Vtable 0x0125C404 (NCameraModes::CStringAndCage).
// Leaf: ??0CStringAndCage
// Callees: NCameraModes::CBase::CBase(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) @ 0x0084E420.
// Layout: Ego_r.pdb members, 0x24 bytes lower in retail (the landed CSimplePointBase 0x00873A10 shows the same shift):
//   CBase::DefaultFOV +0x04; CStringAndCageBase DesiredHeightOffset +0xa4, DesiredStringLength +0xa8, DesiredCageRadius +0xac,
//   Current* +0xb0..+0xb8, NextAutoGoBehindWF +0xbc, TimerForExpansion +0xc0, DriftToMaxStringLength +0xc4;
//   CStringAndCage AutoGoBehind +0xc8, AutoGoBehindTime +0xcc.
// Shape: CStringAndCageBase is a __declspec(novtable) intermediate base (only the derived vtable is stored);
//        DefaultFOV is a body assignment (stored after the vtable).
struct CGameCameraManager;
struct CCameraModeDef;
template <class T> struct CDefPointer;
namespace NCameraModes {
struct CBase {
    virtual void v00();
    float         DefaultFOV;                      // +0x04
    unsigned char _body[0x9c];                     // +0x08..+0xa3 (retail)
    CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
};
struct __declspec(novtable) CStringAndCageBase : CBase {
    float DesiredHeightOffset;                     // +0xa4
    float DesiredStringLength;                     // +0xa8
    float DesiredCageRadius;                       // +0xac
    float CurrentHeightOffset;                     // +0xb0
    float CurrentStringLength;                     // +0xb4
    float CurrentCageRadius;                       // +0xb8
    long  NextAutoGoBehindWF;                      // +0xbc
    long  TimerForExpansion;                       // +0xc0
    bool  DriftToMaxStringLength;                  // +0xc4
    CStringAndCageBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def,
                       float stringLength, float cageRadius, float heightOffset)
        : CBase(manager, def), DesiredHeightOffset(heightOffset), DesiredStringLength(stringLength),
          DesiredCageRadius(cageRadius), NextAutoGoBehindWF(-1), TimerForExpansion(-1)
    {
    }
};
struct CStringAndCage : CStringAndCageBase {       // vtable 0x0125C404
    bool  AutoGoBehind;                            // +0xc8
    float AutoGoBehindTime;                        // +0xcc
    virtual void v00();
    CStringAndCage(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def,
                   float stringLength, float cageRadius, float heightOffset, bool autoGoBehind,
                   float autoGoBehindTime, float defaultFOV);
};

CStringAndCage::CStringAndCage(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def,
                               float stringLength, float cageRadius, float heightOffset, bool autoGoBehind,
                               float autoGoBehindTime, float defaultFOV)
    : CStringAndCageBase(manager, def, stringLength, cageRadius, heightOffset),
      AutoGoBehind(autoGoBehind), AutoGoBehindTime(autoGoBehindTime)
{
    DefaultFOV = defaultFOV;
}
}
