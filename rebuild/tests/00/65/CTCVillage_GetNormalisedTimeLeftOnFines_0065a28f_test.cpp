// Behaviour test for CTCVillage::GetNormalisedTimeLeftOnFines (0x0065A28F): 0 when the region has no def or no fines;
// otherwise, over the hero's recorded crimes, the largest (last-seen + knowledge life - now) divided by that crime's
// knowledge life (later crimes win ties); 0 when there is no hero; def reference counts balance.
#include <cstdio>
#include <set>
#include <map>

static long g_now = 1000;
static long g_fps = 10;
static int g_defRefs = 0;

struct CRegionDef { long m_count; bool HasFines; };
struct CDefPointer {
    CRegionDef* Object;
    CDefPointer(CRegionDef* o) : Object(o) { if (Object) { ++Object->m_count; ++g_defRefs; } }
    ~CDefPointer() { if (Object) { --Object->m_count; --g_defRefs; } }
    CRegionDef* operator->() const { return Object; }
};
struct COpinionDeed { long DeedType; };
struct CTCHeroOpinionDeedLog {
    std::map<long, COpinionDeed> deeds;
    std::map<long, long> life;              // deed type -> knowledge life seconds
    COpinionDeed* GetDeedFromHandle(long h) { std::map<long, COpinionDeed>::iterator i = deeds.find(h); return i == deeds.end() ? 0 : &i->second; }
    long GetKnowledge(long type) { return life[type]; }
};
struct CThing { CTCHeroOpinionDeedLog* log; };
static CThing* g_hero = 0;

struct CTCVillage {
    CRegionDef* regionDef;
    std::set<long> PlayersCrimes;
    long FramePlayerLastSeenByGuard;
    float GetNormalisedTimeLeftOnFines();
};

float CTCVillage::GetNormalisedTimeLeftOnFines()
{
    CDefPointer def = regionDef;
    if (def.Object && def->HasFines) {
        long longest = 0;
        long life = 1;
        CThing* hero = g_hero;
        if (hero) {
            CTCHeroOpinionDeedLog* log = hero->log;
            for (std::set<long>::iterator it = PlayersCrimes.begin(); it != PlayersCrimes.end(); ++it) {
                COpinionDeed* deed = log->GetDeedFromHandle(*it);
                if (deed) {
                    long knowledge = log->GetKnowledge(deed->DeedType) * g_fps;
                    long left = knowledge - (g_now - FramePlayerLastSeenByGuard);
                    if (left >= longest) {
                        life = knowledge;
                        longest = left;
                    }
                }
            }
        }
        return (float)longest / life;
    }
    return 0.0f;
}

int main()
{
    int ok = 1;
    CRegionDef rd = { 1, false };
    CTCVillage v; v.regionDef = 0; v.FramePlayerLastSeenByGuard = 950;
    ok &= v.GetNormalisedTimeLeftOnFines() == 0.0f;              // no def
    v.regionDef = &rd;
    ok &= v.GetNormalisedTimeLeftOnFines() == 0.0f;              // no fines
    rd.HasFines = true;
    ok &= v.GetNormalisedTimeLeftOnFines() == 0.0f;              // no hero: 0 / 1
    CTCHeroOpinionDeedLog log; CThing hero = { &log }; g_hero = &hero;
    COpinionDeed d1 = { 1 }, d2 = { 2 };
    log.deeds[10] = d1; log.deeds[20] = d2;
    log.life[1] = 10;                                            // 100 frames: left 50
    log.life[2] = 20;                                            // 200 frames: left 150
    v.PlayersCrimes.insert(10); v.PlayersCrimes.insert(30);      // 30 has no deed
    ok &= v.GetNormalisedTimeLeftOnFines() == 0.5f;
    v.PlayersCrimes.insert(20);
    ok &= v.GetNormalisedTimeLeftOnFines() == 0.75f;
    ok &= rd.m_count == 1 && g_defRefs == 0;
    std::printf(ok ? "0065a28f_TEST PASS\n" : "0065a28f_TEST FAIL\n");
    return ok ? 0 : 1;
}