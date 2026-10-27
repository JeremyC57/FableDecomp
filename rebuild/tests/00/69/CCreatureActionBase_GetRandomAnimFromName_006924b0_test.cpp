// Behaviour test for CCreatureActionBase::GetRandomAnimFromName (0x006924B0): dereferences the creature's loaded-anim lookup with loadedOnly = true.
#include <cstdio>
class CAnimationEntry { public: int Id; };
class CCharString { public: const char* Text; };
static bool g_lastLoaded = false;
class CThingCreatureBase { public:
    CAnimationEntry Entry;
    const CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString&, bool loadedOnly) const { g_lastLoaded = loadedOnly; return &Entry; }
};
class CCreatureActionBase { public:
    static const CAnimationEntry& GetRandomAnimFromName(const CThingCreatureBase& creature, const CCharString& name)
    {
        return *creature.GetRandomLoadedAnimFromName(name, true);
    }
};
int main()
{
    CThingCreatureBase c; c.Entry.Id = 42;
    CCharString n; n.Text = "walk";
    const CAnimationEntry& e = CCreatureActionBase::GetRandomAnimFromName(c, n);
    if (&e != &c.Entry || e.Id != 42 || !g_lastLoaded) { std::printf("006924b0_TEST FAIL\n"); return 1; }
    std::printf("006924b0_TEST PASS\n");
    return 0;
}