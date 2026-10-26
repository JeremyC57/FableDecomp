// Behaviour test for 0x008bcbe0 CCreatureAction_ReleaseWillLightningSpell constructor: action code, members, animation name, def lookup and the sound played;
// the def's reference count is back to its starting value afterwards.
#include <cstdio>
#include <cstring>
class CCharString { public: char text[64]; CCharString(const char* s) { std::strcpy(text, s); } };
struct CDef { long RefCount; long CastSound; long ReleaseSound; int destroyed; virtual void Destroy() { destroyed = 1; } };
static __inline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointer {
    CDef* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    CDef* operator->() const { return Object; }
};
static CDef g_def;
static char g_requested[64];
class CDefinitionManager { public:
    long GetGlobalIndexFromClassIndex(const CCharString& name, long) const { std::strcpy(g_requested, name.text); return 7; }
    bool GetDef(long index, CDefPointer* out) const { if (index != 7) return false; out->Object = &g_def; ++g_def.RefCount; return true; }
    static CDefinitionManager& Get() { static CDefinitionManager m; return m; }
};
static long g_playedId = -1;
struct CDefStringTable { CCharString GetString(long id) { g_playedId = id; return CCharString("sound"); } };
static CDefStringTable g_DefStringTable;
struct CTCSound { int calls; void PlaySound(const CCharString& s, long, long, long, long d) { if (std::strcmp(s.text, "sound") == 0 && d == -1) ++calls; } };
class CThing { public: bool hasSound; CTCSound sound; bool GetInterface(CTCSound** out, long type) { if (type == 0x10 && hasSound) { *out = &sound; return true; } return false; } };
class CCombatActionBase { public:
    CThing* creature; long code; char anim[64];
    CCombatActionBase(CThing* owner, long actionCode) : creature(owner), code(actionCode) { anim[0] = 0; }
    void SetAnimationName(const CCharString& s) { std::strcpy(anim, s.text); }
};
struct CAction : CCombatActionBase {
    long m_a, m_b;
    CAction(CThing* owner);
};
CAction::CAction(CThing* owner) : CCombatActionBase(owner, 145), m_a(0), m_b(0)
{
    { CCharString animation("WILL_FINISH_LIGHTNING"); SetAnimationName(animation); }
    CDefPointer def;
    long index;
    { CCharString name("SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF"); index = CDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1); }
    CDefinitionManager::Get().GetDef(index, &def);
    CTCSound* sound;
    if (creature->GetInterface(&sound, 0x10)) { CCharString s = g_DefStringTable.GetString(def->ReleaseSound); sound->PlaySound(s, 0, 0, 0, -1); }
}
int main()
{
    g_def.RefCount = 1; g_def.CastSound = 11; g_def.ReleaseSound = 22; g_def.destroyed = 0;
    CThing c; c.hasSound = true; c.sound.calls = 0;
    CAction a(&c);
    int ok = a.code == 145 && std::strcmp(a.anim, "WILL_FINISH_LIGHTNING") == 0 && std::strcmp(g_requested, "SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF") == 0;
    ok = ok && c.sound.calls == 1 && g_playedId == g_def.ReleaseSound && g_def.RefCount == 1 && !g_def.destroyed && a.m_a == 0;
    CThing q; q.hasSound = false; q.sound.calls = 0; g_playedId = -1;
    CAction b(&q);
    ok = ok && q.sound.calls == 0 && g_playedId == -1 && g_def.RefCount == 1;
    if (!ok) { std::printf("008bcbe0_TEST FAIL\n"); return 1; }
    std::printf("008bcbe0_TEST PASS\n");
    return 0;
}