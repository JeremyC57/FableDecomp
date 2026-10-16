// CKrakenTentacleDef::operator=(const CKrakenTentacleDef&) -- def copy: CDefClassBase part, then the CKrakenTentacleDef fields memberwise in declaration order
// Leaf: ??4CKrakenTentacleDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1494, 105 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x004E148F in slot 19 of vtable 0x0123DA24 = CKrakenTentacleDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10.
// Layout (engine/CKrakenTentacleDef.h, matches retail): TentacleSmackDownTime +0x28, TentacleTurnSpeed +0x2c, TentacleMoveSpeed +0x30, TentacleStrikeAngleTolerence +0x34, TentacleStrikeDistTolerence +0x38, TentacleHeightOffset +0x3c, TentacleTimeBeforeExplosion +0x40, TentacleTimeBetweenAttacks +0x44, TentacleTimeBeforeObstructionRemoval +0x48, TentacleObstructionRadius +0x4c, TentacleExplosionBodgeAngle +0x50, TentacleExplosionRadius +0x54, TentacleObstructionRadiusMin +0x58, TentacleObstructionRadiusMax +0x5c.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
#pragma optimize("s", on)

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CKrakenTentacleDef : CDefClassBase {
    float                    TentacleSmackDownTime;       // +0x28
    float                    TentacleTurnSpeed;           // +0x2c
    float                    TentacleMoveSpeed;           // +0x30
    float                    TentacleStrikeAngleTolerence; // +0x34
    float                    TentacleStrikeDistTolerence; // +0x38
    float                    TentacleHeightOffset;        // +0x3c
    float                    TentacleTimeBeforeExplosion; // +0x40
    float                    TentacleTimeBetweenAttacks;  // +0x44
    float                    TentacleTimeBeforeObstructionRemoval; // +0x48
    float                    TentacleObstructionRadius;   // +0x4c
    float                    TentacleExplosionBodgeAngle; // +0x50
    float                    TentacleExplosionRadius;     // +0x54
    float                    TentacleObstructionRadiusMin; // +0x58
    float                    TentacleObstructionRadiusMax; // +0x5c
    CKrakenTentacleDef& operator=(const CKrakenTentacleDef& other);
};

CKrakenTentacleDef& CKrakenTentacleDef::operator=(const CKrakenTentacleDef& other)
{
    CDefClassBase::operator=(other);
    TentacleSmackDownTime = other.TentacleSmackDownTime;
    TentacleTurnSpeed = other.TentacleTurnSpeed;
    TentacleMoveSpeed = other.TentacleMoveSpeed;
    TentacleStrikeAngleTolerence = other.TentacleStrikeAngleTolerence;
    TentacleStrikeDistTolerence = other.TentacleStrikeDistTolerence;
    TentacleHeightOffset = other.TentacleHeightOffset;
    TentacleTimeBeforeExplosion = other.TentacleTimeBeforeExplosion;
    TentacleTimeBetweenAttacks = other.TentacleTimeBetweenAttacks;
    TentacleTimeBeforeObstructionRemoval = other.TentacleTimeBeforeObstructionRemoval;
    TentacleObstructionRadius = other.TentacleObstructionRadius;
    TentacleExplosionBodgeAngle = other.TentacleExplosionBodgeAngle;
    TentacleExplosionRadius = other.TentacleExplosionRadius;
    TentacleObstructionRadiusMin = other.TentacleObstructionRadiusMin;
    TentacleObstructionRadiusMax = other.TentacleObstructionRadiusMax;
    return *this;
}
