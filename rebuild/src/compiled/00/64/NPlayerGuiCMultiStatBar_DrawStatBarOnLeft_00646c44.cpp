// NPlayerGui::CMultiStatBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank&, const C2DVector&, const C2DVector&, const CRGBColour&, unsigned char, bool) -- advance the multi-stat bar's update queue by the frame time: pick up the front update's colour/graphic/effect, count its display time down (then switch to waiting) or count the wait down (then drop the update)
// Leaf: DrawStatBarOnLeft
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00646C44, length 330 bytes (unaligned: a size-optimised TU), `ret 1Ch`.
// Ego_r.pdb: `?DrawStatBarOnLeft@CMultiStatBar@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@ABVCGraphicDataBank@@ABVC2DVector@@2ABVCRGBColour@@E_N@Z`.
// Pragma: #pragma optimize("s", on) (as the landed sibling AddStatUpdate 0x00647457).
// Callees: list<CStatUpdate>::pop_front @ 0x00655A86 (manifest owner label propagated).
// Globals: the game @ 0x013B86A0 (+0x24 the game state, paused byte +0xa4), 0.0f @ 0x0122DEDC.
// Layout: as the landed AddStatUpdate: DisplayMode +0x28 (0 showing, 1 waiting, 2 updating, 3 done), Updates (STLport list, the
//   4-byte sentinel pointer) +0x68, CurrentType +0x6c, CurrentColour +0x70, CurrentGraphic +0x74, CurrentEffect +0x78,
//   WaitingTimeLeft +0x80; CStatUpdate (PDB, 12 bytes) Type, Change, TimeRemaining. The owner (+4) holds the update display
//   time +0x520, the wait time +0x524, the gold graphic +0x530 and the per-type colour/graphic/effect vectors +0x540/+0x54c/+0x558.
//   CEnginePrimitivePrepareInfo's first word is the frame time.
// Shape: the front node is re-read from the sentinel for each use; both countdowns are `t -= frameTime / period` stored with
//   `fst` and tested `<= 0`.
#pragma optimize("s", on)

struct CRGBColour {
    unsigned char B, G, R, A;
};
struct C2DVector {
    float X, Y;
};
struct CGraphicDataBank;
struct CEnginePrimitivePrepareInfo {
    float FrameTime;                       // +0x00
};

struct CGameState {
    unsigned char _pad00[0xa4];
    bool          Paused;                  // +0xa4
};
struct CGame {
    unsigned char _pad00[0x24];
    CGameState*   GameState;               // +0x24
};
extern CGame* g_pGame;                     // 0x013B86A0

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

struct CMultiStatBar {
    struct CStatUpdate {                   // 12 bytes
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
        void pop_front();                  // 0x00655A86
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