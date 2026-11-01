// GetHeroTitleAsLHTSTag() -- the hero's current title as the "_<NAME>" suffix of its LHTS text tag ("" when out of range)
#pragma optimize("y", off)
#pragma optimize("s", on)
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
