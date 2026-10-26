#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
class CBaseObject { public: int id; };
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
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class CCPPointerInfo { public: long m_count; };
class CCreatureActionBase { public: int id; };
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* Data;
    CCPPointerInfo* Info;
    CCountedPointer_CCreatureActionBase() : Data(0), Info(0) { }
    CCountedPointer_CCreatureActionBase(const CCountedPointer_CCreatureActionBase& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};
class CCreatureAction_CastAssassinRush : public CActionBase { public:
    bool Override;
    C3DVector Dir;
    float Distance;
    long Level;
    CCountedPointer_CCreatureActionBase FinalAction;
    bool UseCollision;
    bool IsZTargetting;
    CCreatureAction_CastAssassinRush() { }
    CCreatureAction_CastAssassinRush(const CCreatureAction_CastAssassinRush& other);
};
CCreatureAction_CastAssassinRush::CCreatureAction_CastAssassinRush(const CCreatureAction_CastAssassinRush& other)
    : CActionBase(other)
    , Override(other.Override)
    , Dir(other.Dir)
    , Distance(other.Distance)
    , Level(other.Level)
    , FinalAction(other.FinalAction)
    , UseCollision(other.UseCollision)
    , IsZTargetting(other.IsZTargetting)
{
}
int main()
{
    CBaseObject a; a.id = 1;
    CBaseObject b; b.id = 2;
    (void)a; (void)b;
    CCreatureAction_CastAssassinRush src;
    src.m_baseWord = 77;
    static CCreatureActionBase finalAction; static CCPPointerInfo info; info.m_count = 1;
    src.Override = true; src.Dir.X = 1.0f; src.Dir.Y = 2.0f; src.Dir.Z = 3.0f; src.Distance = 4.5f; src.Level = 2;
    src.FinalAction.Data = &finalAction; src.FinalAction.Info = &info; src.UseCollision = false; src.IsZTargetting = true;
    CCreatureAction_CastAssassinRush copy(src);
    bool ok = g_baseCopies == 1 && copy.m_baseWord == 77 && copy.Override && copy.Dir.X == 1.0f && copy.Dir.Y == 2.0f && copy.Dir.Z == 3.0f && copy.Distance == 4.5f && copy.Level == 2 && copy.FinalAction.Data == src.FinalAction.Data && copy.FinalAction.Info == src.FinalAction.Info && src.FinalAction.Info->m_count == 2 && !copy.UseCollision && copy.IsZTargetting && (CCreatureAction_CastAssassinRush(CCreatureAction_CastAssassinRush()), true);
    if (!ok) { std::printf("008cef50_TEST FAIL\n"); return 1; }
    std::printf("008cef50_TEST PASS\n");
    return 0;
}