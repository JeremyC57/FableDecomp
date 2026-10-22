#include <cstdio>
// Behaviour model of CInputProcessYesNoQuestion::ProcessInput (0x0068C880).
struct Event { bool pressed; long device, key, action; };
struct Result { int handled; int sent; long answer, mode; };
static bool g_keyboard = false, g_canCancel = true;
static void Send(Result& r, long answer, long mode) { ++r.handled; ++r.sent; r.answer = answer; r.mode = mode; }
static void Process(const Event& e, Result& r, long previousMode)
{
    if (!e.pressed) return;
    if (g_keyboard) {
        if (e.device == 3 && e.key == 4) Send(r, 1, previousMode);
        else if (e.device == 3 && e.key == 10) Send(r, 0, previousMode);
    } else if (e.action == 0x16) Send(r, 1, previousMode);
    else if (e.action == 0x17) Send(r, 0, previousMode);
    else if (e.action == 0x18 && g_canCancel) Send(r, 2, previousMode);
}
int main()
{
    Result r = { 0, 0, -1, -1 };
    Event yes = { true, 0, 0, 0x16 }; Process(yes, r, 7);
    int ok = r.sent == 1 && r.answer == 1 && r.mode == 7;
    Event no = { true, 0, 0, 0x17 }; Process(no, r, 7); ok = ok && r.sent == 2 && r.answer == 0;
    Event cancel = { true, 0, 0, 0x18 }; Process(cancel, r, 7); ok = ok && r.sent == 3 && r.answer == 2;
    g_canCancel = false; Process(cancel, r, 7); ok = ok && r.sent == 3;
    Event released = { false, 0, 0, 0x16 }; Process(released, r, 7); ok = ok && r.sent == 3;
    g_keyboard = true;
    Event keyNo = { true, 3, 10, 0 }; Process(keyNo, r, 5); ok = ok && r.sent == 4 && r.answer == 0 && r.mode == 5;
    Event keyYes = { true, 3, 4, 0 }; Process(keyYes, r, 5); ok = ok && r.sent == 5 && r.answer == 1;
    Process(yes, r, 5); ok = ok && r.sent == 5;
    if (!ok) { std::printf("0068c880_TEST FAIL\n"); return 1; }
    std::printf("0068c880_TEST PASS\n");
    return 0;
}
