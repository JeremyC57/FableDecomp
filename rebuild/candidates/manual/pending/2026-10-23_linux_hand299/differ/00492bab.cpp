// Grade: DIFFER(670v672) -- register choice: retail caches L"" in edi and "" / the player mode / 0xcb in ebx (ours swaps them); the queue test loads finish first and re-reads start for front() (ours keeps start in a register)
// CSubtitleRenderer::Clear(bool keepText) -- drop every primitive handle; unless keeping the text, reset the captions and
// overrides, end the player mode it added, unfreeze the hero, post the "text finished" messages its flags ask for, unpause
// the game, and show the next queued text
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: Clear
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00492BAB, 672 bytes, `ret 4`.
// Callees: for_each<CEnginePrimitiveHandle*, mem_fun_ref_t> 0x0043F360 (/Gr __fastcall) with CEnginePrimitiveHandle::Clear
//   0x0041BE70; GetMainPlayer 0x00449970, GetHeroThing 0x00487DC0, CWideString::operator=(const wchar_t*) 0x0099B800,
//   CCharString 0x0099EBF0/0x0099EAE0, SetButton1/2OverrideText 0x0048F7C0/0x0048F881, IS_THING_ALIVE 0x0043DBD0,
//   CPlayer::RemoveMode 0x00634190, bitset::test 0x00410DE0, LowerBound 0x0040F020, CMessageEventManager::AddMessage
//   0x00687540, CMemoryBuffer::Copy 0x009F1760, AddGameEvent 0x0041649C, CWideString(const wchar_t*) 0x0099B6B0 /
//   ~CWideString 0x0099B510, SetText 0x00492973, queue<CSetTextInfo>::pop 0x00493CF5, __security_check_cookie.
// Layout: the landed CSubtitleRenderer ctor 0x0049166E; CGameEvent as the landed Pause 0x0048DD94.
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
    void pop();                                           // 0x00493CF5
};
struct CFontBank {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void LayoutText(const CWideString& text, bool wrap);   // slot 4
};
struct CTCBase;
struct CHeroControls { unsigned char _pad[0x27]; bool Unfrozen; };   // +0x27
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
struct CPlayer { CThing* GetHeroThing(); };               // 0x00487DC0
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
struct CGameEvent;
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                        // +0x1c
    unsigned char   _pad20[4];
    CWorld*         World;                                // +0x24
    void AddGameEvent(const CGameEvent& event);           // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;          // 0x013B86A0

struct CEnginePrimitiveHandle {
    void Clear();                                        // 0x0041BE70
    unsigned long m_handle;
};
struct CMemFunRefClear {
    void (CEnginePrimitiveHandle::*m_pmf)();
    explicit CMemFunRefClear(void (CEnginePrimitiveHandle::*pmf)()) : m_pmf(pmf) { }
};
CMemFunRefClear __fastcall for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CMemFunRefClear f);   // 0x0043F360
struct CHandleVector { CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd; };
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CMessageEventManager2 {
    CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector* position, CThing* thing1, CThing* thing2, float a, float b, long priority);   // 0x00687540
};
struct CMemoryBuffer { void Copy(unsigned long length, const void* data); };   // 0x009F1760
struct CGameEvent : CMemoryBuffer {
    long          Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool          Valid;
    bool          Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};
struct CWideStringC { CWideStringData* PStringData; CWideStringC(const unsigned short* text); ~CWideStringC(); };   // 0x0099B6B0 / 0x0099B510
struct CPlayer2 { void RemoveMode(long mode); };          // 0x00634190
struct CDequeIterator {
    CSetTextInfo* _M_cur; CSetTextInfo* _M_first; CSetTextInfo* _M_last; CSetTextInfo** _M_node;
    bool operator==(const CDequeIterator& x) const { return _M_cur == x._M_cur; }
    CSetTextInfo& operator*() const { return *_M_cur; }
};
struct CSetTextInfoQueue2 {
    CDequeIterator _M_start;
    CDequeIterator _M_finish;
    bool empty() const { return _M_finish == _M_start; }
    CSetTextInfo& front() { return *_M_start; }
};
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
    long              PlayerModeAdded;                    // +0x90
    CHandleVector     PrimitiveHandles;                   // +0x94
    CHandleVector     ButtonPrimitiveHandles;             // +0xa0
    CHandleVector     EmbeddedPrimitiveHandles;           // +0xac
    unsigned char     _padb8[0xdc - 0xb8];
    bool              PausedGame;                         // +0xdc
    bool              AllCharactersDisplayed;             // +0xdd
    unsigned char     _padde[0xe5 - 0xde];
    bool              DisplayingText;                     // +0xe5
    bool              UpdateWhilePaused;                  // +0xe6
    bool              UpdateTimer;                        // +0xe7
    void SetButton1OverrideText(const CCharString& text); // 0x0048F7C0
    void SetButton2OverrideText(const CCharString& text); // 0x0048F881
    void SetText(const CFontBank& font, const CWideString& text, const CWideStringC& name, const CWideString& button1,
                 const CWideString& button2, const CWideString& button3, unsigned long flags, float time,
                 bool updateWhilePaused, bool changePlayerMode);   // 0x00492973
    void Clear(bool keepText);
};

void CSubtitleRenderer::Clear(bool keepText)
{
    for_each(PrimitiveHandles.m_pFirst, PrimitiveHandles.m_pLast, CMemFunRefClear(&CEnginePrimitiveHandle::Clear));
    for_each(ButtonPrimitiveHandles.m_pFirst, ButtonPrimitiveHandles.m_pLast, CMemFunRefClear(&CEnginePrimitiveHandle::Clear));
    for_each(EmbeddedPrimitiveHandles.m_pFirst, EmbeddedPrimitiveHandles.m_pLast, CMemFunRefClear(&CEnginePrimitiveHandle::Clear));
    if (keepText)
        return;
    CPlayer* player = g_pMainGameComponent->PlayerManager->GetMainPlayer();
    CThing* hero = player->GetHeroThing();
    DisplayingText = false;
    UpdateTimer = true;
    Button1Alpha = 0xff;
    Button2Alpha = 0xff;
    Button1Text = (const unsigned short*)L"";
    Button2Text = (const unsigned short*)L"";
    Button3Text = (const unsigned short*)L"";
    SetButton1OverrideText("");
    SetButton2OverrideText("");
    long mode = PlayerModeAdded;
    if (mode) {
        if (IS_THING_ALIVE(hero))
            ((CPlayer2*)player)->RemoveMode(mode);
        PlayerModeAdded = 0;
    }
    if ((DisplayFlags & 0x40) && IS_THING_ALIVE(hero) && hero->Present.test(0xcb))
        hero->GetTC<CHeroControls>(0xcb)->Unfrozen = true;
    if (DisplayFlags & 8)
        ((CMessageEventManager2*)g_pMainGameComponent->World->PMessageEventManager)->AddMessage(0x12, 0x32, &C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
    if (DisplayFlags & 2)
        ((CMessageEventManager2*)g_pMainGameComponent->World->PMessageEventManager)->AddMessage(0x11, 0x32, &C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
    DisplayFlags = 0;
    if (PausedGame) {
        CGameEvent event(9);
        event.AddUBYTE(0);
        g_pMainGameComponent->AddGameEvent(event);
        PausedGame = false;
    }
    CSetTextInfoQueue2* queue = (CSetTextInfoQueue2*)&TextInfoQueue;
    if (!queue->empty()) {
        CSetTextInfo* info = &queue->front();
        CWideStringC empty((const unsigned short*)L"");
        SetText(*PFont, info->Text, empty, info->Button1Text, info->Button2Text,
                info->Button3Text, info->DisplayFlags, info->DisplayTime, info->UpdateWhilePaused, info->ChangePlayerMode);
        TextInfoQueue.pop();
    }
}
