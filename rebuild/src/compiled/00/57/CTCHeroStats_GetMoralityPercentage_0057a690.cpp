// CTCHeroStats::GetMoralityPercentage() const -- the hero's morality mapped to 0..1: (Morality + max) / (2 * max)
// Leaf: ?GetMoralityPercentage@CTCHeroStats@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A690, 40 bytes (frame pointer: `push ebp ; mov ebp,esp`): GetMoralityMax 0x0057A67E
//   twice (the doubled maximum first, `shl eax,1`), `add eax,[esi+28h]` (Morality), `fild ; fidiv`.
// NF167 (2026-11-20, q6155): self rename + port. Was the free stand-in `CTCHeroStats_GetMoralityPercentage(CTCHeroStats*)` on a row-local
//   CTCHeroStats model, calling 0x0057A67E as two different extern "C" stand-ins (@CTCHeroStats_GetMax@4 / @CTCHeroStats_GetVal@4).
// Identity: Ego_r.pdb 0x0056DA5D ?GetMoralityPercentage@CTCHeroStats@@QBEMXZ (donor order GetMoralityMax 0x0056DA4B / GetMoralityPercentage /
//   GetMoralityCategory 0x0056DA8B = retail 0x0057A67E / 0x0057A690 / 0x0057A6B8); the landed GetMoralityCategory 0x0057A6B8 already binds
//   this spelling (its thresholds are sevenths of this 0..1 value); catalog CTCHeroStats::GetMoralityPercentage.
// Callee: ?GetMoralityMax@CTCHeroStats@@QBEJXZ 0x0057A67E (Ego_r 0x0056DA4B / FableWin 0x01E64768; shared header opt-in MORALITYMAX).
// Layout: Morality +0x28 (engine/CTCHeroStats.h FABLE_RETAIL_CTCHEROSTATS_MORALITY).
#pragma optimize("s",on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_MORALITY
#define FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX
#include "engine/CTCHeroStats.h"

float CTCHeroStats::GetMoralityPercentage() const
{
    long denom = GetMoralityMax() * 2;
    long num = GetMoralityMax() + Morality;
    return (float)num / (float)denom;
}
