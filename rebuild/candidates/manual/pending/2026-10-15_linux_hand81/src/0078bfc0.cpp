// CTCBerserk::ShouldAttackDecapitate() -- roll a random number in [0, 1) from the world seed and compare it with the def's decapitation chance for the current spell level
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078BFC0, length 59 bytes (+ int3 padding).
// Callees: the world accessor @ 0x00686A80 (a thiscall CTCBase member that never reads this: the game
// singleton @ 0x013B8A1C, +0x24; manifest "CViewBase::GetDisplayEngine" is propagated), CWorld::GetPRandomSeed @ 0x0049D810
// (landed name; manifest "CVertexBufferWin32::DoSizeof" is propagated), GFFloatRandom(float, unsigned
// long*) @ 0x004978A0 (__fastcall: the range on the stack, the seed in ecx).
// Layout: engine/CTCBerserk.h SpellLevel +0x44; PDef retail +0x68 (PDB +0x6c, see 0x0078BE70), so the
// class is modelled locally. The def's per-level chance array (a vector<float> data pointer at
// +0x138) is unnamed (descriptive name).
// Shape: the chance's address is formed before the calls (a pointer local in esi); the accessor is a
// MEMBER (ecx = this stays live into the call, so the index is loaded into edx, not ecx); `fcomp ;
// test ah,41h ; jp` on the false edge is `random <= chance`; `mov eax,1` / `xor eax,eax` is a full
// register (long) result.
struct CWorld {
    unsigned long* GetPRandomSeed();                        // 0x0049D810
};

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CSpecialAbilitiesBerserkDef {
    unsigned char _pad00[0x138];
    float*        DecapitateChances;                        // +0x138 (per spell level)
};

struct CTCBerserk_Retail {
    unsigned char                _pad00[0x44];
    long                         SpellLevel;                // +0x44
    unsigned char                _pad48[0x68 - 0x48];
    CSpecialAbilitiesBerserkDef* PDef;                      // +0x68 (PDB +0x6c)
    CWorld* GetWorld();                                     // 0x00686A80 (a CTCBase member that ignores this)
    long ShouldAttackDecapitate();
};

long CTCBerserk_Retail::ShouldAttackDecapitate()
{
    float* chance = &PDef->DecapitateChances[SpellLevel];
    if (GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed()) <= *chance)
        return 1;
    return 0;
}
