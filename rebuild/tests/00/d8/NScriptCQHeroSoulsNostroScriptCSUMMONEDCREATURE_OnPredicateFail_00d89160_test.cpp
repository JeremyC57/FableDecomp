// NScript::CQ_HeroSoulsNostroScript::CSUMMONED_CREATURE::OnPredicateFail() -- a summoned undead died: one fewer alive for the Nostro script, and if Nostro's vulnerability timer is below the def threshold it gets the def bonus time added
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D89160, length 107 bytes (+5 int3 padding).
// Which CSUMMONED_CREATURE: the PDB has two (CQ_ArenaScript's and CQ_HeroSoulsNostroScript's); the counter at
// ParentClass+0x50 and the CTimer at +0x54 are CQ_HeroSoulsNostroScript::UndeadAlive / NostroVulnerableTimer
// (PDB 100 / 104, retail 0x14 lower like the other Hero Souls scripts); the arena script has CCharString tags there.
// Callees: the global CGameScriptInterface @ 0x0143E8F8, slots 90 GetTimer(long) (+0x168, twice) and 89
// SetTimer(long, long) (+0x164). Data: the script def @ 0x0143E90C, threshold +0xC88 and bonus +0xC8C.
// Shape: CTimer's inline GetTime/SetTime accessors forward the TimerIndex to the global interface; the def global
// is read through an inline accessor GetScriptDef(), so VC evaluates it (edi) BEFORE each GetTimer call and reads
// the field after it (a plain `g_pScriptDef->Field` loads the pointer after the call); `this->ParentClass` is re-read
// for every access.
class CGameScriptInterface { public:                     // vtable 0x01260F0C (rtti-vtables.tsv slot names)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    virtual void SetTimer(long timer, long time) const;    // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;               // slot 90 (+0x168)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

class CScriptDef { public:
    unsigned char _pad[0xC88];
    long          NostroVulnerableTimerThreshold;      // +0xC88
    long          NostroVulnerableTimerBonus;          // +0xC8C
};
extern CScriptDef* g_pScriptDef;                            // 0x0143E90C
inline CScriptDef* GetScriptDef() { return g_pScriptDef; }

class CTimer { public:
    long TimerIndex;                                   // +0x00
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};

class CScriptThing { public:                                  // 12 bytes (vtable 0x01238C8C)
    void* __vftable;
    long  m_data[2];
};

namespace NScript {

class CQ_SunnyvaleMasterData;

class CQ_HeroSoulsNostroScript { public:
    unsigned char _pad[0x50];
    long          UndeadAlive;                         // +0x50
    CTimer        NostroVulnerableTimer;               // +0x54

    class CSUMMONED_CREATURE {
    public:
        void*                     __vftable;           // +0x00
        CGameScriptInterface*     Game;                // +0x04
        CScriptThing              Me;                  // +0x08
        CQ_HeroSoulsNostroScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData*   PMasterData;         // +0x18
        void OnPredicateFail();
    };
};

void CQ_HeroSoulsNostroScript::CSUMMONED_CREATURE::OnPredicateFail()
{
    --ParentClass->UndeadAlive;
    if (ParentClass->NostroVulnerableTimer.GetTime() < GetScriptDef()->NostroVulnerableTimerThreshold)
        ParentClass->NostroVulnerableTimer.SetTime(ParentClass->NostroVulnerableTimer.GetTime() + GetScriptDef()->NostroVulnerableTimerBonus);
}

} // namespace NScript

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define VDEF(n) void CGameScriptInterface::v##n() { }
VDEF(00) VDEF(01) VDEF(02) VDEF(03) VDEF(04) VDEF(05) VDEF(06) VDEF(07) VDEF(08) VDEF(09)
VDEF(10) VDEF(11) VDEF(12) VDEF(13) VDEF(14) VDEF(15) VDEF(16) VDEF(17) VDEF(18) VDEF(19)
VDEF(20) VDEF(21) VDEF(22) VDEF(23) VDEF(24) VDEF(25) VDEF(26) VDEF(27) VDEF(28) VDEF(29)
VDEF(30) VDEF(31) VDEF(32) VDEF(33) VDEF(34) VDEF(35) VDEF(36) VDEF(37) VDEF(38) VDEF(39)
VDEF(40) VDEF(41) VDEF(42) VDEF(43) VDEF(44) VDEF(45) VDEF(46) VDEF(47) VDEF(48) VDEF(49)
VDEF(50) VDEF(51) VDEF(52) VDEF(53) VDEF(54) VDEF(55) VDEF(56) VDEF(57) VDEF(58) VDEF(59)
VDEF(60) VDEF(61) VDEF(62) VDEF(63) VDEF(64) VDEF(65) VDEF(66) VDEF(67) VDEF(68) VDEF(69)
VDEF(70) VDEF(71) VDEF(72) VDEF(73) VDEF(74) VDEF(75) VDEF(76) VDEF(77) VDEF(78) VDEF(79)
VDEF(80) VDEF(81) VDEF(82) VDEF(83) VDEF(84) VDEF(85) VDEF(86) VDEF(87) VDEF(88)
static long g_timers[8]; static int g_sets = 0, g_gets = 0;
void CGameScriptInterface::SetTimer(long timer, long time) const { ++g_sets; g_timers[timer] = time; }
long CGameScriptInterface::GetTimer(long timer) const { ++g_gets; return g_timers[timer]; }
static CGameScriptInterface g_gsi;
static CScriptDef g_def;
CGameScriptInterface* g_pGameScriptInterface = &g_gsi;
CScriptDef* g_pScriptDef = &g_def;

int main()
{
    static NScript::CQ_HeroSoulsNostroScript nostro;
    static NScript::CQ_HeroSoulsNostroScript::CSUMMONED_CREATURE c;
    c.ParentClass = &nostro;
    nostro.UndeadAlive = 3; nostro.NostroVulnerableTimer.TimerIndex = 2;
    g_def.NostroVulnerableTimerThreshold = 100; g_def.NostroVulnerableTimerBonus = 30;
    g_timers[2] = 50;
    c.OnPredicateFail();
    CHECK(nostro.UndeadAlive == 2 && g_timers[2] == 80 && g_sets == 1 && g_gets == 2);
    c.OnPredicateFail();
    CHECK(nostro.UndeadAlive == 1 && g_timers[2] == 110 && g_sets == 2);
    c.OnPredicateFail();                        // 110 >= 100: no bonus
    CHECK(nostro.UndeadAlive == 0 && g_timers[2] == 110 && g_sets == 2 && g_gets == 5);
    g_timers[2] = 99;
    c.OnPredicateFail();
    CHECK(nostro.UndeadAlive == -1 && g_timers[2] == 129 && g_sets == 3);
    if (fails == 0) printf("00d89160_TEST PASS\n");
    return fails;
}