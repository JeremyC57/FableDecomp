#include <cstdio>
// Behaviour test for COpinionReactionManager::GetAttitudeType @ 0x00681500
class CThing { public: int m_id; };
class CTCOpinionSource { public: int m_id; };
static const CThing* g_thing = 0; static bool g_attitudeFlag = false; static const bool* g_mask = 0; static CTCOpinionSource* g_source = 0;
class COpinionReactionManager {
public:
    bool AttitudeMask[79];
    long DeedReactionSpeechPlaying[92];
    long GetAttitudeOrReactionType(const CThing* thing, bool attitude, const bool* mask, CTCOpinionSource* source) const
    { g_thing = thing; g_attitudeFlag = attitude; g_mask = mask; g_source = source; return 11; }
    long GetThingAttitudeToPlayer(const CThing* thing) const { g_thing = thing; return 22; }
    long GetAttitudeType(const CThing* thing, CTCOpinionSource* source) const
    {
        if (source)
            return GetAttitudeOrReactionType(thing, true, AttitudeMask, source);
        return GetThingAttitudeToPlayer(thing);
    }
};
int main()
{
    COpinionReactionManager m; CThing t; CTCOpinionSource s;
    if (m.GetAttitudeType(&t, &s) != 11 || g_thing != &t || !g_attitudeFlag || g_mask != m.AttitudeMask || g_source != &s) { std::printf("00681500_TEST FAIL\n"); return 1; }
    g_thing = 0;
    if (m.GetAttitudeType(&t, 0) != 22 || g_thing != &t) { std::printf("00681500_TEST FAIL\n"); return 1; }
    std::printf("00681500_TEST PASS\n");
    return 0;
}
