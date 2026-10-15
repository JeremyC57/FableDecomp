// CPlayer::GlobalEventCreatureDied(CThing* creature) -- when the creature that died is this player's character: the main player enters the dead mode (3), any other player is handed to the player manager's death handling
// Grade: DIFFER(99v102) -- the non-main-player arm: retail re-reads Number (`mov eax,[ebp+28h] ; push eax`), ours pushes the copy it loaded before GetMainPlayerNumber (`push esi`); tried the plain compare, a 64-bit id local (fixed the id compare), an inline IsMainPlayer(), an inline else helper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087C580, length 100 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CPlayerManager::GetMainPlayerNumber @ 0x00449700,
// CPlayer::AddMode(EPlayerMode, bool) @ 0x006345C0, and the player manager's per-player handler @ 0x00449B40
// (`GetPlayer(n)->...` at 0x00487BD0; its manifest label CEngine::GetEditorAnimation is propagated).
// Layout: engine/CPlayer.h (PlayerManager +0x10, Number +0x28, PlayerCharacter +0x34); CThing UniqueID (unsigned
// __int64) +0x18 (Ego_r.pdb).
// Shape: the dead creature's 64-bit id is loaded before the second GetPItem and compared word by word (one __int64
// equality, the id in a local); the main-player test is an inline IsMainPlayer() (call result on the left, the
// number read before the call and NOT reused by the else arm, which re-reads it).
#include "engine/CPlayer.h"

typedef long EPlayerMode;

struct CThingIdentity {
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                              // +0x18
};

struct CBaseObject;
struct CBaseIntelligentPointer {
    void*        vfptr;
    CBaseObject* m_pItem;
    CBaseObject* GetPItem();                                // 0x00A01B50
};

struct CPlayerManagerCalls {
    long GetMainPlayerNumber();                             // 0x00449700
    void OnPlayerCharacterDied(long player);                // 0x00449B40 (descriptive)
};

struct CPlayer_Methods : CPlayer {
    void AddMode(EPlayerMode mode, bool flag);              // 0x006345C0
    CBaseIntelligentPointer& PlayerCharacterPointer() { return *reinterpret_cast<CBaseIntelligentPointer*>(this->PlayerCharacter); }
    bool IsMainPlayer() { return reinterpret_cast<CPlayerManagerCalls*>(PlayerManager)->GetMainPlayerNumber() == Number; }
    void GlobalEventCreatureDied(CThingIdentity* creature);
};

void CPlayer_Methods::GlobalEventCreatureDied(CThingIdentity* creature)
{
    if (creature && PlayerCharacterPointer().GetPItem()) {
        unsigned __int64 id = creature->UniqueID;
        if (id == ((CThingIdentity*)PlayerCharacterPointer().GetPItem())->UniqueID) {
            if (IsMainPlayer())
                AddMode(3, true);
            else
                reinterpret_cast<CPlayerManagerCalls*>(PlayerManager)->OnPlayerCharacterDied(Number);
        }
    }
}
