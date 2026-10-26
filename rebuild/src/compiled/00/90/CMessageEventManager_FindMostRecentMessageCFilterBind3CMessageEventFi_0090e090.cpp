// CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<CMessageEventFilter_IsOfType,CMessageEventFilter_IsOriginatedByThing,CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> > >(const CFilter_Bind3<...>&) -- first message that is of the type, originated by the thing and whose originator passes the 2D range filter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090E090, 144 bytes (padding included).
// Leaf: FindMostRecentMessage
// Shape: the landed FindMostRecentMessage template (0x00746260) with a three-filter bind: the first two
// filters are out-of-line operator() calls (0x006AF1C0 IsOfType, 0x006AF200 IsOriginatedByThing),
// the third (ThingSearchFilter<In2DRange>) is inlined: the event's POriginator intelligent pointer
// (+0x48) is fetched with GetPItem (0x00A01B50) three times (non-null, all five low bits of the
// thing's +0x6c word set, non-null again) and the wrapped CThingFilter_In2DRange (0x006A76B0) decides.
// The manager is spilled to the `push ecx` word because ebx is reused for the third filter.
class CThing;
class CMessageEvent;

class CBaseObject { public:
    unsigned char _pad[0x6c];
    unsigned long m_flags6c;                       // +0x6c
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PItem;                            // +4
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};

struct SMessageNode {                              // list node: next, prev, then the event at +8
    SMessageNode*  m_pNext;
    SMessageNode*  m_pPrev;
    unsigned char  m_value[0x58];                  // +0x08 CMessageEvent
    CMessageEvent* GetEvent() { return (CMessageEvent*)m_value; }
};
struct CMessageList {
    SMessageNode* m_pHead;
};

class CMessageEventFilter_IsOfType { public:
    bool operator()(CMessageEvent* event) const;   // 0x006AF1C0
};
class CMessageEventFilter_IsOriginatedByThing { public:
    bool operator()(CMessageEvent* event) const;   // 0x006AF200
};
class CThingFilter_In2DRange { public:
    bool operator()(CThing* thing) const;          // 0x006A76B0
};

inline const CBaseIntelligentPointer& GetOriginator(CMessageEvent* event)   // POriginator at +0x48
{
    return *(const CBaseIntelligentPointer*)((char*)event + 0x48);
}

template <class F> class CMessageEventFilter_ThingSearchFilter { public:
    const F* m_pFilter;
    bool operator()(CMessageEvent* event) const
    {
        const CBaseIntelligentPointer& originator = GetOriginator(event);
        if (originator.GetPItem() && !(~originator.GetPItem()->m_flags6c & 0x1f)) {   // all five low bits set: `not eax ; test al,1Fh`
            CThing* thing = (CThing*)originator.GetPItem();
            if (thing)
                return (*m_pFilter)(thing);
        }
        return false;
    }
};

template <class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    bool operator()(CMessageEvent* event) const { return (*m_pFirst)(event) && (*m_pSecond)(event) && (*m_pThird)(event); }
};

class CWorld;
class CMessageEventManager { public:
    CWorld*      World;                            // +0x00
    CMessageList Messages;                         // +0x04
    template <class TFilter> CMessageEvent* FindMostRecentMessage(const TFilter& filter)
    {
        for (SMessageNode* node = Messages.m_pHead->m_pNext; node != Messages.m_pHead; node = node->m_pNext) {
            CMessageEvent* event = node->GetEvent();
            if (filter(event))
                return event;
        }
        return 0;
    }
};

typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing,
                      CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> > CBoundFilter_0090e090;
template CMessageEvent* CMessageEventManager::FindMostRecentMessage<CBoundFilter_0090e090>(const CBoundFilter_0090e090& filter);