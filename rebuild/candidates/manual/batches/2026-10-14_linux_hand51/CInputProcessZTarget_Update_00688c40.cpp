// CInputProcessZTarget::Update(CProcessedInput& processed) -- touch the hero creature (player +0x18, 0x00487DC0), count the lock timer (+0x34) down while it is not negative, and when the target (+0x24) is no longer valid (0x00445B80) and the (1, 2) button query (0x00687D50) does not hold, deactivate the Z-target (0x00688BA0 with the processed input)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00688C40, length 80 bytes.
// Leaf: ?Update@CInputProcessZTarget@@UAEXAAVCProcessedInput@@@Z
// The labels "CTCShotDefinitionBase::CanBeHitByMeleeStrike" (0x00445B80) and "CPersistContext::Transfer" (0x00687D50) are propagated. Shape: `test eax,eax ; jl ; dec eax`, `ret 4`.
struct CThing_Retail;
struct CPlayer {
    CThing_Retail* GetHeroCreature();              // 0x00487DC0
};
struct CTarget_00445b80 {
    bool IsValid();                                // 0x00445B80
};
struct CProcessedInput;
struct CInputProcessZTarget {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void Update(CProcessedInput& processed);
    unsigned char      _pad04[0x14];
    CPlayer*           m_pPlayer;                  // +0x18
    unsigned char      _pad1c[8];
    CTarget_00445b80*  m_pTarget;                  // +0x24
    unsigned char      _pad28[0xc];
    long               m_lockTimer;                // +0x34
    bool IsButtonHeld(long a, long b);             // 0x00687D50
    void DeactivateZTarget(CProcessedInput& processed);   // 0x00688BA0
};

void CInputProcessZTarget::Update(CProcessedInput& processed)
{
    m_pPlayer->GetHeroCreature();
    if (m_lockTimer >= 0)
        m_lockTimer = m_lockTimer - 1;
    if (!m_pTarget->IsValid() && !IsButtonHeld(1, 2))
        DeactivateZTarget(processed);
}
