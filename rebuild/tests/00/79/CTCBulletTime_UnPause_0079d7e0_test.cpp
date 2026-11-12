// Behaviour test for CTCBulletTime::UnPause (0x0079D7E0): a copy of the logic over a small model.
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
struct Emitter { bool dead; bool alwaysUpdate; };
struct Bypass { int thing; bool set; };
struct Def {
    std::string loopSound; bool hasEffects; bool knowsSlowtime;
    std::vector<float> alpha, sat, con, bri, r, g, b;
};
struct World {
    int playCount; std::string lastSound; bool lastLoop; int created; int fadeCalls;
    float fAlpha, fSat, fCon, fBri, fR, fG, fB, fA, fTime; int fPriority; std::vector<Bypass> fBypass;
    bool carrying; void* carryKey; int carryCalls;
};
static World w;
static Emitter emitters[4];
struct BulletTime {
    Def* PDef; long m_nLevel; Emitter* m_tpLoopingEffect; unsigned long LoopSound; void* ScreenFilterKey;
    bool Pausable, Paused; int parent;
    unsigned long PlaySoundW(const std::string& s, bool loop) { ++w.playCount; w.lastSound = s; w.lastLoop = loop; return 77; }
    void UnPause()
    {
        if (Pausable && Paused) {
            Paused = false;
            LoopSound = PlaySoundW(PDef->loopSound, true);
            if (!m_tpLoopingEffect || m_tpLoopingEffect->dead) {
                long effect = 0;
                if (PDef->hasEffects && (std::strcmp("SLOWTIME", "SLOWTIME") == 0 && PDef->knowsSlowtime) && (effect = 5) != 0) {
                    Emitter* e = &emitters[w.created++];
                    e->dead = false; e->alwaysUpdate = false;
                    m_tpLoopingEffect = e;
                    if (m_tpLoopingEffect && !m_tpLoopingEffect->dead)
                        m_tpLoopingEffect->alwaysUpdate = true;
                }
            }
            std::vector<Bypass> bypass;
            Bypass bp = { parent, true };
            bypass.push_back(bp);
            ++w.fadeCalls; w.fTime = 0.0f; w.fPriority = 2;
            w.fAlpha = PDef->alpha[m_nLevel]; w.fSat = PDef->sat[m_nLevel]; w.fCon = PDef->con[m_nLevel]; w.fBri = PDef->bri[m_nLevel];
            w.fR = PDef->r[m_nLevel]; w.fG = PDef->g[m_nLevel]; w.fB = PDef->b[m_nLevel]; w.fA = 1.0f; w.fBypass = bypass;
            ScreenFilterKey = (void*)0x1234;
            if (w.carrying) { ++w.carryCalls; w.carryKey = ScreenFilterKey; }
        }
    }
};
static bool near(float a, float b) { return a - b < 0.0001f && b - a < 0.0001f; }
int main()
{
    Def d; d.loopSound = "BT_LOOP"; d.hasEffects = true; d.knowsSlowtime = true;
    for (int i = 0; i < 3; ++i) {
        d.alpha.push_back(0.1f * (i + 1)); d.sat.push_back(0.2f * (i + 1)); d.con.push_back(0.3f * (i + 1));
        d.bri.push_back(0.4f * (i + 1)); d.r.push_back(0.5f + i); d.g.push_back(0.6f + i); d.b.push_back(0.7f + i);
    }
    BulletTime bt; bt.PDef = &d; bt.m_nLevel = 1; bt.m_tpLoopingEffect = 0; bt.LoopSound = 0; bt.ScreenFilterKey = 0;
    bt.Pausable = true; bt.Paused = false; bt.parent = 9;
    w.carrying = true;
    bt.UnPause();   // not paused: nothing
    if (w.playCount != 0 || w.fadeCalls != 0) { std::printf("0079d7e0_TEST FAIL\n"); return 1; }
    bt.Paused = true; bt.UnPause();
    if (bt.Paused || bt.LoopSound != 77 || w.lastSound != "BT_LOOP" || !w.lastLoop || w.created != 1 ||
        bt.m_tpLoopingEffect != &emitters[0] || !emitters[0].alwaysUpdate || w.fadeCalls != 1 || w.fPriority != 2 ||
        !near(w.fAlpha, 0.2f) || !near(w.fBri, 0.8f) || !near(w.fR, 1.5f) || !near(w.fB, 1.7f) || w.fA != 1.0f ||
        w.fBypass.size() != 1 || w.fBypass[0].thing != 9 || !w.fBypass[0].set || w.carryCalls != 1 || w.carryKey != bt.ScreenFilterKey) {
        std::printf("0079d7e0_TEST FAIL\n"); return 1;
    }
    bt.Paused = true; bt.UnPause();   // effect alive: not recreated
    if (w.created != 1 || w.fadeCalls != 2) { std::printf("0079d7e0_TEST FAIL\n"); return 1; }
    emitters[0].dead = true; bt.Paused = true; bt.UnPause();   // effect dead: recreated
    if (w.created != 2 || bt.m_tpLoopingEffect != &emitters[1]) { std::printf("0079d7e0_TEST FAIL\n"); return 1; }
    bt.Pausable = false; bt.Paused = true; bt.UnPause();   // not pausable: nothing
    if (!bt.Paused || w.fadeCalls != 3) { std::printf("0079d7e0_TEST FAIL\n"); return 1; }
    std::printf("0079d7e0_TEST PASS\n");
    return 0;
}