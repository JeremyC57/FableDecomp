// Behaviour test for 0x00C43B30 CAudioObject::AttachConfiguredBackend (libpk voice selection)
#include <stdio.h>

struct SBufferDesc { unsigned long m_mode; unsigned long m_channels; };
struct CSoundBuffer {
    unsigned long m_state;
    SBufferDesc m_desc;
    bool IsStreamed() const { return (m_state >> 2) & 1; }
    bool IsSeekable() const { return (m_state >> 3) & 1; }
    const SBufferDesc* GetDesc() const { return &m_desc; }
};
struct SVoiceCaps { unsigned long m_id, m_available, m_objectCaps, m_formatCaps; };
class CAudioObject;
class CVoice {
public:
    long m_rebindResult;
    int m_released;
    long Rebind(CAudioObject*) { return m_rebindResult; }
};
static CVoice g_voice;
static int g_lastPath;                       // 1 hw3D, 2 stream, 3 hw2D, 4 sw3D, 5 sw2D
static long g_acquireResult;
class CVoiceManager {
public:
    long Get(int path, CVoice** v) { g_lastPath = path; *v = &g_voice; return g_acquireResult; }
    long AcquireStreamVoice(CAudioObject*, CVoice** v) { return Get(2, v); }
    long ApplySlotA(CAudioObject*, CVoice** v) { return Get(3, v); }
    long ApplySlotB(CAudioObject*, CVoice** v) { return Get(1, v); }
    long AcquireVoiceA(CAudioObject*, CVoice** v) { return Get(5, v); }
    long AcquireVoiceB(CAudioObject*, CVoice** v) { return Get(4, v); }
    void Release(CVoice* v) { v->m_released = 1; }
};
static CVoiceManager g_manager;
static CVoiceManager* g_pVoiceManager = &g_manager;
class CAudioObject {
public:
    bool m_is3D;
    unsigned long m_caps;
    CSoundBuffer* m_pCurrentBuffer;
    unsigned long m_queueCapacity;
    bool Is3D() const { return m_is3D; }
    long AttachConfiguredBackend(CSoundBuffer* buffer, unsigned long flags, const SVoiceCaps* hardware,
                                 const SVoiceCaps* software, CVoice** ppVoice)   // the row's body
    {
        *ppVoice = 0;
        unsigned long unsupported = m_caps & ~hardware->m_objectCaps;
        bool streamed = buffer && buffer->IsStreamed();
        if (streamed && unsupported)
            return -14;
        bool useSoftware = unsupported != 0
            || (buffer && (((flags & 8) && !buffer->IsStreamed() && !(flags & 0x10))
                           || (buffer->GetDesc()->m_mode & ~hardware->m_formatCaps)));
        CVoiceManager* manager = g_pVoiceManager;
        CVoice* voice;
        if (!useSoftware) {
            if (hardware->m_available) {
                long result;
                if (Is3D())
                    result = manager->ApplySlotB(this, &voice);
                else if (buffer->IsStreamed() && buffer->IsSeekable()
                         && (buffer->GetDesc()->m_mode == 1 || buffer->GetDesc()->m_mode == 0x40))
                    result = manager->AcquireStreamVoice(this, &voice);
                else
                    result = manager->ApplySlotA(this, &voice);
                if (result)
                    return result;
                if (m_pCurrentBuffer || m_queueCapacity) {
                    result = voice->Rebind(this);
                    if (result) {
                        manager->Release(voice);
                        return result;
                    }
                }
                *ppVoice = voice;
                return 0;
            }
            if (streamed)
                return hardware->m_id ? -9 : -11;
        }
        if (software->m_available) {
            long result;
            if (Is3D())
                result = manager->AcquireVoiceB(this, &voice);
            else
                result = manager->AcquireVoiceA(this, &voice);
            if (result)
                return result;
            if (m_pCurrentBuffer || m_queueCapacity) {
                result = voice->Rebind(this);
                if (result) {
                    manager->Release(voice);
                    return result;
                }
            }
            *ppVoice = voice;
            return 0;
        }
        return software->m_id ? -9 : -11;
    }
};

int main()
{
    int ok = 1;
    SVoiceCaps hw = { 1, 4, 0x3, 0x41 };
    SVoiceCaps sw = { 0, 8, 0xff, 0xff };
    CSoundBuffer buffer = { 0, { 1, 2 } };
    CAudioObject object = { false, 0x1, 0, 0 };
    CVoice* v = 0;
    g_acquireResult = 0; g_voice.m_rebindResult = 0; g_voice.m_released = 0;
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == 0 && g_lastPath == 3 && v == &g_voice;
    object.m_is3D = true;
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == 0 && g_lastPath == 1;
    object.m_is3D = false;
    buffer.m_state = 0xc;                                                // streamed + seekable, mode 1: stream voice
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == 0 && g_lastPath == 2;
    object.m_caps = 0x4;                                                 // unsupported on hardware + streamed
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == -14 && v == 0;
    buffer.m_state = 0;                                                  // not streamed: software
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == 0 && g_lastPath == 5;
    object.m_caps = 0x1;
    ok &= object.AttachConfiguredBackend(&buffer, 8, &hw, &sw, &v) == 0 && g_lastPath == 5;   // flag 8, not 0x10
    ok &= object.AttachConfiguredBackend(&buffer, 0x18, &hw, &sw, &v) == 0 && g_lastPath == 3;
    buffer.m_desc.m_mode = 2;                                            // format not on hardware
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == 0 && g_lastPath == 5;
    buffer.m_desc.m_mode = 1;
    hw.m_available = 0;                                                  // no hardware voices, not streamed
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == 0 && g_lastPath == 5;
    buffer.m_state = 0x4;                                                // ... streamed: hardware error (id 1)
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == -9;
    hw.m_id = 0;
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == -11;
    buffer.m_state = 0; sw.m_available = 0;
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == -11;
    sw.m_available = 8; g_acquireResult = -5;
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == -5 && v == 0;
    g_acquireResult = 0; g_voice.m_rebindResult = -3; object.m_queueCapacity = 4;
    ok &= object.AttachConfiguredBackend(&buffer, 0, &hw, &sw, &v) == -3 && v == 0 && g_voice.m_released;
    printf(ok ? "00c43b30_TEST PASS\n" : "00c43b30_TEST FAIL\n");
    return ok ? 0 : 1;
}