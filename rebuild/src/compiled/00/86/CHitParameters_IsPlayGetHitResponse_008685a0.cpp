// CHitParameters::IsPlayGetHitResponse(CThing* target) -- when the override flag (+0x21) is set, the stored answer (+0x20); else false when the shot definition (+0x48) exists and cannot be hit by a melee strike (0x00815270); else true when there is no target, the target's +0x91 bit 0 is set, or the target plays hit responses (0x0085EE90, `__fastcall`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008685A0, length 80 bytes.
// Shape: byte loads, the shared `xor al,al` / `mov al,1` tails, `ret 4`.
struct CThing_008685a0 {
    unsigned char _pad[0x91];
    unsigned char m_91_0 : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
struct CTCShotDefinitionBase {
    bool CanBeHitByMeleeStrike();                  // 0x00815270
};
bool __fastcall IsPlayHitResponse_0085ee90(CThing_008685a0* thing);   // 0x0085EE90
struct CHitParameters {
    unsigned char          _pad[0x20];
    bool                   m_playGetHitResponse;   // +0x20
    bool                   m_overrideGetHitResponse;   // +0x21
    unsigned char          _pad22[0x26];
    CTCShotDefinitionBase* m_pShotDef;             // +0x48
    bool IsPlayGetHitResponse(CThing_008685a0* target);
};

bool CHitParameters::IsPlayGetHitResponse(CThing_008685a0* target)
{
    if (m_overrideGetHitResponse)
        return m_playGetHitResponse;
    if (m_pShotDef && !m_pShotDef->CanBeHitByMeleeStrike())
        return false;
    if (!target || target->m_91_0 || IsPlayHitResponse_0085ee90(target))
        return true;
    return false;
}