// NCameraModes::COrbitGame::OnInit() -- when the hero is playing a tavern game, orbit the camera around a point 1 unit above the game's thing, starting at distance 1 with the default orbit angles, a 0.2 FOV and the previous mode's Z offset
// Grade: RELOCATION_MATCH (agent902) -- the current mode is read into a local BEFORE the DefaultFOV store: retail loads `[ecx+98h]` above the `[esi+4]` store and the Z offset after it, and VC7.1 cannot move a load across a store through a different base, so the source order is mode read, FOV store, offset read.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087C070, 214 bytes, plain `ret`.
// Leaf: ?OnInit@COrbitGame
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Name: vtable 0x0125C6DC slot 21 (the CBase OnInit slot, as CSimplePointBase::OnInit 0x008734F0); FableWin.pdb
//       ?OnInit@COrbitGame@NCameraModes@@UAEXXZ.
// Callees: NCameraModes::CBase::GetWorld 0x0084D050, CWorld::PeekMyPlayer 0x0049D9D0, CPlayer::GetPControlledCreature 0x00487DC0,
//          CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inlined out-parameter GetInterface for CTCGamePlayer,
//          type 0xd0 = bit 16 of the bitset word +0x38), CTCGamePlayer::GetGame 0x005EDC90 (twice), CThing::GetPos 0x004C73D0.
//          The callee names and the GetInterface model are the landed COrbitGame::Update 0x0087C150's.
// Layout: Ego_r.pdb COrbitGame Focus +0xc8, FocusDistance +0xd4, ZAngle +0xd8, XAngle +0xdc, 0x24 lower in retail (+0xa4..+0xb8);
//         CBase DefaultFOV +0x04, CameraManager +0x40, ObservedPositionZOffset +0x8c; CGameCameraManager CurrentMode +0x98.
// Shape: the Z lift adds a double literal 1.0 (`fadd qword`); the constant stores, the manager load and the copy of the current
//        mode's Z offset are scheduled around it.
class CTCBase;
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CTCTavernGame { public:
    void*   __vftable;
    CThing* ParentThing;                             // +0x04 (CTCBase)
};
class CTCGamePlayer { public:
    CTCTavernGame* GetGame() const;                  // 0x005EDC90
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                    // 0x00487DC0
};
class CWorld { public:
public:
    const CPlayer* PeekMyPlayer() const;   // 0x0049D9D0
};
namespace NCameraModes {
class CBase;
}
class CGameCameraManager { public:                          // retail offsets
    unsigned char        _pad000[0x98];
    NCameraModes::CBase* CurrentMode;                // +0x98
};
namespace NCameraModes {
class CBase { public:
    virtual void v00();
    float               DefaultFOV;                  // +0x04
    float               DefaultRollAngle;            // +0x08
    float               DefaultObservedPositionZOffset;   // +0x0c
    unsigned char       LookAt[0x30];                // +0x10 CPhysicsState
    CGameCameraManager* CameraManager;               // +0x40
    void*               PCameraModeDef;              // +0x44
    void*               SubComponents[3];            // +0x48
    unsigned char       GoTo[0x30];                  // +0x54 CPhysicsState
    float               CameraFOV;                   // +0x84
    float               RollAngle;                   // +0x88
    float               ObservedPositionZOffset;     // +0x8c
    unsigned char       _pad90[0xa4 - 0x90];
    const CWorld& GetWorld() const;                  // 0x0084D050
};
struct COrbitGame : CBase {
    C3DVector Focus;                                 // +0xa4
    float     FocusDistance;                         // +0xb0
    float     ZAngle;                                // +0xb4
    float     XAngle;                                // +0xb8
    virtual void OnInit();
};
void COrbitGame::OnInit()
{
    CThing* hero = GetWorld().PeekMyPlayer()->GetPControlledCreature();
    CTCGamePlayer* player;
    hero->GetTC(TCI_GAME_PLAYER, &player);
    if (player && player->GetGame()) {
        Focus = player->GetGame()->ParentThing->GetPos();
        Focus.Z += 1.0;
        FocusDistance = 1.0f;
        ZAngle = 0.0f;
        XAngle = 0.134f;
        const CBase* mode = CameraManager->CurrentMode;
        DefaultFOV = 0.2f;
        ObservedPositionZOffset = mode->ObservedPositionZOffset;
    }
}
}