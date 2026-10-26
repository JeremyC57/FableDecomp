// CSystemManager::IsApplicationInputFocus() -- return ApplicationHasInputFocus (+0x0A)
// Leaf: ?IsApplicationInputFocus@CSystemManager@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A4FB0, 4 bytes (`mov al,[ecx+0Ah] ; ret`).
// Identity: Ego_r.pdb ?IsApplicationInputFocus@CSystemManager@@QAE_NXZ, and Ego_r CSystemManager's ApplicationHasInputFocus
//   is the bool at +10 (WindowsQuit +8, ApplicationActive +9); all eight callers (CMouseDX / CJoystickDX at 0x00AB484A ..
//   0x00AB6E8F) call it on the GFGetSystemManager() result (0x009A4EC0, `mov ecx,eax`), right after
//   CSystemManager::IsApplicationActive 0x009A57B0 -- e.g. CMouseDX::UpdateAcquired 0x00AB4830. The landed stand-in
//   (C_009a4fb0::M, leaf "GetCanItemBeQuickAssigned") carried the manifest label CTCInventoryBase::GetCanItemBeQuickAssigned,
//   propagated from a byte-identical getter of another class.
// Layout: Ego_r.pdb CSystemManager (376 bytes): WindowsQuit +8, ApplicationActive +9, ApplicationHasInputFocus +10.
class CSystemManager {
public:
    unsigned char _pad00[0x8];
    bool WindowsQuit;                              // +0x08
    bool ApplicationActive;                        // +0x09
    bool ApplicationHasInputFocus;                 // +0x0a
    bool IsApplicationInputFocus();
};

bool CSystemManager::IsApplicationInputFocus()
{
    return ApplicationHasInputFocus;
}
