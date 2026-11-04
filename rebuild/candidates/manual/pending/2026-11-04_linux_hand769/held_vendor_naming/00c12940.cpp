// CSoundEventManager::StartEvent(long sample, const CSoundPlayParameters* source, long flags, CDriver* driver, const CPKVoiceParams* voice, long* eventId) -- under the library lock: take the driver's play parameters (or the defaults), apply the voice's overrides, roll the play chance (frequency scale against rand()/32767; none when it is not positive), let the manager resolve an existing instance of the sample (0x00C11230), default the priority to 1, acquire a free event (stealing a lower-priority one), and initialise it with the levels (volume, pitch + pan-scaled random spread, fade-in, group, the voice's volume and pitch), the playback request built from the voice and the (flags, delay) key under the caller's id or the next one; returns the event when it went active, else null
// Leaf: ?StartEvent@CSoundEventManager@@QAEPAUCSoundEvent@@JPBUCSoundPlayParameters@@JPAUCDriver@@PBUCPKVoiceParams@@PAJ@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C12940, 912 bytes, thiscall `ret 18h`, EH frame (state 0 = the scoped lock), `edi` pushed late.
// Identity: libpk audio (no PDB / RTTI): descriptive names. The only caller is the landed 0x00C12CD0 (it forwards (driver +4,
//   &driver +8, flags, driver, voice, id) under the same lock; landed as CTCHeroExperience_00c12cd0::Lookup, a propagated label),
//   in the CSoundEventManager method range 0x00C10C90..0x00C13470 (vtable 0x012B2748). The manifest label
//   "CTCHero_HandleCombatActions" is propagated and wrong. `voice` is the landed 0x50-byte CPKVoiceParams (its first 0x1C bytes
//   are the CSoundPlayParameterOverride whose ApplyTo is 0x00C20580); the result is the CSoundEvent the event set hands out.
// Callees: PKCProtectionInstance 0x00C0FFC0 / 0x00C0FFE0 over g_lock_0143bb14; CSoundPlayParameters() 0x00C1BD10, Initialise
//   0x00C1BC90, the getters 0x00C1B9C0..0x00C1BB00 (hand595's spellings; GetStreamed 0x00C1BAE0 = bit 1 of +2, SetStreamed's
//   mirror) and SetPriority 0x00C1BBC0; CSoundPlayParameterOverride::ApplyTo 0x00C20580; CSoundEventManager::CheckExisting
//   0x00C11230 (finds a live event of the same sample/driver/category/loop through the +0x1F4 map: stops it when inaudible,
//   restarts it when looped or loop-counted and the priority allows; 0 refuses the play) and AcquireEvent 0x00C11F00 (landed
//   as a void OnKey: it returns the event set's free slot, after UpdateItems and stealing below `priority`; NAME_FIXES);
//   rand through the msvcrt import 0x01440124; SPKPlaybackRequest() 0x00C108A0; CPKVoiceParams track bits 0x00C27B70 /
//   0x00C27B80 / 0x00C27B90 (bits 0..2 of +0x44, which CSoundEvent::Initialise turns into its +0xAE track flags);
//   CSoundEvent::Initialise 0x00C21C20 (16 arguments) and IsActive 0x00C20840.
// Shape: the chance tests are written as retail branches them (`!(x <= 0)`: `test ah,41h ; jnp`; `!(x < chance)`:
//   `test ah,5 ; jnp`), nested so the three refusals share the closing `return 0` block; the bool getters feed `long`
//   parameters (`movzx`); the random spread is `rand() * 2` in its own float, then `* 1/32767 - 1` in a second local,
//   then `* GetPan()` (one expression folds 2 * 1/32767 into one constant and calls GetPan first).
extern "C" __declspec(dllimport) int __cdecl rand(void);
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;                  // 0x0143BB14

class PKCProtectionInstance { public:
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);     // 0x00C0FFC0
    ~PKCProtectionInstance();                               // 0x00C0FFE0
};

struct CSoundPlayParameters {                               // 0x14 bytes
    CSoundPlayParameters();                                 // 0x00C1BD10
    void Initialise();                                      // 0x00C1BC90
    float GetVolume() const;                                // 0x00C1B9C0
    float GetPitch() const;                                 // 0x00C1B9E0
    float GetPan() const;                                   // 0x00C1BA00
    float GetLoopStart() const;                             // 0x00C1BA60
    float GetLoopEnd() const;                               // 0x00C1BA70
    long GetPriority() const;                               // 0x00C1BA80
    long GetDelay() const;                                  // 0x00C1BA90
    unsigned short GetFadeIn() const;                       // 0x00C1BAA0
    unsigned short GetGroup() const;                        // 0x00C1BAB0
    long GetCategory() const;                               // 0x00C1BAC0
    bool GetLooped() const;                                 // 0x00C1BAD0
    bool GetStreamed() const;                               // 0x00C1BAE0
    float GetFrequencyScale() const;                        // 0x00C1BB00
    void SetPriority(long v);                               // 0x00C1BBC0
    unsigned long m_data[5];
};

struct CSoundPlayParameterOverride {                        // 0x1c bytes
    unsigned long        m_overrides;
    unsigned short       m_fadeInMask;
    unsigned short       m_groupMask;
    CSoundPlayParameters m_params;
    void ApplyTo(CSoundPlayParameters& params) const;      // 0x00C20580
};

struct SPKHandle {                                          // 0x10 bytes
    void*         m_pObject;
    unsigned long m_id;
    unsigned long m_extra;
    bool          m_valid;
};

struct CPKVoiceParams {                                     // 0x50 bytes
    CSoundPlayParameterOverride m_override;                 // +0x00
    long      m_loopCount;                                  // +0x1c
    long      m_delay;                                      // +0x20
    float     m_volume;                                     // +0x24
    float     m_pitch;                                      // +0x28
    long      m_priority;                                   // +0x2c
    SPKHandle m_links;                                      // +0x30
    long      m_flags;                                      // +0x40
    short     m_group;                                      // +0x44
    long      m_owner;                                      // +0x48
    long      m_user;                                       // +0x4c
    bool GetTrackBit0() const;                              // 0x00C27B70
    bool GetTrackBit1() const;                              // 0x00C27B80
    bool GetTrackBit2() const;                              // 0x00C27B90
};

struct SPKPlaybackRequest {                                 // 0x28 bytes
    unsigned long m_eventId;                                // +0x00
    SPKHandle     m_handle;                                 // +0x04
    float         m_volume;                                 // +0x14
    float         m_pitch;                                  // +0x18
    unsigned long m_flags;                                  // +0x1c
    unsigned long m_userData;                               // +0x20
    unsigned long m_callback;                               // +0x24
    SPKPlaybackRequest();                                   // 0x00C108A0
};

struct SVoiceLevels {                                       // 0x14 bytes
    float          m_volume;
    float          m_voiceVolume;
    float          m_pitch;
    float          m_voicePitch;
    unsigned short m_fadeIn;
    unsigned short m_group;
    SVoiceLevels() : m_volume(1.0f), m_voiceVolume(1.0f), m_pitch(1.0f), m_voicePitch(1.0f), m_fadeIn(0), m_group(0) {}
};

struct SEventKey {
    long m_flags;
    long m_delay;
};

struct CDriver;
struct CSoundEventManager;
struct CSoundEvent {
    long IsActive() const;                                  // 0x00C20840
    void Initialise(CSoundEventManager* manager, long id, long delay, long priority, long sample, CDriver* driver,
                    long category, long loopCount, long looped, const SPKPlaybackRequest* request, const SEventKey* key,
                    const SVoiceLevels* levels, bool track0, bool track1, bool track2, bool streamed);   // 0x00C21C20
};

struct CSoundEventManager {
    unsigned char _pad00[0x88c];
    long          m_nextId;                                 // +0x88c
    long CheckExisting(long sample, CDriver* driver, long category, long loopCount, long looped, long priority);   // 0x00C11230
    CSoundEvent* AcquireEvent(unsigned long priority);      // 0x00C11F00
    CSoundEvent* StartEvent(long sample, const CSoundPlayParameters* source, long flags, CDriver* driver,
                            const CPKVoiceParams* voice, long* eventId);
};

CSoundEvent* CSoundEventManager::StartEvent(long sample, const CSoundPlayParameters* source, long flags, CDriver* driver,
                                            const CPKVoiceParams* voice, long* eventId)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CSoundPlayParameters params;
    if (source)
        params = *source;
    else
        params.Initialise();
    voice->m_override.ApplyTo(params);
    if (!(params.GetFrequencyScale() <= 0.0f)) {
        float chance = (float)rand() * (1.0f / 32767.0f);
        if (!(params.GetFrequencyScale() < chance)
            && CheckExisting(sample, driver, params.GetCategory(), voice->m_loopCount, params.GetLooped(), params.GetPriority())) {
    if (params.GetPriority() == 0)
        params.SetPriority(1);
    CSoundEvent* event = AcquireEvent(params.GetPriority());
    if (!event)
        return 0;
    SVoiceLevels levels;
    levels.m_volume = params.GetVolume();
    float r = (float)rand() * 2.0f;
    float t = r * (1.0f / 32767.0f) - 1.0f;
    float spread = t * params.GetPan();
    levels.m_pitch = params.GetPitch() + spread;
    levels.m_fadeIn = params.GetFadeIn();
    levels.m_group = params.GetGroup();
    levels.m_voiceVolume = voice->m_volume;
    levels.m_voicePitch = voice->m_pitch;
    SPKPlaybackRequest request;
    request.m_eventId = voice->m_priority;
    request.m_flags = voice->m_flags;
    request.m_pitch = params.GetLoopEnd();
    request.m_volume = params.GetLoopStart();
    request.m_handle = voice->m_links;
    request.m_userData = voice->m_owner;
    request.m_callback = voice->m_user;
    SEventKey key;
    key.m_delay = params.GetDelay();
    key.m_flags = flags;
    long id = eventId ? *eventId : m_nextId++;
    event->Initialise(this, id, voice->m_delay, params.GetPriority(), sample, driver, params.GetCategory(), voice->m_loopCount,
                      params.GetLooped(), &request, &key, &levels, voice->GetTrackBit0(), voice->GetTrackBit1(),
                      voice->GetTrackBit2(), params.GetStreamed());
    return event->IsActive() ? event : 0;
        }
    }
    return 0;
}
