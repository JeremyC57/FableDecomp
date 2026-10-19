// CGameScriptInterface::CheckForCameraMessage(const CCharString& message) const -- whether the most recent camera
// message event (type 0x2B) targets a live thing whose scripted camera spline (TC 0x3F) carries this message; a
// match consumes the event
// Leaf: ?CheckForCameraMessage@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E7920, length 192 bytes, `ret 4`.
// Callees: CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType> @ 0x004AB8B0 (CWorld+0x60),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the TC map lower bound @ 0x0040F020,
// CTCCameraPointScriptedSpline::HasMessage(const CCharString&) @ 0x0075D2B0, CMessageEventManager::RemoveMessage
// @ 0x00493B5B.
// Layout: CGameScriptInterface World +0x04; CWorld message manager +0x60; CMessageEvent PTarget +0x48
// (CIntelligentPointer<CThing>); CThing ThingTypeFlags +0x6C, TC bitset +0x20 (TC 0x3F = +0x24 bit 31), TC map
// +0x44, dead bit +0x91 bit 0.
// Shape: the event's target comes through the intelligent pointer's typed getter -- GetPItem() null test, then the
// THING_TYPE test of IsThingOfType<CThing> whose zero mask is copied into a local (VC7.1 folds `flags & 0` late and
// keeps the dead store of the item plus `xor al,al ; jne`, as in the landed 0x008ABDF0), then GetPItem() again; the
// filter type and the TC key share the `push ecx` frame word; `HasMessage(..) == true` gives `cmp al,1`; the world's
// message manager is reached through its CCountedPointer member's inline operator-> (a plain pointer member rotates
// eax/ecx/edx through the whole body).

class CCharString;

struct CThing {
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
    CMessageEventFilter_IsOfType(long type) : m_type(type) {}
};
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter);   // 0x004AB8B0
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
template<class T> struct CCountedPointer {
    T* Data;
    void* Info;
    T* operator->() const { return Data; }
};
struct CWorld {
    virtual void w000();
    unsigned char _pad04[0x5c];
    CCountedPointer<CMessageEventManager> PMessageEventManager;    // +0x60
};

struct CTCCameraPointScriptedSpline {
    bool HasMessage(const CCharString& message);    // 0x0075D2B0
};

struct CGameScriptInterface {
    virtual bool CheckForCameraMessage(const CCharString& message) const;
    CWorld* World;                                  // +0x04
};

bool CGameScriptInterface::CheckForCameraMessage(const CCharString& message) const
{
    CMessageEvent* event = World->PMessageEventManager->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x2b));
    if (event) {
        CThing* thing = event->PTarget.GetPTypedItem();
        CTCCameraPointScriptedSpline* spline;
        if (thing && !thing->m_dead && thing->GetInterface(&spline, 0x3f)) {
            if (spline->HasMessage(message) == true) {
                World->PMessageEventManager->RemoveMessage(event);
                return true;
            }
        }
    }
    return false;
}