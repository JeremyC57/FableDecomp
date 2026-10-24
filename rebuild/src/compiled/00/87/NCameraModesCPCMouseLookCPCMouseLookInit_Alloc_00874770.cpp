// NCameraModes::CPCMouseLook::CPCMouseLookInit::Alloc(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) const -- allocate and construct a PC mouse-look camera mode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00874770, 28 bytes, `ret 8`.
// Leaf: ?Alloc@CPCMouseLookInit
// Name: vtable 0x0125C284 slot 0; FableWin.pdb ?Alloc@CPCMouseLookInit@CPCMouseLook@NCameraModes@@UBEPAVCBase@3@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z.
// Callees: operator new @ 0x00BFEA1A (0x10c bytes), NCameraModes::CPCMouseLook::CPCMouseLook(CGameCameraManager&,
//          const CDefPointer<const CCameraModeDef>&) @ 0x00874E00 (out of line, `ret 8`).
// Shape: the constructor takes exactly Alloc's two stack arguments and returns `this`, so VC7.1 forwards them with a
//        sibling `jmp` on the non-null path (no pushes); a null block returns 0.
struct CGameCameraManager;
struct CCameraModeDef;
template <class T> struct CDefPointer;
namespace NCameraModes {
struct CBase {
    virtual void v00();
    unsigned char _body[0xa0];                     // +0x04..+0xa3 (retail)
};
struct CPCMouseLook : CBase {                      // 0x10c bytes
    unsigned char _body[0x10c - 0xa4];
    CPCMouseLook(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);   // 0x00874E00
    struct CPCMouseLookInit {
        virtual CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const;
    };
};
CBase* CPCMouseLook::CPCMouseLookInit::Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const
{
    return new CPCMouseLook(manager, def);
}
}