// Behaviour test for 0x0088A4E0: vector<CEnvironmentThemePriorityStackEntry>::resize(n) forwards the default entry (weight 1, empty themes)
#include <cstdio>
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {
public:
    struct CThemeWeight { public: long ThemeID; float Weight; bool LockWeight; };
    class CThemeVector {                            // the empty STLport vector: three null pointers
    public:
        CThemeWeight* _M_start;
        CThemeWeight* _M_finish;
        CThemeWeight* _M_end_of_storage;
        CThemeVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    };
    float BaseWeight;
    bool BaseWeightLocked;
    CThemeVector Themes;
    long ThemeTransitionTargetID;
    float ThemeTransitionInitialWeight;
    bool ThemeTransitionActive;
    double ThemeTransitionStartTime;
    float ThemeTransitionDuration;
    CEnvironmentThemePriorityStackEntry() : BaseWeight(1.0f), ThemeTransitionActive(false) { BaseWeightLocked = false; }
};
}
static NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight g_dummyWeight;

static bool FloatNear(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

template <class T> class TestVector {               // records what the out-of-line resize(n, x) receives
public:
    unsigned int lastN;
    int calls;
    T lastX;
    TestVector() : lastN(0), calls(0) { }
    void resize(unsigned int n, const T& x) { lastN = n; lastX = x; ++calls; }
    void resize(unsigned int n) { resize(n, T()); }  // the reconstructed forwarder
};

int main()
{
    TestVector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry > v;
    NEnvironmentInternal::CEnvironmentThemePriorityStackEntry& x = v.lastX;
    x.BaseWeight = 0.25f; x.BaseWeightLocked = true; x.Themes._M_start = &g_dummyWeight; x.Themes._M_finish = &g_dummyWeight; x.Themes._M_end_of_storage = &g_dummyWeight; x.ThemeTransitionActive = true;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(FloatNear(x.BaseWeight, 1.0f) && !x.BaseWeightLocked && x.Themes._M_start == 0 && x.Themes._M_finish == 0 && x.Themes._M_end_of_storage == 0 && !x.ThemeTransitionActive)) {
        std::printf("0088a4e0_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(FloatNear(x.BaseWeight, 1.0f) && !x.BaseWeightLocked && x.Themes._M_start == 0 && x.Themes._M_finish == 0 && x.Themes._M_end_of_storage == 0 && !x.ThemeTransitionActive)) {
        std::printf("0088a4e0_TEST FAIL\n");
        return 1;
    }
    std::printf("0088a4e0_TEST PASS\n");
    return 0;
}