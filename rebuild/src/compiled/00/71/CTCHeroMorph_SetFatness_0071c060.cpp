// CTCHeroMorph::SetFatness(float fatness) -- clamp the hero's Fatness morph value to [0, 1] and flag the morph as changed
// Leaf: ?SetFatness@CTCHeroMorph@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071C060, 93 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: CTCHeroMorph::SetFatness(float) -- proven; evidence: FableWin.pdb name set holds ?SetFatness@CTCHeroMorph@@QAEXM@Z; the
//   store goes to +0x58 = Ego_r.pdb CTCHeroMorph::Fatness (+112 there; retail sits 24 bytes lower: two 4-byte STLport
//   lists, a 12-byte vector and a 12-byte map before it), with Changed (+85 -> +0x3D) set; the eight setters are
//   consecutive in the CTCHeroMorph module (0x0071BE20..0x0071C0C0) in member order Strength, Will, Berserk, Skill, Age,
//   Morality, Fatness, Tan. Family G043 (the propagated label "CTCHeroMorph::SetAge" is right only for 0x0071BFA0).
// Shape: `Fatness = GFLimitReturn(fatness, 0.0f, 1.0f)` -- the inlined reference-returning clamp (FableWin.pdb
//   ??$GFLimitReturn@M@@YIABMABM00@Z; out of line at 0x0057EF86 elsewhere) keeps 0.0f and 1.0f in stack temporaries (the
//   result is loaded through their address, one return path per arm) while the compares use the 0.0f / 1.0f constants
//   0x0122DEDC / 0x0122DED8: `fld value ; fcomp low` (`test ah,5 ; jp`) then `fld value ; fcomp high` (`test ah,41h`).
template <class T> inline const T& __fastcall GFLimitReturn(const T& value, const T& low, const T& high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}
class CThing;
class CTCHeroMorph {                                           // retail layout (STLport containers)
public:
    void SetStrength(float strength);
    void SetWill(float will);
    void SetBerserkSkin(float berserk);
    void SetSkill(float skill);
    void SetAge(float age);
    void SetMorality(float morality);
    void SetFatness(float fatness);
    void SetTan(float tan);
    void*         VTable;                                      // +0x00 (CTCBase)
    CThing*       ParentThing;                                 // +0x04
    bool          ToBeRemoved;                                 // +0x08
    bool          IsInGlobalUpdateTCs;                         // +0x09
    unsigned char PMeshMorphSet[8];                            // +0x0c CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>
    unsigned char ParticleEmitters[4];                         // +0x14 list<CIntelligentPointer<CThing> >
    unsigned char IdleParticleEmitters[4];                     // +0x18 list<CIntelligentPointer<CThing> >
    unsigned char Tattoos[12];                                 // +0x1c vector<CDefPointer<const CThingBaseDef> >
    unsigned char CustomToBankIndex[12];                       // +0x28 map<CCharString, pair<long, CDateAndTime> >
    long          FrameTattoosChanged;                         // +0x34
    long          FrameHeroBecameIdle;                         // +0x38
    bool          IdleParticlesAdded;                          // +0x3c
    bool          Changed;                                     // +0x3d
    float         Strength;                                    // +0x40
    float         Berserk;                                     // +0x44
    float         Will;                                        // +0x48
    float         Skill;                                       // +0x4c
    float         Age;                                         // +0x50
    float         Morality;                                    // +0x54
    float         Fatness;                                     // +0x58
    float         Tan;                                         // +0x5c
    bool          Teenager;                                    // +0x60
};

void CTCHeroMorph::SetFatness(float fatness)
{
    Fatness = GFLimitReturn(fatness, 0.0f, 1.0f);
    Changed = true;
}