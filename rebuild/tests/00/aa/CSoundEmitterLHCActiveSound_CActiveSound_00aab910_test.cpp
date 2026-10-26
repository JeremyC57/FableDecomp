// Behaviour test for 00aab910 (CSoundEmitterLH::CActiveSound::CActiveSound): state 2, the handle, zero counters, the volume, playing, not stopping, an empty name, the source.
#include <cstdio>

struct CSoundEmitterSource { long m_id; };

class CCharString { public:
    const char* m_pData;
    CCharString() : m_pData(0) { }
};

struct CBaseClassNonCopyable {
    CBaseClassNonCopyable() { }
    virtual ~CBaseClassNonCopyable() { }
};

struct CActiveSound : CBaseClassNonCopyable {
    long                 m_state;
    long                 m_handle;
    long                 m_elapsed;
    float                m_volume;
    bool                 m_playing;
    bool                 m_stopping;
    long                 m_flags;
    CCharString          m_name;
    CSoundEmitterSource* m_pSource;
    CActiveSound(long handle, CSoundEmitterSource* source, float volume)
        : m_state(2), m_handle(handle), m_elapsed(0), m_volume(volume), m_playing(true), m_stopping(false), m_flags(0), m_name(), m_pSource(source)
    {
    }
};

int main()
{
    CSoundEmitterSource src = { 4 };
    CActiveSound s(17, &src, 0.75f);
    if (s.m_state != 2 || s.m_handle != 17 || s.m_elapsed != 0 || s.m_volume != 0.75f || !s.m_playing || s.m_stopping
        || s.m_flags != 0 || s.m_name.m_pData != 0 || s.m_pSource != &src) { std::printf("00aab910_TEST FAIL\n"); return 1; }
    std::printf("00aab910_TEST PASS\n");
    return 0;
}