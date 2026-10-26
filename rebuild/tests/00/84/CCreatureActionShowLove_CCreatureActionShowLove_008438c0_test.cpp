#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
class CThingCreatureBase { public: int id; };
class CThing { public: int id; };
class CAnimationEntry { public: int id; };
class CBaseObject { public: int id; };
class CCharString { public:
    const char* m_text;
    CCharString(const char* text, long length = -1) : m_text(text) { (void)length; }
};
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static CAnimationEntry g_anim;
static const char* g_lookedUpName = 0;
static CThingCreatureBase* g_lookedUpOwner = 0;
CAnimationEntry* FASTCALL GetRandomLoadedAnimation(CThingCreatureBase* owner, const CCharString& name)
{
    g_lookedUpOwner = owner; g_lookedUpName = name.m_text; return &g_anim;
}
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct SBaseArgs { CThingCreatureBase* owner; float a; float b; CAnimationEntry* anim; long first; long last; long c; long d; };
static SBaseArgs g_base;
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase() { }
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d)
    {
        g_base.owner = owner; g_base.a = a; g_base.b = b; g_base.anim = animation;
        g_base.first = sound.FirstSound; g_base.last = sound.LastSound; g_base.c = c; g_base.d = d;
    }
};
class CCreatureAction_ShowLove : public CCreatureActionBase { public:
    CIntelligentPointer PCreatureLoving;
    CCreatureAction_ShowLove(CThingCreatureBase* owner);
};
CCreatureAction_ShowLove::CCreatureAction_ShowLove(CThingCreatureBase* owner)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetRandomLoadedAnimation(owner, "STANDARD_EXCITED"), CSoundPair(), 0, 0)
    , PCreatureLoving((CBaseObject*)owner)
{
}
int main()
{
    CThingCreatureBase owner; owner.id = 7;
    CThing thing; thing.id = 9;
    (void)thing;
    g_base.first = 5; g_base.last = 5;
    CCreatureAction_ShowLove action(&owner);
    bool ok = g_base.owner == &owner && g_base.a == -1.0f && g_base.b == 0.0f && g_base.anim == &g_anim
        && g_base.first == 0 && g_base.last == 0 && g_base.c == 0 && g_base.d == 0
        && g_lookedUpOwner == &owner && std::strcmp(g_lookedUpName, "STANDARD_EXCITED") == 0 && action.PCreatureLoving.GetPItem() == (CBaseObject*)&owner;
    if (!ok) { std::printf("008438c0_TEST FAIL\n"); return 1; }
    std::printf("008438c0_TEST PASS\n");
    return 0;
}