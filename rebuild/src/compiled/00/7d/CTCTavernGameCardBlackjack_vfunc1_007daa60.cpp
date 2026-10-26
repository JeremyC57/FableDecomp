// CTCTavernGameCardBlackjack::vfunc_1(CPersistContext* context, long arg) -- persist hook: card-base persist, then "MostMoneyWon" (+0x348, zero default)
// Retail: Fable.exe @ 0x007DAA60, 56 bytes (vtable 0x0126F8F4 slot 1, `ret 8`).
// Callees: CTCTavernGameCardBase::vfunc_1 0x008F0920 (the card base's slot 1, called non-virtually
//          with both arguments), CPersistContext::Transfer<long> 0x00410620 (name, value, const& default).
// Shape: the zero default is a `const long&` temporary built from the literal right before the call;
//        VC7.1 homes it in the dead `arg` slot (`lea ecx,[esp+10h]` ... `mov [esp+1Ch],0`). The member
//        address is formed by advancing `this` (`add esi,348h`) since `this` is dead afterwards.
// The retail card games are far larger than the PDB's CTCTavernGame (0x108): the card base is a
// local model padded to the Blackjack members (+0x318.. zeroed by the constructor at 0x007DB280).
class CPersistContext { public:
    void Transfer_00410620(const char* name, long& value, const long& defaultValue);   // 0x00410620
};

struct CTCTavernGameCardBase {
    unsigned char _pad[0x318];
    void vfunc_1(CPersistContext* context, long arg);                         // 0x008F0920
};

struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    unsigned char _pad_0x318[0x30];     // +0x318 hand vectors / round state
    long m_mostMoneyWon;                // +0x348
    void vfunc_1(CPersistContext* context, long arg);
};

void CTCTavernGameCardBlackjack::vfunc_1(CPersistContext* context, long arg)
{
    CTCTavernGameCardBase::vfunc_1(context, arg);
    context->Transfer_00410620("MostMoneyWon", m_mostMoneyWon, 0);
}