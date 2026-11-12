// CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<CMessageEventFilter_IsOfType,CMessageEventFilter_IsOriginatedByThing,CMessageEventFilter_IsTargettedAtThing> >(const CFilter_Bind3<...>& filter) -- the first event of the message list (+4, newest first) accepted by all three bound filters (IsOfType @ 0x006AF1C0, IsOriginatedByThing @ 0x006AF200, IsTargettedAtThing @ 0x006AF1E0), null when none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091A5B0, length 85 bytes.
// Callees: 0x006AF1C0, 0x006AF200, 0x006AF1E0.
// Name fix (naming agent NF21, 2026-11-11): the three filters' Ego_r.pdb / FableWin.pdb spellings take `const CMessageEvent&`
//   (??RCMessageEventFilter_IsOfType@@QBE_NABVCMessageEvent@@@Z etc.). By body: 0x006AF1C0 compares event +0x08 (CMessageEvent::Type) =
//   IsOfType; 0x006AF200 compares the 64-bit event +0x20 (OriginatorUID) = IsOriginatedByThing (Ego_r { ulong64 UID; }; the landed
//   0x00959040 binds it so); 0x006AF1E0 compares +0x28 (TargetUID) = IsTargettedAtThing (the landed 0x008D46D0 binds it so). The row
//   called the last two HasExtraData (which reads ExtraData +0x3C) and OccuredBetween (TimeStamp +0x34, 0x006AF180). The bound filter holds POINTERS to its three operands (the landed CFilter_Bind3 model, 0x00910D50); each is re-read from the bind for every pass. Shape: the manager in ebp, the bind in ebx, the node in edi and the event (node + 8) in esi, `ret 4`.
class CMessageEvent;                              // PDB layout: Type +8, OriginatorUID +0x20, TimeStamp +0x34, Lifespan +0x38, MessageID +0x40, POriginator +0x48 (0x58 bytes)
struct SMessageNode {                              // the STLport-style list node: next, prev, then the event at +8
    SMessageNode*  m_pNext;
    SMessageNode*  m_pPrev;
    unsigned char  m_value[0x58];                  // +0x08 CMessageEvent
    const CMessageEvent* GetEvent() const { return (const CMessageEvent*)m_value; }
};
struct CMessageList {                              // list<CMessageEvent>: one sentinel pointer, no size word (size() walks)
    SMessageNode* m_pHead;
};

class CMessageEventFilter_IsOfType { public:
    long Type;
    bool operator()(const CMessageEvent& event) const;   // 0x006AF1C0 ??RCMessageEventFilter_IsOfType@@QBE_NABVCMessageEvent@@@Z
};
class CMessageEventFilter_IsOriginatedByThing { public:
    bool operator()(const CMessageEvent& event) const;   // 0x006AF200 ??RCMessageEventFilter_IsOriginatedByThing@@QBE_NABVCMessageEvent@@@Z
};
class CMessageEventFilter_IsTargettedAtThing { public:
    unsigned long UID[2];                                // +0x00 ulong64 UID
    bool operator()(const CMessageEvent& event) const;   // 0x006AF1E0 ??RCMessageEventFilter_IsTargettedAtThing@@QBE_NABVCMessageEvent@@@Z
};

template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    bool operator()(const CMessageEvent* event) const { return (*m_pFirst)(*event) && (*m_pSecond)(*event) && (*m_pThird)(*event); }
};

class CWorld;
class CMessageEventManager { public:
    CWorld*      World;                            // +0x00
    CMessageList Messages;                         // +0x04
    template<class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const   // Ego_r.pdb 0x0093E080: QBEPBV
    {
        for (const SMessageNode* node = Messages.m_pHead->m_pNext; node != Messages.m_pHead; node = node->m_pNext) {
            const CMessageEvent* event = node->GetEvent();   // a local: retail keeps node + 8 in a register and returns it
            if (filter(event))
                return event;
        }
        return 0;
    }
};

typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing, CMessageEventFilter_IsTargettedAtThing> CBoundFilter_0091a5b0;
template const CMessageEvent* CMessageEventManager::FindMostRecentMessage<CBoundFilter_0091a5b0>(const CBoundFilter_0091a5b0& filter) const;