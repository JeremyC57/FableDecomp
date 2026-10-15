#include <cstdio>
struct CThing { int id; };
struct CBaseObject;
static int g_sets = 0;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase_Calls() { }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem((CBaseObject*)1) { }
    void SetPItem(CBaseObject* item) { m_pItem = item; ++g_sets; }
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer_CThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
struct CTCWaterWader : CTCBase_Calls {
    CIntelligentPointer_CThing PParticle;
    CIntelligentPointer_CThing PWaterfallParticle;
    bool OldWade;
    virtual void v00() { }
    CTCWaterWader(CThing* thing) : CTCBase_Calls(thing), PParticle(0), PWaterfallParticle(0), OldWade(false) { }
};
int main()
{
    CThing t; t.id = 3;
    CTCWaterWader w(&t);
    if (w.ParentThing != &t || g_sets != 2 || w.PParticle.m_pItem != 0 || w.PWaterfallParticle.m_pItem != 0 || w.OldWade) { std::printf("007b5b40_TEST FAIL\n"); return 1; }
    std::printf("007b5b40_TEST PASS\n");
    return 0;
}