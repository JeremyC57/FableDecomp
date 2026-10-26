// CTCInventoryTrade::ConstructSubtitlesPC(NUISystem::CComponent* menu, CSubtitleMap& subtitles, NUISystem::CComponent* list) -- PC layout: build the subtitle of each of the thirteen trading-menu category entries (melee .. others) found by name under `menu`
// Leaf: ConstructSubtitlesPC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059E4D7, 486 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y",off) +
//   ("s",on). Ego_r.pdb: ?ConstructSubtitlesPC@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@...@std@@0@Z.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the menu's virtual FindChild(CCharString) (slot 3,
//   +0x0C, as the landed FinalConstructSubtitlesPC 0x0059D65A models it), FinalConstructSubtitlesPC @ 0x0059D65A.
// Shape: thirteen `FinalConstructSubtitlesPC(menu->FindChild("<entry>"), subtitles, list)` calls: the outer arguments are pushed
//   first, then the menu's vptr is loaded and the by-value CCharString argument is built in its stack slot (`push ecx ; mov
//   ecx,esp ; push -1 ; push offset`) from the implicit const char* conversion; the callee destroys it.
// Strings: PC_TRADING_MENU_{MELEE,RANGED,HEAD,TORSO,HANDS,LEGS,FEET,SUITS,TROPHIES,POTIONS,GIFTS,PRODUCE,OTHERS}_ENTRY
//   (0x01252E34 .. 0x01252D38, read from the exe).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace NUISystem {
class CComponent { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name);    // slot 3 (+0x0c)
};
}
struct CSubtitleMap;
class CTCInventoryTrade { public:
    void FinalConstructSubtitlesPC(NUISystem::CComponent* component, CSubtitleMap& subtitles, NUISystem::CComponent* list);   // 0x0059D65A
    void ConstructSubtitlesPC(NUISystem::CComponent* menu, CSubtitleMap& subtitles, NUISystem::CComponent* list);
};

void CTCInventoryTrade::ConstructSubtitlesPC(NUISystem::CComponent* menu, CSubtitleMap& subtitles, NUISystem::CComponent* list)
{
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_MELEE_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_RANGED_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_HEAD_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_TORSO_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_HANDS_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_LEGS_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_FEET_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_SUITS_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_TROPHIES_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_POTIONS_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_GIFTS_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_PRODUCE_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_OTHERS_ENTRY"), subtitles, list);
}