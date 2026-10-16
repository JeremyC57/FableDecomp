// Behaviour test for SendPauseEvent (006dd760): event 9 for the current player carrying the normalised pause byte.
#include <cstdio>
struct Event { long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid, Replacement; };
static Event g_sent;
static void AddBool(Event& e, bool& value) { value = value ? true : false; e.Data[e.EndPos++] = value; }
static void SendPauseEvent(signed char player, bool pause)
{
    Event e; e.Type = 9; e.Player = -1; e.EndPos = 0; e.Valid = true; e.Replacement = false;
    e.Player = player;
    AddBool(e, pause);
    g_sent = e;
}
int main()
{
    SendPauseEvent(2, true);
    if (g_sent.Type != 9 || g_sent.Player != 2 || g_sent.EndPos != 1 || g_sent.Data[0] != 1 || !g_sent.Valid) { std::printf("006dd760_TEST FAIL\n"); return 1; }
    SendPauseEvent(0, false);
    if (g_sent.Data[0] != 0 || g_sent.Player != 0) { std::printf("006dd760_TEST FAIL\n"); return 1; }
    std::printf("006dd760_TEST PASS\n");
    return 0;
}
