// CInputProcessWatchForRangedWeaponThirdPersonModeTermination::Update(CProcessedInput& processed) -- with a live hero creature (player +0x18, 0x00487DC0; +0x91 bit 0 clear): while the hero keeps strafing (0x0068EB20, `__fastcall`) the countdown (+0x28) is reset to 5; otherwise it is decremented and, below zero, the player leaves modes 0x22 and 0x2e (0x00634190)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068F250, length 80 bytes.
// Leaf: ?Update@CInputProcessWatchForRangedWeaponThirdPersonModeTermination@@UAEXAAVCProcessedInput@@@Z
// Shape: `test byte ptr [eax+91h],1`, `dec dword ptr [esi+28h] ; jns`, `ret 4`.
struct CThing_Retail {
    unsigned char _pad[0x91];
    unsigned char m_91_0 : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
struct CPlayer {
    CThing_Retail* GetHeroCreature();              // 0x00487DC0 (the manifest label "_Cons_val<...>" is propagated)
    void RemoveMode(long mode);                    // 0x00634190
};
bool __fastcall IsThingToKeepStrafing_0068eb20(CThing_Retail* thing);   // 0x0068EB20
struct CProcessedInput;
struct CInputProcessWatchForRangedWeaponThirdPersonModeTermination {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void Update(CProcessedInput& processed);
    unsigned char _pad04[0x14];
    CPlayer*      m_pPlayer;                       // +0x18
    unsigned char _pad1c[0xc];
    long          m_countdown;                     // +0x28
};

void CInputProcessWatchForRangedWeaponThirdPersonModeTermination::Update(CProcessedInput& processed)
{
    CThing_Retail* hero = m_pPlayer->GetHeroCreature();
    if (hero && !hero->m_91_0) {
        if (IsThingToKeepStrafing_0068eb20(hero)) {
            m_countdown = 5;
            return;
        }
        if (--m_countdown < 0) {
            m_pPlayer->RemoveMode(0x22);
            m_pPlayer->RemoveMode(0x2e);
        }
    }
}
