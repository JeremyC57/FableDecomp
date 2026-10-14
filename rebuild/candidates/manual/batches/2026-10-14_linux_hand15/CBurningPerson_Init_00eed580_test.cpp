#include <stdio.h>
#define SCRIPT_SLOT(n) virtual void slot##n() {}
#define SCRIPT_SLOTS10(d) SCRIPT_SLOT(d##0) SCRIPT_SLOT(d##1) SCRIPT_SLOT(d##2) SCRIPT_SLOT(d##3) SCRIPT_SLOT(d##4) SCRIPT_SLOT(d##5) SCRIPT_SLOT(d##6) SCRIPT_SLOT(d##7) SCRIPT_SLOT(d##8) SCRIPT_SLOT(d##9)
struct CScriptThing { long m_id; long m_uid; long m_type; };
static const CScriptThing* g_thing; static long g_on; static int g_calls;
struct CGameInterface {
    SCRIPT_SLOTS10(0) SCRIPT_SLOTS10(1) SCRIPT_SLOTS10(2) SCRIPT_SLOTS10(3) SCRIPT_SLOTS10(4) SCRIPT_SLOTS10(5) SCRIPT_SLOTS10(6) SCRIPT_SLOTS10(7) SCRIPT_SLOTS10(8) SCRIPT_SLOTS10(9)
    SCRIPT_SLOTS10(10) SCRIPT_SLOTS10(11) SCRIPT_SLOTS10(12) SCRIPT_SLOTS10(13) SCRIPT_SLOTS10(14) SCRIPT_SLOTS10(15) SCRIPT_SLOTS10(16) SCRIPT_SLOTS10(17) SCRIPT_SLOTS10(18) SCRIPT_SLOTS10(19)
    SCRIPT_SLOTS10(20) SCRIPT_SLOTS10(21) SCRIPT_SLOTS10(22) SCRIPT_SLOTS10(23) SCRIPT_SLOTS10(24) SCRIPT_SLOTS10(25) SCRIPT_SLOTS10(26) SCRIPT_SLOTS10(27) SCRIPT_SLOTS10(28) SCRIPT_SLOTS10(29)
    SCRIPT_SLOTS10(30) SCRIPT_SLOTS10(31) SCRIPT_SLOTS10(32) SCRIPT_SLOTS10(33) SCRIPT_SLOTS10(34) SCRIPT_SLOTS10(35) SCRIPT_SLOTS10(36) SCRIPT_SLOTS10(37) SCRIPT_SLOTS10(38) SCRIPT_SLOTS10(39)
    SCRIPT_SLOTS10(40) SCRIPT_SLOTS10(41) SCRIPT_SLOTS10(42) SCRIPT_SLOTS10(43) SCRIPT_SLOTS10(44) SCRIPT_SLOTS10(45) SCRIPT_SLOTS10(46)
    SCRIPT_SLOT(470) SCRIPT_SLOT(471) SCRIPT_SLOT(472) SCRIPT_SLOT(473) SCRIPT_SLOT(474) SCRIPT_SLOT(475) SCRIPT_SLOT(476)
    virtual void SetThingOnFire(const CScriptThing& thing, long on) { g_thing = &thing; g_on = on; ++g_calls; }
};
struct CBurningPerson { void* _vptr; CGameInterface* m_pGame; CScriptThing m_me; unsigned char _pad[8]; long SpeakCounter; void Init(); };
void CBurningPerson::Init() { m_pGame->SetThingOnFire(m_me, 1); SpeakCounter = 0; }
int main()
{
    CGameInterface game; CBurningPerson p; p.m_pGame = &game; p.SpeakCounter = 9; p.m_me.m_id = 1;
    p.Init();
    bool ok = g_calls == 1 && g_thing == &p.m_me && g_on == 1 && p.SpeakCounter == 0 && sizeof(CBurningPerson) == 32;
    if (!ok) { printf("00eed580_TEST FAIL\n"); return 1; }
    printf("00eed580_TEST PASS\n"); return 0;
}
