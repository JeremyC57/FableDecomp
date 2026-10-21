// CNewFrontendGameComponent::ReleaseXMVCode() -- unloads the XMV video player code if this component loaded it
// Leaf: ReleaseXMVCode@CNewFrontendGameComponent
// Retail: Fable.exe @ 0x0042DBD8, 22 bytes; Ego_r.pdb ?ReleaseXMVCode@CNewFrontendGameComponent@@AAEXXZ (private). Calls
// CXMVPlayer::StaticReleaseXMVCode() @ 0x009BCFC0 (public static __fastcall, no arguments). Callers 0x0042EC2C, 0x0042F23A.
// Layout: retail CNewFrontendGameComponent XMVCodeLoaded +0xb1 (Ego_r.pdb +0xb9; retail is 8 below after its vectors).
// Shape: size TU (`lea esi,[ecx+0xb1]` keeps the flag address in a callee-saved register).
#pragma optimize("s", on)
class CXMVPlayer {
public:
    static void __fastcall StaticReleaseXMVCode();      // 0x009BCFC0
};
class CNewFrontendGameComponent {
public:
    unsigned char _pad00[0xb1];
    bool          XMVCodeLoaded;                         // +0xb1
private:
    void ReleaseXMVCode();
};

void CNewFrontendGameComponent::ReleaseXMVCode()
{
    if (XMVCodeLoaded)
    {
        CXMVPlayer::StaticReleaseXMVCode();
        XMVCodeLoaded = false;
    }
}