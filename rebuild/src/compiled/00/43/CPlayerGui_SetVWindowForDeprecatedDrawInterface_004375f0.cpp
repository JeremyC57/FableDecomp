// CPlayerGui::SetVWindowForDeprecatedDrawInterface(const C2DBoxF& window) -- store the virtual window and pass it with the visible TV area to the subtitle renderer
// Leaf: ?SetVWindowForDeprecatedDrawInterface@CPlayerGui@@QAEXABVC2DBoxF@@@Z
// Retail: Fable.exe @ 0x004375F0, 79 bytes, `ret 4`; caller 0x0069EB2A; Ego_r.pdb
// ?SetVWindowForDeprecatedDrawInterface@CPlayerGui@@QAEXABVC2DBoxF@@@Z.
// Body: VWindow (+0x280) = window; GFGetSystemManager() (0x009A4EC0, __fastcall) ->PDisplayManager (+0x60)
// ->GetVVisibleTelevisionScreenArea() (0x009BECE0, C2DBoxF by hidden pointer); then
// SubtitleRenderer (+0x18).SetVWindowsForDeprecatedDrawInterface(window, area) (0x0048DBA6).
// Layout: retail CPlayerGui -- SubtitleRenderer +0x18, VWindow +0x280 (Ego_r.pdb offsets shifted by the 12-byte STLport
// vectors); C2DBoxF {float TLX, TLY, BRX, BRY} with a constructor (returned through memory).
// Shape: the display manager bound to a local first (system->PDisplayManager then `mov ecx,eax`); /O2 (16-aligned entry).
class C2DBoxF {
public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() {}
};
class CDisplayManager {
public:
public:
    C2DBoxF GetVVisibleTelevisionScreenArea() const;   // 0x009BECE0
};
class CSystemManager {
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* PDisplayManager;                        // +0x60 CCountedPointer<CDisplayManager>::Data
};
CSystemManager* __fastcall GFGetSystemManager();             // 0x009A4EC0
class CSubtitleRenderer {
public:
    void SetVWindowsForDeprecatedDrawInterface(const C2DBoxF& window, const C2DBoxF& tv_area);   // 0x0048DBA6
};
class CPlayerGui {
public:
    void*             _vfptr;                                // +0x000
    unsigned char     _pad004[0x18 - 0x4];
    CSubtitleRenderer SubtitleRenderer;                      // +0x018
    unsigned char     _pad019[0x280 - 0x19];
    C2DBoxF           VWindow;                               // +0x280
    void SetVWindowForDeprecatedDrawInterface(const C2DBoxF& window);
};
void CPlayerGui::SetVWindowForDeprecatedDrawInterface(const C2DBoxF& window)
{
    VWindow = window;
    CDisplayManager* display = GFGetSystemManager()->PDisplayManager;
    SubtitleRenderer.SetVWindowsForDeprecatedDrawInterface(window, display->GetVVisibleTelevisionScreenArea());
}