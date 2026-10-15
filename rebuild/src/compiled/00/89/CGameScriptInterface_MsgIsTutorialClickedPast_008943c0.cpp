// CGameScriptInterface::MsgIsTutorialClickedPast() -- true unless tutorials are on (the global switch and the active user profile) and no "tutorial clicked past" message (0x12) was seen or heard this frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008943C0, length 125 bytes.
// Callees: CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0 (static), the two frame getters @ 0x006E7510 / 0x006E7530,
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>> @ 0x008ABED0 through CWorld+0x60.
// Shape: `message ? true : false` gives the `setne` tail (`message != 0` gives `neg ; sbb ; neg`). The global tutorial switch byte @ 0x01375740 is read before the frame is set up; the profile's "tutorials on" test is an
// inline accessor choosing between two bytes (+0xd7 when the profile flag at +0xc is set, else +0x17); `setne` for `message != 0`.
// Leaf: MsgIsTutorialClickedPast
struct CCharString {
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator=(const char* text);           // 0x0099EFE0
    ~CCharString();                                  // 0x0099EAE0
};
struct CMessageEvent_Retail {
    unsigned char _pad[0x3c];
    long          m_extraData;                      // +0x3c (the message's payload word: region/quest index, feat id, record pointer ...)
};
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
struct CMessageEventFilter_CanBeSeenOrHeard {
    long m_frame;
    long m_maxFrame;
    CMessageEventFilter_CanBeSeenOrHeard(long frame, long maxFrame) : m_frame(frame), m_maxFrame(maxFrame) { }
};
template<class A, class B> struct CFilter_Bind2 {   // the bound filter holds POINTERS to its operands
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CSeenOrHeardFilter;
struct CMessageEventManager_Retail {
    CMessageEvent_Retail* FindMostRecentMessage(const CSeenOrHeardFilter& filter);   // 0x008ABED0
};
struct CUserProfileManager_Retail {
    unsigned char _pad00[0xc];
    bool          m_hasActiveProfile;               // +0x0c
    unsigned char _pad0d[0xa];
    bool          m_defaultTutorialsOn;             // +0x17
    unsigned char _pad18[0xbf];
    bool          m_profileTutorialsOn;             // +0xd7
    static CUserProfileManager_Retail& Get();       // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)
    bool AreTutorialsOn() const { return m_hasActiveProfile ? m_profileTutorialsOn : m_defaultTutorialsOn; }
};
extern bool g_tutorialsEnabled;                     // 0x01375740
struct CWorldMap_Retail;
struct CDefinitionManager_Retail;
struct CPlayerManager_Retail;
struct CRegion_Retail {
    unsigned char _pad[0x18];
    CCharString   m_name;                           // +0x18
};
struct CWorldMap_Retail {
    CRegion_Retail& GetRegion(long index);          // 0x004FC180
};
struct CWorld {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012();
    virtual CWorldMap_Retail* PeekWorldMap();       // slot 13 (+0x34)
    unsigned char                _pad04[0x5c];
    CMessageEventManager_Retail* PMessageEventManager_Data;   // +0x60 (engine/CWorld.h)
};
struct CGameScriptInterface {
    virtual void v000();
    CWorld* World;                                  // +0x04 (engine/CGameScriptInterface.h)
    void*   Component;                              // +0x08
    void*   DisplayEngine;                          // +0x0c
    CDefinitionManager_Retail* DefinitionManager;   // +0x10
    CPlayerManager_Retail*     PlayerManager;       // +0x14
    long GetCurrentWorldFrameForMessages() const;   // 0x006E7510 (labelled GetMaxWorldFrameForMessages)
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
    bool MsgIsTutorialClickedPast() const;
};

bool CGameScriptInterface::MsgIsTutorialClickedPast() const
{
    if (g_tutorialsEnabled && CUserProfileManager_Retail::Get().AreTutorialsOn()) {
        long frame = GetCurrentWorldFrameForMessages();
        long maxFrame = GetMaxWorldFrameForMessages();
        CMessageEventManager_Retail* messages = World->PMessageEventManager_Data;
        CMessageEvent_Retail* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x12) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame));
        return message ? true : false;
    }
    return true;
}