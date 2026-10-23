// Grade: DIFFER(572v568) -- tail only: retail homes the found list in the dead text slot (+0xc, tag at +0xf) and the type filter in +0x14, ours list +8 / filter +0xc (a {} block swaps them to +0x14/+0xc); retail's removal loop is unrotated (jmp to the test, head in edi, manager in ebx)
// CSubtitleRenderer::SetText(const CFontBank& font, const CWideString& text, const CWideString& name, const CWideString& button1, const CWideString& button2, const CWideString& button3, unsigned long flags, float time, bool updateWhilePaused, bool changePlayerMode) -- while text is showing queue the request; otherwise resolve the text and captions through the text bank, reset the timers and state, lay the text out with the font, pause the game for blocking text (and freeze the hero's controls), switch player modes, and drop pending "text finished" messages
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCWideString@@1111KM_N2@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00492973, 568 bytes, `ret 28h`.
// Callees: CSetTextInfo ctor 0x0048DDF6 / dtor 0x0043E3D0, queue<CSetTextInfo>::push 0x004941C0, CWideString::operator=
//   (const wchar_t*) 0x0099B800, GetTextFromTextTag 0x0048F6BD, ProcessEmbeddedGraphicTags 0x00492381,
//   ProcessEmbeddedHighlightTags 0x00491AD2, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   SetButton1OverrideText 0x0048F7C0 / SetButton2OverrideText 0x0048F881, CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetHeroThing 0x00487DC0, IS_THING_ALIVE<CThing*> 0x0043DBD0, bitset::test 0x00410DE0, LowerBound
//   0x0040F020, Pause 0x0048DD94, ProcessPlayerModes 0x0048F939, list ctor 0x00493BC2 / dtor 0x00493BDD,
//   FindAllMessages<CMessageEventFilter_IsOfType> 0x00493EAB, RemoveMessage 0x00493B5B.
// Layout: the landed CSubtitleRenderer ctor 0x0049166E (retail offsets).
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString& operator=(const unsigned short* text);   // 0x0099B800
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};
struct CSetTextInfo {
    CWideString Text, Button1Text, Button2Text, Button3Text;
    unsigned long DisplayFlags;
    float DisplayTime;
    bool UpdateWhilePaused;
    bool ChangePlayerMode;
    CSetTextInfo(const CWideString& text, const CWideString& button1Text, const CWideString& button2Text,
                 const CWideString& button3Text, unsigned long displayFlags, float displayTime,
                 bool updateWhilePaused, bool changePlayerMode);   // 0x0048DDF6
    ~CSetTextInfo();                                      // 0x0043E3D0
};
struct CSetTextInfoQueue {
    unsigned char _body[0x28];
    void push(const CSetTextInfo& info);                  // 0x004941C0
};
struct CFontBank {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void LayoutText(const CWideString& text, bool wrap);   // slot 4
};
struct CTCBase;
struct CHeroControls { unsigned char _pad[0x26]; bool Frozen; };   // +0x26
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                 // 0x0040F020
};
struct CBitset { bool test(unsigned long bit) const; };    // 0x00410DE0
struct CThing {
    unsigned char _pad00[0x20];
    CBitset       Present;                                // +0x20
    unsigned char _pad24[0x44 - 0x24];
    CInterfaceMap Interfaces;                             // +0x44
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBD0
struct CThingCreatureBase : CThing { };
struct CPlayer { CThingCreatureBase* GetHeroThing(); };               // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };      // 0x00449970
struct CMessageEvent;
struct CMessageEventFilter_IsOfType { long Type; CMessageEventFilter_IsOfType(long type) : Type(type) {} };
struct CAllocatorTag { CAllocatorTag() {} };
struct CMessagePtrNode { CMessagePtrNode* m_pNext; CMessagePtrNode* m_pPrev; CMessageEvent* m_value; };
struct CMessagePtrList {
    CMessagePtrNode* m_pHead;
    CMessagePtrList(const CAllocatorTag& tag = CAllocatorTag());   // 0x00493BC2
    ~CMessagePtrList();                                   // 0x00493BDD
};
struct CMessageEventManager {
    template <class F> long FindAllMessages(const F& filter, CMessagePtrList& found);   // 0x00493EAB
    void RemoveMessage(const CMessageEvent* event);       // 0x00493B5B
};
struct CWorld { unsigned char _pad[0x60]; CMessageEventManager* PMessageEventManager; };   // +0x60
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                        // +0x1c
    unsigned char   _pad20[4];
    CWorld*         World;                                // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;          // 0x013B86A0

struct CSubtitleRenderer {
    void*             __vftable;                          // +0x00
    unsigned char     _pad04[0x24 - 0x04];
    long              TextCharactersShowing;              // +0x24
    bool              TextGroupHasEntriesRemaining;       // +0x28
    long              TextGroupIndex;                     // +0x2c
    CWideString       TextGroupTag;                       // +0x30
    CSetTextInfoQueue TextInfoQueue;                      // +0x34
    CWideString       Text;                               // +0x5c
    CWideString       NameText;                           // +0x60
    CWideString       Button1Text;                        // +0x64
    CWideString       Button2Text;                        // +0x68
    CWideString       Button3Text;                        // +0x6c
    CWideString       Button1OverrideText;                // +0x70
    CWideString       Button2OverrideText;                // +0x74
    unsigned char     Button1Alpha;                       // +0x78
    unsigned char     Button2Alpha;                       // +0x79
    unsigned long     DisplayFlags;                       // +0x7c
    CFontBank*        PFont;                              // +0x80
    void*             Graphics;                           // +0x84
    float             MaxDisplayTime;                     // +0x88
    float             TextTimer;                          // +0x8c
    unsigned char     _pad90[0xdd - 0x90];
    bool              AllCharactersDisplayed;             // +0xdd
    unsigned char     _padde[0xe5 - 0xde];
    bool              DisplayingText;                     // +0xe5
    bool              UpdateWhilePaused;                  // +0xe6
    bool              UpdateTimer;                        // +0xe7
    bool GetTextFromTextTag(const CWideString& tag, CWideString& text);   // 0x0048F6BD
    void ProcessEmbeddedGraphicTags();                    // 0x00492381
    void ProcessEmbeddedHighlightTags();                  // 0x00491AD2
    void SetButton1OverrideText(const CCharString& text); // 0x0048F7C0
    void SetButton2OverrideText(const CCharString& text); // 0x0048F881
    void Pause(bool updateWhilePaused);                   // 0x0048DD94
    void ProcessPlayerModes();                            // 0x0048F939
    void SetText(const CFontBank& font, const CWideString& text, const CWideString& name, const CWideString& button1,
                 const CWideString& button2, const CWideString& button3, unsigned long flags, float time,
                 bool updateWhilePaused, bool changePlayerMode);
};

void CSubtitleRenderer::SetText(const CFontBank& font, const CWideString& text, const CWideString& name,
                                const CWideString& button1, const CWideString& button2, const CWideString& button3,
                                unsigned long flags, float time, bool updateWhilePaused, bool changePlayerMode)
{
    if (DisplayingText) {
        CSetTextInfo info(text, button1, button2, button3, flags, time, updateWhilePaused, changePlayerMode);
        TextInfoQueue.push(info);
        return;
    }
    PFont = (CFontBank*)&font;
    TextGroupIndex = -1;
    TextGroupHasEntriesRemaining = false;
    TextGroupTag = (const unsigned short*)L"";
    GetTextFromTextTag(text, Text);
    ProcessEmbeddedGraphicTags();
    ProcessEmbeddedHighlightTags();
    GetTextFromTextTag(name, NameText);
    GetTextFromTextTag(button1, Button2Text);
    GetTextFromTextTag(button2, Button1Text);
    GetTextFromTextTag(button3, Button3Text);
    SetButton1OverrideText("");
    SetButton2OverrideText("");
    TextTimer = 0.0f;
    MaxDisplayTime = time;
    TextCharactersShowing = 0;
    Button1Alpha = 0xff;
    Button2Alpha = 0xff;
    DisplayingText = true;
    DisplayFlags = flags;
    UpdateTimer = true;
    AllCharactersDisplayed = true;
    if (flags & 0x10)
        AllCharactersDisplayed = false;
    PFont->LayoutText(Text, true);
    if (!(DisplayFlags & 0x4c) && !(DisplayFlags & 0x100)) {
        UpdateWhilePaused = updateWhilePaused;
    } else {
        if (DisplayFlags & 0x40) {
            CThingCreatureBase* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
            if (IS_THING_ALIVE<CThing*>(hero) && hero->Present.test(0xcb))
                hero->GetTC<CHeroControls>(0xcb)->Frozen = true;
        }
        Pause(true);
    }
    if (changePlayerMode)
        ProcessPlayerModes();
    CMessageEventManager* manager = g_pMainGameComponent->World->PMessageEventManager;
    CMessagePtrList found;
    if (manager->FindAllMessages(CMessageEventFilter_IsOfType(0x11), found) > 0) {
        CMessagePtrNode* end = found.m_pHead;
        for (CMessagePtrNode* it = end->m_pNext; it != end; it = it->m_pNext)
            manager->RemoveMessage(it->m_value);
    }
}
