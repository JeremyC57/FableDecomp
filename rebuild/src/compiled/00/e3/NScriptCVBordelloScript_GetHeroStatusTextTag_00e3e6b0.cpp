// CV_BordelloScript::GetHeroStatusTextTag() -- the text-tag suffix for the hero's status in the bordello: "_HEROWHORE" while tricking in a dress, otherwise "_HEROLADY" in a dress and "_HEROMAN" without, returned by value
// Leaf: GetHeroStatusTextTag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E3E6B0, length 100 bytes (padded to 112), `ret 4` (hidden result).
// Callees: CV_BordelloScript::IsHeroWearingDress @ 0x00E3E320 (catalog "IsHeroWearingBeard"; it tests the hero for
// "OBJECT_HERO_TROUSERS_DRESS"), CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literals "_HEROWHORE"
// 0x012E4208, "_HEROLADY" 0x012E41FC, "_HEROMAN" 0x012E41F0).
// Layout: HeroTricking at retail +0x49 (Ego_r.pdb CV_BordelloScript 0x5d, quest scripts sit 0x14 below the PDB).
// Shape: three returns of a constructed result, each its own tail.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
};
namespace NScript {
class CV_BordelloScript {
public:
    unsigned char _base[0x48];                     // CScriptBase + interface/data pair
    bool HeroPartying;                             // +0x48
    bool HeroTricking;                             // +0x49
    bool IsHeroWearingDress();                     // 0x00E3E320
    CCharString GetHeroStatusTextTag();
};
} // namespace NScript
using NScript::CV_BordelloScript;
CCharString NScript::CV_BordelloScript::GetHeroStatusTextTag()
{
    if (HeroTricking && IsHeroWearingDress())
        return CCharString("_HEROWHORE");
    if (IsHeroWearingDress())
        return CCharString("_HEROLADY");
    return CCharString("_HEROMAN");
}