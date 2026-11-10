// CFrontendGameComponent::ChangeSelection @ 0x00494380.
// Name fix (agentNF6 2026-11-10): GFGetTime 0x009E1BC0 is ?GFGetTime@@YINXZ (Ego_r.pdb / FableWin.pdb: global __fastcall, double, no arguments).

extern double g_FrontendInitialRepeatDelay_00494380;
extern double g_FrontendHeldRepeatDelay_00494380;
double __fastcall GFGetTime();   // 0x009E1BC0 ?GFGetTime@@YINXZ

class CFrontendGameComponent_00494380
{
    unsigned char m_Pad0000[0x1e48];
    double m_LastSelectionTime;
    bool m_IsRepeatingSelection;
    unsigned char m_Pad1e51[3];
    long m_LastMovement;

public:
    bool ChangeSelection(long);
};

bool CFrontendGameComponent_00494380::ChangeSelection(long movement)
{
    if (m_LastMovement != movement)
    {
        m_IsRepeatingSelection = false;
        m_LastSelectionTime = GFGetTime();
        m_LastMovement = movement;
        return true;
    }

    double delay = m_IsRepeatingSelection
        ? g_FrontendHeldRepeatDelay_00494380
        : g_FrontendInitialRepeatDelay_00494380;

    if (GFGetTime() - m_LastSelectionTime >= delay)
    {
        m_IsRepeatingSelection = true;
        m_LastSelectionTime = GFGetTime();
        return true;
    }

    return false;
}
