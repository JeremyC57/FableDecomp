// CTCHeroStats::SetMaxStamina(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A730, oracle length 35 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov edx,[esp+4] ; lea eax,[ecx+5Ch] ; mov [eax],edx ; mov edx,[ecx+90h] ; inc edx ; cmp [eax],edx ; jge A ; mov [eax],edx ; A: mov eax,[eax] ; cmp [ecx+58h],eax ; jle R ; mov [ecx+58h],eax ; R: ret 4
// Retail CTCHeroStats is 8 bytes smaller than Ego_r.pdb from the money block onward: the map
// and vector before it are each 4 bytes smaller in the shipped build (engine/CTCHeroStats.h:
// Stamina +0x60, MaxStamina +0x64, ExtraMaxStamina +0x98; retail stores hit +0x58/+0x5C/+0x90),
// so the three members are declared at their retail offsets here.
// MaxStamina is set, raised to at least ExtraMaxStamina+1, and Stamina is clamped to it.
//
// Lane feedback: byte-exact only as an inlined clamp helper taking `long&` under
// #pragma optimize("s",on) (the lander sweep applies it). The reference parameter is what stops
// VC7.1 forwarding the fresh store into the compare (retail re-reads MaxStamina through the
// `lea eax,[ecx+5Ch]` pointer), and the size pragma is what makes it address every access through
// that pointer. Writing the clamp inline on the members forwards the store and differs. MATCH["s"].

// Retail CTCHeroStats view (PDB offsets are +8: the two container members ahead of these fields
// are 4 bytes smaller each in retail -- see the batch README container-size rule).
struct CTCHeroStats_Retail {
    unsigned char _pad_0x00[0x58];
    long          Stamina;           // +0x58 (PDB +0x60)
    long          MaxStamina;        // +0x5C (PDB +0x64)
    unsigned char _pad_0x60[0x30];
    long          ExtraMaxStamina;   // +0x90 (PDB +0x98)
    void SetMaxStamina(long maxStamina);
};

// Raise `value` to at least `minimum` (an inline helper the hero-stats code shares).
static inline void ClampMin(long& value, long minimum)
{
    if (value < minimum)
        value = minimum;
}

void CTCHeroStats_Retail::SetMaxStamina(long maxStamina)
{
    // New ceiling, never below the bonus stamina + 1, and the current stamina is cut to fit.
    this->MaxStamina = maxStamina;
    ClampMin(this->MaxStamina, this->ExtraMaxStamina + 1);
    if (this->Stamina > this->MaxStamina)
        this->Stamina = this->MaxStamina;
}
