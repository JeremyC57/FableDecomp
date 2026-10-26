// CGameScriptInterface::WaitForCameraMessage(const CCharString& message) const -- block the calling script, a frame at a time, until a camera message event (type 0x2B) targets a live thing whose scripted camera spline (TC 0x3F) carries this message; the matching event is consumed
// Leaf: ?WaitForCameraMessage@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E7820, length 243 bytes (+ int3 padding), `ret 4`.
// Signature from Ego_r.pdb: ?WaitForCameraMessage@CGameScriptInterface@@UBEXABVCCharString@@@Z.
// Callees: CMessageEventFilter_IsOfType::operator() @ 0x006AF1C0 (the FindMostRecentMessage template inlined),
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the TC map lower bound @ 0x0040F020, CTCCameraPointScriptedSpline::
//   HasMessage(const CCharString&) @ 0x0075D2B0, CMessageEventManager::RemoveMessage @ 0x00493B5B, the script runner's
//   virtual slot 2 (yield one frame). Global: the script runner @ 0x013D2838 (stop byte +5).
// Layout and shape: the landed CheckForCameraMessage 0x006E7920 model (World +4, message manager +0x60, PTarget +0x48,
//   the zero-mask IsThingOfType that keeps `xor cl,cl ; jne` and the dead item store in the filter's frame word), wrapped
//   in a `do { ... yield; } while (!stop)` loop; here the message manager's FindMostRecentMessage template (landed
//   0x00746260 form) is inlined, so the list head is re-read every step and the event (node + 8) lives in ebp; the
//   GetInterface end/hit choice is one ternary so the spline lands straight in ecx for HasMessage.
class CCharString;

class CThing { public:
    enum { ThingTypeMask = 0 };
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];       // +0x20 std::bitset of the interface types present
    struct CInterfaceMap_Retail {
        struct SEntry { long type; void* component; };
        SEntry* first;
        SEntry* last;
        SEntry* LowerBound(const long* type);       // 0x0040F020
    } m_interfaces;                       // +0x44
    unsigned char _pad4c[0x20];
    unsigned long ThingTypeFlags;         // +0x6c
    unsigned char _pad70[0x21];
    unsigned char m_dead : 1;             // +0x91 bit 0
    unsigned char m_91_1 : 7;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
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

class CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPTypedItem() const
    {
        return (GetPItem() != 0 && IsThingOfType<T>((T*)GetPItem())) ? (T*)GetPItem() : 0;
    }
};

class CMessageEvent { public:
    unsigned char _pad[0x48];
    CIntelligentPointer<CThing> PTarget;            // +0x48
};
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) {}
    bool operator()(CMessageEvent* event) const;    // 0x006AF1C0
};
struct SMessageNode {                               // list node: next, prev, then the event at +8
    SMessageNode* m_pNext;
    SMessageNode* m_pPrev;
    unsigned char m_value[0x58];
    CMessageEvent* GetEvent() { return (CMessageEvent*)m_value; }
};
class CMessageEventManager { public:
    void*         World;                            // +0x00
    SMessageNode* m_pHead;                          // +0x04 list<CMessageEvent> sentinel
    template<class TFilter> CMessageEvent* FindMostRecentMessage(const TFilter& filter)
    {
        for (SMessageNode* node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) {
            CMessageEvent* event = node->GetEvent();
            if (filter(event))
                return event;
        }
        return 0;
    }
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
template<class T> class CCountedPointer { public:
    T* Data;
    void* Info;
    T* operator->() const { return Data; }
};
class CWorld { public:
    virtual void w000();
    unsigned char _pad04[0x5c];
    CCountedPointer<CMessageEventManager> PMessageEventManager;    // +0x60
};

class CTCCameraPointScriptedSpline { public:
    bool HasMessage(const CCharString& message);    // 0x0075D2B0
};


struct CScriptRunner {
    virtual void v00();
    virtual void v01();
    virtual void YieldFrame();                      // slot 2 (+8)
    unsigned char m_pad4;                           // +0x04
    bool          m_bStopRequested;                 // +0x05
};
extern CScriptRunner* g_pScriptRunner;              // 0x013D2838

class CGameScriptInterface { public:
    virtual void WaitForCameraMessage(const CCharString& message) const;
    CWorld* World;                                  // +0x04
};

void CGameScriptInterface::WaitForCameraMessage(const CCharString& message) const
{
    do {
        CMessageEvent* event = World->PMessageEventManager->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x2b));
        if (event) {
            CThing* thing = event->PTarget.GetPTypedItem();
            CTCCameraPointScriptedSpline* spline;
            if (thing && !thing->m_dead && thing->GetInterface(&spline, 0x3f)) {
                if (spline->HasMessage(message) == true) {
                    World->PMessageEventManager->RemoveMessage(event);
                    return;
                }
            }
        }
        g_pScriptRunner->YieldFrame();
    } while (!g_pScriptRunner->m_bStopRequested);
}