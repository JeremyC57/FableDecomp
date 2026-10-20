// CGameScriptInterface::MsgOnLeavingQuestStartScreen(CCharString& questName) const -- whether a "leaving quest start screen" message (type 0x49) seen or heard this frame targets a live thing whose quest component (TC 0xFC) names this quest
// Leaf: ?MsgOnLeavingQuestStartScreen@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089AE30, length 250 bytes (+ int3 padding), `ret 4`.
// Signature from Ego_r.pdb: ?MsgOnLeavingQuestStartScreen@CGameScriptInterface@@UBE_NAAVCCharString@@@Z.
// Callees: the message frame getters 0x006E7510 / 0x006E7530 (landed MsgOn* family), CMessageEventManager::
//   FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>> @ 0x008ABED0, CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50, the TC map lower bound @ 0x0040F020, the CCharString compare @ 0x00411570 (__fastcall, the landed body
//   compiled above as a same-TU stand-in so VC7.1 knows it writes no memory).
// Layout: the landed MsgOnHeroPickedPocket 0x00893FF0 filter frame (IsOfType +0, CanBeSeenOrHeard +4, Bind2 pointers +0xc)
//   and the CheckForCameraMessage 0x006E7920 target model (PTarget +0x48, zero-mask IsThingOfType); retail CThing TC 0xFC =
//   +0x3c bit 28; the component's quest name (CCharString) +0x50.
// Shape: the frames are taken as they are (no -1) and the manager read into a local before the filters are built (the
//   landed MsgOnExpressionPerformed 0x00893B00 form); the `__forceinline` CCharString operator== (same block, both non-null, equal
//   length, `Compare ? false : true`) gives the 8-bit `neg ; sbb al,al ; inc al`; true and false keep separate exits.
struct CCharStringData {
    const char*   m_pData;                          // +0x00
    unsigned long m_length;                         // +0x04
};
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
struct CCharString {
    CCharStringData* PStringData;
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

struct CThing {
    enum { ThingTypeMask = 0 };
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    struct CInterfaceMap_Retail {
        struct SEntry { long type; void* component; };
        SEntry* first;
        SEntry* last;
        SEntry* LowerBound(const long* type);       // 0x0040F020
    } m_interfaces;                                 // +0x44
    unsigned char _pad4c[0x20];
    unsigned long ThingTypeFlags;                   // +0x6c
    unsigned char _pad70[0x21];
    unsigned char m_dead : 1;                       // +0x91 bit 0
    unsigned char m_91_1 : 7;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
template <class T> inline bool IsThingOfType(const CThing* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
struct CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPTypedItem() const
    {
        return (GetPItem() != 0 && IsThingOfType<T>((T*)GetPItem())) ? (T*)GetPItem() : 0;
    }
};
struct CMessageEvent {
    unsigned char _pad[0x48];
    CIntelligentPointer<CThing> PTarget;            // +0x48
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
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CSeenOrHeardFilter;
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CSeenOrHeardFilter& filter);   // 0x008ABED0
};
struct CWorld {
    virtual void w000();
    unsigned char _pad04[0x5c];
    CMessageEventManager* PMessageEventManager_Data;   // +0x60
};
struct CTCQuestStartTarget {                        // interface 0xFC
    unsigned char _pad00[0x50];
    CCharString QuestName;                          // +0x50
};

struct CGameScriptInterface {
    virtual bool MsgOnLeavingQuestStartScreen(CCharString& questName) const;
    CWorld* World;                                  // +0x04
    long GetCurrentWorldFrameForMessages() const;   // 0x006E7510
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
};

bool CGameScriptInterface::MsgOnLeavingQuestStartScreen(CCharString& questName) const
{
    long frame = GetCurrentWorldFrameForMessages();
    long maxFrame = GetMaxWorldFrameForMessages();
    CMessageEventManager* messages = World->PMessageEventManager_Data;
    CMessageEvent* message = messages->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(0x49) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame));
    if (message) {
        CThing* thing = message->PTarget.GetPTypedItem();
        CTCQuestStartTarget* target;
        if (thing && !thing->m_dead && thing->GetInterface(&target, 0xfc)) {
            if (target->QuestName == questName)
                return true;
        }
    }
    return false;
}