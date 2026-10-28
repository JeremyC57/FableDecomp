// CThingSwitch::TriggerSetEnvironment() -- when the living hero stands inside the switch's trigger square, transition the environment to the switch's theme def
// Leaf: TriggerSetEnvironment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BE90, length 252 bytes (+ int3 padding).
// Callees: CThing::PeekPos() const @ 0x006A4D00 (this TU's out-of-line copy, four times), the player-manager
// accessor @ 0x004C7A00, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0,
// CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&)
// @ 0x009AD410, CThing::PeekWorld @ 0x004C7AA0, CEnvironment::TransitionToTheme(long, float) @ 0x006B37A0 on the
// world's PEnvironment (+0x18).
// Layout (retail, as the landed CThingSwitch::ConstructFromParams 0x0083BD80): TriggerRadius +0xd0, EnvironmentDef
// +0xd4, TimeToChangeEnvironmentDef +0xd8; CThing PhysicsTC +0x60 (position at +0x0c), Dead bit +0x91.
// Shape: the trigger square is a C2DBoxF filled member by member (TL first, one PeekPos() call per component);
// the hero position is a C2DVector converted from the physics position (only Y lands in the frame, X stays on the
// x87 stack) and tested by the half-open ContainsPoint inlined here (the sibling TriggerSetPlayerAreaName 0x0083C990
// calls the out-of-line copy 0x00535CD0); the world is a named local so PeekWorld runs before the argument pushes.
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    __forceinline bool ContainsPoint(const C2DVector& p) const { return p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY; }
};
class CCharString { public: void* PStringData; };
class CEnvironment {
public:
    void TransitionToTheme(long theme, float time);            // 0x006B37A0
};
class CWorld {
public:
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                                // +0x18
};
class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager
{
};
class CTCPhysicsBase { public:
    unsigned char _pad00[0x0c];
    C3DVector     Position;                                    // +0x0c
};
class CPlayerManager;
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                                 // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   Dead : 1;                                  // +0x91 bit 0
    const C3DVector& GetPhysicsPosition() const { return PhysicsTC->Position; }
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x004C7990
protected:
    CPlayerManager& GetPlayerManager();   // 0x004C7A00
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                                 // 0x00487DC0
};
class CPlayerManager {
public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;                      // 0x006A4D00
};
class CThingSwitch : public CThingPhysical {
public:
    unsigned char _pad94[0xd0 - 0x94];
    float         TriggerRadius;                               // +0xd0
    CCharString   EnvironmentDef;                              // +0xd4
    float         TimeToChangeEnvironmentDef;                  // +0xd8
    CWorld* PeekWorld();                                       // 0x004C7AA0
    void TriggerSetEnvironment();
};

void CThingSwitch::TriggerSetEnvironment()
{
    C2DBoxF area;
    area.TLX = PeekPos().X - TriggerRadius;
    area.TLY = PeekPos().Y - TriggerRadius;
    area.BRX = PeekPos().X + TriggerRadius;
    area.BRY = PeekPos().Y + TriggerRadius;
    CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->Dead && area.ContainsPoint(C2DVector(hero->GetPhysicsPosition()))) {
        long theme = PeekDefinitionManager().GetDefGlobalIndexFromName(EnvironmentDef);
        if (theme) {
            CWorld* world = PeekWorld();
            world->PEnvironment->TransitionToTheme(theme, TimeToChangeEnvironmentDef);
        }
    }
}