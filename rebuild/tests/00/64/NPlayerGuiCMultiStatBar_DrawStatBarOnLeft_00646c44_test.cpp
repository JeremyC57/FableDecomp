// Behaviour test for NPlayerGui::CMultiStatBar::DrawStatBarOnLeft @ 0x00646C44: queue front pickup and the two countdowns.
#include <cstdio>
#include <cstring>
class CRGBColour { public:
    unsigned char B, G, R, A;
};
class C2DVector { public:
    float X, Y;
};
class CGraphicDataBank;
class CEnginePrimitivePrepareInfo { public:
    float FrameTime;                       // +0x00
};

struct CGameState {
    unsigned char _pad00[0xa4];
    bool          Paused;                  // +0xa4
};
class CGame { public:
    unsigned char _pad00[0x24];
    CGameState*   GameState;               // +0x24
};
static CGameState g_state; static CGame g_game; static CGame* g_pGame = &g_game;

static int g_pops = 0;
namespace NPlayerGui {

enum EStatUpdateType { STAT_UPDATE_NONE = 0, STAT_UPDATE_GOLD = 5, STAT_UPDATE_COUNT = 6 };

template <class T> struct CVector {        // STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};

struct CStatBarOwner {                     // the owning player GUI def (only the members read here)
    unsigned char       _pad0[0x520];
    float               UpdateDisplayTime; // +0x520
    float               UpdateWaitTime;    // +0x524
    unsigned char       _pad528[0x8];
    long                GoldGainGraphic;   // +0x530
    unsigned char       _pad534[0xc];
    CVector<CRGBColour> TypeColours;       // +0x540
    CVector<long>       TypeGraphics;      // +0x54c
    CVector<long>       TypeEffects;       // +0x558
};

class CMultiStatBar { public:
    class CStatUpdate { public:                   // 12 bytes
        EStatUpdateType Type;
        long            Change;
        float           TimeRemaining;
    };
    struct CUpdateNode {                   // STLport _List_node<CStatUpdate>
        CUpdateNode* _M_next;
        CUpdateNode* _M_prev;
        CStatUpdate  _M_data;
    };
    struct CUpdateList {                   // STLport list<CStatUpdate>: the sentinel pointer
        CUpdateNode* _M_node;
        bool empty() const { return _M_node->_M_next == _M_node; }
        CStatUpdate& front() { return _M_node->_M_next->_M_data; }
        void pop_front() { CUpdateNode* n = _M_node->_M_next; n->_M_prev->_M_next = n->_M_next; n->_M_next->_M_prev = n->_M_prev; ++g_pops; }
    };

    void*           __vftable;             // +0x00
    CStatBarOwner*  POwner;                // +0x04
    unsigned char   _pad0[0x20];
    long            DisplayMode;           // +0x28
    unsigned char   _pad1[0x3c];
    CUpdateList     Updates;               // +0x68
    EStatUpdateType CurrentType;           // +0x6c
    CRGBColour      CurrentColour;         // +0x70
    long            CurrentGraphic;        // +0x74
    long            CurrentEffect;         // +0x78
    float           CurrentTimeLeft;       // +0x7c
    float           WaitingTimeLeft;       // +0x80

    void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos,
                           const C2DVector& size, const CRGBColour& colour, unsigned char alpha, bool flash);
};

void CMultiStatBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank&, const C2DVector&,
                                      const C2DVector&, const CRGBColour&, unsigned char, bool)
{
    if (DisplayMode != 3 && !Updates.empty()) {
        CStatUpdate& update = Updates.front();
        CurrentColour = POwner->TypeColours._M_start[update.Type];
        CurrentGraphic = POwner->TypeGraphics._M_start[update.Type];
        CurrentEffect = POwner->TypeEffects._M_start[update.Type];
        if (update.Type == STAT_UPDATE_GOLD && update.Change > 0)
            CurrentGraphic = POwner->GoldGainGraphic;
    }
    if (!Updates.empty()) {
        if (DisplayMode == 0) {
            CStatUpdate& update = Updates.front();
            if (!g_pGame->GameState->Paused && update.Type != CurrentType) {
                CurrentType = update.Type;
                update.TimeRemaining = POwner->UpdateDisplayTime;
            }
            if (!g_pGame->GameState->Paused) {
                if ((update.TimeRemaining -= info.FrameTime / POwner->UpdateDisplayTime) <= 0.0f) {
                    DisplayMode = 3;
                    WaitingTimeLeft = POwner->UpdateWaitTime;
                    if (Updates.empty())
                        CurrentType = STAT_UPDATE_COUNT;
                }
            }
        } else if (DisplayMode == 1) {
            if (!g_pGame->GameState->Paused) {
                if ((WaitingTimeLeft -= info.FrameTime / POwner->UpdateWaitTime) <= 0.0f) {
                    if (!Updates.empty())
                        Updates.pop_front();
                    DisplayMode = 2;
                }
            }
        }
    }
}

}
using namespace NPlayerGui;
int main()
{
    g_game.GameState = &g_state; g_state.Paused = false;
    static CRGBColour colours[6]; static long graphics[6]; static long effects[6];
    for (int i = 0; i < 6; ++i) { colours[i].R = (unsigned char)(10 + i); graphics[i] = 100 + i; effects[i] = 200 + i; }
    CStatBarOwner owner; std::memset(&owner, 0, sizeof(owner));
    owner.UpdateDisplayTime = 2.0f; owner.UpdateWaitTime = 1.0f; owner.GoldGainGraphic = 999;
    owner.TypeColours._M_start = colours; owner.TypeGraphics._M_start = graphics; owner.TypeEffects._M_start = effects;
    CMultiStatBar::CUpdateNode sentinel, node;
    sentinel._M_next = &node; sentinel._M_prev = &node; node._M_next = &sentinel; node._M_prev = &sentinel;
    node._M_data.Type = STAT_UPDATE_GOLD; node._M_data.Change = 3; node._M_data.TimeRemaining = 0;
    CMultiStatBar bar; bar.POwner = &owner; bar.DisplayMode = 0; bar.Updates._M_node = &sentinel; bar.CurrentType = STAT_UPDATE_NONE;
    bar.CurrentGraphic = 0; bar.CurrentEffect = 0; bar.WaitingTimeLeft = 0;
    CEnginePrimitivePrepareInfo info; info.FrameTime = 1.0f;
    CRGBColour c; C2DVector v;
    bar.DrawStatBarOnLeft(info, reinterpret_cast<const CGraphicDataBank&>(info), v, v, c, 0, false);   // picks up type 5, timer = 2 then -0.5
    int ok = bar.CurrentColour.R == 15 && bar.CurrentGraphic == 999 && bar.CurrentEffect == 205 && bar.CurrentType == STAT_UPDATE_GOLD
          && node._M_data.TimeRemaining == 1.5f && bar.DisplayMode == 0;
    info.FrameTime = 4.0f;
    bar.DrawStatBarOnLeft(info, reinterpret_cast<const CGraphicDataBank&>(info), v, v, c, 0, false);   // 1.5 - 2 <= 0 -> done, waiting time set
    ok = ok && bar.DisplayMode == 3 && bar.WaitingTimeLeft == 1.0f && bar.CurrentType == STAT_UPDATE_GOLD;
    bar.DisplayMode = 1; info.FrameTime = 0.5f;
    bar.DrawStatBarOnLeft(info, reinterpret_cast<const CGraphicDataBank&>(info), v, v, c, 0, false);
    ok = ok && bar.WaitingTimeLeft == 0.5f && bar.DisplayMode == 1 && g_pops == 0;
    g_state.Paused = true;
    bar.DrawStatBarOnLeft(info, reinterpret_cast<const CGraphicDataBank&>(info), v, v, c, 0, false);
    ok = ok && bar.WaitingTimeLeft == 0.5f;
    g_state.Paused = false; info.FrameTime = 1.0f;
    bar.DrawStatBarOnLeft(info, reinterpret_cast<const CGraphicDataBank&>(info), v, v, c, 0, false);
    ok = ok && g_pops == 1 && bar.DisplayMode == 2 && bar.Updates.empty();
    if (!ok) { std::printf("00646c44_TEST FAIL\n"); return 1; }
    std::printf("00646c44_TEST PASS\n");
    return 0;
}