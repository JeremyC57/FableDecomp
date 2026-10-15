#include <cstdio>
#include <cstring>
// CAIStateGroup_MinionReactToCrime::OnActivate: apply the deed mask (when the opinion component exists), set the flag
struct Opinion { char mask[64]; void SetOpinionDeedMask(const char* s) { std::strcpy(mask, s); } };
struct Group {
    Opinion* opinion; bool hasOpinion; bool deedMaskApplied;
    void OnActivate()
    {
        deedMaskApplied = false;
        if (hasOpinion) { opinion->SetOpinionDeedMask("OPINION_DEED_MASK_REPORTING_CRIME_TO_GUARD"); deedMaskApplied = true; }
    }
};
int main()
{
    Opinion o; o.mask[0] = 0; Group g; g.opinion = &o; g.hasOpinion = true; g.deedMaskApplied = false;
    g.OnActivate(); if (!g.deedMaskApplied || std::strcmp(o.mask, "OPINION_DEED_MASK_REPORTING_CRIME_TO_GUARD") != 0) { std::printf("009587d0_TEST FAIL\n"); return 1; }
    g.hasOpinion = false; g.OnActivate(); if (g.deedMaskApplied) { std::printf("009587d0_TEST FAIL\n"); return 1; }
    std::printf("009587d0_TEST PASS\n"); return 0;
}