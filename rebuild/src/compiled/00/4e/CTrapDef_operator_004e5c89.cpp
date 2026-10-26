// CTrapDef::operator=(const CTrapDef&) -- def copy: CDefClassBase part, then the CTrapDef fields memberwise in declaration order
// Leaf: ??4CTrapDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E5C89, 105 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x004E5C84 in slot 19 of vtable 0x01243054 = CTrapDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10, map<CCharString,long>::operator= @ 0x00432B38.
// Layout (engine/CTrapDef.h, PDB minus 4 after the 12-byte retail map): Sound +0x28, TriggerType +0x34, TrapType +0x38, TriggerRadius +0x3c, TriggerHelperName +0x40, TrapDamage +0x44, TriggeredCollisionMesh +0x48, UntriggeredCollisionMesh +0x4c, ExplosionDefIndex +0x50, PhysicalObstructionDefIndex +0x54, NumShotsToFireBeforeIdle +0x58, SecondsIdle +0x5c, SecondsBetweenShots +0x60.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
// TriggerType/TrapType are 4-byte enums (long).
#pragma optimize("s", on)

class CDefClassBase { public:
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CSoundNameMap {                       // retail std::map<CCharString,long> (12 bytes) (retail 0xc bytes)
    unsigned long m_words[3];
    CSoundNameMap& operator=(const CSoundNameMap& other);   // 0x00432B38
};
class CDefString { public:
    long TablePos;                      // index into the def string table (trivially copied)
};

class CTrapDef : public CDefClassBase { public:
    CSoundNameMap            Sound;                       // +0x28
    long                     TriggerType;                 // +0x34
    long                     TrapType;                    // +0x38
    float                    TriggerRadius;               // +0x3c
    CDefString               TriggerHelperName;           // +0x40
    float                    TrapDamage;                  // +0x44
    long                     TriggeredCollisionMesh;      // +0x48
    long                     UntriggeredCollisionMesh;    // +0x4c
    long                     ExplosionDefIndex;           // +0x50
    long                     PhysicalObstructionDefIndex; // +0x54
    long                     NumShotsToFireBeforeIdle;    // +0x58
    float                    SecondsIdle;                 // +0x5c
    float                    SecondsBetweenShots;         // +0x60
    CTrapDef& operator=(const CTrapDef& other);
};

CTrapDef& CTrapDef::operator=(const CTrapDef& other)
{
    CDefClassBase::operator=(other);
    Sound = other.Sound;
    TriggerType = other.TriggerType;
    TrapType = other.TrapType;
    TriggerRadius = other.TriggerRadius;
    TriggerHelperName = other.TriggerHelperName;
    TrapDamage = other.TrapDamage;
    TriggeredCollisionMesh = other.TriggeredCollisionMesh;
    UntriggeredCollisionMesh = other.UntriggeredCollisionMesh;
    ExplosionDefIndex = other.ExplosionDefIndex;
    PhysicalObstructionDefIndex = other.PhysicalObstructionDefIndex;
    NumShotsToFireBeforeIdle = other.NumShotsToFireBeforeIdle;
    SecondsIdle = other.SecondsIdle;
    SecondsBetweenShots = other.SecondsBetweenShots;
    return *this;
}