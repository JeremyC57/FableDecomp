#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct CThingCreatureBase { int id; };
struct CThing { int id; };
struct CBaseObject { int id; };
static int g_stringCopies = 0;
struct CCharString {
    const char* m_text;
    CCharString(const char* text) : m_text(text) { }
    CCharString(const CCharString& other) : m_text(other.m_text) { ++g_stringCopies; }
};
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
struct SBaseArgs { CThingCreatureBase* owner; long first; long last; long c; long d; };
static SBaseArgs g_base;
struct CCreatureActionBase {
    virtual ~CCreatureActionBase() { }
    CCreatureActionBase(CThingCreatureBase* owner, const CSoundPair& sound, long c, long d)
    { g_base.owner = owner; g_base.first = sound.FirstSound; g_base.last = sound.LastSound; g_base.c = c; g_base.d = d; }
};
struct CCreatureAction_ActivateQuest : CCreatureActionBase {
    CCharString         ScriptName;
    bool                LoadResources;
    CIntelligentPointer PObjectToUse;
    CCreatureAction_ActivateQuest(CThingCreatureBase* owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources);
};
CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(CThingCreatureBase* owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources)
    : CCreatureActionBase(owner, CSoundPair(), 0, 0)
    , ScriptName(scriptName)
    , LoadResources(loadResources)
    , PObjectToUse(objectToUse)
{
}
int main()
{
    CThingCreatureBase owner; owner.id = 1;
    CThing object; object.id = 2;
    CCharString name("QuestScript");
    g_base.first = 5; g_base.last = 5; g_base.c = 9; g_base.d = 9;
    CCreatureAction_ActivateQuest action(&owner, &object, name, true);
    bool ok = g_base.owner == &owner && g_base.first == 0 && g_base.last == 0 && g_base.c == 0 && g_base.d == 0
        && g_stringCopies == 1 && std::strcmp(action.ScriptName.m_text, "QuestScript") == 0 && action.LoadResources
        && action.PObjectToUse.GetPItem() == (CBaseObject*)&object;
    if (!ok) { std::printf("00843f50_TEST FAIL\n"); return 1; }
    std::printf("00843f50_TEST PASS\n");
    return 0;
}