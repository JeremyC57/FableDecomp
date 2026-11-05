// CTCCoopSpirit::SwapToHero() -- hand the co-op spirit's player back to its hero: remember the main player and the spirit's
// position, swap the spirit's player slot to the hero, re-initialise the main player's character as its spirit def, put the
// main player's creature at the spirit's position (physics ResetPosition) and refill the spirit thing's health to its maximum.
// Leaf: ?SwapToHero@CTCCoopSpirit@@AAEXXZ
// Identity: Ego_r.pdb 0x00677640 + FableWin.pdb 0x020D73C0 (?SwapToHero@CTCCoopSpirit@@AAEXXZ, private).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066FF20, length 175 bytes, plain `ret`, 0x10-byte frame (position, the
//   GetSpiritDefName CCharString temporary).
// Callees: CTCBase::GetPlayerManager 0x00686D40 (x5), CPlayerManager::GetMainPlayerNumber 0x00449700, CThing::GetPos 0x004C73D0,
//   CPlayerManager::SwapToHero 0x00449CB0, CPlayerManager::GetSpiritDefName 0x00449730, CPlayerManager::InitialisePlayerCharacterAs
//   0x00449B20, CCharString::~CCharString 0x0099EAE0, CPlayerManager::GetPlayer 0x004498C0, CPlayer::GetPControlledCreature
//   0x00487DC0, CTCPhysicsBase::ResetPosition (virtual slot 31, +0x7C).
// Layout (retail): CThing PhysicsTC +0x60, PlayerNumber +0x90 (signed char); CThingPhysical MaxHealth +0xB0 / Health +0xB4
//   (engine/CThingPhysical.h offsets). The callee classes are method-only views (their shared headers do not declare these
//   members); `this` is CTCBase (engine/CTCBase.h, retail branch).
// Shape: the hero creature is a named local (else VC7.1 computes the argument address before the physics pointer, +2 bytes);
//   the player number is read through GetParentThing() (a `ParentThing->` read is hoisted above the position copy into ecx).
//   Plain C++ (asm cleanup round 2b; replaces the oracle byte bake).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#include "engine/C3DVector.h"
#include "fable_string.h"

class CTCPhysicsBase { public:                             // vtable view up to slot 31
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& position);   // slot 31 (+0x7c)
};
class CThing { public:                                      // retail CThing / CThingPhysical offsets used here
    unsigned char   _pad_0x00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
    unsigned char   _pad_0x64[0x2c];
    signed char     PlayerNumber;                           // +0x90
    unsigned char   _pad_0x91[0x1f];
    float           MaxHealth;                              // +0xb0 (CThingPhysical)
    float           Health;                                 // +0xb4 (CThingPhysical)
    const C3DVector& GetPos() const;                        // 0x004C73D0
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager { public:
    long        GetMainPlayerNumber() const;                // 0x00449700
    void        SwapToHero(long playerNumber);              // 0x00449CB0
    CCharString GetSpiritDefName(long playerNumber);        // 0x00449730
    void        InitialisePlayerCharacterAs(long playerNumber, const CCharString& defName);   // 0x00449B20
    CPlayer&    GetPlayer(long playerNumber);               // 0x004498C0
};

class CTCCoopSpirit : public CTCBase {
    void SwapToHero();
};

void CTCCoopSpirit::SwapToHero()
{
    long mainPlayer = GetPlayerManager().GetMainPlayerNumber();
    C3DVector position = ParentThing->GetPos();
    GetPlayerManager().SwapToHero(GetParentThing().PlayerNumber);
    GetPlayerManager().InitialisePlayerCharacterAs(mainPlayer, GetPlayerManager().GetSpiritDefName(mainPlayer));
    CThingPlayerCreature* hero = GetPlayerManager().GetPlayer(mainPlayer).GetPControlledCreature();
    hero->PhysicsTC->ResetPosition(position);
    ParentThing->Health = ParentThing->MaxHealth;
}
