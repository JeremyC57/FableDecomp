#include <cstdio>
#include <cstring>
struct CCharString { const char* m_pData; };
struct CPersistContext {
    long Mode;
    const char* m_lastName; CCharString* m_lastValue;
    void Transfer(const char* name, CCharString* value) { m_lastName = name; m_lastValue = value; }
};
struct CThingPhysical {
    bool m_baseOk;
    bool SerialiseGameState(CPersistContext* context) { (void)context; return m_baseOk; }
};
struct CThingPhysicalSwitch : CThingPhysical {
    bool          Triggered;
    CCharString   TriggerableObjectScriptName;
    bool SerialiseGameState(CPersistContext* context);
};
bool CThingPhysicalSwitch::SerialiseGameState(CPersistContext* context)
{
    if (CThingPhysical::SerialiseGameState(context))
    {
        if (context->Mode != 1 && context->Mode != 3)
            Triggered = false;
        context->Transfer("TriggerableObjectScriptName", &TriggerableObjectScriptName);
        return true;
    }
    return false;
}
int main()
{
    int bad = 0;
    CThingPhysicalSwitch s; s.m_baseOk = false; s.Triggered = true;
    CPersistContext c; c.Mode = 0; c.m_lastName = 0; c.m_lastValue = 0;
    if (s.SerialiseGameState(&c) || !s.Triggered || c.m_lastName) bad = 1;
    s.m_baseOk = true; c.Mode = 1;
    if (!s.SerialiseGameState(&c) || !s.Triggered || c.m_lastValue != &s.TriggerableObjectScriptName) bad = 2;
    if (std::strcmp(c.m_lastName, "TriggerableObjectScriptName") != 0) bad = 3;
    c.Mode = 3;
    if (!s.SerialiseGameState(&c) || !s.Triggered) bad = 4;
    c.Mode = 2;
    if (!s.SerialiseGameState(&c) || s.Triggered) bad = 5;
    if (bad) { std::printf("0083d940_TEST FAIL\n"); return 1; }
    std::printf("0083d940_TEST PASS\n");
    return 0;
}