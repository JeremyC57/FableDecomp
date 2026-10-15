// CQR_EscortTraderScript::EscortTrader() -- pump the game interface until the trader has departed or the script thread is being terminated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1F3A0, length 52 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed; reads the active thread at
// +0x2c), game-interface vtable slot 7 (+0x1c, Pump) through the interface pointer at +0x40.
// Layout: the retail script base is 0x48 bytes (CScriptBase 0x40 + the quest base's interface
// pointer at +0x40 and one more word); the PDB members of CQR_EscortTraderScript sit 0x14 higher in
// Ego_r.pdb (debug-build containers), so PDB TraderDeparted (+0x65) is retail +0x51.
// Shape: `while (!TraderDeparted) { Pump(); if (terminating) return; }` then a final
// IsActiveThreadTerminating() whose discarded result VC7.1 turns into a tail `jmp`; the loop is
// rotated (entry test + bottom test) with the 6-byte `lea ebx,[ebx]` alignment pad.
struct CGameScriptInterface_Calls {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void Pump();                                    // slot 7 (+0x1c)
};

struct CScriptBase_Retail {
    virtual void v00();
    unsigned char _pad[0x3c];                               // +0x04 .. +0x40 (active thread at +0x2c)
    unsigned char IsActiveThreadTerminating();              // 0x00CB7940
};

struct CQR_EscortTraderScript : CScriptBase_Retail {
    CGameScriptInterface_Calls* m_pInterface;               // +0x40
    unsigned char _pad44[0x0d];                             // +0x44 .. +0x51 (MissionSucceeded.. at +0x48)
    bool          TraderDeparted;                           // +0x51
    void EscortTrader();
};

void CQR_EscortTraderScript::EscortTrader()
{
    while (!TraderDeparted) {
        m_pInterface->Pump();
        if (IsActiveThreadTerminating())
            return;
    }
    IsActiveThreadTerminating();
}