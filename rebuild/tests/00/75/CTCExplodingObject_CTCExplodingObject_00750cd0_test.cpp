#include <cstdio>
static int g_kills = 0, g_lastKillImmediate = 0, g_ptrDtors = 0, g_baseDtors = 0, g_defDestroyed = 0;
struct CThing {
    unsigned char m_flags;
    bool IsDead() const { return (m_flags & 1) != 0; }
    void Kill(bool immediately) { ++g_kills; g_lastKillImmediate = immediately; m_flags |= 1; }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() { return m_pItem; }
    ~CBaseIntelligentPointer() { ++g_ptrDtors; }
};
struct CDefPointee {
    long RefCount;
    virtual void Destroy() { ++g_defDestroyed; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CDefPointer {
    CDefPointee* Object;
    ~CDefPointer() { if (Object) { Object->Release(); Object = 0; } }
};
class CTCBase {
public:
    CThing* ParentThing;
    virtual ~CTCBase() { ++g_baseDtors; }
};
class CTCExplodingObject : public CTCBase {
public:
    CDefPointer PDef;
    CBaseIntelligentPointer PProximityParticle;
    virtual ~CTCExplodingObject()
    {
        if (PProximityParticle.GetPItem() && !PProximityParticle.GetPItem()->IsDead())
            PProximityParticle.GetPItem()->Kill(true);
    }
};
int main()
{
    CThing particle; particle.m_flags = 0;
    CDefPointee def; def.RefCount = 1;
    CTCBase* p = new CTCExplodingObject;
    ((CTCExplodingObject*)p)->PDef.Object = &def;
    ((CTCExplodingObject*)p)->PProximityParticle.m_pItem = &particle;
    delete p;
    bool first = g_kills == 1 && g_lastKillImmediate == 1 && g_defDestroyed == 1 && g_ptrDtors == 1 && g_baseDtors == 1;
    CTCExplodingObject* q = new CTCExplodingObject;           // dead particle: not killed again; no def
    q->PDef.Object = 0; q->PProximityParticle.m_pItem = &particle;
    delete q;
    CTCExplodingObject* r = new CTCExplodingObject;           // no particle
    r->PDef.Object = 0; r->PProximityParticle.m_pItem = 0;
    delete r;
    if (!first || g_kills != 1 || g_ptrDtors != 3 || g_baseDtors != 3 || g_defDestroyed != 1) { std::printf("00750cd0_TEST FAIL\n"); return 1; }
    std::printf("00750cd0_TEST PASS\n");
    return 0;
}