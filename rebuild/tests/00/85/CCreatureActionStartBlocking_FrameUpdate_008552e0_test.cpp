#include <cstdio>

class CCombatActionBase
{ public:
    virtual void FrameUpdate();     // 0x00859D80
    int IntelligentPointer;
    int field_8;
    int field_c;
    int field_10;
};

class CCreatureAction_StartBlocking : public CCombatActionBase
{ public:
    virtual void FrameUpdate();
};

void CCreatureAction_StartBlocking::FrameUpdate()
{
    field_10 = 2;
    CCombatActionBase::FrameUpdate();
}

static int g_called = 0;
static CCombatActionBase* g_self = 0;

void CCombatActionBase::FrameUpdate()
{
    g_called = 1;
    g_self = this;
}

int main()
{
    CCreatureAction_StartBlocking obj;
    obj.field_10 = 0;

    obj.FrameUpdate();

    if (obj.field_10 == 2 && g_called == 1 && g_self == (CCombatActionBase*)&obj)
    {
        printf("TESTPASSMARKER_008552e0\n");
        return 0;
    }

    printf("FAIL field_10=%d called=%d\n", obj.field_10, g_called);
    return 1;
}
