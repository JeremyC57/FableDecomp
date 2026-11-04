// Behaviour test for NParticleEngine::CPSCLight::Update (0x00B18820): with life seconds on, the first update starts the timer at
// -round(tickRate * startTime) (GFFloatToLongNear) and reports finished while the timer is negative; once running the light stays
// alive until the timer passes tickRate * lifeSecs, then (respawning) counts the respawn delay before restarting; with life seconds
// off the light is simply alive.
#include <stdio.h>
#include "engine/GFFloatToLongNear.h"
static long g_TickRate = 10;
struct Light {
    long LifeTimer, RespawnDelayTimer; bool IsAlive; float LifeSecs, RespawnDelaySecs, StartTime, TimelineSecs;
    bool UseLifeSecs, Respawns, UseTimelineSecs, StartedTimer;
    void Update(bool& finished)
    {
        if (UseLifeSecs) {
            if (!StartedTimer) { LifeTimer = -GFFloatToLongNear((float)g_TickRate * StartTime); StartedTimer = true; }
            float interval = 1.0f / g_TickRate;
            float now = (float)LifeTimer * interval + StartTime;
            if (LifeTimer < 0) {
                if (!UseTimelineSecs || now < TimelineSecs) ++LifeTimer;
                IsAlive = false; finished = true; return;
            }
            long tickRate = g_TickRate;
            if (LifeTimer != 0 && (LifeTimer > (long)((float)tickRate * LifeSecs) || !IsAlive)) {
                if (Respawns && (float)RespawnDelayTimer >= (float)tickRate * RespawnDelaySecs) { RespawnDelayTimer = 0; LifeTimer = 0; IsAlive = true; }
                else { ++RespawnDelayTimer; IsAlive = false; }
            } else if (!UseTimelineSecs || now < TimelineSecs) { ++LifeTimer; IsAlive = true; }
            finished |= IsAlive;
        } else
            IsAlive = true;
    }
};
int main()
{
    Light l = { 0, 0, false, 0.5f, 0.25f, 0.3f, 0.0f, true, true, false, false };
    bool f = false;
    l.Update(f);
    bool ok = l.LifeTimer == -2 && f && !l.IsAlive;            // started at -round(10 * 0.3) = -3, then counted up once
    l.Update(f); l.Update(f);                                   // -1, 0
    f = false; l.Update(f);                                     // timer 0: alive, counts to 1
    ok = ok && l.LifeTimer == 1 && l.IsAlive && f;
    for (int i = 0; i < 5; ++i) l.Update(f);                    // up to 6 > 5 life ticks
    f = false; l.Update(f);
    ok = ok && !l.IsAlive && l.RespawnDelayTimer == 1 && !f;
    l.Update(f); l.Update(f); f = false; l.Update(f);           // delay 3 >= 10 * 0.25: respawn
    ok = ok && l.IsAlive && l.LifeTimer == 0 && l.RespawnDelayTimer == 0;
    Light off = { 0, 0, false, 0, 0, 0, 0, false, false, false, false };
    off.Update(f);
    ok = ok && off.IsAlive;
    printf(ok ? "00b18820_TEST PASS\n" : "00b18820_TEST FAIL\n");
    return ok ? 0 : 1;
}