// NUISystem::CFrontEndManager::AddPrefixToTitleBar(FRONTEND_SCREEN screen, const CCharString& title_bar_name, const CWideString& prefix) -- reset a screen's title-bar text to its default with a prefix
// Leaf: ?AddPrefixToTitleBar@CFrontEndManager@NUISystem@@IAEXW4FRONTEND_SCREEN@@ABVCCharString@@ABVCWideString@@@Z
// Retail: Fable.exe @ 0x00595AD9, 75 bytes, `ret 0Ch`; Ego_r.pdb
// ?AddPrefixToTitleBar@CFrontEndManager@NUISystem@@IAEXW4FRONTEND_SCREEN@@ABVCCharString@@ABVCWideString@@@Z.
// Body: if m_mapScreens[screen] (map::operator[] out of line, 0x0059B5D7, called twice) is set, FindChild (vtable +0x0c,
// by-value CCharString copy-built in its argument slot, 0x0099EC30) on it; if found,
// CText::ResetToDefaultValueWithPrefix(prefix) (0x0054F580).
// Layout: retail CFrontEndManager m_mapScreens +0x54 (as 0x00595222 / 0x005966F2); no engine/ header.
// Shape: size TU (unaligned entry, no frame pointer).
#pragma optimize("s", on)
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
};
class CWideString;
enum FRONTEND_SCREEN { FRONTEND_SCREEN_NONE };
namespace std {
template <class K, class V> class map {
public:
    void*        _M_header;
    unsigned int _M_node_count;
    unsigned int _M_key_compare;
    V& operator[](const K& key);                         // 0x0059B5D7 for <FRONTEND_SCREEN, CComponent*>
};
}
namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent();
    virtual void _slot01(); virtual void _slot02();
    virtual CComponent* FindChild(CCharString name);     // slot 3 (+0x0c)
};
class CText : public CComponent {
public:
    void ResetToDefaultValueWithPrefix(const CWideString& prefix);   // 0x0054F580
};
class CFrontEndManager {
public:
    unsigned char                            _pad00[0x54];
    std::map<FRONTEND_SCREEN, CComponent*>   m_mapScreens;   // +0x54
protected:
    void AddPrefixToTitleBar(FRONTEND_SCREEN screen, const CCharString& title_bar_name, const CWideString& prefix);
    friend struct CFrontEndManager_Test;                 // behaviour-test hook
};
void CFrontEndManager::AddPrefixToTitleBar(FRONTEND_SCREEN screen, const CCharString& title_bar_name, const CWideString& prefix)
{
    if (m_mapScreens[screen])
    {
        CText* title = static_cast<CText*>(m_mapScreens[screen]->FindChild(title_bar_name));
        if (title)
            title->ResetToDefaultValueWithPrefix(prefix);
    }
}
}