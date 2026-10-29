// CTCHeroMorph::GetMorphStrength(EHeroMorphType type) const -- the 0..1 strength of one hero morph, derived from the hero's morph stats (Tan, Age, Morality, Strength, Skill, Will, Fatness, Berserk, Teenager)
// Grade: DIFFER(624v624) -- frame `sub esp,14h` v retail `10h`: case 11's three GFLimitReturn temporaries sit at [esp+0Ch..14h] where retail reuses case 9's value slot ([esp+8..10h]); every instruction else matches
// Leaf: ?GetMorphStrength@CTCHeroMorph@@QBEMW4EHeroMorphType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071BA60, 546 bytes + the 19-entry jump table 0x0071BC84; thiscall, `sub esp,10h`,
//   `ret 4`, float in st(0).
// Identity: Ego_r.pdb and FableWin.pdb hold `?GetMorphStrength@CTCHeroMorph@@QBEMW4EHeroMorphType@@@Z` (const, enum
//   argument); the bound spelling `QAEMJ` is wrong. It sits just before the landed setters 0x0071BE20..0x0071C0C0 and
//   recurses into itself (types 9 and 16). Floats: 0.5f 0x0122F59C, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 0.4f 0x0125A318,
//   0.2f 0x012305A0, 1/0.6 0x01265BBC, 1/0.8 0x01265BB8.
// Callees: GFLimitReturn<float> 0x0057EF86 (`??$GFLimitReturn@M@@YIABMABM00@Z`, FableWin.pdb + Ego_r.pdb name sets; out of
//   line here, inline in the landed SetFatness 0x0071C060).
// Layout: the landed CTCHeroMorph setters' retail model (Ego_r.pdb CTCHeroMorph minus 24 bytes for the STLport lists,
//   vector and map): Strength +0x40, Berserk +0x44, Will +0x48, Skill +0x4C, Age +0x50, Morality +0x54, Fatness +0x58,
//   Tan +0x5C, Teenager +0x60. No engine/CTCHeroMorph.h exists.
// Shape: the case bodies are in source order 1,2,4,5,17,6,7,9,10,11,12,13,14,8+15,16,18; types 0 and 3 and the failed
//   threshold tests share one `return 0.0f` block; `case 0:` must be an explicit `return 0.0f` (a bare `break` lets the
//   compiler drop it and rebase the table with `dec eax`), and type 18 is a ternary (its 0.0f arm stays a separate block).
//   EHeroMorphType enumerator names are not recovered (def.xml: "Age=4, Evil=6, Good=7, Will=10", which matches the
//   members read), so the cases use the numbers.
template <class T> const T& __fastcall GFLimitReturn(const T& value, const T& low, const T& high);   // 0x0057EF86 (float)

enum EHeroMorphType { };

class CThing;
class CTCHeroMorph {                                           // retail layout (STLport containers)
public:
    float GetMorphStrength(EHeroMorphType type) const;
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

float CTCHeroMorph::GetMorphStrength(EHeroMorphType type) const
{
    switch (type) {
    case 0:
        return 0.0f;
    case 1:
        if (Tan >= 0.5f)
            return (Tan - 0.5f) * 2.0f;
        break;
    case 2:
        if (Tan < 0.5f)
            return (0.5f - Tan) * 2.0f;
        break;
    case 4: {
        float v = Age - 0.4f;
        if (v < 0.0f)
            v = 0.0f;
        return v * (1.0f / 0.6f);
    }
    case 5: {
        float v = Age - 0.2f;
        if (v < 0.0f)
            v = 0.0f;
        return v * (1.0f / 0.8f);
    }
    case 17:
        return Berserk;
    case 6:
        if (Morality < 0.5f)
            return (0.5f - Morality) * 2.0f;
        break;
    case 7:
        if (Morality >= 0.5f)
            return (Morality - 0.5f) * 2.0f;
        break;
    case 9:
        return GFLimitReturn(GetMorphStrength((EHeroMorphType)8) - Fatness, 0.0f, 1.0f);
    case 10:
        return Will;
    case 11: {
        float difference = Strength - Skill;
        return GFLimitReturn(difference, 0.0f, 1.0f);
    }
    case 12:
        if (Strength >= 0.5f)
            return (Strength - 0.5f) * 2.0f;
        break;
    case 13:
        if (Strength < 0.5f)
            return (0.5f - Strength) * 2.0f;
        break;
    case 14:
        return Fatness;
    case 8:
    case 15:
        return Skill;
    case 16:
        return GetMorphStrength((EHeroMorphType)6);
    case 18:
        return Teenager ? 1.0f : 0.0f;
    }
    return 0.0f;
}
