// GetHeroTitleAsLHTSTag() -- the hero's current title as the "_<NAME>" suffix of its LHTS text tag ("" when out of range)
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBEBA7, 345 bytes (incl. the 21-entry jump table), __fastcall global with the
//   hidden CCharString result in ecx and no arguments, plain `ret`. The odd start address is genuine: the previous function
//   IsCutsceneSkipped (0x00CBEB7E) ends with `ret` at 0x00CBEBA6 and this TU (the NScript helper file 0x00CBE693..) packs
//   its functions without alignment padding, as Ego_r does for the same file (0x00C9F9E2 IsCutsceneSkipped, 0x00C9FA0B this).
// Identity: Ego_r.pdb ?GetHeroTitleAsLHTSTag@@YI?AVCCharString@@XZ, the next Ego_r entry after ?IsCutsceneSkipped@@YI_NXZ
//   (retail 0x00CBEB7E, landed), the same neighbour order as retail. Callers 0x00DD414A / 0x00EAFA59 (quest scripts).
// Callees: CGameScriptInterface::GetHeroTitle() const (slot 205, +0x334, 0x00897880) through g_pGameScriptInterface
//   0x0143E8F8; CCharString::CCharString(const char*, long = -1) 0x0099EBF0 (the shared tail every case jumps to).
// Strings: "_NONE" 0x012C1858 .. "_ARSE" 0x012C1794 (cases 0..20 in jump-table order, read from the image), default "" 0x0122D70E.
//   EHeroTitle's enumerator names are not in the PDB dumps, so the cases are numbers.
// Shape: size TU with frame pointers off-by-pragma like its landed neighbours (IsCutsceneSkipped): each case pushes -1 and
//   its literal and jumps to one shared constructor call (cross-jumping); without the pragmas VC7.1 duplicates the call
//   per case (516v345).
// Leaf: ?GetHeroTitleAsLHTSTag@@YI?AVCCharString@@XZ
class CCharString {
public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

CCharString __fastcall GetHeroTitleAsLHTSTag()
{
    switch (g_pGameScriptInterface->GetHeroTitle()) {
    case 0:  return "_NONE";
    case 1:  return "_REAPER";
    case 2:  return "_SHADOWHUNTER";
    case 3:  return "_MALEFICUS";
    case 4:  return "_DEATH";
    case 5:  return "_ASSASSIN";
    case 6:  return "_NECRO";
    case 7:  return "_AVATAR";
    case 8:  return "_PILGRIM";
    case 9:  return "_LIBERATOR";
    case 10: return "_PALADIN";
    case 11: return "_DRUID";
    case 12: return "_RANGER";
    case 13: return "_RUNE";
    case 14: return "_HOOD";
    case 15: return "_GLADIATOR";
    case 16: return "_SABRE";
    case 17: return "_ARROW";
    case 18: return "_PIE";
    case 19: return "_CHICKEN";
    case 20: return "_ARSE";
    }
    return "";
}