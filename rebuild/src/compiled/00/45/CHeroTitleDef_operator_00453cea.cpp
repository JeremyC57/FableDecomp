// CHeroTitleDef::operator=(const CHeroTitleDef& other) -- the memberwise def assignment: the CDefClassBase part, then every CHeroTitleDef member in declaration order; returns *this
// Leaf: ??4CHeroTitleDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00453CEA, 87 bytes, thiscall `ret 4`; size-optimised def TU.
// Identity (proven): CHeroTitleDef::operator=; evidence: its only reference is the 5-byte `jmp` at 0x00453CE5 (the tail-calling virtual in slot 19 of
//   vtable 0x0123234C, RTTI .?AVCHeroTitleDef@@; as the landed CHeroExperienceDef::operator= 0x004F914A), it copies exactly the CHeroTitleDef members
//   (offsets and sizes asserted against Ego_r.pdb, at their PDB offsets), and the PDB name sets hold ??4CHeroTitleDef@@QAEAAV0@ABV0@@Z.
// Label: the manifest's "CThingBuildingDef::operator=" is propagated (family H055: the six H052 defs' assignments, each its own COMDAT).
// Callee: CDefClassBase::operator= 0x00431F10 (copies PDefManager +0x1C, GlobalIndex +0x20 and the three flag bits at +0x24).
// Layout: CDefClassBase is 0x25 bytes (pack(1) in the PDB, members from +0x28); local classes -- the generated engine headers flatten the
//   bases (no base operator= to call), Review at landing.
// Shape: the compiler-generated assignment written out in declaration order; float members copy through eax as integer moves.
#pragma optimize("s", on)
enum EHeroTitle { };                                        // enumerators not needed
class CDefClassBase {                                         // 37 bytes in retail
public:
    unsigned char _body[0x25];
    CDefClassBase& operator=(const CDefClassBase& other);        // 0x00431F10
};
class CHeroTitleDef : public CDefClassBase {                       // vtable 0x0123234C
public:
    EHeroTitle Title;                                  // +0x28
    long HeroTitleGuiTag;                              // +0x2C
    long HeroTitleGreetToHeroTag;                      // +0x30
    long HeroTitleCommentAtHeroTag;                    // +0x34
    long HeroTitleCommentToSelfTag;                    // +0x38
    long RequiredRenownLevel;                          // +0x3C
    long RequiredStrengthExpLevel;                     // +0x40
    long RequiredSkillExpLevel;                        // +0x44
    long RequiredWillExpLevel;                         // +0x48
    bool IsBuyable;                                    // +0x4C
    bool IsAutomatic;                                  // +0x4D
    CHeroTitleDef& operator=(const CHeroTitleDef& other);
};

CHeroTitleDef& CHeroTitleDef::operator=(const CHeroTitleDef& other)
{
    CDefClassBase::operator=(other);
    Title = other.Title;
    HeroTitleGuiTag = other.HeroTitleGuiTag;
    HeroTitleGreetToHeroTag = other.HeroTitleGreetToHeroTag;
    HeroTitleCommentAtHeroTag = other.HeroTitleCommentAtHeroTag;
    HeroTitleCommentToSelfTag = other.HeroTitleCommentToSelfTag;
    RequiredRenownLevel = other.RequiredRenownLevel;
    RequiredStrengthExpLevel = other.RequiredStrengthExpLevel;
    RequiredSkillExpLevel = other.RequiredSkillExpLevel;
    RequiredWillExpLevel = other.RequiredWillExpLevel;
    IsBuyable = other.IsBuyable;
    IsAutomatic = other.IsAutomatic;
    return *this;
}