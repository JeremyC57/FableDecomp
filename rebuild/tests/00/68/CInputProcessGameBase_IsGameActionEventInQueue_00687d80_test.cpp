#include <cstdio>
#include <cstring>
#include "engine/EGameAction.h"
enum EInputButtonEventType { };                                 // as the engine headers' portable forward form
static char g_seq[32] = ""; static long g_args[3]; static float* g_time = 0;
class CPlayer { public: long m_number; long GetJoystickDeviceNumber() const { std::strcat(g_seq, "N"); return m_number; } };
class CGamePlayerInterface { public: bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long device, float& time) const { std::strcat(g_seq, "I"); g_args[0] = action; g_args[1] = type; g_args[2] = device; g_time = &time; return device == 2; } };
struct CInputProcessGameBase { virtual void v00() { } unsigned char _pad04[0x10]; void* World; CPlayer* MyPlayer; void* DefinitionManager; void* Component; CGamePlayerInterface* PGamePlayerInterface;
    // NF37: the retail (Ego_r.pdb / FableWin.pdb) signature ?IsGameActionEventInQueue@CInputProcessGameBase@@IBE_NW4EGameAction@@W4EInputButtonEventType@@AAM@Z
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, float& time) const { return PGamePlayerInterface->IsGameActionEventInQueue(action, type, MyPlayer->GetJoystickDeviceNumber(), time); } };
int main()
{
    CPlayer player; player.m_number = 2; CGamePlayerInterface gpi; CInputProcessGameBase b; b.MyPlayer = &player; b.PGamePlayerInterface = &gpi; float t = 0.0f, u = 0.0f;
    if (!b.IsGameActionEventInQueue((EGameAction)7, (EInputButtonEventType)3, t) || std::strcmp(g_seq, "NI") || g_args[0] != 7 || g_args[1] != 3 || g_args[2] != 2 || g_time != &t) { std::printf("00687d80_TEST FAIL\n"); return 1; }
    player.m_number = 1; if (b.IsGameActionEventInQueue((EGameAction)7, (EInputButtonEventType)3, u) || g_time != &u) { std::printf("00687d80_TEST FAIL\n"); return 1; }
    std::printf("00687d80_TEST PASS\n");
    return 0;
}
