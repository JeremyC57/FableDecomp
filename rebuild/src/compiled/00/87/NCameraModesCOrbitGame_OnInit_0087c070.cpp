// NCameraModes::COrbitGame::OnInit() -- when the hero is playing a tavern game, orbit the camera around a point 1 unit above the game's thing, starting at distance 1 with the default orbit angles, a 0.2 FOV and the previous mode's Z offset
// Grade: RELOCATION_MATCH (agent902) -- the current mode is read into a local BEFORE the DefaultFOV store: retail loads `[ecx+98h]` above the `[esi+4]` store and the Z offset after it, and VC7.1 cannot move a load across a store through a different base, so the source order is mode read, FOV store, offset read.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087C070, 214 bytes, plain `ret`.
// Leaf: ?OnInit@COrbitGame
// Name: vtable 0x0125C6DC slot 21 (the CBase OnInit slot, as CSimplePointBase::OnInit 0x008734F0); FableWin.pdb
//       ?OnInit@COrbitGame@NCameraModes@@UAEXXZ.
// Callees: NCameraModes::CBase::GetWorld 0x0084D050, CWorld::GetMainPlayer 0x0049D9D0, CPlayer::GetHeroThing 0x00487DC0,
//          CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inlined out-parameter GetInterface for CTCGamePlayer,
//          type 0xd0 = bit 16 of the bitset word +0x38), CTCGamePlayer::GetGame 0x005EDC90 (twice), CThing::GetPosition 0x004C73D0.
//          The callee names and the GetInterface model are the landed COrbitGame::Update 0x0087C150's.
// Layout: Ego_r.pdb COrbitGame Focus +0xc8, FocusDistance +0xd4, ZAngle +0xd8, XAngle +0xdc, 0x24 lower in retail (+0xa4..+0xb8);
//         CBase DefaultFOV +0x04, CameraManager +0x40, ObservedPositionZOffset +0x8c; CGameCameraManager CurrentMode +0x98.
// Shape: the Z lift adds a double literal 1.0 (`fadd qword`); the constant stores, the manager load and the copy of the current
//        mode's Z offset are scheduled around it.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CInterfaceMap {                               // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;            // 0x004C73D0
};
class CTCTavernGame { public:
    void*   __vftable;
    CThing* ParentThing;                             // +0x04 (CTCBase)
};
class CTCGamePlayer { public:
    CTCTavernGame* GetGame() const;                  // 0x005EDC90
};
class CPlayer { public:
    CThing* GetHeroThing() const;                    // 0x00487DC0
};
class CWorld { public:
    CPlayer* GetMainPlayer() const;                  // 0x0049D9D0
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
    CThing* hero = GetWorld().GetMainPlayer()->GetHeroThing();
    CTCGamePlayer* player;
    hero->GetInterface(&player, 0xd0);
    if (player && player->GetGame()) {
        Focus = player->GetGame()->ParentThing->GetPosition();
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