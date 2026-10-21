// Grade: DIFFER(443v443) -- register swap only: retail keeps this in ebx and the temporaries' vtable constant / reloaded other in ebp (ours the reverse), and loads this->Number into the dead this register
// CPlayer::SwitchPlayerCharactersWith(CPlayer* other) -- swap the player characters (and controlled creatures) of two players; refuses (false) when either is the main player without a living character; afterwards each living character gets its initial player mode and its player number
// Leaf: SwitchPlayerCharactersWith
// Retail: Fable.exe (TLC, VC7.1) @ 0x004888C0, 443 bytes (448 with padding), `ret 4`.
// Callees: CPlayerManager::GetMainPlayerNumber 0x00449700 (through the inlined IsMainPlayer, as the landed
//   CPlayer::UninitCharacter 0x00487BD0), CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90 /
//   CBaseIntelligentPointer() 0x00A01B10 / ~CBaseIntelligentPointer 0x00A01C10 (the CIntelligentPointer temporaries,
//   vtable 0x01231C4C, as the landed ctor 0x0048BD00), CPlayer::SetupInitialPlayerMode 0x004887C0, the character's
//   player-number setter 0x004C7440 (manifest "CTCPhysicsNavigator::EnableNavigator" is propagated).
// Layout: engine/CPlayer.h: PlayerManager +0x10, Number +0x28, ControlledCreature +0x2c, PlayerCharacter +0x34
//   (8-byte intelligent pointers); CThing Dead bit at +0x91 bit 0.
// Shape: IS_THING_ALIVE is a macro (GetPItem evaluated twice); the raw pointers saved before the swap go back
//   through converting-constructor temporaries; the trailing checks are an empty inline debug hook.
#include "engine/CPlayer.h"

struct CPlayerManager {
    long GetMainPlayerNumber() const;              // 0x00449700
};
struct CBaseObject;
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                        // +0x91 bit 0
    void SetPlayerNumber(long number);             // 0x004C7440 (descriptive)
    void DebugCheck() const { }
};
#define IS_THING_ALIVE(p) ((p) && !(p)->Dead)

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pLink;                                 // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    ~CBaseIntelligentPointer();                    // 0x00A01C10
    CThing* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CThing* item);                   // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01231C4C
    virtual void v00();
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CPlayer_Methods : CPlayer {
    bool IsMainPlayer() const                      // out of line @ 0x00487520
    {
        long number = this->Number;
        return number == this->PlayerManager->GetMainPlayerNumber();
    }
    CIntelligentPointer& Character() { return *reinterpret_cast<CIntelligentPointer*>(this->PlayerCharacter); }
    CIntelligentPointer& Controlled() { return *reinterpret_cast<CIntelligentPointer*>(this->ControlledCreature); }
    void SetupInitialPlayerMode();                 // 0x004887C0
    bool SwitchPlayerCharactersWith(CPlayer_Methods* other);
};

bool CPlayer_Methods::SwitchPlayerCharactersWith(CPlayer_Methods* other)
{
    if (other->IsMainPlayer() && !IS_THING_ALIVE(other->Character().GetPItem()))
        return false;
    if (IsMainPlayer() && !IS_THING_ALIVE(Character().GetPItem()))
        return false;
    CThing* character = Character().GetPItem();
    CThing* controlled = Controlled().GetPItem();
    Character() = other->Character();
    Controlled() = other->Character();
    other->Character() = character;
    other->Controlled() = controlled;
    if (IS_THING_ALIVE(Character().GetPItem())) {
        SetupInitialPlayerMode();
        Character().GetPItem()->SetPlayerNumber(Number);
    }
    if (IS_THING_ALIVE(other->Character().GetPItem())) {
        other->SetupInitialPlayerMode();
        other->Character().GetPItem()->SetPlayerNumber(other->Number);
    }
    if (Character().GetPItem())
        Character().GetPItem()->DebugCheck();
    if (other->Character().GetPItem())
        other->Character().GetPItem()->DebugCheck();
    return true;
}
