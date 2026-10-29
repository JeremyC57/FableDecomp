// NCameraModes::COrbitGame::Update() -- while the hero is playing a tavern game, take the coin-golf game's camera position and target into the camera's go-to and look-at positions (and clear the observed-position Z offset)
// Leaf: Update
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
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
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
    hero->GetInterface(&player, 0xd0);
    if (player && player->GetGame()) {
        CTCTavernGameCoinGolf* golf;
        player->GetGame()->ParentThing->GetInterface(&golf, 0xcf);
        C3DVector position;
        C3DVector target;
        golf->GetCamera(position, target);
        ObservedPositionZOffset = 0.0f;
        GoTo.Position = position;
        LookAt.Position = target;
    }
}

}