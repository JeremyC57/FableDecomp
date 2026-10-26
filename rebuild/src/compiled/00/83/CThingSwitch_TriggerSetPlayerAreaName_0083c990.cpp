// CThingSwitch::TriggerSetPlayerAreaName() -- once: when the living hero stands inside the switch's trigger square and has a region display, show the switch's script name as the current area name
// Leaf: TriggerSetPlayerAreaName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083C990, length 255 bytes (+ int3 padding).
// Callees: CThing::GetPosition() const @ 0x006A4D00 (four times), the player-manager accessor @ 0x004C7A00,
// CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroCreature @ 0x00487DC0, C2DBoxF::ContainsPoint(const
// C2DVector&) const @ 0x00535CD0 (out of line), CVectorMap::LowerBound @ 0x0040F020 (interface 0x8e = CTCRegionDisplay,
// bit 14 of +0x30), CTCRegionDisplay::SetCurrentAreaName(const CCharString&, float, bool, bool, bool) @ 0x00639BA0.
// Layout (retail): CThing ScriptName +0x74, PhysicsTC +0x60 (position +0x0c), interface bitset +0x20 / map +0x44, Dead
// bit +0x91; CThingSwitch TriggerRadius +0xd0, the area-name-shown flag +0xdc (Triggered in Ego_r.pdb at that offset).
// Shape: the sibling TriggerSetEnvironment 0x0083BE90 with the box test out of line (the C2DVector conversion
// temporary in the frame, passed by reference); the flag is set between the argument pushes and the call.
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    bool ContainsPoint(const C2DVector& p) const;              // 0x00535CD0
};
class CCharString { public: void* PStringData; };
class CTCBase;
class CTCRegionDisplay {
public:
    void SetCurrentAreaName(const CCharString& name, float time, bool a, bool b, bool c);   // 0x00639BA0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
};
class CTCPhysicsBase { public:
    unsigned char _pad00[0x0c];
    C3DVector     Position;                                    // +0x0c
};
class CThing {
public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];                          // +0x20
    CInterfaceMap   m_interfaces;                              // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                                 // +0x60
    unsigned char   _pad64[0x74 - 0x64];
    CCharString     ScriptName;                                // +0x74
    unsigned char   _pad78[0x91 - 0x78];
    unsigned char   Dead : 1;                                  // +0x91 bit 0
    const C3DVector& GetPhysicsPosition() const { return PhysicsTC->Position; }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CPlayer {
public:
    CThing* GetHeroCreature();                                 // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer();                                  // 0x00449970
};
class CThingSwitch : public CThing {
public:
    unsigned char _pad94[0xd0 - 0x94];
    float         TriggerRadius;                               // +0xd0
    CCharString   EnvironmentDef;                              // +0xd4
    float         TimeToChangeEnvironmentDef;                  // +0xd8
    bool          AreaNameShown;                               // +0xdc
    const C3DVector& GetPosition() const;                      // 0x006A4D00
    CPlayerManager* GetPlayerManager();                        // 0x004C7A00
    void TriggerSetPlayerAreaName();
};

void CThingSwitch::TriggerSetPlayerAreaName()
{
    if (AreaNameShown)
        return;
    C2DBoxF area;
    area.TLX = GetPosition().X - TriggerRadius;
    area.TLY = GetPosition().Y - TriggerRadius;
    area.BRX = GetPosition().X + TriggerRadius;
    area.BRY = GetPosition().Y + TriggerRadius;
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
    if (hero && !hero->Dead) {
        const C3DVector& pos = hero->GetPhysicsPosition();
        if (area.ContainsPoint(C2DVector(pos)) && hero->HasTC(0x8e)) {
        CTCRegionDisplay* display = (CTCRegionDisplay*)hero->GetTC(0x8e);
        AreaNameShown = true;
        display->SetCurrentAreaName(ScriptName, -1.0f, false, false, true);
        }
    }
}