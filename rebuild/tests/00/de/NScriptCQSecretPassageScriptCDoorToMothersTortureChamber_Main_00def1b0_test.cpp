#include <cstdio>
class CScriptThing { public: long m_thingId; };
static int g_ticks = 0, g_opened = 0; static CScriptThing* g_door = 0; static int g_abortAt = -1, g_aborts = 0;
struct CQ_SecretPassageScript { unsigned char _pad[0x4b]; bool m_doorReleased; };
static CQ_SecretPassageScript* g_script;
struct CQuestCalls { virtual void Tick() { if (++g_ticks == 2) g_script->m_doorReleased = true; } virtual void OpenDoor(CScriptThing* d, bool o) { ++g_opened; g_door = d; } };
struct CDoor { void* __vftable; CQuestCalls* PQuest; CScriptThing Door; unsigned char _pad0c[8]; CQ_SecretPassageScript* ParentClass;
    bool IsAborted() { return ++g_aborts == g_abortAt; }
    void Main() { while (!ParentClass->m_doorReleased) { PQuest->Tick(); if (IsAborted()) return; } if (IsAborted()) return; PQuest->OpenDoor(&Door, true); } };
int main() { CQ_SecretPassageScript s; s.m_doorReleased = false; g_script = &s; CQuestCalls q; CDoor d; d.PQuest = &q; d.ParentClass = &s;
    d.Main(); if (g_ticks != 2 || g_opened != 1 || g_door != &d.Door) { std::printf("00def1b0_TEST FAIL\n"); return 1; }
    s.m_doorReleased = false; g_ticks = 0; g_abortAt = 1; g_aborts = 0; d.Main(); if (g_ticks != 1 || g_opened != 1) { std::printf("00def1b0_TEST FAIL\n"); return 1; } std::printf("00def1b0_TEST PASS\n"); return 0; }