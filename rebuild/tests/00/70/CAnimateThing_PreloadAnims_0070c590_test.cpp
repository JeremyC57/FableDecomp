#include <cstdio>
#include <vector>
#include <list>
// Behaviour model of CAnimateThing::PreloadAnims: when the main anim set is valid (one anim, or two with a positive
// second; positive first; one blend alpha per anim) its transition sets and then the main set are preloaded; the
// combination anims and the delayed combination anims are always preloaded.
static std::vector<int> g_preloaded;
static void PreloadAnimation(int anim) { g_preloaded.push_back(anim); }
struct AnimSet {
    std::vector<short> Anims; std::vector<float> Alphas;
    void Preload() const { for (unsigned int i = 0; i < Anims.size(); i++) PreloadAnimation(Anims[i]); }
};
struct CompleteAnimation { AnimSet Main; std::list<AnimSet> Transitions; std::list<AnimSet> Combinations; };
struct AnimateThing {
    std::list<AnimSet> DelayedCombinations;
    void PreloadAnims(const CompleteAnimation& a)
    {
        int count = (int)a.Main.Anims.size();
        if ((count == 1 || (count == 2 && a.Main.Anims[1] > 0)) && a.Main.Anims[0] > 0 && count == (int)a.Main.Alphas.size()) {
            for (std::list<AnimSet>::const_iterator it = a.Transitions.begin(); it != a.Transitions.end(); ++it) it->Preload();
            a.Main.Preload();
        }
        for (std::list<AnimSet>::const_iterator it = a.Combinations.begin(); it != a.Combinations.end(); ++it) it->Preload();
        for (std::list<AnimSet>::const_iterator it = DelayedCombinations.begin(); it != DelayedCombinations.end(); ++it) it->Preload();
    }
};
static AnimSet MakeSet(short a, short b, int alphas)
{
    AnimSet s; s.Anims.push_back(a); if (b >= 0) s.Anims.push_back(b);
    for (int i = 0; i < alphas; i++) s.Alphas.push_back(0.5f);
    return s;
}
int main()
{
    AnimateThing t; t.DelayedCombinations.push_back(MakeSet(40, -1, 1));
    CompleteAnimation c; c.Main = MakeSet(10, 11, 2); c.Transitions.push_back(MakeSet(20, -1, 1)); c.Combinations.push_back(MakeSet(30, 31, 2));
    t.PreloadAnims(c);
    int want1[] = { 20, 10, 11, 30, 31, 40 };
    bool ok = g_preloaded.size() == 6;
    for (unsigned int i = 0; ok && i < 6; i++) ok = g_preloaded[i] == want1[i];
    g_preloaded.clear(); c.Main = MakeSet(10, 0, 2); t.PreloadAnims(c);
    ok = ok && g_preloaded.size() == 3 && g_preloaded[0] == 30 && g_preloaded[2] == 40;
    g_preloaded.clear(); c.Main = MakeSet(10, -1, 2); t.PreloadAnims(c);
    ok = ok && g_preloaded.size() == 3;
    std::printf(ok ? "0070c590_TEST PASS\n" : "0070c590_TEST FAIL\n");
    return ok ? 0 : 1;
}