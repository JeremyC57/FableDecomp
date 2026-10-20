// Behaviour test for 0x00E15830 CWB_WhiteBalverine::Init: gate lookup, horn sound on that gate, music override, and the
// gate handle's reference released at the end.
#include <cstdio>
#include <cstring>
struct CCharString { char text[32]; CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; } };
struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void*); void* Data; };
static int g_deleted = 0;
static void DeleteThing(void*) { ++g_deleted; }
struct CScriptThing {
    void* Data; CCPPointerInfo* Info;
    CScriptThing() : Data(0), Info(0) { }
    CScriptThing(const CScriptThing& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    virtual ~CScriptThing()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
struct CGameScriptInterface {
    CCPPointerInfo* block; char lastName[32], sound[32]; const CScriptThing* soundThing; long music; bool m1, m2;
    virtual CScriptThing GetThingWithScriptName(const CCharString& name)
    { std::strcpy(lastName, name.text); CScriptThing t; t.Info = block; ++block->RefCount; t.Data = block; return t; }
    virtual unsigned long PlayCriteriaSoundOnThing(const CScriptThing& thing, const CCharString& criteria) const
    { const_cast<CGameScriptInterface*>(this)->soundThing = &thing; std::strcpy(const_cast<CGameScriptInterface*>(this)->sound, criteria.text); return thing.Info == block ? 1 : 0; }
    virtual void OverrideMusic(long set, bool a, bool b) { music = set; m1 = a; m2 = b; }
};
struct CWB_WhiteBalverine {
    CGameScriptInterface* Game;
    void Init()
    {
        CScriptThing gate = Game->GetThingWithScriptName("KG_Gate");
        Game->PlayCriteriaSoundOnThing(gate, "KNG_HORN");
        Game->OverrideMusic(0x17, false, false);
    }
};
int main()
{
    CGameScriptInterface g; g.block = new CCPPointerInfo; g.block->RefCount = 1; g.block->DeleteFunc = DeleteThing; g.block->Data = 0;
    g.music = 0; g.m1 = g.m2 = true; g.soundThing = 0;
    CWB_WhiteBalverine w; w.Game = &g;
    w.Init();
    bool ok = std::strcmp(g.lastName, "KG_Gate") == 0 && std::strcmp(g.sound, "KNG_HORN") == 0 && g.soundThing != 0
        && g.music == 0x17 && !g.m1 && !g.m2 && g.block->RefCount == 1 && g_deleted == 0;
    if (!ok) { std::printf("00e15830_TEST FAIL\n"); return 1; }
    std::printf("00e15830_TEST PASS\n");
    return 0;
}
