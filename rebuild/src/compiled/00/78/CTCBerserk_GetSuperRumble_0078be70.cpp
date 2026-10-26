// CTCBerserk::GetSuperRumble(float& level, float& smoothness, float& attack, float& decay) -- the def's super-rumble parameters (+0x120..+0x12c) added onto the component's rumble values (+0x30..+0x3c)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078BE70, length 75 bytes (+ int3 padding).
// Callees: none. Layout: engine/CTCBerserk.h (Ego_r.pdb) RumbleLevel +0x30, RumbleSmoothness +0x34,
// RumbleAttack +0x38, RumbleDecay +0x3c; PDef is +0x68 in retail (PDB +0x6c: the DependentThings vector
// at +0x4c is 12 bytes in retail), so the header's later offsets do not fit and the class is modelled
// locally (as the landed sibling 0x0078BEC0). The def's four floats are unnamed (descriptive names).
// Shape: `def->x + member` loads the def value first; PDef is re-read for every output because the
// store through each float reference may alias it.
class CSpecialAbilitiesBerserkDef { public:
    unsigned char _pad00[0x120];
    float         SuperRumbleLevel;                         // +0x120
    float         SuperRumbleSmoothness;                    // +0x124
    float         SuperRumbleAttack;                        // +0x128
    float         SuperRumbleDecay;                         // +0x12c
};

struct CTCBerserk_Retail {
    unsigned char                _pad00[0x30];
    float                        RumbleLevel;               // +0x30
    float                        RumbleSmoothness;          // +0x34
    float                        RumbleAttack;              // +0x38
    float                        RumbleDecay;               // +0x3c
    unsigned char                _pad40[0x68 - 0x40];
    CSpecialAbilitiesBerserkDef* PDef;                      // +0x68 (PDB +0x6c)
    void GetSuperRumble(float& level, float& smoothness, float& attack, float& decay);
};

void CTCBerserk_Retail::GetSuperRumble(float& level, float& smoothness, float& attack, float& decay)
{
    level = PDef->SuperRumbleLevel + RumbleLevel;
    smoothness = PDef->SuperRumbleSmoothness + RumbleSmoothness;
    attack = PDef->SuperRumbleAttack + RumbleAttack;
    decay = PDef->SuperRumbleDecay + RumbleDecay;
}