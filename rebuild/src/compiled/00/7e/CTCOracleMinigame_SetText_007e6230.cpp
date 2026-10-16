// CTCOracleMinigame::SetText(unsigned long text) -- when the text changes and the hero is alive, show the narrated text through the hero's region-display component and remember it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E6230, length 153 bytes (`ret 4`).
// Callees: CTCBase::GetPlayerManager @ 0x00686D50, CPlayerManager::GetMainPlayer @ 0x00449960, CPlayer::GetHeroThing @ 0x00487DC0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for CTCRegionDisplay, type 0x8E = bit 14 of
// CThing+0x30), CTCOracleMinigame::GetTextWithNarrator(unsigned long) @ 0x007E5FF0 (CWideString by value),
// CTCRegionDisplay::SetCurrentAreaName(const CWideString&, float, bool, bool, bool) @ 0x00639A90 (-1.0, false, false, true),
// CWideString::~CWideString @ 0x0099B510.
// Layout: CurrentText +0x0C as in rebuild/include/engine/CTCOracleMinigame.h (data-only header, modelled locally); retail CThing bitset
// +0x20, map +0x44, dead bit +0x91 bit 0.
// Shape: `T* p = 0;` then the out-pointer GetInterface without a test (retail presets esi to 0); the narrated text is an unnamed
// by-value temporary destroyed after the call; CurrentText stored inside the hero block.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                 // 0x0099B510
};
struct CTCRegionDisplay {
    void SetCurrentAreaName(const CWideString& name, float duration, bool a, bool b, bool c);   // 0x00639A90
};
struct CPlayer {
    CThing* GetHeroThing();                         // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                       // 0x00449960
};
struct CTCOracleMinigame {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _base08[4];
    unsigned long CurrentText;                      // +0x0c
    CPlayerManager* GetPlayerManager();             // 0x00686D50 (CTCBase)
    CWideString GetTextWithNarrator(unsigned long text);   // 0x007E5FF0
    void SetText(unsigned long text);
};

void CTCOracleMinigame::SetText(unsigned long text)
{
    if (CurrentText != text) {
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
        if (hero && !(hero->m_flags91 & 1)) {
            CTCRegionDisplay* display = 0;
            hero->GetInterface(&display, 0x8e);
            display->SetCurrentAreaName(GetTextWithNarrator(text), -1.0f, false, false, true);
            CurrentText = text;
        }
    }
}