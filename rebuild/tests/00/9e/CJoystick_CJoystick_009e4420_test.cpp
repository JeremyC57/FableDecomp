// Behaviour test for 0x009E4420 CJoystick::~CJoystick: list cleared and freed, then 64 events destroyed last to first.
#include <cstdio>
#include <cstdlib>

static int gEventDtors = 0, gFirstIndex = -1, gLastIndex = -1, gClears = 0, gFrees = 0, gOrderOk = 1;

class CInputEvent {
public:
    int Index;
    CInputEvent() : Index(0) {}
    ~CInputEvent()
    {
        if (gClears == 0) gOrderOk = 0;            // the list member is destroyed first
        if (gFirstIndex < 0) gFirstIndex = Index;
        gLastIndex = Index;
        ++gEventDtors;
    }
};
struct SMaintainedNode { SMaintainedNode* m_pNext; SMaintainedNode* m_pPrev; };
struct CMaintainedEventList {
    SMaintainedNode* m_pNode;
    CMaintainedEventList()
    {
        m_pNode = (SMaintainedNode*)std::malloc(sizeof(SMaintainedNode));
        m_pNode->m_pNext = m_pNode->m_pPrev = m_pNode;
    }
    void clear() { ++gClears; m_pNode->m_pNext = m_pNode->m_pPrev = m_pNode; }
    ~CMaintainedEventList()
    {
        clear();
        if (m_pNode) { ++gFrees; std::free(m_pNode); }
    }
};
class CJoystick {
public:
    virtual void v00() {}
    CInputEvent          EventStore[64];
    void*                InputManager;
    bool                 ExclusiveMode;
    CMaintainedEventList MaintainedEventsList;
    long                 NoStoredEvents;
    long                 NoEventScansRunning;
    CJoystick() : InputManager(0), ExclusiveMode(false), NoStoredEvents(0), NoEventScansRunning(0)
    {
        for (int i = 0; i < 64; ++i) EventStore[i].Index = i;
    }
    virtual ~CJoystick() {}
};

int main()
{
    CJoystick* j = new CJoystick;
    delete j;
    bool ok = gEventDtors == 64 && gFirstIndex == 63 && gLastIndex == 0 && gClears == 1 && gFrees == 1 && gOrderOk;
    if (!ok) { std::printf("009e4420_TEST FAIL\n"); return 1; }
    std::printf("009e4420_TEST PASS\n");
    return 0;
}