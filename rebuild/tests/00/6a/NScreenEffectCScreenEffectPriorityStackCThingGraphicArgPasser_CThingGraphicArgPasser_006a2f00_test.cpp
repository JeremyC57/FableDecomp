// Behaviour test for 006a2f00 CScreenEffectPriorityStack::CThingGraphicArgPasser(CThing*, bool) (copy of the model).
#include <cstdio>
class CThing { public: int id; };
class CThingObject : public CThing { };
static int g_ctor = 0, g_get = 0, g_set = 0;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() {}
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctor; }
    CThing* GetPItem() const { ++g_get; return m_pItem; }
    void SetPItem(CThing* item) { ++g_set; m_pItem = item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
struct CThingGraphicArgPasser {
    CIntelligentPointer<CThing> thing;
    bool                        bypassSet;
    CThingGraphicArgPasser(CThing* thing_, bool bypassSet_);
};
CThingGraphicArgPasser::CThingGraphicArgPasser(CThing* thing_, bool bypassSet_)
    : thing(thing_), bypassSet(bypassSet_)
{
}

int main()
{
    CThing t; t.id = 3;
    g_ctor = g_get = g_set = 0;
    CThingGraphicArgPasser a(&t, true);
    int ok = 1;
    if (a.thing.m_pItem != &t || !a.bypassSet || g_ctor != 1 || g_set != 1 || g_get != 0) ok = 0;
    CThingGraphicArgPasser b(0, false);
    if (b.thing.m_pItem != 0 || b.bypassSet || g_ctor != 2 || g_set != 2) ok = 0;
    if (!ok) { std::printf("006a2f00_TEST FAIL\n"); return 1; }
    std::printf("006a2f00_TEST PASS\n");
    return 0;
}