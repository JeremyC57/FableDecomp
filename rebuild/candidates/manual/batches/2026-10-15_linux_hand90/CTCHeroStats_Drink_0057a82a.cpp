// CTCHeroStats::Drink(float) -- pours a drink into the beer reservoir (scaled by the current drink multiplier), counts it, and makes the next drink count for less
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A82A, 74 bytes.
// Callee: CTCBase::GetDefinitions() @ 0x006869D0 (ParentThing's definition holder; +0xDC its CDefPointer<CHeroStatsDef>).
// Globals: 1.0f @ 0x0122DED8.
//
// Shape: `fld amount ; fmul [DrinkAffectMultiplier]` = `DrinkAffectMultiplier * amount` (the right operand is loaded
// first), added into BeerReservoir through one materialised address (`lea eax,[esi+1A0h] ; fadd [eax] ; fstp [eax]`,
// a compound `+=` in a size TU); after the definition call `fld1 ; fsub [def+88h]` = `1.0f - multiplier`, times the
// amount, `fsubr [1.0f]`, and `*=` into DrinkAffectMultiplier; ++NumberOfDrinks is scheduled into the FPU sequence.
// The definition is read straight through the holder's def-pointer word (no counted copy). Source order pins the
// `inc`: `1.0f - multiplier` goes into a named float local BEFORE ++NumberOfDrinks (counter bumped ahead of the call:
// the `inc` is hoisted above the reservoir store; bumped after a def-pointer local: it lands between `fld1` and
// `fsub`); reaching the definition through a def-pointer object's operator-> materialises `add eax,0DCh`.
// Layout: retail CTCHeroStats sits 0x18 below the PDB here (the map, three vectors and HousePositions are 12 bytes,
// not 16): NumberOfDrinks +0xF0 (PDB +0xF8), BeerReservoir +0x1A0 (PDB +0x1B8), DrinkAffectMultiplier +0x1A4
// (PDB +0x1BC); CHeroStatsDef::DrinkAffectMultiplierMultiplier +0x88 (PDB +0x90).
#pragma optimize("s", on)

struct CHeroStatsDef {
    unsigned char _pad_0x00[0x88];
    float         DrinkAffectMultiplierMultiplier;     // +0x88 (PDB +0x90)
};

struct CDefinitionHolder {
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;              // +0xDC CDefPointer<const CHeroStatsDef>::Object
};

struct CTCHeroStats_Retail {
    void*         __vftable;                   // +0x00
    void*         ParentThing;                 // +0x04 CTCBase
    unsigned char _pad_0x08[0xE8];
    long          NumberOfDrinks;              // +0xF0 (PDB +0xF8)
    unsigned char _pad_0xf4[0xAC];
    float         BeerReservoir;               // +0x1A0 (PDB +0x1B8)
    float         DrinkAffectMultiplier;       // +0x1A4 (PDB +0x1BC)

    CDefinitionHolder* GetDefinitions() const; // 0x006869D0
    void Drink(float amount);
};

void CTCHeroStats_Retail::Drink(float amount)
{
    BeerReservoir += DrinkAffectMultiplier * amount;
    float sobering = 1.0f - GetDefinitions()->PHeroStatsDef->DrinkAffectMultiplierMultiplier;
    ++NumberOfDrinks;
    DrinkAffectMultiplier *= 1.0f - sobering * amount;
}
