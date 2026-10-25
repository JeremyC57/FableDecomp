// Behaviour test for 0x00766C00 CTCObjectAugmentations::SetupAugmentationSounds().
#include <cstdio>
#include <map>
#include <string>

struct SoundSet { bool Played; std::string Sound; unsigned int Handle; SoundSet() : Played(false), Handle(0xFFFFFFFFu) { } };

static std::string GetString(int id)                       // CDefStringTable::GetString stand-in
{
    char buf[16];
    std::sprintf(buf, "snd%d", id);
    return std::string(buf);
}

struct Def {
    std::map<int, int> InitSounds;                          // type -> def string id
    std::map<int, int> LoopingSounds;
};

struct Augs {
    bool inLimbo;
    unsigned int fittedMask;                                // stand-in for HasCombinationOfAugmentations
    const Def* PDef;
    std::map<int, SoundSet> InitSounds;
    std::map<int, SoundSet> LoopingSounds;
    int cleared;
    bool HasCombination(int type) const { return (fittedMask & (1u << type)) != 0; }
    void ClearAllSounds() { ++cleared; InitSounds.clear(); LoopingSounds.clear(); }
    void Setup()
    {
        ClearAllSounds();
        if (!inLimbo) {
            std::map<int, int>::const_iterator it;
            for (it = PDef->InitSounds.begin(); it != PDef->InitSounds.end(); ++it) {
                int type = it->first;
                if (HasCombination(type)) { std::string sound = GetString(it->second); InitSounds[type].Sound = sound; }
            }
            for (it = PDef->LoopingSounds.begin(); it != PDef->LoopingSounds.end(); ++it) {
                int type = it->first;
                if (HasCombination(type)) { std::string sound = GetString(it->second); LoopingSounds[type].Sound = sound; }
            }
        }
    }
};

int main()
{
    int ok = 1;
    Def def;
    def.InitSounds[1] = 11; def.InitSounds[2] = 12; def.InitSounds[4] = 14;
    def.LoopingSounds[2] = 22; def.LoopingSounds[3] = 23;
    Augs a;
    a.PDef = &def; a.cleared = 0; a.inLimbo = true; a.fittedMask = (1u << 2) | (1u << 4);
    a.InitSounds[9].Sound = "old";
    a.Setup();                                              // in limbo: only cleared
    if (a.cleared != 1 || !a.InitSounds.empty() || !a.LoopingSounds.empty()) ok = 0;
    a.inLimbo = false;
    a.Setup();
    if (a.cleared != 2 || a.InitSounds.size() != 2 || a.LoopingSounds.size() != 1) ok = 0;
    if (a.InitSounds[2].Sound != "snd12" || a.InitSounds[4].Sound != "snd14" || a.LoopingSounds[2].Sound != "snd22") ok = 0;
    if (a.InitSounds[2].Played || a.InitSounds[2].Handle != 0xFFFFFFFFu) ok = 0;   // fresh default sound sets
    if (!ok) { std::printf("00766c00_TEST FAIL\n"); return 1; }
    std::printf("00766c00_TEST PASS\n");
    return 0;
}