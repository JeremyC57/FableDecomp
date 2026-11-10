// Behaviour test for 0x00478722 CAbilityTrackDef::~CAbilityTrackDef(): every level def of the Abilities vector is destroyed, its
// storage released, then the CParentDefClassBase destructor runs.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_base = 0, g_levels = 0, g_freed = 0;
class CParentDefClassBase { public: virtual ~CParentDefClassBase() { ++g_base; } };
class CAbilityTrackLevelDef { public: long V; virtual ~CAbilityTrackLevelDef() { ++g_levels; } };
template <class T> struct CVector { T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CVector() { for (T* p = _M_start; p != _M_finish; ++p) p->~T(); if (_M_start) { std::free(_M_start); ++g_freed; } } };
enum EHeroExperienceStatCategory { HERO_STAT_STRENGTH = 0 };
class CAbilityTrackDef : public CParentDefClassBase { public:
    EHeroExperienceStatCategory ExperienceStat;
    CVector<CAbilityTrackLevelDef> Abilities;
    long MaxedOutDescription;
    ~CAbilityTrackDef() { }
};
int main()
{
    {
        CAbilityTrackDef d;
        d.Abilities._M_start = (CAbilityTrackLevelDef*)std::malloc(3 * sizeof(CAbilityTrackLevelDef));
        for (int i = 0; i < 3; ++i) new (&d.Abilities._M_start[i]) CAbilityTrackLevelDef();
        d.Abilities._M_finish = d.Abilities._M_start + 3; d.Abilities._M_end_of_storage = d.Abilities._M_finish;
    }
    if (g_levels != 3 || g_freed != 1 || g_base != 1) { std::printf("00478722_TEST FAIL\n"); return 1; }
    std::printf("00478722_TEST PASS\n");
    return 0;
}
