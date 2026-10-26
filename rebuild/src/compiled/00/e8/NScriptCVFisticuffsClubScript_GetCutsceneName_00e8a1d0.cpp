// CV_FisticuffsClubScript::GetCutsceneName(CCharString suffix) -- returns "CS_PUNCHCLUB_<region>_" + suffix for the four punch-club regions (RelevantRegion 1 BS, 2 OV, 3 BC, 4 KG), "An error" otherwise
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
// Table: 0x00E8A298 = { 0x00E8A20F BS, 0x00E8A23D OV, 0x00E8A254 BC, 0x00E8A226 KG } for regions 1..4. The arms sit in
//   the bytes as BS, KG, OV, BC (regions 1, 4, 2, 3), so the cases are written in that order; an earlier version read
//   the cases off the arm order and returned KG/OV/BC for regions 2/3/4.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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
namespace NScript {
class CV_FisticuffsClubScript {
public:
    unsigned char _base[0x48];                     // CScriptBase + interface/data pair
    long RelevantRegion;                           // +0x48
    CCharString GetCutsceneName(CCharString suffix);
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;
CCharString NScript::CV_FisticuffsClubScript::GetCutsceneName(CCharString suffix)
{
    CCharString cutscene = ErrorCutsceneName();
    switch (RelevantRegion)
    {
    case 1: return "CS_PUNCHCLUB_BS_" + suffix;
    case 4: return "CS_PUNCHCLUB_KG_" + suffix;
    case 2: return "CS_PUNCHCLUB_OV_" + suffix;
    case 3: return "CS_PUNCHCLUB_BC_" + suffix;
    }
    return CCharString("An error", -1);
}