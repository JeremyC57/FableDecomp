// CTCMultiArrow::SetAICast(long) -- shot count and volley size from the def's per-level tables, volleys = shots / per-volley
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CBF20, length 38 bytes.
// PDB CTCMultiArrow (retail offsets): PDef +0x0c, NumberOfShots +0x10, NumberInVolley +0x14, NumOfVolleys +0x18,
// CreateAmmoEffectCount +0x24; CSpecialAbilitiesMultiArrowDef::NumberOfShots[4] at +0x3c, NumberInVolley[4] at +0x4c
// (PDB +0x40/+0x50: the retail def base is 4 bytes narrower).
// Shape: NumberOfShots is re-read after its store (the def table load in between may alias it), the quotient goes
// to both NumOfVolleys and CreateAmmoEffectCount.
struct CSpecialAbilitiesMultiArrowDef {
    unsigned char _pad00[0x3c];
    long NumberOfShots[4];                          // +0x3c
    long NumberInVolley[4];                         // +0x4c
};

struct CTCBase_007cbf20 {
    virtual void v00();
    unsigned char _pad04[8];
};

struct CTCMultiArrow : CTCBase_007cbf20 {
    const CSpecialAbilitiesMultiArrowDef* PDef;     // +0x0c CDefPointer
    long          NumberOfShots;                    // +0x10
    long          NumberInVolley;                   // +0x14
    long          NumOfVolleys;                     // +0x18
    long          SpellLevel;                       // +0x1c
    unsigned char RemovedEffect;                    // +0x20
    long          CreateAmmoEffectCount;            // +0x24
    void SetAICast(long level);
};

void CTCMultiArrow::SetAICast(long level)
{
    NumberOfShots = PDef->NumberOfShots[level];
    NumberInVolley = PDef->NumberInVolley[level];
    NumOfVolleys = NumberOfShots / NumberInVolley;
    CreateAmmoEffectCount = NumOfVolleys;
}