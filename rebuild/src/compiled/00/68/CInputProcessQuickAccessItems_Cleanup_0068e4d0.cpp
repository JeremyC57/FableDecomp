// CInputProcessQuickAccessItems::Cleanup() -- when the player (+0x18) is in mode 0x1c (CPlayer::HasMode @ 0x00634160) and a quick-access charge is pending (0x0137EE78 != -1), cancel it (0x0068E3C0) and reset both global charge words (0x0137EE78 / 0x0137EE7C) to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068E4D0, length 64 bytes.
// Leaf: ?Cleanup@CInputProcessQuickAccessItems@@UAEXXZ
// Shape: `this` in esi, `cmp dword ptr [g],-1`, two global stores.
extern long g_quickAccessChargeItem_0137ee78;      // 0x0137EE78
extern long g_quickAccessChargeSlot_0137ee7c;      // 0x0137EE7C
class CPlayer { public:
    bool HasMode(long mode);                       // 0x00634160
};
class CInputProcessQuickAccessItems { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    unsigned char _pad04[0x14];
    CPlayer*      m_pPlayer;                       // +0x18
    void CancelQuickAccessCharge();                // 0x0068E3C0
    virtual void Cleanup();
};

void CInputProcessQuickAccessItems::Cleanup()
{
    if (m_pPlayer->HasMode(0x1c) && g_quickAccessChargeItem_0137ee78 != -1) {
        CancelQuickAccessCharge();
        g_quickAccessChargeItem_0137ee78 = -1;
        g_quickAccessChargeSlot_0137ee7c = -1;
    }
}