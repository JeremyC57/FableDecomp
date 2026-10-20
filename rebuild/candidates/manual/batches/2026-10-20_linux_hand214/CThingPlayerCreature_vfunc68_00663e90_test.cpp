// behaviour test for CThingPlayerCreature::vfunc_68 (0x00663E90): skip when already the unfinished main anim, else SetMainAnim with the guard bit held
#include <cstdio>
struct CTCBase;
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; } };
struct CAnimInit { long AnimType, AnimIndex, Mode, Percent; bool Flag05;
    void Set(long animType, long, long animIndex, long mode, bool flag05, unsigned long, long percent) { AnimType = animType; AnimIndex = animIndex; Mode = mode; Flag05 = flag05; Percent = percent; }
    CAnimInit(long a, long b, long c, long d, bool e, unsigned long f, long g) { Set(a, b, c, d, e, f, g); } };
struct CShortVector { short* _M_start; short* _M_finish; short* begin() const { return _M_start; } bool empty() const { return _M_start == _M_finish; } };
struct CAnimSet { CShortVector Anims; unsigned char Finished : 1; };
static int g_calls = 0; static bool g_guardSeen = false; static CAnimInit g_last(0, 0, 0, 0, false, 0, 0);
struct CAnimateThing { CAnimSet MainAnimSet; unsigned char SettingMainAnim : 1;
    void SetMainAnim(const CAnimInit& init, long mode, long delay) { ++g_calls; g_guardSeen = SettingMainAnim && mode == 1 && delay == 0; g_last = init; } };
struct CTCAnimation { CAnimateThing m_anim; CAnimateThing& GetAnimation() { return m_anim; } };
class CThing { public:
    unsigned long UsingComponents[9]; CInterfaceMap Components;
    bool HasTC(long type) const { if (UsingComponents[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template<class T> T* GetTC(long t) { long type = t; CInterfaceMap* map = &Components; CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || t < it->type) ? map->last : it)->component; } };
class CThingPlayerCreature : public CThing { public:
    void vfunc_68(long animIndex, bool flag)
    {
        CTCAnimation* component = 0;
        if (HasTC(0x5a)) component = GetTC<CTCAnimation>(0x5a);
        CAnimateThing& animation = component->GetAnimation();
        CAnimSet& main = animation.MainAnimSet;
        if (*main.Anims.begin() == animIndex && !main.Anims.empty() && !main.Finished) return;
        animation.SettingMainAnim = 1;
        animation.SetMainAnim(CAnimInit(1, 0, animIndex, 1, flag, 0, 100), 1, 0);
        animation.SettingMainAnim = 0;
    } };
int main()
{
    short anims[2] = { 7, 9 };
    CTCAnimation tc; tc.m_anim.MainAnimSet.Anims._M_start = anims; tc.m_anim.MainAnimSet.Anims._M_finish = anims + 2;
    tc.m_anim.MainAnimSet.Finished = 0; tc.m_anim.SettingMainAnim = 0;
    CInterfaceMap::SEntry e[1] = { { 0x5a, (CTCBase*)&tc } };
    CThingPlayerCreature p; for (int i = 0; i < 9; ++i) p.UsingComponents[i] = 0; p.UsingComponents[2] = 1UL << 26;
    p.Components.first = e; p.Components.last = e + 1;
    int bad = 0;
    p.vfunc_68(7, true);  if (g_calls != 0) bad = 1;                 // already playing, unfinished
    p.vfunc_68(8, true);
    if (g_calls != 1 || !g_guardSeen || tc.m_anim.SettingMainAnim || g_last.AnimIndex != 8 || g_last.AnimType != 1 || g_last.Mode != 1 || g_last.Percent != 100 || !g_last.Flag05) bad = 1;
    tc.m_anim.MainAnimSet.Finished = 1; p.vfunc_68(7, false);          // finished: restart
    if (g_calls != 2 || g_last.Flag05) bad = 1;
    if (bad) { std::printf("00663e90_TEST FAIL\n"); return 1; }
    std::printf("00663e90_TEST PASS\n");
    return 0;
}
