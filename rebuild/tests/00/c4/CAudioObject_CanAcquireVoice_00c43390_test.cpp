// Behaviour test for 00c43390: CAudioObject::CanAcquireVoice -- hardware when the pool covers the caps and format, else the software pool
#include <cstdio>

static int g_error = 0;
struct SBufferDesc { unsigned int m_mode; unsigned int m_channels; };
struct CSoundBuffer {
    unsigned int m_state;
    SBufferDesc m_desc;
    bool IsStreamed() const { return (m_state >> 2) & 1; }
    const SBufferDesc* GetDesc() const { return &m_desc; }
};
struct SVoiceCaps { unsigned int m_id, m_available, m_objectCaps, m_formatCaps, m_words[2]; };
struct CAudioSystem { void SetError(int code) { g_error = code; } };
static CAudioSystem g_audioSystem;

struct CAudioObject {
    unsigned int m_caps;
    bool CanAcquireVoice(CSoundBuffer* buffer, unsigned int flags, const SVoiceCaps* hardware, const SVoiceCaps* software)
    {
        unsigned int missing = m_caps & ~hardware->m_objectCaps;
        bool streamed;
        if (buffer && buffer->IsStreamed()) {
            streamed = true;
            if (missing) {
                g_audioSystem.SetError(-14);
                return false;
            }
        } else
            streamed = false;
        if (!missing) {
            bool ok = true;
            if (buffer) {
                if ((flags & 8) && !buffer->IsStreamed() && !(flags & 0x10))
                    ok = false;
                else if (buffer->GetDesc()->m_mode & ~hardware->m_formatCaps)
                    ok = false;
            }
            if (ok) {
                if (hardware->m_available)
                    return true;
                if (streamed)
                    return false;
            }
        }
        if (software->m_available)
            return true;
        return false;
    }
};

#define CHECK(c) do { if (!(c)) { std::printf("00c43390_TEST FAIL line %d\n", __LINE__); return 1; } } while (0)

int main()
{
    SVoiceCaps hw = { 0, 2, 0x3, 0x1, { 0, 0 } };
    SVoiceCaps hwNone = { 0, 0, 0x3, 0x1, { 0, 0 } };
    SVoiceCaps sw = { 0, 1, 0, 0, { 0, 0 } };
    SVoiceCaps swNone = { 0, 0, 0, 0, { 0, 0 } };
    CAudioObject o; o.m_caps = 0x1;
    CSoundBuffer plain = { 0, { 0x1, 2 } };
    CSoundBuffer streamed = { 4, { 0x1, 2 } };
    CSoundBuffer badFormat = { 0, { 0x2, 2 } };
    CHECK(o.CanAcquireVoice(0, 0, &hw, &swNone));              // no buffer, hardware free
    CHECK(o.CanAcquireVoice(&plain, 0, &hw, &swNone));         // format covered
    CHECK(!o.CanAcquireVoice(&badFormat, 0, &hw, &swNone));    // format not covered, no software
    CHECK(o.CanAcquireVoice(&badFormat, 0, &hw, &sw));         // falls back to software
    CHECK(!o.CanAcquireVoice(&plain, 8, &hw, &swNone));        // flag 8 without 0x10 refuses a plain buffer
    CHECK(o.CanAcquireVoice(&plain, 0x18, &hw, &swNone));      // ... unless 0x10
    CHECK(o.CanAcquireVoice(&streamed, 8, &hw, &swNone));      // streamed passes flag 8
    CHECK(!o.CanAcquireVoice(&streamed, 0, &hwNone, &sw) && g_error == 0);   // streamed, no hardware: false, no error
    o.m_caps = 0x4;                                            // caps the hardware lacks
    CHECK(!o.CanAcquireVoice(&streamed, 0, &hw, &sw) && g_error == -14);
    g_error = 0;
    CHECK(o.CanAcquireVoice(&plain, 0, &hw, &sw) && g_error == 0);
    CHECK(!o.CanAcquireVoice(&plain, 0, &hw, &swNone));
    std::printf("00c43390_TEST PASS\n");
    return 0;
}