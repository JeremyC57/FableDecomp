// CCombatSequence_BanditKingBackOff::GetName @ 008b3b80
// Real __fastcall member returning CCharString by value: this in ecx (unused),
// hidden UDT return pointer on the stack ([esp+8] after push esi), ret 4.
// Retail: push esi; mov esi,[esp+8]; push -1; push offset "..."; mov ecx,esi;
//         call CCharString::ctor(const char*,long); mov eax,esi; pop esi; ret 4
// The name string lives at 0x12788b0 (pushed as an immediate offset).
// Literal (NF418, 2026-11-25): was extern data `kNameStr` (table ?kNameStr@@3PADA -> 0x012788B0); now spelled at the use site:
//   "COMBAT_SEQUENCE_BANDIT_KING_BACK_OFF", retail's pooled literal at 0x012788B0 (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).

class CCharString { public:
    int d;
    CCharString(const char* s, long n); // ctor @ 0x99ebf0
};


struct CCombatSequence_BanditKingBackOff {
    int x;
    CCharString GetName();
};

CCharString CCombatSequence_BanditKingBackOff::GetName()
{
    return CCharString("COMBAT_SEQUENCE_BANDIT_KING_BACK_OFF", -1);
}