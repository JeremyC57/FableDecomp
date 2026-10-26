// Behaviour test for CQ_SecretPassageScript::EndMission (0x00DF0A50): prison escape, black fade, no fading on the
// next region change, the quest completed and deactivated, the hero teleported to the PrisonHSP marker, and the
// marker's counted pointer released (pointee deleted) when the function returns.
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long = -1) : PStringData(text) { ++g_live; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_live; }
    ~CCharString() { --g_live; }
};
class CCPPointerInfo { public:
    long RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = 0; Info = 0;
    }
};
class CBaseClass { public: CBaseClass() {} virtual ~CBaseClass() {} };
class CScriptThing : public CBaseClass { public:
    CCountedPointer<CScriptThing> PImp;
    CScriptThing() { PImp.Data = 0; PImp.Info = 0; }
    CScriptThing(const CScriptThing& o) : CBaseClass() { PImp.Data = o.PImp.Data; PImp.Info = o.PImp.Info; if (PImp.Info) ++PImp.Info->RefCount; }
    ~CScriptThing() {}
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& o) { R = o.R; G = o.G; B = o.B; A = o.A; }
};
static int g_deleted = 0;
static void DeleteThing(void*) { ++g_deleted; }
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static CScriptThing g_hero;
static CScriptThing g_markerThing;
class CGameScriptInterface { public:
    float dur, hold; CRGBColour col; bool allow; const CScriptThing* tpEntity; const CScriptThing* tpTarget; long refAtTp;
    CGameScriptInterface() : col(1, 1, 1, 1) {}
    virtual CScriptThing& GetHero() { Log("hero"); return g_hero; }
    virtual CScriptThing GetThingWithScriptName(const CCharString& n)
    {
        Log("find"); Log(n.PStringData);
        CScriptThing t; t.PImp.Data = &g_markerThing; t.PImp.Info = new CCPPointerInfo; t.PImp.Info->RefCount = 1;
        t.PImp.Info->DeleteFunc = DeleteThing; t.PImp.Info->Data = &g_markerThing;
        return t;
    }
    virtual void DeactivateQuestLater(const CCharString& q, unsigned long d) { Log("deact"); Log(q.PStringData); if (d) Log("delay"); }
    virtual void SetQuestAsCompleted(const CCharString& q, bool a, bool b, bool c) { Log("done"); Log(q.PStringData); if (a || b || c) Log("flag"); }
    virtual bool FadeScreenOut(float d, float h, CRGBColour c) { Log("fade"); dur = d; hold = h; col = c; return true; }
    virtual void SetAllowScreenFadingOnNextRegionChange(bool a) { Log("allow"); allow = a; }
    virtual void EntityTeleportToThing(const CScriptThing& e, const CScriptThing& t, bool k)
    { Log("tp"); tpEntity = &e; tpTarget = &t; refAtTp = t.PImp.Info ? t.PImp.Info->RefCount : -1; if (k) Log("keep"); }
    virtual CCharString GetActiveQuestName() { Log("name"); return CCharString("Q_Prison"); }
};
struct CQ_SecretPassageScript {
    CGameScriptInterface* m_pInterface;
    void PrisonEscape() { Log("escape"); }
    void EndMission();
};
void CQ_SecretPassageScript::EndMission()
{
    PrisonEscape();
    m_pInterface->FadeScreenOut(0.5f, 0.0f, CRGBColour(0, 0, 0, 255));
    m_pInterface->SetAllowScreenFadingOnNextRegionChange(false);
    CScriptThing marker = m_pInterface->GetThingWithScriptName("PrisonHSP");
    m_pInterface->SetQuestAsCompleted(m_pInterface->GetActiveQuestName(), false, false, false);
    m_pInterface->DeactivateQuestLater(m_pInterface->GetActiveQuestName(), 0);
    m_pInterface->EntityTeleportToThing(m_pInterface->GetHero(), marker, false);
}
int main()
{
    CGameScriptInterface gi; gi.allow = true; gi.tpEntity = 0; gi.tpTarget = 0;
    CQ_SecretPassageScript s; s.m_pInterface = &gi;
    g_log[0] = 0;
    s.EndMission();
    const char* want = "escape;fade;allow;find;PrisonHSP;name;done;Q_Prison;name;deact;Q_Prison;hero;tp;";
    float half = 0.5f;
    int bad = std::strcmp(g_log, want) != 0;
    if (gi.dur != half || gi.hold != 0.0f || gi.col.R != 0 || gi.col.G != 0 || gi.col.B != 0 || gi.col.A != 255) bad = 1;
    if (gi.allow || gi.tpEntity != &g_hero || gi.refAtTp != 1 || g_deleted != 1 || g_live != 0) bad = 1;
    if (bad) { std::printf("00df0a50_TEST FAIL\n"); return 1; }
    std::printf("00df0a50_TEST PASS\n");
    return 0;
}