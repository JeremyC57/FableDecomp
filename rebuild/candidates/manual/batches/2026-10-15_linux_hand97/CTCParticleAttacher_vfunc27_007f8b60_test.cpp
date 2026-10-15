// Behaviour test for CTCParticleAttacher::vfunc_27 @ 0x007F8B60: on type 5, kill each particle, clear the list, re-attach.
#include <cstdio>
static int g_step = 0, g_clearStep = 0, g_attachStep = 0, g_kills = 0;
struct CTCMessage { long type; virtual long GetType() const { return type; } virtual ~CTCMessage() {} };
struct CParticlePointer { int alive; };
struct CKillParticle { CKillParticle() {} void operator()(CParticlePointer& p) const { if (p.alive) { p.alive = 0; ++g_kills; } } };
static CKillParticle for_each(CParticlePointer* first, CParticlePointer* last, CKillParticle f)
{ for (; first != last; ++first) f(*first); return f; }
struct CParticleVector {
    CParticlePointer* m_pFirst; CParticlePointer* m_pLast;
    CParticlePointer* erase(CParticlePointer* first, CParticlePointer*) { g_clearStep = ++g_step; m_pLast = first; return first; }
    void clear() { erase(m_pFirst, m_pLast); }
};
struct CTCParticleAttacher {
    CParticleVector ParticleList;
    void AttachParticles() { g_attachStep = ++g_step; }
    void vfunc_27(CTCMessage* msg)
    {
        if (msg->GetType() == 5) {
            for_each(ParticleList.m_pFirst, ParticleList.m_pLast, CKillParticle());
            ParticleList.clear();
            AttachParticles();
        }
    }
};
int main()
{
    CParticlePointer ps[3] = { { 1 }, { 0 }, { 1 } };
    CTCParticleAttacher a; a.ParticleList.m_pFirst = ps; a.ParticleList.m_pLast = ps + 3;
    CTCMessage other; other.type = 4;
    a.vfunc_27(&other);
    if (g_kills != 0 || g_step != 0) { std::printf("007f8b60_TEST FAIL\n"); return 1; }
    CTCMessage replaced; replaced.type = 5;
    a.vfunc_27(&replaced);
    if (g_kills != 2 || g_clearStep != 1 || g_attachStep != 2 || a.ParticleList.m_pLast != ps) { std::printf("007f8b60_TEST FAIL\n"); return 1; }
    std::printf("007f8b60_TEST PASS\n");
    return 0;
}
