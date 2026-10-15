// CGameScriptInterface::MsgIsBoastMade(long boast) -- true when a "boast made" message (0x29) carrying this boast id occurred between the current and the maximum world frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00893960, length 99 bytes.
// Callees: the two frame getters @ 0x006E7510 / 0x006E7530 (both labelled GetMaxWorldFrameForMessages),
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<IsOfType, HasExtraData, OccuredBetween>> @ 0x008ABF20 through CWorld+0x60.
// Shape: the filter is the chain `IsOfType(0x29) && HasExtraData(boast) && OccuredBetween(frame, maxFrame)`; the 0x18-byte frame
// holds HasExtraData at +8, OccuredBetween at +0xc..+0x10 and the Bind3 pointers at +0x14..+0x1c, while VC builds the IsOfType
// temporary in the dead argument slot (after copying the boast id out of it); `neg ; sbb ; neg` for `message != 0`.
// Leaf: MsgIsBoastMade
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
struct CMessageEventFilter_HasExtraData {
    long m_data;
    CMessageEventFilter_HasExtraData(long data) : m_data(data) { }
};
struct CMessageEventFilter_OccuredBetween {
    long m_frame;
    long m_maxFrame;
    CMessageEventFilter_OccuredBetween(long frame, long maxFrame) : m_frame(frame), m_maxFrame(maxFrame) { }
};
template<class A, class B, class C> struct CFilter_Bind3 {   // three operand pointers
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_HasExtraData, CMessageEventFilter_OccuredBetween> CBoastFilter;
struct CMessageEventManager_Retail {
    CMessageEvent_Retail* FindMostRecentMessage(const CSeenOrHeardFilter& filter);   // 0x008ABED0
    CMessageEvent_Retail* FindMostRecentMessage(const CBoastFilter& filter);         // 0x008ABF20
};
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
    bool MsgIsBoastMade(long boast) const;
};

bool CGameScriptInterface::MsgIsBoastMade(long boast) const
{
    long frame = GetCurrentWorldFrameForMessages();
    long maxFrame = GetMaxWorldFrameForMessages();
    CMessageEventManager_Retail* messages = World->PMessageEventManager_Data;
    CMessageEvent_Retail* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x29) && CMessageEventFilter_HasExtraData(boast) && CMessageEventFilter_OccuredBetween(frame, maxFrame));
    return message != 0;
}
