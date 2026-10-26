// Behaviour test for retail 0x004de122: the candidate source below is embedded verbatim (from src/004de122.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
#pragma optimize("s",on)
// CTrophyDef::Transfer(CPersistContext&) -- serialize each trophy-definition field in the retail order
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE122, 189 bytes; slot 18 of vtable 0x0123AC1C = CTrophyDef (rtti-vtables.tsv).
// Callees (CPersistContext): Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(bool&) 0x0043314A,
//   Transfer(CDefString&) 0x0045228F (dword write / CDefString read 0x0044FC00), Transfer(CCharString&) 0x00431143
//   (labelled CPersistContext::TransferString).
// Layout: PDB (Ego_r) names/types; for this def the retail offsets equal the PDB offsets (+0x28..+0x60).
//   Local model: no engine/ header exists for CTrophyDef or CPersistContext.
class CDefString { public: long TablePos; };
class CCharString { public: void* PStringData; };
class CPersistContext { public:
    void Transfer(float&);                              // 0x00431061
    void Transfer(long&);                               // 0x00431102
    void Transfer(bool&);                               // 0x0043314A
    void Transfer(CDefString&);                         // 0x0045228F
    void Transfer(CCharString&);                        // 0x00431143
};
class CTrophyDef { public:
    virtual void Transfer(CPersistContext&);            // vtable slot 18 in retail
    unsigned char _base[0x24];                          // +0x04..+0x27 def base
    float MinigameInitialTimerSecs;                     // +0x28
    float TimeBonusPerWitness;                          // +0x2c
    long  RenownGainedPerWitnessAhead;                  // +0x30
    bool  EvilTrophy;                                   // +0x34
    bool  FakeTrophy;                                   // +0x35
    long  EmoteIconForAvailableWitness;                 // +0x38
    CDefString ShowTrophyAnimName;                      // +0x3c
    float GameDebugTextScale;                           // +0x40
    float SecsDelayPerGainedWitnessSound;               // +0x44
    CCharString SoundStartGame;                         // +0x48
    CCharString SoundShowTrophy;                        // +0x4c
    CCharString SoundGainedWitness;                     // +0x50
    CCharString SoundOutOfTime;                         // +0x54
    CCharString SoundGotAllWitnesses;                   // +0x58
    CCharString SoundEndNewHighScore;                   // +0x5c
    CCharString SoundEndNoNewScore;                     // +0x60
};
void CTrophyDef::Transfer(CPersistContext& c)
{
    c.Transfer(MinigameInitialTimerSecs);
    c.Transfer(TimeBonusPerWitness);
    c.Transfer(RenownGainedPerWitnessAhead);
    c.Transfer(EvilTrophy);
    c.Transfer(FakeTrophy);
    c.Transfer(EmoteIconForAvailableWitness);
    c.Transfer(ShowTrophyAnimName);
    c.Transfer(GameDebugTextScale);
    c.Transfer(SecsDelayPerGainedWitnessSound);
    c.Transfer(SoundStartGame);
    c.Transfer(SoundShowTrophy);
    c.Transfer(SoundGainedWitness);
    c.Transfer(SoundOutOfTime);
    c.Transfer(SoundGotAllWitnesses);
    c.Transfer(SoundEndNewHighScore);
    c.Transfer(SoundEndNoNewScore);
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
static int g_n; static int g_off[32]; static char g_kind[32]; static unsigned char* g_base;
static void rec(void* p, char k) { g_off[g_n] = (int)((unsigned char*)p - g_base); g_kind[g_n++] = k; }
void CPersistContext::Transfer(float& v) { rec(&v, 'f'); }
void CPersistContext::Transfer(long& v) { rec(&v, 'l'); }
void CPersistContext::Transfer(bool& v) { rec(&v, 'b'); v = !v; }
void CPersistContext::Transfer(CDefString& v) { rec(&v, 'd'); v.TablePos = 42; }
void CPersistContext::Transfer(CCharString& v) { rec(&v, 's'); }
int main() {
    static const int eo[16] = {0x28,0x2c,0x30,0x34,0x35,0x38,0x3c,0x40,0x44,0x48,0x4c,0x50,0x54,0x58,0x5c,0x60};
    static const char ek[] = "fflbbldffsssssss";
    CTrophyDef d; memset((char*)&d + 4, 0, sizeof d - 4); CPersistContext c; g_base = (unsigned char*)&d;
    CTrophyDef* pd = &d; pd->Transfer(c);              /* virtual dispatch through the real vtable */
    int ok = sizeof(CTrophyDef) == 0x64 && g_n == 16;
    for (int i = 0; ok && i < 16; ++i) ok = g_off[i] == eo[i] && g_kind[i] == ek[i];
    ok = ok && d.EvilTrophy && d.FakeTrophy && d.ShowTrophyAnimName.TablePos == 42;
    printf(ok ? "004de122_TEST PASS\n" : "004de122_TEST FAIL\n"); return ok ? 0 : 1;
}