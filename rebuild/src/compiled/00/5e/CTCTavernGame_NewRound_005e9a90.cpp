// CTCTavernGame::NewRound() -- start a new round of the tavern game: the game is in session again
// Leaf: ?NewRound@CTCTavernGame@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E9A90, 5 bytes (`mov byte ptr [ecx+29h],1 ; ret`).
// Identity: vtable slot 32 of CTCTavernGame (rtti-vtables.tsv: 0x012560BC slot 32 = 0x005E9A90); Ego_r/FableWin.pdb name
//   set ?NewRound@CTCTavernGame@@UAEXXZ (public virtual); callers: CTCTavernGameCardBase::NewGame 0x008F0960 (direct base
//   call) and the coin game's slot-32 override 0x008EF180, which tail-jumps here. Retail +0x29 is GameInSession (the
//   landed CTCTavernGame constructor 0x005EC8A0 retail layout: STLport 12-byte UIComponents vector, GameFinished +0x28,
//   GameInSession +0x29; Ego_r.pdb +0x31 / CTCTavernGame.h disagree -- donor Dinkumware vector). Was landed as a
//   gap-scan stub `C005e9a90::M` (Leaf ??0CXMVPlaybackInfo@, a propagated label) and catalogued CTCTavernGame::vfunc_32.
class CThing;
namespace NUISystem { class CComponent; }

template <class T> class CVector {                  // STLport vector<T> (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing*       ParentThing;                      // +0x04
    bool          ToBeRemoved;                      // +0x08
    bool          IsInGlobalUpdateTCs;              // +0x09
};

class CTCTavernGame : public CTCBase {              // vtable 0x012560BC
public:
    CVector<NUISystem::CComponent*> UIComponents;   // +0x0c
    double        LastRenderTime;                   // +0x18
    bool          IsBeingPlayed;                    // +0x20
    bool          GameWon;                          // +0x21
    long          NumRounds;                        // +0x24
    bool          GameFinished;                     // +0x28
    bool          GameInSession;                    // +0x29
    virtual void NewRound();
};

void CTCTavernGame::NewRound()
{
    GameInSession = true;
}
