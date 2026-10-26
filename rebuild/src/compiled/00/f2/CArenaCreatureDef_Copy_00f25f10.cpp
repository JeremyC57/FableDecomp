// CArenaCreatureDef::Copy(const CArenaCreatureDef&) -- copy assignment: the 0x28-byte def base is copied by its own
// out-of-line Copy, then CreatureType, NumCreatures, HUDType and DeathScore memberwise; returns *this.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F25F10, length 57 bytes.
// Callees: CDefBase::Copy 0x00431F10 (the base assignment, labelled CThingBuildingDef::operator= by propagation),
//          CCharString::operator= 0x0099EFB0 (twice).
// Layout (PDB CArenaCreatureDef, 0x38 bytes): base 0x28, CreatureType +0x28, NumCreatures +0x2c, HUDType +0x30,
// DeathScore +0x34 -- the members are copied in declaration order, exactly as the compiler-generated assignment would.
class CCharString { public:
    void* PStringData;
    CCharString& operator=(const CCharString& o);        // 0x0099EFB0
};

struct CDefBase {
    unsigned char _body[0x28];                           // vptr + def header
    CDefBase& Copy(const CDefBase& o);                   // 0x00431F10
};

class CArenaCreatureDef : public CDefBase { public:
    CCharString CreatureType;                            // +0x28
    long NumCreatures;                                   // +0x2c
    CCharString HUDType;                                 // +0x30
    long DeathScore;                                     // +0x34
    CArenaCreatureDef& Copy(const CArenaCreatureDef& o);
};

CArenaCreatureDef& CArenaCreatureDef::Copy(const CArenaCreatureDef& o)
{
    CDefBase::Copy(o);
    CreatureType = o.CreatureType;
    NumCreatures = o.NumCreatures;
    HUDType = o.HUDType;
    DeathScore = o.DeathScore;
    return *this;
}