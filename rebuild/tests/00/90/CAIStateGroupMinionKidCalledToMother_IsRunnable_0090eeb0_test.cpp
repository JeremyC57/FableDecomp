// Behaviour test for 0x0090EEB0 CAIStateGroup_MinionKidCalledToMother::IsRunnable(): runnable while FindCallHomeEvent() finds a message.
#include <cstdio>
class CMessageEvent { public: int x; };
static const CMessageEvent* g_ret = 0;
class CAIStateGroup_MinionKidCalledToMother { public:
    virtual ~CAIStateGroup_MinionKidCalledToMother() { }
    virtual bool IsRunnable();
private:
    const CMessageEvent* FindCallHomeEvent() { return g_ret; }
};
bool CAIStateGroup_MinionKidCalledToMother::IsRunnable() { return FindCallHomeEvent() != 0; }
int main()
{
    CAIStateGroup_MinionKidCalledToMother k; CMessageEvent m;
    g_ret = 0; if (k.IsRunnable()) { std::printf("0090eeb0_TEST FAIL\n"); return 1; }
    g_ret = &m; if (!k.IsRunnable()) { std::printf("0090eeb0_TEST FAIL\n"); return 1; }
    std::printf("0090eeb0_TEST PASS_MARKER_OK\n"); return 0;
}
