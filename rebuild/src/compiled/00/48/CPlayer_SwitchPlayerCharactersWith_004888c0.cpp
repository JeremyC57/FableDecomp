// CPlayer::SwitchPlayerCharactersWith(CPlayer& other) -- swap the player characters (and controlled creatures) of two players; refuses (false) when either is the main player without a living character; afterwards each living character gets its initial player mode and its player's number
// Leaf: ?SwitchPlayerCharactersWith@CPlayer@@QAE_NAAV1@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004888C0, 443 bytes (448 with padding), `ret 4`; Ego_r.pdb / FableWin.pdb
//   ?SwitchPlayerCharactersWith@CPlayer@@QAE_NAAV1@@Z (other is a reference).
// Callees: CPlayerManager::GetMainPlayerNumber 0x00449700 (through the inline CPlayer::IsMainPlayer() const), CBaseIntelligentPointer
//   GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ctor 0x00A01B10 / dtor 0x00A01C10 (the CIntelligentPointer<CThingPlayerCreature>
//   temporaries, vtable 0x01231C4C), CPlayer::SetupInitialPlayerMode 0x004887C0 (private), the character's player-number setter
//   0x004C7440 (descriptive CThing::SetPlayerNumber; manifest "CTCPhysicsNavigator::EnableNavigator" is propagated).
// Layout: engine/CPlayer.h retail branch (FABLE_RETAIL_PLAYER + FABLE_RETAIL_CINTELLIGENTPOINTER): PlayerManager +0x10, Number +0x28,
//   ControlledCreature +0x2c, PlayerCharacter +0x34 (CIntelligentPointer<CThingPlayerCreature>); new nested opt-in
//   FABLE_RETAIL_PLAYER_SWITCHCHARACTERS declares this member, SetupInitialPlayerMode and IsMainPlayer. CThing Dead bit +0x91 bit 0.
// Fix (chunk 762): the shared engine/CIntelligentPointer.h (and CBaseObject.h) replace the local pointer model (24 -> 8 lines: the
//   vtable constant / this registers), and each player number is read into a named local before the character's GetPItem call,
//   so VC7.1 loads Number into the dead player register and pushes it first, as retail does (`mov ebx,[ebx+28h] ; push ebx`).
// Shape: IS_THING_ALIVE is a macro (GetPItem evaluated twice); the raw pointers saved before the swap go back through
//   converting-constructor temporaries; the trailing checks are an empty inline debug hook.
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_SWITCHCHARACTERS
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

class CThing : public CBaseObject { public:
    unsigned char _pad08[0x91 - 0x08];
    unsigned char Dead : 1;                        // +0x91 bit 0
    void SetPlayerNumber(long number);             // 0x004C7440 (descriptive)
    void DebugCheck() const { }
};
class CThingPlayerCreature : public CThing { };
#define IS_THING_ALIVE(p) ((p) && !(p)->Dead)

inline bool CPlayer::IsMainPlayer() const
{
    long number = Number;
    return number == PlayerManager->GetMainPlayerNumber();
}

bool CPlayer::SwitchPlayerCharactersWith(CPlayer& other)
{
    if (other.IsMainPlayer() && !IS_THING_ALIVE(other.PlayerCharacter.Get()))
        return false;
    if (IsMainPlayer() && !IS_THING_ALIVE(PlayerCharacter.Get()))
        return false;
    CThingPlayerCreature* character = PlayerCharacter.Get();
    CThingPlayerCreature* controlled = ControlledCreature.Get();
    PlayerCharacter = other.PlayerCharacter;
    ControlledCreature = other.PlayerCharacter;
    other.PlayerCharacter = CIntelligentPointer<CThingPlayerCreature>(character);
    other.ControlledCreature = CIntelligentPointer<CThingPlayerCreature>(controlled);
    if (IS_THING_ALIVE(PlayerCharacter.Get())) {
        SetupInitialPlayerMode();
        long number = Number;
        PlayerCharacter.Get()->SetPlayerNumber(number);
    }
    if (IS_THING_ALIVE(other.PlayerCharacter.Get())) {
        other.SetupInitialPlayerMode();
        long otherNumber = other.Number;
        other.PlayerCharacter.Get()->SetPlayerNumber(otherNumber);
    }
    if (PlayerCharacter.Get())
        PlayerCharacter.Get()->DebugCheck();
    if (other.PlayerCharacter.Get())
        other.PlayerCharacter.Get()->DebugCheck();
    return true;
}