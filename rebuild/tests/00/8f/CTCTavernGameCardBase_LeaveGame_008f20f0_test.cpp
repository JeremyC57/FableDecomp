#include <cstdio>
#include <cstring>
// Behaviour test for CTCTavernGameCardBase::LeaveGame (0x008F20F0): the body of src/ over stand-ins -- the base leave,
// live particle effects killed and the effect list cleared, the table cards dropped, every pack card's thing killed and
// the card freed, the selection cleared and the camera released and reset.
static int g_baseLeave = 0, g_kills = 0, g_killThing = 0, g_release = 0, g_reset = 0, g_deleted = 0;
class CThing { public:
    bool dead;
    int killed;
    bool IsDead() const { return dead; }
    void Kill(bool) { ++killed; ++g_kills; }
};
template<class T> class CIntelligentPointer { public:
    T* item;
    T* Get() const { return item; }
};
struct CEffectVector {
    typedef CIntelligentPointer<CThing> T;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    void clear() { _M_finish = _M_start; }
};
class CCard { public: CThing* Thing; };
struct CCardVector {
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CCard*& back() { return *(_M_finish - 1); }
    void pop_back() { --_M_finish; }
    void clear() { _M_finish = _M_start; }
};
class CGameCameraManager { public:
    void ReleaseScriptedControl() { ++g_release; }
    void ResetCamera() { if (g_release) ++g_reset; }
};
static CGameCameraManager g_camera;
struct CCameraManagerSource { bool GetCameraManager(CGameCameraManager** camera) { *camera = &g_camera; return true; } };
static CCameraManagerSource g_source;
class CWorld { public: CCameraManagerSource* GetCameraSource() { return &g_source; } };
static CWorld g_world;
class CTCTavernGame { public:
    void LeaveGame() { ++g_baseLeave; }
    void KillThing(CThing* thing) { ++thing->killed; ++g_killThing; }
    CWorld* GetWorld() { return &g_world; }
};
struct CTCTavernGameCardBase : CTCTavernGame {
    CCardVector   Cards;
    CCardVector   Pack;
    CEffectVector ParticleEffects;
    CCard*        SelectedCard;
    CCard*        HighlightedCard;
    void LeaveGame();
};

void CTCTavernGameCardBase::LeaveGame()
{
    CTCTavernGame::LeaveGame();
    for (unsigned long i = 0; i < ParticleEffects.size(); ++i) {
        const CIntelligentPointer<CThing>& effect = ParticleEffects[i];
        if (effect.Get() && !effect.Get()->IsDead())
            ParticleEffects[i].Get()->Kill(true);
    }
    ParticleEffects.clear();
    Cards.clear();
    while (Pack.size() > 0) {
        CCard* card = Pack.back();
        Pack.pop_back();
        KillThing(card->Thing);
        ++g_deleted;
        delete card;
    }
    SelectedCard = 0;
    HighlightedCard = 0;
    CGameCameraManager* camera;
    GetWorld()->GetCameraSource()->GetCameraManager(&camera);
    camera->ReleaseScriptedControl();
    camera->ResetCamera();
}

static int fail(int line) { printf("008f20f0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CThing effects[3]; memset(effects, 0, sizeof(effects));
    effects[1].dead = true;
    CIntelligentPointer<CThing> eff[4] = { { &effects[0] }, { &effects[1] }, { 0 }, { &effects[2] } };
    CThing cardThings[2]; memset(cardThings, 0, sizeof(cardThings));
    CCard* tableCards[1] = { 0 };
    CCard* pack[2] = { new CCard, new CCard };
    pack[0]->Thing = &cardThings[0]; pack[1]->Thing = &cardThings[1];
    CCard dummy;
    CTCTavernGameCardBase game;
    game.ParticleEffects._M_start = eff; game.ParticleEffects._M_finish = eff + 4; game.ParticleEffects._M_end_of_storage = eff + 4;
    game.Cards._M_start = tableCards; game.Cards._M_finish = tableCards + 1; game.Cards._M_end_of_storage = tableCards + 1;
    game.Pack._M_start = pack; game.Pack._M_finish = pack + 2; game.Pack._M_end_of_storage = pack + 2;
    game.SelectedCard = &dummy; game.HighlightedCard = &dummy;
    game.LeaveGame();
    CHECK(g_baseLeave == 1);
    CHECK(g_kills == 2 && effects[0].killed == 1 && effects[1].killed == 0 && effects[2].killed == 1);
    CHECK(game.ParticleEffects.size() == 0 && game.Cards.size() == 0 && game.Pack.size() == 0);
    CHECK(g_killThing == 2 && cardThings[0].killed == 1 && cardThings[1].killed == 1 && g_deleted == 2);
    CHECK(game.SelectedCard == 0 && game.HighlightedCard == 0);
    CHECK(g_release == 1 && g_reset == 1);
    printf("008f20f0_TEST PASS\n");
    return 0;
}