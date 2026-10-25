// behaviour test for 0x005E9A90 CTCTavernGame::NewRound(), linked against the built member: only GameInSession (+0x29)
// is set
#include <cstdio>
#include <cstring>
#include <cstddef>
class CThing;
namespace NUISystem { class CComponent; }
template <class T> class CVector { public: T* _M_start; T* _M_finish; T* _M_end_of_storage; };
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
};
class CTCTavernGame : public CTCBase {
public:
    CVector<NUISystem::CComponent*> UIComponents; double LastRenderTime; bool IsBeingPlayed; bool GameWon; long NumRounds;
    bool GameFinished; bool GameInSession;
    virtual void NewRound();
};
int main()
{
    if (offsetof(CTCTavernGame, GameInSession) != 0x29) { std::printf("005e9a90_TEST FAIL layout\n"); return 1; }
    static unsigned char mem[sizeof(CTCTavernGame)];
    std::memset(mem, 0, sizeof(mem));
    CTCTavernGame* g = (CTCTavernGame*)mem;
    g->CTCTavernGame::NewRound();
    bool ok = g->GameInSession && !g->GameFinished && !g->GameWon && !g->IsBeingPlayed;
    unsigned int set = 0;
    for (unsigned int i = 0; i < sizeof(mem); ++i) if (mem[i]) ++set;
    ok = ok && set == 1;
    if (!ok) { std::printf("005e9a90_TEST FAIL\n"); return 1; }
    std::printf("005e9a90_TEST PASS\n");
    return 0;
}
