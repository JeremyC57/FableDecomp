// CV_FisticuffsClubScript::GetCutsceneName(CCharString suffix) -- returns "CS_PUNCHCLUB_<region>_" + suffix for the four punch-club regions (BS, KG, OV, BC by RelevantRegion 1..4), "An error" otherwise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8A1D0, 200 bytes + a four-entry jump table (row 224), `ret 8`
//   (hidden CCharString result + the by-value suffix, destroyed by the callee at the shared tail).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CCharString copy ctor 0x0099EC30, ~CCharString
//   0x0099EAE0, operator+(const char*, const CCharString&) 0x0099F690 (`__fastcall`, hidden result in ecx, left in edx).
// Strings: "An error" 0x012EB2AC, "CS_PUNCHCLUB_BS_" 0x012EB708, "CS_PUNCHCLUB_KG_" 0x012EB6F4, "CS_PUNCHCLUB_OV_"
//   0x012EB6E0, "CS_PUNCHCLUB_BC_" 0x012EB6CC.
// Shape: before the switch retail builds "An error" in a temporary, copies it into a local and destroys the
//   temporary; the local is never read and dies at the shared tail. VC7.1 has no NRVO, so that is a local
//   initialised from an inline by-value getter that returns a named local (build/copy/destroy). The switch over
//   RelevantRegion (retail +0x48 = Ego_r.pdb 92 - 0x14) is dense 1..4 -> `lea eax,[esi-1] ; cmp eax,3 ; ja`.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
static inline CCharString ErrorCutsceneName()
{
    CCharString name("An error", -1);
    return name;
}
struct CV_FisticuffsClubScript {
    unsigned char _base[0x48];                     // CScriptBase + interface/data pair
    long RelevantRegion;                           // +0x48
    CCharString GetCutsceneName(CCharString suffix);
};
CCharString CV_FisticuffsClubScript::GetCutsceneName(CCharString suffix)
{
    CCharString cutscene = ErrorCutsceneName();
    switch (RelevantRegion)
    {
    case 1: return "CS_PUNCHCLUB_BS_" + suffix;
    case 2: return "CS_PUNCHCLUB_KG_" + suffix;
    case 3: return "CS_PUNCHCLUB_OV_" + suffix;
    case 4: return "CS_PUNCHCLUB_BC_" + suffix;
    }
    return CCharString("An error", -1);
}
