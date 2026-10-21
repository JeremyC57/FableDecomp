// CMainGameComponent::OnPostDeviceReset() -- CDeviceResetCallback override: re-creates the fonts after a device reset and reports success
// Retail: Fable.exe @ 0x004175DA, 11 bytes: `add ecx,-4 ; call 0x004168DC ; mov al,1 ; ret`.
// RTTI (COL at vtable-4): CMainGameComponent : CGameComponent (: CBaseClassNonCopyable : CBaseClass) at +0,
// CDeviceResetCallback at +4; this is slot 1 of the callback vtable 0x0122F174, so VC7.1 compiles it with `this`
// at the +4 interface and moves it back for the call to CMainGameComponent::InitialiseFonts 0x004168DC.
// Pragma: CMainGameComponent size TU (`optimize("s", on)` + `("y", off)`, as landed 0x004168DC).
#pragma optimize("s", on)
#pragma optimize("y", off)
struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();
};
struct CGameComponent : CBaseClassNonCopyable {
};
struct CDeviceResetCallback {
    virtual bool OnPreDeviceReset() = 0;
    virtual bool OnPostDeviceReset() = 0;
};
struct CMainGameComponent : CGameComponent, CDeviceResetCallback {
    void InitialiseFonts();                              // 0x004168DC
    virtual bool OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
};

bool CMainGameComponent::OnPostDeviceReset()
{
    InitialiseFonts();
    return true;
}