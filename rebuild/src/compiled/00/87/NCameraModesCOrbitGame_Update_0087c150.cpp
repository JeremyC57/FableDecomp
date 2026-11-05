// NCameraModes::COrbitGame::Update() -- while the hero is playing a tavern game, take the coin-golf game's camera position and target into the camera's go-to and look-at positions (and clear the observed-position Z offset)
// Leaf: Update
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087C150, 252 bytes, plain `ret`.
// Callees: NCameraModes::CBase::GetWorld 0x0084D050, CWorld::PeekMyPlayer 0x0049D9D0, CPlayer::GetPControlledCreature 0x00487DC0,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (two inlined unchecked out-parameter GetInterface lookups:
//   CTCGamePlayer 0xd0, CTCTavernGameCoinGolf 0xcf), CTCGamePlayer::GetGame 0x005EDC90 (twice),
//   CTCTavernGameCoinGolf::GetCamera(C3DVector&, C3DVector&) 0x007D3560.
// Layout (retail NCameraModes::CBase, Ego_r.pdb less the 0x10-byte smaller CPhysicsState and the 12-byte vector):
//   LookAt +0x10 (position first), CameraManager +0x40, GoTo +0x54 (position first), ObservedPositionZOffset +0x8c.
//   Thing: interface bitset +0x20, map +0x44; CTCBase ParentThing +4.
// Shape: the GetInterface template's out pointer is never initialised (the absent arm reads the shared key word); the
//   C3DVector copies go through the implicit assignment of a class with a user copy constructor (`lea` destination).
class CTCBase;
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CTCTavernGame { public:
    void*   __vftable;
    CThing* ParentThing;                        // +0x04 (CTCBase)
};
class CTCGamePlayer { public:
    CTCTavernGame* GetGame() const;             // 0x005EDC90
};
class CTCTavernGameCoinGolf { public:
    void GetCamera(C3DVector& position, C3DVector& target);   // 0x007D3560
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;               // 0x00487DC0
};
class CWorld { public:
public:
    const CPlayer* PeekMyPlayer() const;   // 0x0049D9D0
};
struct CPhysicsState {                          // retail 0x30 bytes
    C3DVector     Position;
    unsigned char _rest[0x30 - 0x0c];
};

namespace NCameraModes {

class CBase { public:
    virtual void v00();
    float         DefaultFOV;                   // +0x04
    float         DefaultRollAngle;             // +0x08
    float         DefaultObservedPositionZOffset;   // +0x0c
    CPhysicsState LookAt;                       // +0x10
    void*         CameraManager;                // +0x40
    void*         PCameraModeDef;               // +0x44
    void*         SubComponents[3];             // +0x48
    CPhysicsState GoTo;                         // +0x54
    float         CameraFOV;                    // +0x84
    float         RollAngle;                    // +0x88
    float         ObservedPositionZOffset;      // +0x8c
    const CWorld& GetWorld() const;             // 0x0084D050
};

struct COrbitGame : CBase {
    virtual void Update();
};

void COrbitGame::Update()
{
    CThing* hero = GetWorld().PeekMyPlayer()->GetPControlledCreature();
    CTCGamePlayer* player;
    hero->GetTC(TCI_GAME_PLAYER, &player);
    if (player && player->GetGame()) {
        CTCTavernGameCoinGolf* golf;
        player->GetGame()->ParentThing->GetTC(TCI_TAVERN_GAME, &golf);
        C3DVector position;
        C3DVector target;
        golf->GetCamera(position, target);
        ObservedPositionZOffset = 0.0f;
        GoTo.Position = position;
        LookAt.Position = target;
    }
}

}