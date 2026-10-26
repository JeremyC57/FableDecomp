#include <cstdio>
#include <cstring>

static int g_strings, g_events, g_lastType;
static unsigned char g_lastData[32]; static int g_lastLen;
class CInputEvent { public: int button; int action; float pressure; };
class CCharString { public:
    const char* p;
    CCharString(const char* text, long = -1) : p(text) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
class CGameEvent { public:
    long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void Copy(unsigned long length, const void* data) { std::memcpy(Data + EndPos, data, length); EndPos = (unsigned char)(EndPos + length); }
    void AddUWORD(long value) { Copy(2, &value); }
    void AddFloat(float value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};
class CProcessedInput { public:
    void AddGameEvent(const CGameEvent& e) { ++g_events; g_lastType = e.Type; g_lastLen = e.EndPos; std::memcpy(g_lastData, e.Data, 32);
        if (e.Player != -1 || !e.Valid || e.Replacement) g_lastType = -99; }
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) { return std::strcmp(name.p, "CREATURE_ABILITY") == 0 ? 0x1234 : -1; }
};
class CInputProcessCombat { public:
    CDefinitionManager* DefinitionManager;
    bool IsEventButtonPressed(const CInputEvent& e) { return e.button == 0x13; }
    bool IsEventGameAction(long action, const CInputEvent& e) { return e.action == action; }
    float GetButtonPressureForEvent(const CInputEvent& e) { return e.pressure; }
    void ProcessUnarmedInput(const CInputEvent& event, CProcessedInput& processed)
    {
        if (!IsEventButtonPressed(event))
            return;
        if (IsEventGameAction(0xb, event)) {
            CGameEvent gameEvent(0x16);
            gameEvent.AddUWORD(DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY"));
            gameEvent.AddFloat(GetButtonPressureForEvent(event));
            processed.AddGameEvent(gameEvent);
        } else if (IsEventGameAction(0xd, event)) {
            CGameEvent gameEvent(0x21);
            gameEvent.AddBool(true);
            processed.AddGameEvent(gameEvent);
        } else if (IsEventGameAction(0xe, event)) {
            CGameEvent gameEvent(0x22);
            gameEvent.AddBool(true);
            processed.AddGameEvent(gameEvent);
        }
    }
};
static int Fail(int n) { std::printf("0068e8f0_TEST FAIL %d\n", n); return 1; }
int main()
{
    CDefinitionManager defs; CInputProcessCombat c; c.DefinitionManager = &defs; CProcessedInput out;
    CInputEvent e = { 0x13, 0xb, 0.75f };
    c.ProcessUnarmedInput(e, out);
    float f; std::memcpy(&f, g_lastData + 2, 4);
    if (g_events != 1 || g_lastType != 0x16 || g_lastLen != 6 || g_lastData[0] != 0x34 || g_lastData[1] != 0x12 || f != 0.75f || g_strings != 0) return Fail(1);
    e.action = 0xd; c.ProcessUnarmedInput(e, out);
    if (g_events != 2 || g_lastType != 0x21 || g_lastLen != 1 || g_lastData[0] != 1) return Fail(2);
    e.action = 0xe; c.ProcessUnarmedInput(e, out);
    if (g_events != 3 || g_lastType != 0x22 || g_lastLen != 1 || g_lastData[0] != 1) return Fail(3);
    e.action = 0xc; c.ProcessUnarmedInput(e, out);          // other actions: nothing queued
    if (g_events != 3) return Fail(4);
    e.action = 0xb; e.button = 0x15; c.ProcessUnarmedInput(e, out);   // not a press: nothing queued
    if (g_events != 3) return Fail(5);
    std::printf("0068e8f0_TEST PASS\n");
    return 0;
}