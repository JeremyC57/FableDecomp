#include <cstdio>

struct CThing { int id; };
struct CTCTavernGame { int id; };

struct CTCBase {
    virtual ~CTCBase() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* owner) : ParentThing(owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    virtual int Kind() const { return 0; }
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
};

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    virtual int Kind() const { return 1; }
    CIntelligentPointer_CThing() : CBaseIntelligentPointer() { }
};

struct CTCGamePlayer : CTCBase {
    CTCTavernGame*             Game;
    CIntelligentPointer_CThing GameTable;
    CTCGamePlayer(CThing* owner) : CTCBase(owner), Game(0), GameTable() { }
};

int main()
{
    CThing owner = { 7 };
    CTCGamePlayer p(&owner);
    if (p.ParentThing != &owner || p.Game != 0 || p.GameTable.m_pItem != 0 || p.GameTable.Kind() != 1 || p.ToBeRemoved)
        { std::printf("004d8fdd_TEST FAIL\n"); return 1; }
    std::printf("004d8fdd_TEST PASS\n");
    return 0;
}
