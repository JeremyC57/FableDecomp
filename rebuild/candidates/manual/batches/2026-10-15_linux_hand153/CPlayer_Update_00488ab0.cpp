// CPlayer::Update() -- per-frame player tick: set up the initial player mode once loading has finished, count the respawn delay down, and keep the world map's record of this player's position in step with the controlled creature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00488AB0, length 111 bytes.
// Callees: the component loader's IsLoading @ 0x004349E0, CPlayer::SetupInitialPlayerMode @ 0x004887C0,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CThing::GetPosition @ 0x006A4D00, the world's GetWorldMap
// (vtable slot 12, +0x30), CWorldMap::SetPlayerPos(long, const C3DVector&, ELoadType) @ 0x005063E0.
// Layout: engine/CPlayer.h (Component +0x14, World +0x1C, Number +0x28, ControlledCreature +0x2C,
// PlayerRespawnDelay +0x3C); InitialPlayerModeSet is retail +0x216 (PDB +0x21E: PlayerModes is 8 bytes narrower).
// Shape: the landed CPlayer::InitialiseIfUninitialised body (0x00488A80) inlined first; the position update is
// `World->GetWorldMap()->SetPlayerPos(Number, creature->GetPosition(), 0)` (world in edi, its vtable read after the
// argument pushes).
#include "engine/CPlayer.h"

struct CLoaderCalls {
    bool IsLoading();                                       // 0x004349E0
};

struct CMainGameComponentCalls {
    unsigned char _pad_0x00[0x28];
    CLoaderCalls* Loader;                                   // +0x28 (name inferred)
};

struct C3DVector;
struct CThingCalls {
    const C3DVector& GetPosition() const;                   // 0x006A4D00
};

struct CBaseObject;
struct CBaseIntelligentPointer {
    void*        vfptr;
    CBaseObject* m_pItem;
    CBaseObject* GetPItem();                                // 0x00A01B50
};

typedef long ELoadType;
struct CWorldMapCalls {
    void SetPlayerPos(long player, const C3DVector& position, ELoadType loadType);   // 0x005063E0
};

struct CWorldCalls {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMapCalls* GetWorldMap();                  // slot 12 (+0x30)
};

struct CPlayer_Methods : CPlayer {
    void SetupInitialPlayerMode();                          // 0x004887C0
    void InitialiseIfUninitialised()                        // 0x00488A80 (landed), inlined here
    {
        bool* initialPlayerModeSet = (bool*)&this->_pad_0x20d[0x216 - 0x20D];   // +0x216 InitialPlayerModeSet (PDB +0x21E)
        if (!*initialPlayerModeSet) {
            if (!reinterpret_cast<CMainGameComponentCalls*>(this->Component)->Loader->IsLoading()) {
                *initialPlayerModeSet = true;
                SetupInitialPlayerMode();
            }
        }
    }
    CBaseIntelligentPointer& ControlledCreaturePointer() { return *reinterpret_cast<CBaseIntelligentPointer*>(this->ControlledCreature); }
    void Update();
};

void CPlayer_Methods::Update()
{
    InitialiseIfUninitialised();
    if (PlayerRespawnDelay > 0)
        --PlayerRespawnDelay;
    if (ControlledCreaturePointer().GetPItem())
        reinterpret_cast<CWorldCalls*>(World)->GetWorldMap()->SetPlayerPos(Number, ((CThingCalls*)ControlledCreaturePointer().GetPItem())->GetPosition(), 0);
}
