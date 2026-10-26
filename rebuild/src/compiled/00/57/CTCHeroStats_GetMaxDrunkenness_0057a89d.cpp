struct CTCHeroStatsInner { char pad[0xa4]; float maxDrunkenness; };
class CTCHeroStats { public: char pad[0xdc]; CTCHeroStatsInner* inner; };

extern CTCHeroStats* __fastcall CTCHeroStats_GetOwner(CTCHeroStats* stats);

float __fastcall CTCHeroStats_GetMaxDrunkenness(CTCHeroStats* stats)
{
    CTCHeroStats* p = CTCHeroStats_GetOwner(stats);
    return p->inner->maxDrunkenness;
}