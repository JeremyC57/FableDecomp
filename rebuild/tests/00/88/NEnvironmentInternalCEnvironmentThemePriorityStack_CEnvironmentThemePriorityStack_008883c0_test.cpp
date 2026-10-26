// Behaviour test for NEnvironmentInternal::CEnvironmentThemePriorityStack(long count) (0x008883C0): the stack holds
// exactly count entries, each a default entry (base weight 1, unlocked, no theme weights, no transition); count 0
// gives an empty stack.
#include <cstdio>
#include <cstdlib>
#include <new>

template <class T> class CArray {                      // STLport-vector stand-in with resize(n) = resize(n, T())
public:
    T* Start;
    T* Finish;
    T* End;
    CArray() : Start(0), Finish(0), End(0) { }
    ~CArray()
    {
        for (T* p = Start; p != Finish; ++p)
            p->~T();
        std::free(Start);
    }
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    void resize(unsigned int n, const T& x)              // 0x0088A390's role (growth from empty here)
    {
        while (size() > n)
            (--Finish)->~T();
        if (n > size()) {
            T* mem = (T*)std::malloc(n * sizeof(T));
            T* cur = mem;
            for (T* p = Start; p != Finish; ++p, ++cur)
                new (cur) T(*p);
            for (; cur != mem + n; ++cur)
                new (cur) T(x);
            for (T* p = Start; p != Finish; ++p)
                p->~T();
            std::free(Start);
            Start = mem;
            Finish = End = mem + n;
        }
    }
    void resize(unsigned int n) { resize(n, T()); }
};

namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {
public:
    struct CThemeWeight { public: long ThemeID; float Weight; };
    float                BaseWeight;
    bool                 BaseWeightLocked;
    CArray<CThemeWeight> Themes;
    long                 ThemeTransitionTargetID;
    float                ThemeTransitionInitialWeight;
    bool                 ThemeTransitionActive;
    double               ThemeTransitionStartTime;
    float                ThemeTransitionDuration;
    CEnvironmentThemePriorityStackEntry() : BaseWeight(1.0f), ThemeTransitionActive(false)
    {
        BaseWeightLocked = false;
    }
    CEnvironmentThemePriorityStackEntry(const CEnvironmentThemePriorityStackEntry& o)
        : BaseWeight(o.BaseWeight), BaseWeightLocked(o.BaseWeightLocked), ThemeTransitionActive(o.ThemeTransitionActive) { }
};

class CEnvironmentThemePriorityStack {
public:
    CArray<CEnvironmentThemePriorityStackEntry> Ents;
    CEnvironmentThemePriorityStack(long count) { Ents.resize(count); }
};
}

int main()
{
    using NEnvironmentInternal::CEnvironmentThemePriorityStack;
    int fails = 0;
    {
        CEnvironmentThemePriorityStack stack(5);
        if (stack.Ents.size() != 5u) ++fails;
        for (unsigned int i = 0; i < stack.Ents.size(); ++i) {
            const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry& e = stack.Ents.Start[i];
            float d = e.BaseWeight - 1.0f;
            if (d > 1e-6f || d < -1e-6f) ++fails;
            if (e.BaseWeightLocked || e.ThemeTransitionActive || e.Themes.size() != 0u) ++fails;
        }
        CEnvironmentThemePriorityStack empty(0);
        if (empty.Ents.size() != 0u) ++fails;
    }
    if (fails) { std::printf("008883c0_TEST FAIL\n"); return 1; }
    std::printf("008883c0_TEST PASS\n");
    return 0;
}