// Behaviour test for CSoundEventManager::StartEvent 0x00C12940: defaults vs copied parameters, overrides applied, the
// non-positive and failed chance refusals, the existing-instance refusal, priority 0 promoted to 1 before the event is
// acquired, no event -> null, and the Initialise arguments (id from the caller or the manager's counter, levels with the
// random pitch spread, request and key) with the active test deciding the result.
#include <cstdio>
#include <cstring>
#include <cmath>

static int g_rand; static int rand_() { return g_rand; }
struct Params { float volume, pitch, pan, loopStart, loopEnd, freq; long priority, delay, category; unsigned short fadeIn, group;
                bool looped, streamed; };
struct Voice { bool overrideFreq; float freq; long loopCount, delay; float volume, pitch; long priority; long links[4];
               long flags; long owner, user; bool t0, t1, t2; };
struct Request { long eventId; long handle[4]; float loopStart, loopEnd; long flags, owner, user; };
struct Levels { float volume, voiceVolume, pitch, voicePitch; unsigned short fadeIn, group; };
struct Key { long flags, delay; };
struct Event { bool active; long id, delay, priority, sample, category, loopCount; long looped; Request req; Key key; Levels lv;
               bool t0, t1, t2, streamed; int inits; };
struct Manager {
    long nextId; int checkResult; long checkedPriority, acquiredFor; Event* free; int lockDepth;
    long CheckExisting(long, void*, long, long, long, long priority) { checkedPriority = priority; return checkResult; }
    Event* AcquireEvent(unsigned long priority) { acquiredFor = (long)priority; return free; }
    Event* StartEvent(long sample, const Params* source, long flags, void* driver, const Voice* voice, long* eventId)
    {
        ++lockDepth;
        Params p; Params def = { 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, false, false };
        p = source ? *source : def;
        if (voice->overrideFreq) p.freq = voice->freq;
        Event* result = 0;
        if (!(p.freq <= 0.0f)) {
            float chance = (float)rand_() * (1.0f / 32767.0f);
            if (!(p.freq < chance) && CheckExisting(sample, driver, p.category, voice->loopCount, p.looped, p.priority)) {
                if (p.priority == 0) p.priority = 1;
                Event* e = AcquireEvent(p.priority);
                if (e) {
                    Levels lv = { p.volume, voice->volume, 0, voice->pitch, p.fadeIn, p.group };
                    float r = (float)rand_() * 2.0f; float t = r * (1.0f / 32767.0f) - 1.0f; lv.pitch = p.pitch + t * p.pan;
                    Request rq = { voice->priority, { voice->links[0], voice->links[1], voice->links[2], voice->links[3] },
                                   p.loopStart, p.loopEnd, voice->flags, voice->owner, voice->user };
                    Key k = { flags, p.delay };
                    long id = eventId ? *eventId : nextId++;
                    e->id = id; e->delay = voice->delay; e->priority = p.priority; e->sample = sample; e->category = p.category;
                    e->loopCount = voice->loopCount; e->looped = p.looped; e->req = rq; e->key = k; e->lv = lv;
                    e->t0 = voice->t0; e->t1 = voice->t1; e->t2 = voice->t2; e->streamed = p.streamed; ++e->inits;
                    result = e->active ? e : 0;
                }
            }
        }
        --lockDepth;
        return result;
    }
};
int main()
{
    bool ok = true;
    Manager m; std::memset(&m, 0, sizeof m); m.nextId = 100; m.checkResult = 1;
    Event ev; std::memset(&ev, 0, sizeof ev); ev.active = true; m.free = &ev;
    Voice v; std::memset(&v, 0, sizeof v); v.volume = 0.5f; v.pitch = 2.0f; v.priority = 9; v.links[2] = 7; v.delay = 33; v.t1 = true;
    Params p = { 0.8f, 1.5f, 0.5f, 0.1f, 0.2f, 1.0f, 0, 4, 3, 11, 12, true, true };
    g_rand = 32767;                                     // chance 1.0 passes a scale of 1.0; spread +1
    Event* r = m.StartEvent(5, &p, 0x40, &m, &v, 0);
    ok = ok && r == &ev && ev.id == 100 && m.nextId == 101 && m.acquiredFor == 1 && ev.priority == 1 && ev.key.flags == 0x40
            && ev.key.delay == 4 && std::fabs(ev.lv.pitch - 2.0f) < 1e-6f && ev.lv.voiceVolume == 0.5f && ev.req.eventId == 9
            && ev.req.handle[2] == 7 && ev.delay == 33 && ev.t1 && !ev.t0 && ev.streamed && ev.looped == 1 && m.lockDepth == 0;
    long id = 77; g_rand = 0;                           // spread -1
    r = m.StartEvent(5, &p, 0, &m, &v, &id);
    ok = ok && r == &ev && ev.id == 77 && m.nextId == 101 && std::fabs(ev.lv.pitch - 1.0f) < 1e-6f && ev.inits == 2;
    v.overrideFreq = true; v.freq = 0.0f;               // non-positive chance: refused before the lookup
    m.checkedPriority = -5; r = m.StartEvent(5, &p, 0, &m, &v, 0);
    ok = ok && r == 0 && m.checkedPriority == -5 && ev.inits == 2;
    v.freq = 0.25f; g_rand = 16384;                     // roll 0.5 above 0.25: refused
    r = m.StartEvent(5, &p, 0, &m, &v, 0);
    ok = ok && r == 0 && ev.inits == 2;
    v.overrideFreq = false; m.checkResult = 0;          // the existing instance refuses
    r = m.StartEvent(5, &p, 0, &m, &v, 0);
    ok = ok && r == 0 && ev.inits == 2 && m.checkedPriority == 0;
    m.checkResult = 1; m.free = 0;                      // no free event
    r = m.StartEvent(5, 0, 0, &m, &v, 0);
    ok = ok && r == 0 && ev.inits == 2;
    m.free = &ev; ev.active = false;                    // initialised but not active
    r = m.StartEvent(5, &p, 0, &m, &v, 0);
    ok = ok && r == 0 && ev.inits == 3 && m.nextId == 102;
    if (ok) std::printf("00c12940_TEST PASS\n"); else std::printf("00c12940_TEST FAIL\n");
    return ok ? 0 : 1;
}
