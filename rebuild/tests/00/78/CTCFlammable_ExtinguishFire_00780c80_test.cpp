// Behaviour test for 00780c80: the model below is a copy of src/00780c80.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_resets = 0, g_stops = 0, g_inits = 0, g_clears = 0, g_worldCalls = 0, g_lowerBounds = 0;
struct CTCBase;
struct CThing;
namespace NFire {
struct CFirePoint {
    unsigned char _data[0x2c];
    void Reset() { ++g_resets; }
};
}
template<class R, class T> struct mem_fun_ref_t {
    R (T::*m_pmf)();
    explicit mem_fun_ref_t(R (T::*pmf)()) : m_pmf(pmf) { }
    R operator()(T& item) const { return (item.*m_pmf)(); }
};
template<class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pmf)()) { return mem_fun_ref_t<R, T>(pmf); }
template<class It, class F> F __fastcall for_each(It first, It last, F f) { for (; first != last; ++first) f(*first); return f; }
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
};
struct CTCSound {
    void StopSound() { ++g_stops; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { ++g_lowerBounds; SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
static CThing* g_clearedThing = 0; static bool g_clearedNow = false;
struct COpinionReactionManager {
    void ClearOpinionSource(CThing* source, bool immediately) { ++g_clears; g_clearedThing = source; g_clearedNow = immediately; }
};
struct CWorld {
    unsigned char            _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager;
};
struct CFlammableDef {
    unsigned char _pad[0x28];
    long FireResistance;
};
static CWorld g_world; static COpinionReactionManager g_opinions;
struct CTCFlammable {
    virtual void v00() {}
    CThing*        ParentThing;
    bool           ToBeRemoved;
    bool           IsInGlobalUpdateTCs;
    CFlammableDef* PDef;
    CRetailVector<NFire::CFirePoint> FirePoints;
    bool           Burning;
    long           FireDamageAfterFrame;
    long           FireResistance;
    long           FireStrength;
    float          DamageCaused;
    bool           DamagePerPoint;
    bool           HasFirePoint;
    CWorld* GetWorld() { ++g_worldCalls; return &g_world; }
    void InitialiseFirePoints() { ++g_inits; }
    void ExtinguishFire();
};

void CTCFlammable::ExtinguishFire()
{
    for_each(FirePoints.begin(), FirePoints.end(), mem_fun_ref(&NFire::CFirePoint::Reset));
    if (ParentThing->HasInterface(16))
        ((CTCSound*)ParentThing->GetInterface(16))->StopSound();
    Burning = false;
    FireDamageAfterFrame = 0;
    FireResistance = PDef->FireResistance;
    FireStrength = 1;
    DamageCaused = 0.0f;
    DamagePerPoint = false;
    InitialiseFirePoints();
    CThing* thing = ParentThing;
    CWorld* world = GetWorld();
    world->POpinionReactionManager->ClearOpinionSource(thing, true);
}

static bool Run(bool hasSound, int points)
{
    static CTCSound sound; static NFire::CFirePoint pts[3]; static CInterfaceMap::SEntry entries[2];
    CThing thing; std::memset(&thing, 0, sizeof(thing));
    entries[0].type = 3; entries[0].component = 0; entries[1].type = 16; entries[1].component = (CTCBase*)&sound;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    if (hasSound) thing.m_presentBits[0] |= 1UL << 16;
    CFlammableDef def; std::memset(&def, 0, sizeof(def)); def.FireResistance = 77;
    g_world.POpinionReactionManager = &g_opinions;
    CTCFlammable f; std::memset((void*)&f, 0, sizeof(f)); f.ParentThing = &thing; f.PDef = &def;
    f.FirePoints._M_start = pts; f.FirePoints._M_finish = pts + points;
    f.Burning = true; f.FireDamageAfterFrame = 9; f.FireStrength = 5; f.DamageCaused = 2.5f; f.DamagePerPoint = true;
    g_resets = g_stops = g_inits = g_clears = g_worldCalls = g_lowerBounds = 0; g_clearedThing = 0; g_clearedNow = false;
    f.ExtinguishFire();
    if (g_resets != points) return false;
    if (g_stops != (hasSound ? 1 : 0) || g_lowerBounds != (hasSound ? 1 : 0)) return false;
    if (f.Burning || f.FireDamageAfterFrame != 0 || f.FireResistance != 77 || f.FireStrength != 1 || f.DamageCaused != 0.0f || f.DamagePerPoint) return false;
    if (g_inits != 1 || g_worldCalls != 1 || g_clears != 1 || g_clearedThing != &thing || !g_clearedNow) return false;
    return true;
}
int main()
{
    bool ok = Run(true, 3) && Run(false, 0) && Run(false, 2);
    if (!ok) { std::printf("00780c80_TEST FAIL\n"); return 1; }
    std::printf("00780c80_TEST PASS\n");
    return 0;
}