#include <cstdio>
struct CSoundObject { int id; };
static CSoundObject* g_released = 0;
static int g_releaseCount = 0;
struct CSoundSystem {
    virtual void ReleaseSoundObject(CSoundObject* sound) { g_released = sound; ++g_releaseCount; }
};
static CSoundSystem g_system;
static CSoundSystem* g_pSoundSystem = &g_system;
static int g_thingDtors = 0;
class CThing { public:
    virtual ~CThing() { ++g_thingDtors; }
};
static int g_ptrDtors = 0;
struct CBaseIntelligentPointer {
    CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) {}
    virtual ~CBaseIntelligentPointer() { ++g_ptrDtors; }
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* item) { PItem = item; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
static CThing* g_seen = (CThing*)1;
class CThingSoundEmitter : public CThing { public:
    CSoundObject* m_pSoundObject;
    bool AttachedToThing;
    CIntelligentPointerThing PAttachedThing;
    CThingSoundEmitter() : m_pSoundObject(0), AttachedToThing(false), PAttachedThing(0) {}
    ~CThingSoundEmitter();
};
CThingSoundEmitter::~CThingSoundEmitter()
{
    if (m_pSoundObject)
        g_pSoundSystem->ReleaseSoundObject(m_pSoundObject);
    CIntelligentPointerThing cleared(0);
    CThing* item = cleared.GetPItem();
    PAttachedThing.SetPItem(item);
    g_seen = PAttachedThing.GetPItem();
}
int main()
{
    CSoundObject sound;
    CThing other;
    {
        CThingSoundEmitter e;
        e.m_pSoundObject = &sound;
        e.PAttachedThing.SetPItem(&other);
    }
    if (g_released != &sound || g_releaseCount != 1 || g_seen != 0 || g_ptrDtors != 2 || g_thingDtors != 1) { std::printf("00836a50_TEST FAIL\n"); return 1; }
    {
        CThingSoundEmitter e;
    }
    if (g_releaseCount != 1 || g_ptrDtors != 4 || g_thingDtors != 2) { std::printf("00836a50_TEST FAIL\n"); return 1; }
    std::printf("00836a50_TEST PASS\n");
    return 0;
}