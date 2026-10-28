// CTCHeroOnlineScoreboard::OnPlayerSelectedClan(CWideString clanName, NUISystem::CComponent* clanMenuEntry) -- when no clan entry is pending: remember the chosen clan, show the connecting message, start downloading its scoreboard (state 6, the old download cleared), stop the screen's two buttons and the mouse pointer from taking event 0x19 and fade the pointer out
// Leaf: OnPlayerSelectedClan
//
// Retail 0x005625F0, 288 bytes incl. padding (`ret 8`; the by-value clan name destroyed at the end). Ego_r.pdb
// `?OnPlayerSelectedClan@CTCHeroOnlineScoreboard@@QAEXVCWideString@@PAVCComponent@NUISystem@@@Z`.
// Callees: CWideString::operator= @ 0x0099B7D0, the connecting message's SetActive (component slot 48, +0xC0), the clan
// scoreboard map's _M_erase @ 0x00569A80 (the inline STLport clear), the scoreboard screen's two child getters (slots 175/176,
// +0x2BC/+0x2C0), the components' observer slot 4 (+0x10, event 0x19), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// @ 0x0040F020 (inlined HasTC + GetTC for CTCInventory, type 0x11 = bit 17 of +0x20), the mouse pointer's slot 38 (+0x98,
// colour, 0.1f, 0), ~CWideString @ 0x0099B510.
// Layout: Ego_r.pdb CTCHeroOnlineScoreboard with retail 12-byte maps/vectors and 4-byte lists: SelectedClan +0x3c,
// DownloadedClanScoreboard +0x44, ScoreboardState +0x50, PScoreboardScreen +0x58, PClanMenuEntry +0x5c,
// PConnectingMessage1 +0x64; retail CTCInventory PMouse_Pointer +0x214 (as the landed OnOkClicked 0x0055EE20).
// Shape: the map clear is STLport `clear()` inlined (erase from the root, then leftmost/root/rightmost/count reset through
// the re-read header); the colour is a CRGBColour(255,255,255,0) temporary whose body assigns R,G,B,A (+2,+1,+0,+3), homed in
// the dead clanMenuEntry slot; the same-TU LowerBound stand-in lets the lookup key share that slot too.
class CTCBase;

class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString(const CWideString& other);              // (declared: by-value arguments are built in place)
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

class CRGBColour { public:                                     // B, G, R, A bytes
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

namespace NUISystem {
class CObserver { public:                                      // the observer interface at +4 of every component
    virtual void o00(); virtual void o01(); virtual void o02(); virtual void o03();
    virtual void StopObservingEvent(long eventType);    // slot 4 (+0x10)
};
class CComponent;
struct CComponentBase {                                 // the primary vtable
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37)
    virtual void FadeColour(const CRGBColour& colour, float time, long flags);   // slot 38 (+0x98)
    V(39) V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47)
    virtual void SetActive_slot48(bool active);         // slot 48 (+0xC0)
    V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68)
    V(69) V(70) V(71) V(72) V(73) V(74) V(75) V(76) V(77) V(78) V(79) V(80) V(81) V(82) V(83) V(84) V(85) V(86) V(87) V(88)
    V(89) V(90) V(91) V(92) V(93) V(94) V(95) V(96) V(97) V(98) V(99) V(100) V(101) V(102) V(103) V(104) V(105) V(106)
    V(107) V(108) V(109) V(110) V(111) V(112) V(113) V(114) V(115) V(116) V(117) V(118) V(119) V(120) V(121) V(122) V(123)
    V(124) V(125) V(126) V(127) V(128) V(129) V(130) V(131) V(132) V(133) V(134) V(135) V(136) V(137) V(138) V(139) V(140)
    V(141) V(142) V(143) V(144) V(145) V(146) V(147) V(148) V(149) V(150) V(151) V(152) V(153) V(154) V(155) V(156) V(157)
    V(158) V(159) V(160) V(161) V(162) V(163) V(164) V(165) V(166) V(167) V(168) V(169) V(170) V(171) V(172) V(173) V(174)
#undef V
    virtual CComponent* GetFirstButton();               // slot 175 (+0x2BC)
    virtual CComponent* GetSecondButton();              // slot 176 (+0x2C0)
};
class CComponent : public CComponentBase, public CObserver { public:
};
}

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                      // +0x00
    SEntry* last;                                       // +0x04
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (its retail body: STLport lower_bound over the 8-byte
// {type, component} entries, as the landed CCreatureActionPickUpGoods::PerformAction 0x00848730 stand-in): compiled above
// the caller so VC7.1 knows the key's address does not escape -- the key then shares the dead argument slot like retail's.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad_0x00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                         // +0x44
    bool HasTC(long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || wanted < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
class CTCInventory { public:                                   // interface type 0x11
    unsigned char          _pad_0x000[0x214];
    NUISystem::CComponent* PMouse_Pointer;              // +0x214 (PDB +0x260)
};

struct SScoreboardNode;
struct CClanScoreboardMap {                             // STLport map<EOnlineScoreboardActivity, vector<pair<float,CWideString>>>
    struct SHeader { long _M_color; SScoreboardNode* _M_parent; SHeader* _M_left; SHeader* _M_right; };
    SHeader*      _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    void _M_erase(SScoreboardNode* node);               // 0x00569A80
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};

class CTCHeroOnlineScoreboard { public:
    void*                  __vftable;                   // +0x00
    CThing*                ParentThing;                 // +0x04 CTCBase
    unsigned char          _pad_0x08[0x3c - 0x08];
    CWideString            SelectedClan;                // +0x3c
    void*                  ClanMembers;                 // +0x40 list (header pointer)
    CClanScoreboardMap     DownloadedClanScoreboard;    // +0x44
    long                   ScoreboardState;             // +0x50
    long                   TaskState;                   // +0x54
    NUISystem::CComponent* PScoreboardScreen;           // +0x58
    NUISystem::CComponent* PClanMenuEntry;              // +0x5c
    NUISystem::CComponent* PConnectingMessage;          // +0x60
    NUISystem::CComponent* PConnectingMessage1;         // +0x64
    void OnPlayerSelectedClan(CWideString clanName, NUISystem::CComponent* clanMenuEntry);
};

void CTCHeroOnlineScoreboard::OnPlayerSelectedClan(CWideString clanName, NUISystem::CComponent* clanMenuEntry)
{
    if (PClanMenuEntry == 0) {
        SelectedClan = clanName;
        PConnectingMessage1->SetActive_slot48(true);
        ScoreboardState = 6;
        DownloadedClanScoreboard.clear();
        PClanMenuEntry = clanMenuEntry;
        NUISystem::CComponent* screen = PScoreboardScreen;
        screen->GetFirstButton()->StopObservingEvent(0x19);
        screen->GetSecondButton()->StopObservingEvent(0x19);
        CThing* thing = ParentThing;
        if (thing->HasTC(0x11)) {
            CTCInventory* inventory = thing->GetTC<CTCInventory>(0x11);
            inventory->PMouse_Pointer->StopObservingEvent(0x19);
            inventory->PMouse_Pointer->FadeColour(CRGBColour(255, 255, 255, 0), 0.1f, 0);
        }
    }
}