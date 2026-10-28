// Test: CCodaVoice::Play -- start time and buffer cleared, the sound's buffer set with the loop bit (flag 0x100), its format set, the voice reset, and 2 returned.
#include <cstdio>
#include <cstring>
static char g_seq[8];
struct CSound { void* m_pBuffer; unsigned int m_flags; void* m_pFormat; };
struct CVoice { void* m_pBuffer; double m_startTime; void* buf; unsigned char loop; void* fmt;
    void SetBuffer(void* b, unsigned char l) { std::strcat(g_seq, "B"); buf = b; loop = l; }
    void SetFormat(void* f) { std::strcat(g_seq, "F"); fmt = f; }
    void Reset() { std::strcat(g_seq, "R"); }
    int Play(CSound* s) { m_startTime = 0; m_pBuffer = 0; SetBuffer(s->m_pBuffer, (unsigned char)((s->m_flags >> 8) & 1)); SetFormat(s->m_pFormat); Reset(); return 2; } };
int main()
{
    int b, f; CSound s; s.m_pBuffer = &b; s.m_flags = 0x1FF; s.m_pFormat = &f;
    CVoice v; v.m_pBuffer = &v; v.m_startTime = 5.0;
    bool ok = v.Play(&s) == 2 && v.m_pBuffer == 0 && v.m_startTime == 0 && v.buf == &b && v.loop == 1 && v.fmt == &f && !std::strcmp(g_seq, "BFR");
    s.m_flags = 0x2FF; g_seq[0] = 0; ok = ok && v.Play(&s) == 2 && v.loop == 0;
    std::printf(ok ? "00c59720_TEST PASS\n" : "00c59720_TEST FAIL\n");
    return ok ? 0 : 1;
}