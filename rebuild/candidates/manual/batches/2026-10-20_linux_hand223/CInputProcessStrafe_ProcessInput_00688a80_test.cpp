#include <cstdio>
#include <cstring>

static int g_events, g_lastType, g_lastLen; static unsigned char g_lastData[32];
struct CInputEvent {
    int device, key, released, action; float x, y;
    long GetDeviceType() const { return device; }
    long GetKey() const { return key; }
};
struct C2DVector { float X, Y; };
struct CGameEvent {
    long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void Copy(unsigned long length, const void* data) { std::memcpy(Data + EndPos, data, length); EndPos = (unsigned char)(EndPos + length); }
    void AddUID(unsigned long long value) { Copy(sizeof(value), &value); }
    void AddFloat(float value) { Copy(sizeof(value), &value); }
    void AddLong(long value) { int v = (int)value; Copy(4, &v); }
};
struct CProcessedInput {
    void AddGameEvent(const CGameEvent& e) { ++g_events; g_lastType = e.Type; g_lastLen = e.EndPos; std::memcpy(g_lastData, e.Data, 32); }
};
struct CThing { unsigned long long UniqueID; bool Dead; };
struct CPlayer { CThing* creature; CThing* GetControlledCreature() const { return creature; } };
struct CInputProcessStrafe {
    CPlayer* MyPlayer;
    bool IsEventButtonReleased(const CInputEvent& e) { return e.released != 0; }
    bool GetEventGameActionVector(long action, const CInputEvent& e, C2DVector* v)
    {
        if (e.action != action) return false;
        v->X = e.x; v->Y = e.y; return true;
    }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        CThing* creature = MyPlayer->GetControlledCreature();
        C2DVector strafe;
        if (!GetEventGameActionVector(0x3c, event, &strafe))
            return;
        if (event.GetDeviceType() == 2 && !IsEventButtonReleased(event))
            return;
        CGameEvent gameEvent(0x2b);
        if (creature == 0 || creature->Dead)
            return;
        long key = -1;
        if (event.GetDeviceType() == 2)
            key = event.GetKey();
        gameEvent.AddUID(creature->UniqueID);
        gameEvent.AddFloat(strafe.X);
        gameEvent.AddFloat(strafe.Y);
        gameEvent.AddLong(key);
        processed.AddGameEvent(gameEvent);
    }
};
static int Check(unsigned long long uid, float x, float y, int key)
{
    unsigned long long u; float fx, fy; int k;
    std::memcpy(&u, g_lastData, 8); std::memcpy(&fx, g_lastData + 8, 4); std::memcpy(&fy, g_lastData + 12, 4); std::memcpy(&k, g_lastData + 16, 4);
    return g_lastType == 0x2b && g_lastLen == 20 && u == uid && fx == x && fy == y && k == key;
}
static int Fail(int n) { std::printf("00688a80_TEST FAIL %d\n", n); return 1; }
int main()
{
    CThing hero = { 0x1122334455667788ULL, false }; CPlayer player = { &hero };
    CInputProcessStrafe s; s.MyPlayer = &player; CProcessedInput out;
    CInputEvent e = { 3, 77, 0, 0x3c, 0.5f, -1.0f };
    s.ProcessInput(e, out);                                   // joystick: key -1
    if (g_events != 1 || !Check(0x1122334455667788ULL, 0.5f, -1.0f, -1)) return Fail(1);
    e.device = 2; s.ProcessInput(e, out);                     // keyboard, not released: nothing
    if (g_events != 1) return Fail(2);
    e.released = 1; s.ProcessInput(e, out);                   // keyboard release: the key
    if (g_events != 2 || !Check(0x1122334455667788ULL, 0.5f, -1.0f, 77)) return Fail(3);
    e.action = 0x3d; s.ProcessInput(e, out);                  // other action: nothing
    hero.Dead = true; e.action = 0x3c; s.ProcessInput(e, out); // dead creature: nothing
    player.creature = 0; hero.Dead = false; s.ProcessInput(e, out);   // no creature: nothing
    if (g_events != 2) return Fail(4);
    std::printf("00688a80_TEST PASS\n");
    return 0;
}
