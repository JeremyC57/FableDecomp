// Behaviour test for 0x006DFB90 CCreatureAction_ShowTrophy copy constructor.
#include <cstdio>
struct CThing { int id; };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* p) { m_pItem = p; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CCreatureActionBase { int m_base; CCreatureActionBase() : m_base(4) { } CCreatureActionBase(const CCreatureActionBase& o) : m_base(o.m_base) { } };
struct CCreatureAction_ShowTrophy : CCreatureActionBase {
    CIntelligentPointer PTrophy; int TrophyState; unsigned char Shown; CIntelligentPointer PTrophyObject;
    CCreatureAction_ShowTrophy() : TrophyState(0), Shown(0) { }
    CCreatureAction_ShowTrophy(const CCreatureAction_ShowTrophy& other)
        : CCreatureActionBase(other), PTrophy(other.PTrophy), TrophyState(other.TrophyState), Shown(other.Shown), PTrophyObject(other.PTrophyObject) { }
};
int main()
{
    CThing t1, t2;
    CCreatureAction_ShowTrophy a; a.PTrophy.SetPItem(&t1); a.TrophyState = 3; a.Shown = 1; a.PTrophyObject.SetPItem(&t2);
    CCreatureAction_ShowTrophy b(a);
    if (b.m_base != 4 || b.PTrophy.GetPItem() != &t1 || b.TrophyState != 3 || b.Shown != 1 || b.PTrophyObject.GetPItem() != &t2) { std::printf("006dfb90_TEST FAIL\n"); return 1; }
    std::printf("006dfb90_TEST PASS\n");
    return 0;
}
