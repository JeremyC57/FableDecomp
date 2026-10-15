#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct CBaseObject { int id; };
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
static int g_baseCopies = 0;
struct CActionBase {
    virtual ~CActionBase() { }
    long m_baseWord;
    CActionBase() : m_baseWord(0) { }
    CActionBase(const CActionBase& other) : m_baseWord(other.m_baseWord) { ++g_baseCopies; }
};
static int g_stringCopies = 0;
struct CCharString {
    const char* m_text;
    CCharString() : m_text(0) { }
    CCharString(const CCharString& other) : m_text(other.m_text) { ++g_stringCopies; }
};
struct CCreatureAction_ActivateQuest : CActionBase {
    CCharString ScriptName;
    bool LoadResources;
    CIntelligentPointer PObjectToUse;
    CCreatureAction_ActivateQuest() { }
    CCreatureAction_ActivateQuest(const CCreatureAction_ActivateQuest& other);
};
CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(const CCreatureAction_ActivateQuest& other)
    : CActionBase(other)
    , ScriptName(other.ScriptName)
    , LoadResources(other.LoadResources)
    , PObjectToUse(other.PObjectToUse)
{
}
int main()
{
    CBaseObject a; a.id = 1;
    CBaseObject b; b.id = 2;
    (void)a; (void)b;
    CCreatureAction_ActivateQuest src;
    src.m_baseWord = 77;
    src.ScriptName.m_text = "QuestScript";
    src.LoadResources = true;
    src.PObjectToUse.SetPItem(&a);
    CCreatureAction_ActivateQuest copy(src);
    bool ok = g_baseCopies == 1 && copy.m_baseWord == 77 && g_stringCopies == 1 && std::strcmp(copy.ScriptName.m_text, "QuestScript") == 0 && copy.LoadResources && copy.PObjectToUse.GetPItem() == &a;
    if (!ok) { std::printf("0084c900_TEST FAIL\n"); return 1; }
    std::printf("0084c900_TEST PASS\n");
    return 0;
}