// CTCTeleporter::SetAsActive(bool active) -- on a change of TeleportActive stores it, latches TeleportActivated when switching on, and refreshes the player GUI
// Retail: Fable.exe @ 0x007DFEB0, 42 bytes (`ret 4`).
// Callees: the player-GUI refresh 0x004378C0 on the global 0x013B8790 (as the landed
//          CGameScriptInterface::SetRegionExitAsActive 0x0089EEC0; the "_Cons_val" label is propagated).
// Members: TeleportActive +0x10, TeleportActivated +0x11 (the keys persisted by vfunc_1 0x007DFF40).
// Shape: `active != member` in source order (`cmp al,[ecx+10h]`); the refresh is the last statement,
//        so it becomes a tail jump that reuses the dead argument slot for its 0 argument
//        (`mov [esp+4],0 ; jmp`).
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool flag); // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;        // 0x013B8790

class CTCTeleporter { public:
    unsigned char _pad[0x10];
    bool m_teleportActive;              // +0x10
    bool m_teleportActivated;           // +0x11
    void SetAsActive(bool active);
};

void CTCTeleporter::SetAsActive(bool active)
{
    if (active != m_teleportActive) {
        m_teleportActive = active;
        if (active)
            m_teleportActivated = true;
        g_pPlayerGui->UpdateMiniMapMarkers(false);
    }
}