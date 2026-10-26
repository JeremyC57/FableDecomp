// Behaviour test for CThingSoundEmitter::OnCreate (0x00836740): base OnCreate, then a sound object from the game state.
#include <cstdio>
class C3DVector { public: float x, y, z; };
struct CSoundPlayer { int id; };
static CSoundPlayer g_player;
static const C3DVector* g_lastPos = 0;
static bool g_returnNull = false;
static bool g_baseResult = true;

struct CGameState {
    virtual CSoundPlayer* CreateSoundPlayer(const C3DVector& pos) { g_lastPos = &pos; return g_returnNull ? 0 : &g_player; }
};
static CGameState* g_pGameState = 0;

class CThing { public: bool OnCreate() { return g_baseResult; } };

class CThingSoundEmitter : public CThing { public:
    CSoundPlayer* m_pSoundPlayer;
    C3DVector     Pos;
    bool OnCreate()
    {
        if (CThing::OnCreate() && g_pGameState) {
            m_pSoundPlayer = g_pGameState->CreateSoundPlayer(Pos);
            if (m_pSoundPlayer)
                return true;
        }
        return false;
    }
};

int main()
{
    CGameState state;
    CThingSoundEmitter e;
    e.m_pSoundPlayer = 0;
    if (e.OnCreate()) { std::printf("00836740_TEST FAIL\n"); return 1; }          // no game state
    g_pGameState = &state;
    g_baseResult = false;
    if (e.OnCreate() || g_lastPos) { std::printf("00836740_TEST FAIL\n"); return 1; }   // base failed: no query
    g_baseResult = true;
    if (!e.OnCreate() || e.m_pSoundPlayer != &g_player || g_lastPos != &e.Pos) { std::printf("00836740_TEST FAIL\n"); return 1; }
    g_returnNull = true;
    if (e.OnCreate() || e.m_pSoundPlayer != 0) { std::printf("00836740_TEST FAIL\n"); return 1; }
    std::printf("00836740_TEST PASS\n");
    return 0;
}