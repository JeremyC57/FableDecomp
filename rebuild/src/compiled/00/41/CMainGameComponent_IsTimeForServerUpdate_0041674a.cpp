// CMainGameComponent::IsTimeForServerUpdate(long frame) const -- whether the server should step: never while the component is inactive, always under the forced/unthrottled flags, else when the next present lies more than one world frame past `frame`
// Leaf: ?IsTimeForServerUpdate@CMainGameComponent@@QBE_NJ@Z
// Retail: Fable.exe @ 0x0041674A, 93 bytes, thiscall `ret 4`; al doubles as the zero register and the false result.
// Body: if (m_b09) { if ((g_b13B860C && g_pInputManager->b38 && !g_b13B8629) || g_b13B8688) return true;
// float framesAhead = ConvertGTToWF(GetGameTimeOfNextPresentCompletion() /*0x004166E2*/) - frame; if (framesAhead > 1.0f)
// return true; } return false.
// Layout: CGameComponent base (two vptrs, bool +0x08, bool +0x09) as in the landed ForceRender model; no engine header
// exists for CMainGameComponent. g_pInputManager @ 0x013B8388 (bool at +0x38); the three bool flags 0x013B860C,
// 0x013B8629, 0x013B8688 are unnamed globals; g_WorldFramesPerSecond is the long at 0x01375550 (1.0f at 0x0122DED8).
// Names: Ego_r.pdb ?IsTimeForServerUpdate@CMainGameComponent@@QBE_NJ@Z (0x00415A13 in Ego_r); ConvertGTToWF is
// ?ConvertGTToWF@CMainGameComponent@@IBENN@Z (out-of-line copy 0x0041623E: gt / (1.0f / WFPS)).
// Shape: the `float framesAhead` local is what keeps a copy for the compare (fisub; fstp st(1); fld st(0); fcomp dword
// 1.0f); a double local or a direct compare gives `fcomp qword` with no copy. The nested if with returns inside the
// m_b09 block gives the single `xor al,al` false exit.
// Opt: size TU (`#pragma optimize("s", on)`: cmp byte [mem], al against the zeroed al).
#pragma optimize("s", on)

extern long g_WorldFramesPerSecond;               // 0x01375550
struct CInputManager { char _pad[0x38]; bool b38; };
extern CInputManager* g_pInputManager;            // 0x013B8388
extern bool g_b13B860C;                           // 0x013B860C
extern bool g_b13B8629;                           // 0x013B8629
extern bool g_b13B8688;                           // 0x013B8688

class CMainGameComponent
{
public:
    void* _vfptr;                                 // +0x00 CBaseClass
    void* _vfptrEntry;                            // +0x04 CComponentListEntry
    bool m_b08;                                   // +0x08
    bool m_b09;                                   // +0x09
    double GetGameTimeOfNextPresentCompletion() const;   // 0x004166E2
    bool IsTimeForServerUpdate(long frame) const;
protected:
    double ConvertGTToWF(double gameTime) const { return gameTime / (1.0f / g_WorldFramesPerSecond); }
};

bool CMainGameComponent::IsTimeForServerUpdate(long frame) const
{
    if (m_b09) {
        if ((g_b13B860C && g_pInputManager->b38 && !g_b13B8629) || g_b13B8688) return true;
        float framesAhead = ConvertGTToWF(GetGameTimeOfNextPresentCompletion()) - frame;
        if (framesAhead > 1.0f) return true;
    }
    return false;
}