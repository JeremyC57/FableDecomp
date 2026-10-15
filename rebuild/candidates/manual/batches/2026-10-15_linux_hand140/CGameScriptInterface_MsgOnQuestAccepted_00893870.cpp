// CGameScriptInterface::MsgOnQuestAccepted(CCharString& name) -- the name of the quest of the most recent "quest accepted" message seen or heard this frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00893870. Message type 0x37.
// Callees: the frame getters, FindMostRecentMessage @ 0x008ABED0, CQuestManager::GetRegisteredQuestFromIndex @ 0x004AF3C0 on the global @ 0x013B89FC, CCharString::operator= @ 0x0099EFB0.
// Shape (shared by the Msg family): the two frame getters 0x006E7510 / 0x006E7530 (both labelled GetMaxWorldFrameForMessages), the 0x14-byte frame holds the IsOfType filter at +8, the CanBeSeenOrHeard filter at +0xc..+0x10 and the Bind2 pair of pointers at +0x14..+0x18; CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>> @ 0x008ABED0 through CWorld+0x60 (engine/CWorld.h).
// Leaf: MsgOnQuestAccepted
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
struct CQuestManager_Retail {
    const CCharString& GetRegisteredQuestFromIndex(long index);   // 0x004AF3C0 (an empty-string global for a bad index)
};
extern CQuestManager_Retail* g_pQuestManager;       // 0x013B89FC
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
    bool MsgOnQuestAccepted(CCharString& name) const;
};

bool CGameScriptInterface::MsgOnQuestAccepted(CCharString& name) const
{
    long frame = GetCurrentWorldFrameForMessages();
    long maxFrame = GetMaxWorldFrameForMessages();
    CMessageEventManager_Retail* messages = World->PMessageEventManager_Data;
    CMessageEvent_Retail* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x37) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame));
    if (message) {
        name = g_pQuestManager->GetRegisteredQuestFromIndex(message->m_extraData);
        return true;
    }
    return false;
}
