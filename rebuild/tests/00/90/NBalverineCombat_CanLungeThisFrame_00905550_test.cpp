// Behaviour test for 0x00905550 CanLungeThisFrame: default 5 s interval, def override, release of the def.
#include <cstdio>

static int g_destroyed = 0;
struct CDefBase {
    virtual void v00() { }
    virtual void Destroy() { ++g_destroyed; }
    long m_count;
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CBalverineBattleDef : CDefBase {
    float SecondsBetweenLunges;
};
template<class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    T* operator->() const { return m_p; }
};
struct CDefClassBase {
    CBalverineBattleDef* m_sub;
    template<class T> bool GetSubDef(CDefPointer<T>& out)
    {
        if (!m_sub)
            return false;
        ++m_sub->m_count;
        out.m_p = m_sub;
        return true;
    }
};
struct CThingCreatureBase {
    CDefClassBase* m_pDef;
};
static long g_frame = 0;
long GFGetCurrentFrame() { return g_frame; }
long g_framesPerSecond = 10;
long g_lastLungeFrame = 100;

bool CanLungeThisFrame(CThingCreatureBase* creature)
{
    float seconds = 5.0f;
    CDefPointer<CBalverineBattleDef> def;
    if (creature->m_pDef->GetSubDef(def))
        seconds = def->SecondsBetweenLunges;
    long fps = g_framesPerSecond;
    if ((float)(GFGetCurrentFrame() - g_lastLungeFrame) > seconds * fps)
        return true;
    return false;
}

int main()
{
    CDefClassBase defs;
    defs.m_sub = 0;
    CThingCreatureBase c;
    c.m_pDef = &defs;
    int ok = 1;
    g_frame = 150;                      // 50 frames < 5 s * 10
    if (CanLungeThisFrame(&c)) ok = 0;
    g_frame = 151;                      // 51 > 50
    if (!CanLungeThisFrame(&c)) ok = 0;
    g_frame = 150;                      // exactly equal is not enough
    if (CanLungeThisFrame(&c)) ok = 0;
    CBalverineBattleDef bd;
    bd.m_count = 1;
    bd.SecondsBetweenLunges = 2.0f;
    defs.m_sub = &bd;
    g_frame = 121;                      // 21 > 20
    if (!CanLungeThisFrame(&c)) ok = 0;
    if (bd.m_count != 1 || g_destroyed != 0) ok = 0;
    g_frame = 119;
    if (CanLungeThisFrame(&c)) ok = 0;
    bd.m_count = 0;                     // the only reference: released to zero after the call
    CanLungeThisFrame(&c);
    if (g_destroyed != 1) ok = 0;
    if (!ok) { std::printf("00905550_TEST FAIL\n"); return 1; }
    std::printf("00905550_TEST PASS\n");
    return 0;
}