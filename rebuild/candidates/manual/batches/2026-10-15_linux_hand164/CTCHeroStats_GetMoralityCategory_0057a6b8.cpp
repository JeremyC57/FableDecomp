// CTCHeroStats::GetMoralityCategory() const -- bucket the morality percentage into seven equal bands (0 = most evil .. 6 = most good)
// Leaf: GetMoralityCategory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A6B8, length 120 bytes. Size-optimised TU (`#pragma optimize("s", on)`:
// `push N ; pop eax` returns, unaligned start), as the landed neighbours GetMoralityPercentage 0x0057A690 / Drink 0x0057A82A.
// Callees: CTCHeroStats::GetMoralityPercentage() const @ 0x0057A690 (float on the x87 stack).
// Shape: a chain of `if (m < k/7) return n;` tests (`fcom ; fnstsw ; test ah,5 ; jp`), the value popped (`fstp st(0)`) on each
// early return and consumed by the final `fcomp`; the band limits 1/7 .. 6/7 are float literals (pool 0x01230788, 0x0124F310,
// 0x0124F30C, 0x0124F308, 0x0124F304, 0x0124F300).
#pragma optimize("s", on)
enum EMoralityCategory { MORALITY_CATEGORY_0 };
class CThing;
class CTCHeroStats {
public:
    virtual ~CTCHeroStats();
    CThing* ParentThing;                        // +0x04
    float GetMoralityPercentage() const;        // 0x0057A690
    EMoralityCategory GetMoralityCategory() const;
};

EMoralityCategory CTCHeroStats::GetMoralityCategory() const
{
    float morality = GetMoralityPercentage();
    if (morality < 0.142857149f)
        return (EMoralityCategory)0;
    if (morality < 0.285714298f)
        return (EMoralityCategory)1;
    if (morality < 0.428571433f)
        return (EMoralityCategory)2;
    if (morality < 0.571428597f)
        return (EMoralityCategory)3;
    if (morality < 0.714285731f)
        return (EMoralityCategory)4;
    if (morality < 0.857142866f)
        return (EMoralityCategory)5;
    return (EMoralityCategory)6;
}
