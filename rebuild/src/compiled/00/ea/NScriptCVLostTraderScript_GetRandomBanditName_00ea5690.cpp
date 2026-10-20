// NScript::CV_LostTraderScript::GetRandomBanditName() -- returns one of eleven bandit creature def names picked by rand() % 10 (level-2/level-3 grunts and archers; the out-of-range default is GRUNT_LEVEL3_D)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA5690, 194 bytes + a ten-entry jump table (row 240), `ret 4` (hidden result;
//   `this` is never read). PDB name ?GetRandomBanditName@CV_LostTraderScript@NScript@@QAE?AVCCharString@@XZ.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CCharString copy ctor 0x0099EC30, ~CCharString
//   0x0099EAE0, CRT rand 0x00BFEB16.
// Strings: "Should not happen" 0x012EB098; cases 0..9: CREATURE_BANDIT_GRUNT_LEVEL2_A/B/C/D 0x012EE134/114/0F4/0D4,
//   CREATURE_BANDIT_FIRE_ARCHER_LEVEL2/LEVEL3 0x012EE0B0/08C, CREATURE_BANDIT_GRUNT_LEVEL3_A/B/C 0x012CA728/708/6E8,
//   CREATURE_BANDIT_ARCHER_LEVEL3 0x012EE06C; default CREATURE_BANDIT_GRUNT_LEVEL3_D 0x012CA6C8.
// Shape: the "Should not happen" local is built in a temporary, copied and the temporary destroyed (inline by-value
//   getter returning a named local; VC7.1 has no NRVO) and it is never read; every case returns a fresh
//   CCharString(literal) into the hidden result, which VC tail-merges into one `push -1 ; push lit ; jmp` per case.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
extern "C" int __cdecl rand();                     // 0x00BFEB16
static inline CCharString ShouldNotHappenName()
{
    CCharString name("Should not happen", -1);
    return name;
}
namespace NScript {
struct CV_LostTraderScript {
    unsigned char _base[0x58];
    CCharString GetRandomBanditName();
};
CCharString CV_LostTraderScript::GetRandomBanditName()
{
    CCharString bandit = ShouldNotHappenName();
    switch (rand() % 10)
    {
    case 0: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL2_A", -1);
    case 1: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL2_B", -1);
    case 2: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL2_C", -1);
    case 3: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL2_D", -1);
    case 4: return CCharString("CREATURE_BANDIT_FIRE_ARCHER_LEVEL2", -1);
    case 5: return CCharString("CREATURE_BANDIT_FIRE_ARCHER_LEVEL3", -1);
    case 6: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL3_A", -1);
    case 7: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL3_B", -1);
    case 8: return CCharString("CREATURE_BANDIT_GRUNT_LEVEL3_C", -1);
    case 9: return CCharString("CREATURE_BANDIT_ARCHER_LEVEL3", -1);
    }
    return CCharString("CREATURE_BANDIT_GRUNT_LEVEL3_D", -1);
}
}