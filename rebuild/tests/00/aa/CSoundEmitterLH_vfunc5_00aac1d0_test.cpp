#include <cstdio>
#include <new>
// Behaviour model of CSoundEmitterLH::vfunc_5 @ 0x00AAC1D0 (copy of the source logic, stand-in callees).
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static double GFGetTime() { return 2.5; }
static int g_released = 0;
struct CSoundEmitterSource {
    long result; long owner; float volume; C3DVector pos;
    long Play(long o, float v, const C3DVector* p, long, long) { owner = o; volume = v; pos = *p; return result; }
};
struct CSoundBankLH { CSoundEmitterSource* s; long id; CSoundEmitterSource* FindSample(long i) { return i == id ? s : 0; } };
struct CActiveSound { long handle; CSoundEmitterSource* source; float time; CActiveSound(long h, CSoundEmitterSource* s, float t) : handle(h), source(s), time(t) {} };
static void DestroySound(void* p) { ++g_released; delete (CActiveSound*)p; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
template<class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* o) : m_pObject(o) { m_pCount = new SCountBlock; m_pCount->m_references = 1; m_pCount->m_pfnDestroy = DestroySound; m_pCount->m_pObject = o; }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); operator delete(m_pCount); }
        }
    }
};
struct CActiveSoundList {
    CActiveSound* items[4]; SCountBlock* blocks[4]; int n;
    void push_back(const CCountedPointer<CActiveSound>& s) { items[n] = s.m_pObject; blocks[n++] = s.m_pCount; ++s.m_pCount->m_references; }
};
struct CSoundEmitterLH {
    CSoundBankLH* m_pBank; CActiveSoundList m_sounds; bool m_started; long m_scriptCounter; C3DVector where;
    C3DVector GetPosition() { return where; }
    long vfunc_5(long soundId)
    {
        CSoundEmitterSource* sample = m_pBank->FindSample(soundId);
        if (sample) {
            C3DVector position(0.0f, 0.0f, 0.0f);
            position.X = GetPosition().X;
            position.Y = GetPosition().Y;
            position.Z = GetPosition().Z;
            long handle = sample->Play(m_scriptCounter, 1.0f, &position, 0, 0);
            if (handle >= 0) {
                m_sounds.push_back(CCountedPointer<CActiveSound>(new CActiveSound(handle, sample, (float)GFGetTime())));
                m_started = true;
                return handle;
            }
        }
        return -1;
    }
};
int main()
{
    CSoundEmitterSource src; src.result = 42;
    CSoundBankLH bank = { &src, 7 };
    CSoundEmitterLH e; e.m_pBank = &bank; e.m_sounds.n = 0; e.m_started = false; e.m_scriptCounter = 99; e.where = C3DVector(1, 2, 3);
    int ok = e.vfunc_5(3) == -1 && e.m_sounds.n == 0;                 // unknown sound
    ok = ok && e.vfunc_5(7) == 42 && e.m_sounds.n == 1 && e.m_started && src.owner == 99 && src.volume == 1.0f
            && src.pos.Y == 2.0f && src.pos.Z == 3.0f && e.m_sounds.items[0]->handle == 42 && e.m_sounds.items[0]->time == 2.5f
            && e.m_sounds.blocks[0]->m_references == 1 && g_released == 0;
    src.result = -5; e.m_started = false;
    ok = ok && e.vfunc_5(7) == -1 && e.m_sounds.n == 1 && !e.m_started;
    if (!ok) { std::printf("00aac1d0_TEST FAIL\n"); return 1; }
    std::printf("00aac1d0_TEST PASS\n");
    return 0;
}