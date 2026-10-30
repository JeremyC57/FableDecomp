// Behaviour test for CSoundEvent::Update (0x00C21A50): the manager's change bits are merged into the event's; with none
// pending nothing is pushed; otherwise gain (bit 2), pitch (bit 1) and the 3D record or distance (bit 4, by the
// parameter mode) go to the player carrying the event, then a gain/3D change restarts a virtual event that has gain
// (ReturnToIdle + Play) or sends a waiting/physical event without gain to virtual playback (ReturnToIdle + PlayVirtual);
// the change bits end cleared.
#include <cstdio>
struct CSoundEventManager { int id; };
struct SEventRecord { int a, b, c; unsigned char flag; SEventRecord() : a(0), b(0), c(0), flag(0) {} };
static int g_log[16]; static int g_n = 0;
struct CSoundPlayer {
    float pitch, gain, distance; int records; CSoundEventManager* manager;
    CSoundPlayer() : pitch(0), gain(0), distance(0), records(0), manager(0) {}
    virtual void SetPitch(float p) { pitch = p; g_log[g_n++] = 1; }
    virtual void SetGain(CSoundEventManager* m, float g) { manager = m; gain = g; g_log[g_n++] = 2; }
    virtual void SetDistance(float d) { distance = d; g_log[g_n++] = 6; }
    virtual void SetRecord(SEventRecord* r) { records += r->a; g_log[g_n++] = 7; }
};
struct CSoundParams {
    long m_mode; float gain;
    float GetPitch(CSoundEventManager*) { return 1.5f; }
    float GetGain(CSoundEventManager*) { return gain; }
    float GetDistance() { return 30.0f; }
    SEventRecord GetRecord() const { SEventRecord r; r.a = 9; return r; }
};
struct CSoundEvent {
    CSoundParams m_params; CSoundPlayer m_player; CSoundPlayer* m_pActive;
    unsigned long m_dirty; unsigned short m_state; int m_tracked;
    void CheckState(long) {}
    CSoundPlayer* GetActivePlayer() { CheckState(-1); return m_pActive; }
    long IsPlayingVirtual() { CheckState(-1); return m_state == 5; }
    long IsWaitingOrPlayingPhysical() { return (m_state >= 2 && m_state <= 4) || m_state == 6; }
    long HasPositiveGain(CSoundEventManager* m) { return m_params.GetGain(m) > 0.0f; }
    void UpdateTrackedParameters(CSoundEventManager*) { ++m_tracked; }
    void ReturnToIdle(CSoundEventManager*) { g_log[g_n++] = 10; m_state = 1; }
    void Play(CSoundEventManager*) { g_log[g_n++] = 11; m_state = 6; }
    void PlayVirtual(CSoundEventManager*) { g_log[g_n++] = 12; m_state = 5; }
    void Update(CSoundEventManager* manager, unsigned long changes);
};
void CSoundEvent::Update(CSoundEventManager* manager, unsigned long changes)
{
    CheckState(-1);
    UpdateTrackedParameters(manager);
    if ((m_dirty |= changes) == 0)
        return;
    CSoundPlayer* player = GetActivePlayer();
    if (player) {
        if (m_dirty & 2)
            player->SetGain(manager, m_params.GetGain(manager));
        if (m_dirty & 1)
            player->SetPitch(m_params.GetPitch(manager));
        if (m_dirty & 4) {
            if (m_params.m_mode) {
                player->SetGain(manager, m_params.GetGain(manager));
                SEventRecord record = m_params.GetRecord();
                player->SetRecord(&record);
            } else {
                float distance = m_params.GetDistance();
                player->SetDistance(distance);
            }
        }
    }
    if (m_dirty & 6) {
        CheckState(-1);
        if (HasPositiveGain(manager)) {
            if (IsPlayingVirtual()) {
                ReturnToIdle(manager);
                Play(manager);
            }
        } else if (IsWaitingOrPlayingPhysical()) {
            ReturnToIdle(manager);
            PlayVirtual(manager);
        }
    }
    m_dirty = 0;
}
static int fail(const char* why) { std::printf("00c21a50_TEST FAIL: %s\n", why); return 1; }
int main()
{
    CSoundEventManager mgr; mgr.id = 1;
    CSoundEvent e; e.m_pActive = &e.m_player; e.m_dirty = 0; e.m_state = 5; e.m_tracked = 0; e.m_params.m_mode = 0; e.m_params.gain = 0.5f;
    // no changes: only the tracked parameters are polled
    g_n = 0; e.Update(&mgr, 0);
    if (g_n != 0 || e.m_tracked != 1) return fail("no changes");
    // pitch only (bit 1): no state change
    g_n = 0; e.Update(&mgr, 1);
    if (g_n != 1 || g_log[0] != 1 || e.m_player.pitch != 1.5f || e.m_dirty != 0 || e.m_state != 5) return fail("pitch only");
    // gain + 3D distance mode on a virtual event with gain: push both, restart through ReturnToIdle + Play
    g_n = 0; e.m_dirty = 4; e.Update(&mgr, 2);
    if (g_n != 4 || g_log[0] != 2 || g_log[1] != 6 || g_log[2] != 10 || g_log[3] != 11 || e.m_player.distance != 30.0f
        || e.m_player.manager != &mgr || e.m_state != 6 || e.m_dirty != 0) return fail("gain + distance, restart");
    // 3D record mode on a physical event without gain: gain + record pushed, then virtual playback
    e.m_params.m_mode = 1; e.m_params.gain = 0.0f;
    g_n = 0; e.Update(&mgr, 4);
    if (g_n != 4 || g_log[0] != 2 || g_log[1] != 7 || g_log[2] != 10 || g_log[3] != 12 || e.m_player.records != 9 || e.m_state != 5) return fail("record, go virtual");
    // no active player: nothing pushed, state logic still runs (virtual + no gain stays virtual)
    e.m_pActive = 0; g_n = 0; e.Update(&mgr, 6);
    if (g_n != 0 || e.m_state != 5 || e.m_dirty != 0) return fail("no player");
    std::printf("00c21a50_TEST PASS\n");
    return 0;
}