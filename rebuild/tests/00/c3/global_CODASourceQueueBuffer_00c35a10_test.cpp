// Behaviour test for 00c35a10: AudioObject_PlaySource -- a streamed+locked source with pending data is refused with error -6; otherwise the object's result is returned as 1/0
#include <cstdio>
struct CAudioSystem { int errors; long last; void SetError(long code) { ++errors; last = code; } };
static CAudioSystem g_audioSystem;
struct CAudioSource {
    long m_pending; unsigned int m_flags;
    bool IsStreamed() const { return (m_flags >> 2) & 1; }
    bool IsLocked() const { return (m_flags >> 3) & 1; }
};
struct CAudioObject {
    int calls;
    bool PlaySource(CAudioSource*, long a, long b) { ++calls; return a == b; }
};
static int AudioObject_PlaySource(CAudioObject* object, CAudioSource* source, long a, long b)
{
    if (source->IsStreamed() && source->IsLocked() && source->m_pending) { g_audioSystem.SetError(-6); return 0; }
    return object->PlaySource(source, a, b) ? 1 : 0;
}
int main()
{
    CAudioObject o; o.calls = 0;
    CAudioSource s; s.m_pending = 1; s.m_flags = 0xC;
    if (AudioObject_PlaySource(&o, &s, 1, 1) != 0 || g_audioSystem.errors != 1 || g_audioSystem.last != -6 || o.calls != 0) { std::printf("00c35a10_TEST FAIL\n"); return 1; }
    s.m_flags = 0x4;
    if (AudioObject_PlaySource(&o, &s, 1, 1) != 1 || o.calls != 1) { std::printf("00c35a10_TEST FAIL\n"); return 1; }
    s.m_flags = 0xC; s.m_pending = 0;
    if (AudioObject_PlaySource(&o, &s, 1, 2) != 0 || o.calls != 2 || g_audioSystem.errors != 1) { std::printf("00c35a10_TEST FAIL\n"); return 1; }
    std::printf("00c35a10_TEST PASS\n");
    return 0;
}