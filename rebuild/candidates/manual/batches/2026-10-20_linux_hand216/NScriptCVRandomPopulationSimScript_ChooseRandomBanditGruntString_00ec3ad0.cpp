// NScript::CV_RandomPopulationSimScript::ChooseRandomBanditGruntString(long level) -- returns "CREATURE_BANDIT_GRUNT_LEVEL<level>_<A..D>" with the variant letter picked by rand() % 4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC3AD0, 214 bytes + a four-entry jump table (row 240), `ret 8` (hidden result +
//   level; `this` is never read). PDB name ?ChooseRandomBanditGruntString@CV_RandomPopulationSimScript@NScript@@QAE?AVCCharString@@J@Z.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, copy ctor 0x0099EC30, ~CCharString 0x0099EAE0,
//   CCharString::operator=(const char*) 0x0099EFE0, CRT rand 0x00BFEB16, GFIntToCharString(long) 0x0099F830,
//   operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&, const CCharString&) 0x0099F570
//   (all three `__fastcall`, hidden result in ecx).
// Strings: "" 0x0122D70E, "_A" 0x012F15C0, "_B" 0x012F15BC, "_C" 0x012F15B8, "_D" 0x012F15B4,
//   "CREATURE_BANDIT_GRUNT_LEVEL" 0x012F1598.
// Shape: an unused "" local built by an inline by-value getter (build/copy/destroy, no NRVO); the suffix starts as
//   "_A" and is re-assigned in each case (one merged `operator=` call); the concatenation's middle temporary is homed
//   in the dead `level` argument slot once its value is in edx.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
extern "C" int __cdecl rand();                     // 0x00BFEB16
CCharString __fastcall GFIntToCharString(long value);                              // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);       // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const CCharString& right); // 0x0099F570
static inline CCharString EmptyName()
{
    CCharString name("", -1);
    return name;
}
namespace NScript {
struct CV_RandomPopulationSimScript {
    unsigned char _base[0x48];
    CCharString ChooseRandomBanditGruntString(long level);
};
CCharString CV_RandomPopulationSimScript::ChooseRandomBanditGruntString(long level)
{
    CCharString grunt = EmptyName();
    CCharString variant("_A", -1);
    switch (rand() % 4)
    {
    case 0: variant = "_A"; break;
    case 1: variant = "_B"; break;
    case 2: variant = "_C"; break;
    case 3: variant = "_D"; break;
    }
    return "CREATURE_BANDIT_GRUNT_LEVEL" + GFIntToCharString(level) + variant;
}
}
