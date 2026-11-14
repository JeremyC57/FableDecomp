// CPhysicalPlayer::Play(CSoundEventManager* manager, SPlayDesc* desc, CSoundEvent* event, SEventLevel* level, float pitch, float gain) -- under the audio module lock, start a sound event on this physical (CODA) player: create a 3D or 2D CODA source, fade envelope, sample description from the buffer's format (loop window, block-aligned start offset), the CODA sample, then position / range, pitch, sample, play cursor, play (looping) and volume
// Grade: RELOCATION_MATCH 846v846 (VC7.1 RTM, /EHsc). Closer: the event position read through a named const reference (FITTED; the direct member and an inline const-ref accessor both rotate the tail's scratch registers and push ebp at entry).
// Leaf: ?Play@CPhysicalPlayer@@QAEXPAUCSoundEventManager@@PAUSPlayDesc@@PAUCSoundEvent@@PAUSEventLevel@@MM@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C271F0, 846 bytes (`ret 18h`, EH-framed). Descriptive names (no PDB or RTTI covers the libpk
//   audio library): the reloc binding of the landed caller CSoundEvent::PlayPhysical 0x00C20A20. The player is the class the landed
//   0x00C27550 / 0x00C276B0 rows call CCodaPlayback (vtable 0x012B4034; same member offsets).
#include "libpk/PKCProtectionInstance.h"

extern PKCCriticalSection g_soundLock;             // 0x0143BAFC the audio library's module lock
__declspec(dllimport) double __cdecl ceil(double);  // IAT [0x014400F4]

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector SwapYZ() const;                   // 0x00C1C070
};

struct CODASource;                              // opaque CODA source object
struct CODASample;                              // opaque CODA sample object
struct SCodaEnvelope {                          // 24-byte fade description handed to CODA (frame +0x30)
    float m_startVolume;
    float m_fadeInTime;
    float m_holdA;
    float m_holdB;
    float m_endVolume;
    float m_fadeOutTime;
};
struct SCodaSampleDesc {                        // 28-byte sample description (frame +0x48)
    long m_type;                                // 1 PCM, 8 ADPCM
    long m_channels;
    long m_sampleRate;
    unsigned long m_length;
    unsigned long m_loopStart;
    unsigned long m_loopEnd;
    long m_data;
};
typedef float (__stdcall* CODARangeCallback)(long, long, long, long);
extern "C" {                                    // the CODA C API (__stdcall; nothrow under /EHsc)
CODASource* __stdcall AudioSystem_SetModeFlags(unsigned long flags);                 // 0x00C33DC0 (3D source)
CODASource* __stdcall AudioSystem_SetOutputFlags(unsigned long flags);               // 0x00C35EC0 (2D source)
void __stdcall CODA_SetSourceEnvelope(CODASource* source, const SCodaEnvelope* envelope);   // 0x00C35B30
CODASample* __stdcall CODA_CreateSample(const SCodaSampleDesc* desc);                // 0x00C36080
void __stdcall CODA_SetSourcePosition(CODASource* source, const C3DVector* position);   // 0x00C33FD0
void __stdcall CODA_SetSourceRangeCallback(CODASource* source, CODARangeCallback callback, long user);   // 0x00C33F20
void __stdcall CODA_SetSourcePropertyFCUnlessAccepted(CODASource* source, float value);  // 0x00C35F20
void __stdcall CODA_SetSourceVolume(CODASource* source, float volume);              // 0x00C35AF0
void __stdcall CODA_SetSourcePitch(CODASource* source, float pitch);                // 0x00C35B10
long __stdcall AudioObject_SetSource(CODASource* source, CODASample* sample);       // 0x00C35990
void __stdcall CODA_SourceSetPlayCursor(CODASource* source, unsigned long cursor);  // 0x00C35E30
long __stdcall CODA_SourcePlay(CODASource* source, long looping);                   // 0x00C35DD0
}

struct CAudioFormat {                           // +0x18 of the stream buffer
    unsigned long m_length;                     // +0x00
    unsigned char _pad04[8];
    unsigned short m_channels;                  // +0x0C
    unsigned short m_formatTag;                 // +0x0E
    long m_sampleRate;                          // +0x10
    unsigned long GetBlockAlign();              // 0x00C2D7D0
    long HasLoop();                             // 0x00C2D8E0
    unsigned long GetLoopStart() const;         // 0x00C2D900
    unsigned long GetLoopEnd() const;           // 0x00C2D910
    float GetBytesPerSample();                  // 0x00C2D970
};
struct CSoundEventManager;
struct CStreamBuffer {                          // the event's buffer (+0xA4)
    unsigned char _pad00[0x18];
    CAudioFormat m_format;                      // +0x18
    long GetData(CSoundEventManager* manager);  // 0x00C26240
};
struct CSoundEvent {
    unsigned char _pad00[0x14];
    long m_priority;                            // +0x14
    unsigned char _pad18[0xa4 - 0x18];
    CStreamBuffer* m_pBuffer;                   // +0xA4
};
struct CPhysicalPlayer;
struct CPlayerList { void Add(CPhysicalPlayer* player); };   // 0x00C21E90
struct CSoundTicker { void Update(); };         // 0x00C1C460
struct CSoundEventManager {
    unsigned char _pad000[0x2c];
    CPlayerList m_players;                      // +0x2C
    unsigned char _pad030[0x7bc - 0x30];
    CSoundTicker* m_pTicker;                    // +0x7BC
    float m_masterVolume;                       // +0x7C0
    unsigned char _pad7c4[0x7d0 - 0x7c4];
    float m_fadeInTime;                         // +0x7D0
    float m_fadeOutTime;                        // +0x7D4
};
struct SPlayDesc {
    long m_mode;                                // +0x00 non-zero: positioned
    C3DVector m_position;                       // +0x04
    unsigned char _pad10[0x1c - 0x10];
    float m_distance;                           // +0x1C
};
struct SEventLevel {
    float m_startFraction;                      // +0
    long m_loops;                               // +4
};

struct CPhysicalPlayer {                        // vtable 0x012B4034
    virtual void SetSourceProperty20(float value);   // slot 0
    CSoundEvent*  m_pEvent;                     // +0x04
    CODASample*   m_pSample;                    // +0x08
    CODASource*   m_pSource;                    // +0x0C
    long          m_loopsLeft;                  // +0x10
    unsigned long m_loopPoint;                  // +0x14
    long          m_bArmed;                     // +0x18
    long          m_priority;                   // +0x1C
    unsigned long m_length;                     // +0x20
    bool          m_bLooping;                   // +0x24
    bool          m_b3D;                        // +0x25
    bool          m_bStarted;                   // +0x26
    bool          m_bStopped;                   // +0x27
    void UpdateManagerTime(CSoundEventManager* manager);   // 0x00C271E0
    bool IsLooping() const { return m_loopsLeft != 0; }
    void Play(CSoundEventManager* manager, SPlayDesc* desc, CSoundEvent* event, SEventLevel* level, float pitch, float gain);
};
float __stdcall FullRange(long, long, long, long);    // 0x00C271B0

void CPhysicalPlayer::Play(CSoundEventManager* manager, SPlayDesc* desc, CSoundEvent* event, SEventLevel* level, float pitch, float gain)
{
    m_pEvent = event;
    PKCProtectionInstance lock(&g_soundLock);
    m_loopsLeft = level->m_loops;
    m_priority = m_pEvent->m_priority;
    m_b3D = desc->m_mode != 0;
    if (desc->m_mode != 0 && m_pEvent->m_pBuffer->m_format.m_channels > 1)
        m_b3D = false;
    if (m_b3D)
        m_pSource = AudioSystem_SetModeFlags(0x100813);
    else
        m_pSource = AudioSystem_SetOutputFlags(0x10813);
    CStreamBuffer* buffer = m_pEvent->m_pBuffer;
    unsigned long offset = (unsigned long)(buffer->m_format.m_length * level->m_startFraction);
    unsigned long aligned = offset - offset % buffer->m_format.GetBlockAlign();
    if (offset != aligned)
        offset = aligned;
    SCodaEnvelope envelope;
    if (offset)
        envelope.m_fadeInTime = manager->m_fadeInTime * 0.001f;
    else
        envelope.m_fadeInTime = 0.0f;
    envelope.m_fadeOutTime = manager->m_fadeOutTime * 0.001f;
    envelope.m_holdB = 0.0f;
    envelope.m_startVolume = 0.0f;
    envelope.m_holdA = 0.0f;
    envelope.m_endVolume = 1.0f;
    CODA_SetSourceEnvelope(m_pSource, &envelope);
    SCodaSampleDesc sample;
    switch (buffer->m_format.m_formatTag) {
    case 0: sample.m_type = 1; break;
    case 1: sample.m_type = 8; break;
    }
    sample.m_channels = buffer->m_format.m_channels;
    sample.m_sampleRate = buffer->m_format.m_sampleRate;
    if (m_loopsLeft != 0) {
        m_bArmed = 1;
        m_bLooping = true;
        if (buffer->m_format.HasLoop()) {
            sample.m_loopStart = buffer->m_format.GetLoopStart();
            float tail = (float)ceil(buffer->m_format.GetBytesPerSample());
            sample.m_loopEnd = buffer->m_format.GetLoopEnd() - (unsigned long)tail;
            m_loopPoint = sample.m_loopStart + (sample.m_loopEnd - sample.m_loopStart) / 2;
        } else {
            sample.m_loopStart = 0;
            sample.m_loopEnd = 0;
            m_loopPoint = buffer->m_format.m_length / 2;
        }
    } else {
        sample.m_loopStart = 0;
        sample.m_loopEnd = 0;
        m_loopPoint = buffer->m_format.m_length;
        m_bArmed = 0;
        m_bLooping = false;
    }
    sample.m_length = buffer->m_format.m_length;
    sample.m_data = buffer->GetData(manager);
    m_pSample = CODA_CreateSample(&sample);
    m_length = buffer->m_format.m_length;
    m_bStarted = false;
    m_bStopped = false;
    manager->m_players.Add(this);
    if (offset < m_length) {
        if (desc->m_mode != 0) {
            if (m_b3D) {
                C3DVector position;
                const C3DVector& eventPosition = desc->m_position;
                position = eventPosition.SwapYZ();
                CODA_SetSourcePosition(m_pSource, &position);
                CODA_SetSourceRangeCallback(m_pSource, FullRange, 0);
            }
        } else {
            CODA_SetSourcePropertyFCUnlessAccepted(m_pSource, desc->m_distance);
        }
        UpdateManagerTime(manager);
        CODA_SetSourceVolume(m_pSource, 0.0f);
        CODA_SetSourcePitch(m_pSource, pitch);
        AudioObject_SetSource(m_pSource, m_pSample);
        CODA_SourceSetPlayCursor(m_pSource, offset);
        manager->m_pTicker->Update();
        CODA_SourcePlay(m_pSource, IsLooping());
        if (!m_b3D)
            gain *= manager->m_masterVolume;
        CODA_SetSourceVolume(m_pSource, gain);
    }
}
