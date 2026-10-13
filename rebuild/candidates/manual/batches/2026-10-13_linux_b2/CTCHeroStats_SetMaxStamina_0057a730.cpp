// CTCHeroStats::SetMaxStamina(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A730, oracle length 35 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov edx,[esp+4] ; lea eax,[ecx+5Ch] ; mov [eax],edx ; mov edx,[ecx+90h] ; inc edx ; cmp [eax],edx ; jge A ; mov [eax],edx ; A: mov eax,[eax] ; cmp [ecx+58h],eax ; jle R ; mov [ecx+58h],eax ; R: ret 4
// Retail CTCHeroStats is 8 bytes smaller than Ego_r.pdb from the money block onward: the map
// and vector before it are each 4 bytes smaller in the shipped build (engine/CTCHeroStats.h:
// Stamina +0x60, MaxStamina +0x64, ExtraMaxStamina +0x98; retail stores hit +0x58/+0x5C/+0x90),
// so the three members are declared at their retail offsets here.
// MaxStamina is set, raised to at least ExtraMaxStamina+1, and Stamina is clamped to it.

struct CTCHeroStats_Retail {
    unsigned char _pad_0x00[0x58];
    long          Stamina;           // +0x58 (PDB +0x60)
    long          MaxStamina;        // +0x5C (PDB +0x64)
    unsigned char _pad_0x60[0x30];
    long          ExtraMaxStamina;   // +0x90 (PDB +0x98)
    void SetMaxStamina(long maxStamina);
};

void CTCHeroStats_Retail::SetMaxStamina(long maxStamina)
{
    this->MaxStamina = maxStamina;
    if (this->MaxStamina < this->ExtraMaxStamina + 1)
        this->MaxStamina = this->ExtraMaxStamina + 1;
    if (this->Stamina > this->MaxStamina)
        this->Stamina = this->MaxStamina;
}
