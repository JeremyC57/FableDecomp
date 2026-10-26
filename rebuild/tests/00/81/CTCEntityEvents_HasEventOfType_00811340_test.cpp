// Test for 0x00811340 CTCEntityEvents::HasEventOfType: true when an event of the type is on the list.
#include <cstdio>
namespace NEntityEvents {
enum EEventType { EVENT_TYPE_0 = 0 };
class CEventBase { public: EEventType m_type; virtual void v00() { } virtual EEventType GetType() const { return m_type; } };
}
struct SEventNode { SEventNode* m_pNext; SEventNode* m_pPrev; NEntityEvents::CEventBase* m_pEvent; void* m_pEventInfo; };
class CTCEntityEvents {
public:
    unsigned char _pad00[0xc];
    SEventNode*   Events;
    bool HasEventOfType(NEntityEvents::EEventType type) const;
};
int main()
{
    SEventNode head; NEntityEvents::CEventBase e1, e2; e1.m_type = (NEntityEvents::EEventType)4; e2.m_type = (NEntityEvents::EEventType)13;
    SEventNode n1, n2;
    head.m_pNext = &head; head.m_pPrev = &head;
    CTCEntityEvents ev; ev.Events = &head;
    if (ev.HasEventOfType((NEntityEvents::EEventType)13)) { std::printf("00811340_TEST FAIL\n"); return 1; }
    head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &head; n1.m_pEvent = &e1; n2.m_pEvent = &e2;
    if (!ev.HasEventOfType((NEntityEvents::EEventType)4) || !ev.HasEventOfType((NEntityEvents::EEventType)13) || ev.HasEventOfType((NEntityEvents::EEventType)5)) { std::printf("00811340_TEST FAIL\n"); return 1; }
    std::printf("00811340_TEST PASS\n");
    return 0;
}
