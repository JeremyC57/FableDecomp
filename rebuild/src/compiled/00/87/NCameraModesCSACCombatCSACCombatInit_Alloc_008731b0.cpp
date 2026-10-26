// NCameraModes::CSACCombat::CSACCombatInit::Alloc(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) const -- allocate a CSACCombat camera mode set up from the def's string length, cage radius, height offset and FOV
// Retail: Fable.exe (TLC, VC7.1) @ 0x008731B0, 74 bytes, `ret 8`.
// Leaf: ?Alloc@CSACCombatInit
// Name: vtable 0x0125C26C slot 0; FableWin.pdb ?Alloc@CSACCombatInit@CSACCombat@NCameraModes@@UBEPAVCBase@3@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z.
// Callees: operator new @ 0x00BFEA1A (0xd0 bytes), NCameraModes::CSACCombat::CSACCombat(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&,
//          float stringLength, float cageRadius, float heightOffset, bool autoGoBehind, float autoGoBehindTime, float defaultFOV)
//          @ 0x008733D0 (landed; signature from it).
// Layout: the def's fields sit 4 bytes below their Ego_r.pdb offsets (StringLengthXY +0x40, CageRadius +0x44, HeightOffsetZ +0x48,
//         FOV +0x50, the same shift the landed CBaseInit::AllocMain 0x0084D270 reads); the FOV is scaled by the literal 1/360
//         (0x01238E00).
// Shape: arguments pushed right to left: the scaled FOV (`push ecx ; fstp [esp]`), auto-go-behind time 0.0f and flag false as
//        two `push 0`, then the three def floats, the def and the manager; the def pointer is re-read once (`mov edx,[esi]`).
class CGameCameraManager;
class CCameraModeDef { public:
    unsigned char _pad00[0x40];
    float         StringLengthXY;                  // +0x40
    float         CageRadius;                      // +0x44
    float         HeightOffsetZ;                   // +0x48
    float         LookOffsetZ;                     // +0x4c
    float         FOV;                             // +0x50
};
template <class T> class CDefPointer { public:
    T* Def;
    T* operator->() const { return Def; }
};
namespace NCameraModes {
class CBase { public:
    virtual void v00();
    unsigned char _body[0xa0];                     // +0x04..+0xa3 (retail)
};
struct CSACCombat : CBase {                                // 0xd0 bytes
    unsigned char _body[0xd0 - 0xa4];
    CSACCombat(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def,
       float stringLength, float cageRadius, float heightOffset, bool autoGoBehind,
       float autoGoBehindTime, float defaultFOV);  // 0x008733D0
    struct CSACCombatInit {
        virtual CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const;
    };
};
CBase* CSACCombat::CSACCombatInit::Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const
{
    return new CSACCombat(manager, def, def->StringLengthXY, def->CageRadius, def->HeightOffsetZ, false, 0.0f,
                  def->FOV * (1.0f / 360.0f));
}
}