// Behaviour test for 0x006C8D40 CThingFilter_IsDeedTypeEnabled::operator(): the result is the opinion component's
// DeedMask[DeedType]; for deed type 1 the thing must also have a village-member interface whose village is set.
#include <cstdio>

struct CTCVillage { int m_id; };
struct CTCVillageMember { CTCVillage* m_pVillage; CTCVillage* GetPVillage() const { return m_pVillage; } };
struct CTCCreatureOpinionOfHero { bool* DeedMask; };

struct CThing {
    CTCVillageMember* m_member;                 // null = no 0x23 interface
    CTCCreatureOpinionOfHero* m_opinion;        // always present in these tests
    bool GetMember(CTCVillageMember** out) const { if (!m_member) return false; *out = m_member; return true; }
    bool GetOpinion(CTCCreatureOpinionOfHero** out) const { if (!m_opinion) return false; *out = m_opinion; return true; }
};

struct CThingFilter_IsDeedTypeEnabled {
    long DeedType;
    bool operator()(const CThing& thing) const
    {
        if (DeedType == 1) {
            CTCVillageMember* member;
            if (!thing.GetMember(&member) || member->GetPVillage() == 0)
                return false;
        }
        CTCCreatureOpinionOfHero* opinion = 0;
        thing.GetOpinion(&opinion);
        return opinion->DeedMask[DeedType];
    }
};

int main()
{
    bool mask[4] = { false, true, true, false };
    CTCCreatureOpinionOfHero op; op.DeedMask = mask;
    CTCVillage village; CTCVillageMember member; member.m_pVillage = &village;
    CThing t; t.m_member = &member; t.m_opinion = &op;
    CThingFilter_IsDeedTypeEnabled f;
    f.DeedType = 2; if (!f(t)) { std::printf("006c8d40_TEST FAIL\n"); return 1; }
    f.DeedType = 3; if (f(t))  { std::printf("006c8d40_TEST FAIL\n"); return 1; }
    f.DeedType = 1; if (!f(t)) { std::printf("006c8d40_TEST FAIL\n"); return 1; }
    member.m_pVillage = 0; if (f(t)) { std::printf("006c8d40_TEST FAIL\n"); return 1; }    // no village
    member.m_pVillage = &village; t.m_member = 0; if (f(t)) { std::printf("006c8d40_TEST FAIL\n"); return 1; }  // no member
    f.DeedType = 2; if (!f(t)) { std::printf("006c8d40_TEST FAIL\n"); return 1; }    // member only matters for type 1
    std::printf("006c8d40_TEST PASS\n");
    return 0;
}