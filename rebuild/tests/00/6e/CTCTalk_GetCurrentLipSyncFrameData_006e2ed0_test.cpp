// Behaviour test for 0x006E2ED0 CTCTalk::GetCurrentLipSyncFrameData: the row's body over stubs (visibility, bank lookup, the emitter
// path with its active-sample list and the mime-window path), checking the result and the time handed to GetFrameInterpolate.
#include <cstdio>
#include <cstdlib>
#include <vector>
static void check(bool b) { if (!b) { std::puts("006e2ed0_TEST FAIL"); std::exit(1); } }
static bool near(float a, float b) { float d = a - b; return d < 1e-3f && d > -1e-3f; }
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetSquaredMagnitude() const { return X * X + Z * Z + Y * Y; } };
class CLipSyncFrameData { public: float t; int calls; };
class CLipSyncSampleData { public: void GetFrameInterpolate(CLipSyncFrameData& d, float time) const { d.t = time; ++d.calls; } };
static CLipSyncSampleData g_sample;
class CLipSyncDataBank { public: bool has; const void* PeekLipSyncInfo(unsigned long) const { return has ? this : 0; }
    const CLipSyncSampleData* GetLipSyncData(unsigned long) const { return &g_sample; } };
class CASoundBank { public: };
struct CActiveSoundInfo { long a; };
struct CASoundEmitter { bool playing; int active;
    bool IsSamplePlaying(const CASoundBank&, long) const { return playing; }
    void GetActiveSampleInfo(const CASoundBank&, long, std::vector<CActiveSoundInfo>& v) const { for (int i = 0; i < active; ++i) { CActiveSoundInfo x = { i }; v.push_back(x); } } };
class CThingSoundEmitter { public: CASoundEmitter* PSoundEmitter; };
class CTCSoundPlayer { public: CThingSoundEmitter* attached; };
struct CGraphic { bool rendered; bool WasRenderedLastFrame() const { return rendered; } };
class CThing { public: C3DVector pos; CGraphic* graphic; CTCSoundPlayer* player;
    CThing() : pos(0, 0, 0), graphic(0), player(0) { }
    const C3DVector& GetPos() const { return pos; } const CGraphic* GetTCGraphicAppearance() const { return graphic; }
    void GetTC(CTCSoundPlayer** p) { if (player) *p = player; } };
static bool g_movie; static C3DVector g_lod(0, 0, 0); static double g_time; static long g_fps = 15; static CLipSyncDataBank* g_bank;
class CTCTalk { public:
    CThing* parent; long SpeechSample, SampleDurationFrames; float MimeStartTime, SampleLoadedTime; CASoundBank bank;
    CThing& GetParentThing() { return *parent; }
    const CASoundBank& GetSpeechBank() const { return bank; }
    const CLipSyncDataBank* GetPLipSyncBank() const { return g_bank; }
    bool GetCurrentLipSyncFrameData(CLipSyncFrameData& frame_data);
};
bool CTCTalk::GetCurrentLipSyncFrameData(CLipSyncFrameData& frame_data)
{
    if (!g_movie
        && ((GetParentThing().GetPos() - g_lod).GetSquaredMagnitude() > 36.0f
            || !GetParentThing().GetTCGraphicAppearance() || !GetParentThing().GetTCGraphicAppearance()->WasRenderedLastFrame()))
        return false;
    const CLipSyncDataBank* lip_sync_bank = GetPLipSyncBank();
    if (!lip_sync_bank)
        return false;
    float now = (float)g_time;
    CTCSoundPlayer* sound_player = 0;
    GetParentThing().GetTC(&sound_player);
    if (SpeechSample > 0) {
        if (sound_player->attached) {
            CASoundEmitter* emitter = sound_player->attached->PSoundEmitter;
            if (emitter && emitter->IsSamplePlaying(GetSpeechBank(), SpeechSample)
                && GetPLipSyncBank()->PeekLipSyncInfo(SpeechSample)) {
                std::vector<CActiveSoundInfo> active_samples;
                emitter->GetActiveSampleInfo(GetSpeechBank(), SpeechSample, active_samples);
                if (active_samples.empty())
                    return false;
                float time = (float)g_time - SampleLoadedTime;
                const CLipSyncSampleData* sample_data = GetPLipSyncBank()->GetLipSyncData(SpeechSample);
                sample_data->GetFrameInterpolate(frame_data, time);
                return true;
            }
        } else if (MimeStartTime > 0.0f
                   && (float)SampleDurationFrames / g_fps + MimeStartTime > now
                   && lip_sync_bank->PeekLipSyncInfo(SpeechSample)) {
            float time = now - MimeStartTime;
            const CLipSyncSampleData* sample_data = GetPLipSyncBank()->GetLipSyncData(SpeechSample);
            sample_data->GetFrameInterpolate(frame_data, time);
            return true;
        }
    }
    return false;
}
int main()
{
    CLipSyncDataBank bank = { true }; g_bank = &bank;
    CASoundEmitter sound = { true, 2 }; CThingSoundEmitter emitter = { &sound }; CTCSoundPlayer player = { &emitter };
    CGraphic graphic = { true }; CThing thing; thing.pos = C3DVector(3, 4, 0); thing.graphic = &graphic; thing.player = &player;
    CTCTalk talk; talk.parent = &thing; talk.SpeechSample = 5; talk.SampleDurationFrames = 30; talk.MimeStartTime = 0.0f; talk.SampleLoadedTime = 10.0f;
    CLipSyncFrameData d = { 0.0f, 0 };
    g_time = 12.5;
    check(talk.GetCurrentLipSyncFrameData(d) && d.calls == 1 && near(d.t, 2.5f));          // emitter path: time since loaded
    thing.pos = C3DVector(5, 4, 0);                                                       // 41 > 36: too far
    check(!talk.GetCurrentLipSyncFrameData(d));
    g_movie = true; check(talk.GetCurrentLipSyncFrameData(d) && d.calls == 2);            // cut scene ignores distance
    g_movie = false; thing.pos = C3DVector(0, 6, 0); graphic.rendered = false;            // 36 is close enough, but not rendered
    check(!talk.GetCurrentLipSyncFrameData(d));
    graphic.rendered = true; sound.active = 0; check(!talk.GetCurrentLipSyncFrameData(d) && d.calls == 2);   // nothing active
    sound.active = 1; sound.playing = false; check(!talk.GetCurrentLipSyncFrameData(d));
    sound.playing = true; bank.has = false; check(!talk.GetCurrentLipSyncFrameData(d));
    bank.has = true; g_bank = 0; check(!talk.GetCurrentLipSyncFrameData(d)); g_bank = &bank;
    talk.SpeechSample = 0; check(!talk.GetCurrentLipSyncFrameData(d)); talk.SpeechSample = 5;
    // mime path: no attached emitter; window = 30 / 15 = 2 s from MimeStartTime
    player.attached = 0; talk.MimeStartTime = 11.0f; g_time = 12.25;
    check(talk.GetCurrentLipSyncFrameData(d) && d.calls == 3 && near(d.t, 1.25f));
    g_time = 13.5; check(!talk.GetCurrentLipSyncFrameData(d));                            // window over
    talk.MimeStartTime = 0.0f; g_time = 12.0; check(!talk.GetCurrentLipSyncFrameData(d)); // no mime
    std::puts("006e2ed0_TEST PASS");
    return 0;
}