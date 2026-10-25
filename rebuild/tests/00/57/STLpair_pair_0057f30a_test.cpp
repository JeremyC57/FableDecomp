// Behaviour test for 0x0057F30A pair<const CCharString, CTCHeroStats::MirroredCriminalStatus> copy constructor.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    static int s_copies;
    static int s_assigns;
    CCharString(const char* s) : PData(s) {}
    CCharString(const CCharString& o) : PData(o.PData) { ++s_copies; }
    CCharString& operator=(const CCharString& o) { PData = o.PData; ++s_assigns; return *this; }
};
int CCharString::s_copies = 0;
int CCharString::s_assigns = 0;
static bool Same(const CCharString& s, const char* t) { return std::strcmp(s.PData, t) == 0; }
class CTCHeroStats {
public:
    struct MirroredCriminalStatus {
        long  Crimes[10];
        long  LastTimeSeenByGuard;
        long  LastTimeExited;
        bool  ExitedUsingTeleporter;
        float EntrancePosX;
        float EntrancePosY;
        float EntrancePosZ;
        float EntranceAngle;
    };
};
typedef CTCHeroStats::MirroredCriminalStatus TStatus;
static void Fill(TStatus& s)
{
    for (int i = 0; i < 10; ++i) s.Crimes[i] = i * 3;
    s.LastTimeSeenByGuard = 1200; s.LastTimeExited = 1350; s.ExitedUsingTeleporter = true;
    s.EntrancePosX = 1.5f; s.EntrancePosY = -2.25f; s.EntrancePosZ = 10.0f; s.EntranceAngle = 0.75f;
}
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
static bool Check(const TStatus& s)
{
    for (int i = 0; i < 10; ++i) if (s.Crimes[i] != i * 3) return false;
    return s.LastTimeSeenByGuard == 1200 && s.LastTimeExited == 1350 && s.ExitedUsingTeleporter &&
           Near(s.EntrancePosX, 1.5f) && Near(s.EntrancePosY, -2.25f) && Near(s.EntrancePosZ, 10.0f) && Near(s.EntranceAngle, 0.75f);
}
namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}

int main()
{
    TStatus s; Fill(s);
    stlp::pair<const CCharString, TStatus> a(CCharString("OAKVALE"), s);
    CCharString::s_copies = 0;
    stlp::pair<const CCharString, TStatus> b(a);
    if (!(CCharString::s_copies == 1 && Same(b.first, "OAKVALE") && Check(b.second))) {
        std::printf("0057f30a_TEST FAIL\n");
        return 1;
    }
    std::printf("0057f30a_TEST PASS\n");
    return 0;
}