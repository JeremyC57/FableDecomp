// CTCInventoryTrade::RefreshSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles) -- for each of the 13 trading-menu category entries (UI_TRADING_MENU_*_ENTRY) under `component`, refresh its subtitle
// Leaf: ?RefreshSubtitles@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@PAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059E108, 447 bytes, thiscall, `ret 8`, ebp frame.
// Identity (proven): FableWin.pdb / Ego_r.pdb ?RefreshSubtitles@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@...; it hands each UI_ entry to 0x0059D1B1, the UI_ONE_OFF_SUBTITLE twin of the PC refresher 0x0059D761 (FinalRefreshSubtitles); its callers are RefreshBuyPossibilityNumbers 0x005A3734, SellItem 0x005AECC0 and BuyItem 0x005AF64E.
//   The manifest's "ConstructSubtitles" on the refresh twins is propagated.
// Callees: CCharString(const char*, long) 0x0099EBF0 (the by-value FindChild name), NUISystem::CComponent slot 3 FindChild
//   (+0x0C), CTCInventoryTrade::FinalRefreshSubtitles 0x0059D1B1.
// Shape (family M150, representative 0x0059DF49): thirteen `FinalRefreshSubtitles(component->FindChild(CCharString(name)), subtitles);`
//   -- the map reference is pushed first, the component's vtable is read before the name is built in place on the stack.
//   `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
class CWideString;
namespace std {
template <class _Tp> class allocator;
template <class _Tp> struct less;
template <class _T1, class _T2> struct pair;
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map;
}
namespace NUISystem {
class CComponent {
public:
    virtual void _v000(); virtual void _v001(); virtual void _v002();
    virtual CComponent* FindChild(CCharString name);      // slot 3 (+0x0c)
};
}
typedef std::map<NUISystem::CComponent*, std::pair<CWideString, long> > CSubtitleMap;
class CTCInventoryTrade {
    void FinalRefreshSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles);   // 0x0059D1B1
    void RefreshSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles);
};
void CTCInventoryTrade::RefreshSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles)
{
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_MELEE_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_RANGED_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_HEAD_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_TORSO_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_HANDS_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_LEGS_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_FEET_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_SUITS_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_TROPHIES_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_POTIONS_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_GIFTS_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_PRODUCE_ENTRY")), subtitles);
    FinalRefreshSubtitles(component->FindChild(CCharString("UI_TRADING_MENU_OTHERS_ENTRY")), subtitles);
}