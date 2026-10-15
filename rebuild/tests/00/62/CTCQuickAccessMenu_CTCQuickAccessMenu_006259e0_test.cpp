#include <cstdio>
struct CThing { int id; };
static int g_globalAdds = 0;
struct CTCBase {
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase() { }
    void AddToGlobalUpdateTCs() { if (!IsInGlobalUpdateTCs) { IsInGlobalUpdateTCs = true; ++g_globalAdds; } }
};
struct CIntelligentPointer {
    void* PData;
    CIntelligentPointer() : PData(0) { }
    virtual ~CIntelligentPointer() { }
};
struct CComponent;
struct CRetailVector_Component {
    CComponent** _Myfirst; CComponent** _Mylast; CComponent** _Myend;
    CRetailVector_Component() : _Myfirst(0), _Mylast(0), _Myend(0) { }
};
struct CTCQuickAccessMenu : CTCBase {
    bool Active; CRetailVector_Component Components; bool Quitting; double LastRenderTime; bool FirstRenderFrame;
    CTCQuickAccessMenu(CThing* thing) : CTCBase(thing), Active(false), Components(), Quitting(false) { AddToGlobalUpdateTCs(); }
};
int main()
{
    CThing t; t.id = 2;
    CTCQuickAccessMenu m(&t);
    if (m.ParentThing != &t || m.Active || m.Quitting || m.Components._Myfirst || m.Components._Mylast || m.Components._Myend ||
        !m.IsInGlobalUpdateTCs || g_globalAdds != 1) { std::printf("006259e0_TEST FAIL\n"); return 1; }
    std::printf("006259e0_TEST PASS\n");
    return 0;
}