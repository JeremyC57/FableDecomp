// CTCTeleporter::SetAsActive(bool active) -- on a change of TeleportActive stores it, latches TeleportActivated when switching on, and refreshes the player GUI
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x007DFEB0, 42 bytes (`ret 4`).
// Callees: the player-GUI refresh 0x004378C0 on the global 0x013B8790 (as the landed
//          CGameScriptInterface::SetRegionExitAsActive 0x0089EEC0; the "_Cons_val" label is propagated).
// Members: TeleportActive +0x10, TeleportActivated +0x11 (the keys persisted by vfunc_1 0x007DFF40).
// Shape: `active != member` in source order (`cmp al,[ecx+10h]`); the refresh is the last statement,
//        so it becomes a tail jump that reuses the dead argument slot for its 0 argument
//        (`mov [esp+4],0 ; jmp`).
class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool flag); // 0x004378C0
};

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
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
    }
}