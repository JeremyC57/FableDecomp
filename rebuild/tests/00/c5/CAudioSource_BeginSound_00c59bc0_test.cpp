// Test: CAudioSource::BeginSound -- the current queued item (none when the queue has no capacity) and its start time are taken, the buffer set with the loop bit, then the sound's format.
#include <cstdio>
struct CSound { void** m_items; double* m_times; unsigned int m_capacity; unsigned int m_current; unsigned int m_flags; void* m_pFormat; };
struct CSource { double m_startTime; void* buf; unsigned char loop; void* fmt;
    void SetBuffer(void* b, unsigned char l) { buf = b; loop = l; } void SetFormat(void* f) { fmt = f; }
    void BeginSound(CSound* s) { void* item = s->m_capacity ? s->m_items[s->m_current] : 0; m_startTime = s->m_times[s->m_current];
        SetBuffer(item, (unsigned char)((s->m_flags >> 8) & 1)); SetFormat(s->m_pFormat); } };
int main()
{
    int a, b, f; void* items[2] = { &a, &b }; double times[2] = { 1.5, 2.5 };
    CSound s; s.m_items = items; s.m_times = times; s.m_capacity = 2; s.m_current = 1; s.m_flags = 0x100; s.m_pFormat = &f;
    CSource src; src.BeginSound(&s);
    bool ok = src.buf == &b && src.m_startTime == 2.5 && src.loop == 1 && src.fmt == &f;
    s.m_capacity = 0; s.m_current = 0; s.m_flags = 0; src.BeginSound(&s);
    ok = ok && src.buf == 0 && src.m_startTime == 1.5 && src.loop == 0;
    std::printf(ok ? "00c59bc0_TEST PASS\n" : "00c59bc0_TEST FAIL\n");
    return ok ? 0 : 1;
}