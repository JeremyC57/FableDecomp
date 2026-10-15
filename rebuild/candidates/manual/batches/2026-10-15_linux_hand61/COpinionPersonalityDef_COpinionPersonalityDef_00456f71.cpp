// COpinionPersonalityDef::COpinionPersonalityDef() -- default constructor: def base, own vtable, the attitude-enabled map, then a fresh array of five personality-trait records and "attitudes enabled by default" set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00456F71, length 71 bytes. Callees: CParentDefClassBase::CParentDefClassBase
// @ 0x00430370, the AttitudeEnabled map constructor @ 0x004534A6 (manifest "CDisplayManager::CopyBackBufferToTexture",
// propagated), operator new[] @ 0x00BFEB22 (0xB4 bytes), `vector constructor iterator' @ 0x004038C0 with
// COpinionPersonalityTraits::COpinionPersonalityTraits @ 0x0044C26D (5 x 0x24 bytes).
// Layout: the PDB COpinionPersonalityDef (92 bytes) has PersonalityTraits +0x40, AttitudeEnabledDefaultForNotNone +0x44,
// AttitudeEnabled (map) +0x48, ToleranceToBeingHit +0x58; retail is 4 bytes lower (0x3C-byte def base) and the map is
// 12 bytes. Vtable 0x01233F8C = COpinionPersonalityDef (rtti-vtables.tsv). COpinionPersonalityTraits is 0x24 bytes
// (PDB) with an out-of-line constructor and no destructor, so `new T[5]` carries no count cookie (exactly 0xB4 bytes)
// and goes through the non-EH vector constructor iterator. Shape: a size-optimised module (#pragma optimize("s", on):
// `pop ecx` after operator new); the two scalar stores follow the member constructor, so they are body assignments.
// Leaf: ??0COpinionPersonalityDef
#pragma optimize("s", on)
class CParentDefClassBase {                        // 0x3C bytes in retail
public:
    virtual void v000();
    unsigned long _body[0x0E];
    CParentDefClassBase();                         // 0x00430370
};

class COpinionPersonalityTraits {                  // 0x24 bytes
public:
    unsigned long _body[9];
    COpinionPersonalityTraits();                   // 0x0044C26D
};

class CAttitudeEnabledMap {                        // map<EOpinionAttitudeType, bool>, 12 bytes in retail
public:
    void*         m_pHeader;                       // word-aligned: the map starts at +0x44
    unsigned long m_reserved[2];
    CAttitudeEnabledMap();                         // 0x004534A6
};

class COpinionPersonalityDef : public CParentDefClassBase {   // vtable 0x01233F8C
public:
    virtual void v000();
    COpinionPersonalityTraits* PersonalityTraits;                // +0x3C (PDB +0x40)
    bool                       AttitudeEnabledDefaultForNotNone; // +0x40 (PDB +0x44)
    CAttitudeEnabledMap        AttitudeEnabled;                  // +0x44 (PDB +0x48)
    float                      ToleranceToBeingHit;              // +0x50 (PDB +0x58)
    COpinionPersonalityDef();
};

COpinionPersonalityDef::COpinionPersonalityDef()
{
    PersonalityTraits = new COpinionPersonalityTraits[5];
    AttitudeEnabledDefaultForNotNone = true;
}
