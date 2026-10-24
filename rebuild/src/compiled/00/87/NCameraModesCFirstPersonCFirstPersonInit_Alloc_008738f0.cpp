// NCameraModes::CFirstPerson::CFirstPersonInit::Alloc(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) const -- allocate and construct a first-person camera mode
// Retail: Fable.exe (TLC, VC7.1) @ 0x008738F0, 53 bytes, `ret 8`.
// Leaf: ?Alloc@CFirstPersonInit
// Name: vtable 0x0125C274 slot 0; FableWin.pdb ?Alloc@CFirstPersonInit@CFirstPerson@NCameraModes@@UBEPAVCBase@3@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z.
// Callees: operator new @ 0x00BFEA1A (0xd4 bytes, the retail CFirstPerson size), NCameraModes::CBase::CBase(CGameCameraManager&,
//          const CDefPointer<const CCameraModeDef>&) @ 0x0084E420 (as the landed CSACCombat ctor 0x008733D0 declares it).
// Shape: `new` null-tests the block; CFirstPerson's constructor is inline and only stores its vtable 0x012776AC after the root ctor
//        (no members of its own are initialised); a null block returns 0.
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
struct CFirstPerson : CBase {                      // vtable 0x012776AC, 0xd4 bytes
    unsigned char _body[0xd4 - 0xa4];
    virtual void v00();
    CFirstPerson(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) : CBase(manager, def) { }
    struct CFirstPersonInit {
        virtual CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const;
    };
};
CBase* CFirstPerson::CFirstPersonInit::Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const
{
    return new CFirstPerson(manager, def);
}
}