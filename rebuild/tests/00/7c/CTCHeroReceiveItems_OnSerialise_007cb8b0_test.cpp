// Behaviour test for 0x007CB8B0 CTCHeroReceiveItems::OnSerialise: saving (modes 1 and 3) writes the def name of every picked-up
// def index above zero under "PickedUpItems"; loading (any other mode) reads the names back and appends their def indices to the
// picked-up list (keeping what is already there).
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static const char* g_names[] = { "", "OBJECT_SWORD", "OBJECT_APPLE", "OBJECT_KEY" };
class CDefinitionManager { public:
    std::string GetDefNameFromGlobalIndex(long index) const { return g_names[index]; }
    long GetDefGlobalIndexFromName(const std::string& name) const
    {
        for (long i = 1; i < 4; ++i) if (name == g_names[i]) return i;
        return -1;
    }
};
class CPersistContext { public:
    int Mode;
    std::string LastName;
    std::vector<std::string> Stored;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void TransferVector(const char* name, std::vector<std::string>& v)
    {
        LastName = name;
        if (IsSaving()) Stored = v; else v = Stored;
    }
};
class CTCHeroReceiveItems { public:
    CDefinitionManager Defs;
    std::vector<long> PickedUpItems;
    const CDefinitionManager& GetDefinitionManager() const { return Defs; }
    void OnSerialise(CPersistContext& context, bool)
    {
        if (context.IsSaving()) {
            std::vector<std::string> names;
            names.reserve(PickedUpItems.size());
            for (unsigned int i = 0; i < PickedUpItems.size(); ++i)
                if (PickedUpItems[i] > 0)
                    names.push_back(GetDefinitionManager().GetDefNameFromGlobalIndex(PickedUpItems[i]));
            context.TransferVector("PickedUpItems", names);
        } else {
            std::vector<std::string> names;
            context.TransferVector("PickedUpItems", names);
            PickedUpItems.reserve(names.size());
            for (unsigned int i = 0; i < names.size(); ++i)
                PickedUpItems.push_back(GetDefinitionManager().GetDefGlobalIndexFromName(names[i]));
        }
    }
};

#define FAIL { std::printf("007cb8b0_TEST FAIL\n"); return 1; }
int main()
{
    CTCHeroReceiveItems hero;
    hero.PickedUpItems.push_back(3); hero.PickedUpItems.push_back(0); hero.PickedUpItems.push_back(1);
    CPersistContext save; save.Mode = 3;
    hero.OnSerialise(save, true);
    if (save.LastName != "PickedUpItems" || save.Stored.size() != 2 || save.Stored[0] != "OBJECT_KEY" || save.Stored[1] != "OBJECT_SWORD") FAIL;
    save.Mode = 1; save.Stored.clear();
    hero.OnSerialise(save, false);
    if (save.Stored.size() != 2) FAIL;
    CTCHeroReceiveItems loaded;
    loaded.PickedUpItems.push_back(2);
    CPersistContext load; load.Mode = 2; load.Stored = save.Stored;
    loaded.OnSerialise(load, true);
    if (loaded.PickedUpItems.size() != 3 || loaded.PickedUpItems[0] != 2 || loaded.PickedUpItems[1] != 3 || loaded.PickedUpItems[2] != 1) FAIL;
    std::printf("007cb8b0_TEST PASS\n");
    return 0;
}