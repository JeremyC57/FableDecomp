#pragma optimize("s",on)
struct CTCHeroStats {
  int m00[10];
  int m28;
};
extern "C" int __fastcall CTCHeroStats_GetMax(CTCHeroStats* stats);
extern "C" int __fastcall CTCHeroStats_GetVal(CTCHeroStats* stats);

float __fastcall CTCHeroStats_GetMoralityPercentage(CTCHeroStats* stats)
{
  int denom = CTCHeroStats_GetMax(stats) * 2;
  int num = CTCHeroStats_GetVal(stats) + stats->m28;
  return (float)num / (float)denom;
}