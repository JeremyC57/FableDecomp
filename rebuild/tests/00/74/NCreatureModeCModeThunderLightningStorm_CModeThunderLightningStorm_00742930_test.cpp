// Behaviour test for 0x00742930 NCreatureMode::CModeThunderLightningStorm::~CModeThunderLightningStorm()
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_log[64];
static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }

class CBaseIntelligentPointer {
public:
    int tag;
    explicit CBaseIntelligentPointer(int t = 0) : tag(t) {}
    virtual ~CBaseIntelligentPointer() { Log(tag); }      // stand-in for 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    explicit CIntelligentPointer(int t = 0) : CBaseIntelligentPointer(t) {}
};

class CCharString {
public:
    int tag;
    ~CCharString() { Log(tag); }
};
class CAnimFilterBase { public: virtual void v00() {} ~CAnimFilterBase() {} };
class CAnimFilterOfGroup : public CAnimFilterBase { public: CCharString Group; unsigned int GroupCRC; };

static int g_defDeleted = 0;
class CDefPointeeBase {
public:
    virtual void v00() {}
    virtual void DeleteSelf() const { ++g_defDeleted; Log(900); }
    mutable int RefCount;
    void Release() const
    {
        if (--RefCount == 0)
            DeleteSelf();
    }
};
template <class T> class CDefPointer {
public:
    T* PDef;
    ~CDefPointer()
    {
        if (PDef) {
            PDef->Release();
            PDef = 0;
        }
    }
};

template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter> inline void _Destroy(ForwardIter first, ForwardIter last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void Make(int n, int base)
    {
        _M_start = n ? (T*)std::malloc(sizeof(T) * n) : 0;
        for (int i = 0; i < n; ++i) new (_M_start + i) T(base + i);
        _M_finish = _M_start + n;
        _M_end_of_storage = _M_finish;
    }
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            std::free(_M_start);
    }
};

class CThing;
class CModeStandBase {
public:
    virtual void v00() {}
    ~CModeStandBase() { Log(1000); }                      // stand-in for 0x0073D860
};

class CModeThunderLightningStorm : public CModeStandBase {
public:
    CAnimFilterOfGroup                       Filter;
    vector<CIntelligentPointer<CThing> >     LightningBeamParticleEmitters;
    vector<CIntelligentPointer<CThing> >     LightningBeamEndParticleEmitters;
    CIntelligentPointer<CThing>              PSkyBeam;
    CIntelligentPointer<CThing>              PCreatorEffect;
    CIntelligentPointer<CThing>              PHandGlow;
    CDefPointer<const CDefPointeeBase>       PLightningStormDef;
    vector<CIntelligentPointer<CThing> >     HitThings;
    ~CModeThunderLightningStorm() {}
};

int main()
{
    int ok = 1;
    CDefPointeeBase def;
    def.RefCount = 2;
    {
        CModeThunderLightningStorm* m = (CModeThunderLightningStorm*)std::malloc(sizeof(CModeThunderLightningStorm));
        new (m) CModeThunderLightningStorm();
        m->Filter.Group.tag = 10;
        m->LightningBeamParticleEmitters.Make(2, 20);
        m->LightningBeamEndParticleEmitters.Make(1, 30);
        m->PSkyBeam.tag = 40; m->PCreatorEffect.tag = 41; m->PHandGlow.tag = 42;
        m->PLightningStormDef.PDef = &def;
        m->HitThings.Make(3, 50);
        m->~CModeThunderLightningStorm();
        std::free(m);
    }
    // reverse member order; each vector destroys its elements front to back
    int want[] = { 50, 51, 52, 42, 41, 40, 30, 20, 21, 10, 1000 };
    if (g_n != (int)(sizeof(want) / sizeof(want[0]))) ok = 0;
    for (int i = 0; ok && i < g_n; ++i) if (g_log[i] != want[i]) ok = 0;
    if (def.RefCount != 1 || g_defDeleted != 0) ok = 0;   // one reference dropped, def survives
    // the last reference: the def deletes itself
    g_n = 0;
    {
        CModeThunderLightningStorm* m = (CModeThunderLightningStorm*)std::malloc(sizeof(CModeThunderLightningStorm));
        new (m) CModeThunderLightningStorm();
        m->LightningBeamParticleEmitters.Make(0, 0);
        m->LightningBeamEndParticleEmitters.Make(0, 0);
        m->HitThings.Make(0, 0);
        m->PLightningStormDef.PDef = &def;
        m->~CModeThunderLightningStorm();
        std::free(m);
    }
    if (def.RefCount != 0 || g_defDeleted != 1 || g_log[0] != 900) ok = 0;
    if (!ok) { std::printf("00742930_TEST FAIL\n"); return 1; }
    std::printf("00742930_TEST PASS\n");
    return 0;
}