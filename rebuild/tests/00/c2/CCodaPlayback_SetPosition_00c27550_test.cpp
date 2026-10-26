// behaviour test for 0x00C27550 CCodaPlayback::SetPosition: a 3D source gets the Y/Z-swapped position, under the lock
#include <cstdio>

static int g_locks = 0, g_unlocks = 0, g_depth = 0, g_lockedCalls = 0;
struct CScopedLock {
    void* m_pSection; unsigned long m_tick;
    CScopedLock(void* section) : m_pSection(section), m_tick(0) { ++g_locks; ++g_depth; }
    ~CScopedLock() { ++g_unlocks; --g_depth; }
};
unsigned char g_soundLock[0x18];

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector SwapYZ() const { C3DVector v(X, Z, Y); return v; }
};

struct CODASource { float volume; C3DVector position; unsigned long playPosition; int stops; };
void CODA_SetSourceVolume(CODASource* s, float v) { if (g_depth) ++g_lockedCalls; s->volume = v; }
void CODA_SetSourcePosition(CODASource* s, const C3DVector* p) { if (g_depth) ++g_lockedCalls; s->position = *p; }
unsigned long CODA_GetSourcePosition(CODASource* s) { if (g_depth) ++g_lockedCalls; return s->playPosition; }
long CODA_StopSource(CODASource* s, bool) { if (g_depth) ++g_lockedCalls; ++s->stops; return 1; }

struct CPanel { float m_masterVolume; };

struct CCodaPlayback {
    CODASource*   m_pSource;
    int           m_loopsLeft;
    unsigned int  m_loopPoint;
    int           m_bArmed;
    bool          m_bLooping, m_b3D, m_bStarted;
    void SetPosition(const C3DVector& position);
};

void CCodaPlayback::SetPosition(const C3DVector& position)
{
    CScopedLock lock(g_soundLock);
    if (m_b3D) {
        C3DVector v;
        v = position.SwapYZ();
        CODA_SetSourcePosition(m_pSource, &v);
    }
}

int main()
{
    CODASource s; s.position = C3DVector(0.0f, 0.0f, 0.0f);
    CCodaPlayback p; p.m_pSource = &s; p.m_b3D = false;
    p.SetPosition(C3DVector(1.0f, 2.0f, 3.0f));            // not 3D: untouched
    bool ok = s.position.X == 0.0f && s.position.Y == 0.0f && s.position.Z == 0.0f;
    p.m_b3D = true;
    p.SetPosition(C3DVector(1.0f, 2.0f, 3.0f));
    ok = ok && s.position.X == 1.0f && s.position.Y == 3.0f && s.position.Z == 2.0f && g_locks == 2 && g_unlocks == 2 && g_lockedCalls == 1;
    if (!ok) { std::printf("00c27550_TEST FAIL\n"); return 1; }
    std::printf("00c27550_TEST PASS\n");
    return 0;
}