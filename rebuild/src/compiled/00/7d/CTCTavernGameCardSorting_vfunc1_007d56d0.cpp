// CTCTavernGameCardSorting::vfunc_1(CPersistContext* context, long arg) -- persist hook: card-base persist, then "TimeLimitNumRoundsToWin" (+0x318) and "TimeLimitBestNumRounds" (+0x31C), zero defaults
// Retail: Fable.exe @ 0x007D56D0, 88 bytes (vtable 0x0126F5B4 slot 1, `ret 8`).
// Callees: CTCTavernGameCardBase::vfunc_1 0x008F0920 (non-virtual base call with both arguments),
//          CPersistContext::Transfer 0x00410BE0 (a 4-byte instance; the manifest's
//          "Transfer<signed_char>" label is propagated -- the default is a dword).
// Shape: each `const long&` zero default is a fresh temporary right before its call, homed in the
//        dead `arg` slot; the first member address is `lea edx,[esi+318h]`, the second advances `this`.
// The card games' own members start at +0x318 (the card base is far larger than the PDB's
// CTCTavernGame), so the base is a local model padded to there.
class CPersistContext { public:
    void Transfer(const char* name, long& value, const long& defaultValue);   // 0x00410BE0
};

struct CTCTavernGameCardBase {
    unsigned char _pad[0x318];
    void vfunc_1(CPersistContext* context, long arg);                         // 0x008F0920
};

struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    long m_timeLimitNumRoundsToWin;     // +0x318
    long m_timeLimitBestNumRounds;      // +0x31c
    void vfunc_1(CPersistContext* context, long arg);
};

void CTCTavernGameCardSorting::vfunc_1(CPersistContext* context, long arg)
{
    CTCTavernGameCardBase::vfunc_1(context, arg);
    context->Transfer("TimeLimitNumRoundsToWin", m_timeLimitNumRoundsToWin, 0);
    context->Transfer("TimeLimitBestNumRounds", m_timeLimitBestNumRounds, 0);
}