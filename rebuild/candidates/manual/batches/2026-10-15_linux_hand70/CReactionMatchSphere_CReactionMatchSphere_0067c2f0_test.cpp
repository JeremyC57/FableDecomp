// Behaviour test for the CReactionMatch_Sphere constructor @ 0x0067C2F0: type, a copied centre set, the squared radius.
#include <cstdio>

enum EOpinionReactionType { OPINION_REACTION_TYPE_NONE = 0, OPINION_REACTION_TYPE_FEAR = 3 };

static int g_copies = 0;
struct COpinionSet {
    float m_values[4]; int m_count;
    COpinionSet() : m_count(0) {}
    COpinionSet(const COpinionSet& other) : m_count(other.m_count)
    {
        ++g_copies;
        for (int i = 0; i < m_count; ++i) m_values[i] = other.m_values[i];
    }
};

struct CReactionMatch {
    EOpinionReactionType ReactionType;
    CReactionMatch(EOpinionReactionType type) : ReactionType(type) {}
    virtual long GetMatchType() const { return 0; }
    virtual ~CReactionMatch() {}
};

struct CReactionMatch_Sphere : CReactionMatch {
    COpinionSet Centre; float SqRadius;
    CReactionMatch_Sphere(EOpinionReactionType type, const COpinionSet& centre, float radius)
        : CReactionMatch(type), Centre(centre), SqRadius(radius * radius) {}
};

int main()
{
    COpinionSet c; c.m_count = 2; c.m_values[0] = 0.5f; c.m_values[1] = -2.0f;
    CReactionMatch_Sphere m(OPINION_REACTION_TYPE_FEAR, c, 3.0f);
    if (m.ReactionType != OPINION_REACTION_TYPE_FEAR || g_copies != 1 || m.Centre.m_count != 2 || m.Centre.m_values[1] != -2.0f || m.SqRadius != 9.0f) {
        std::printf("0067c2f0_TEST FAIL\n"); return 1;
    }
    std::printf("0067c2f0_TEST PASS\n");
    return 0;
}
