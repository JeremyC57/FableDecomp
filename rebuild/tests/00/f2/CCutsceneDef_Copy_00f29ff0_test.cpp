// Behaviour test for CCutsceneDef::Copy @ 0x00F29FF0 (through operator= @ 0x00F2A000): the implicit assignment copies the resource words,
// PDefManager, GlobalIndex, the three flag bits (leaving the other bits of the flag byte alone), InstantiationName, the SubDefInfoMap
// (vector + Dirty) and the eight string lists, and returns *this; Copy assigns through it.
#include <cstdio>
#include <vector>
#include <string>
struct CResourceModel { unsigned int words[6]; };                 // CResource +0x04..+0x18 (0x00431F60 copies them)
struct CDefClassBaseModel : CResourceModel {
    void* PDefManager; unsigned int GlobalIndex;
    unsigned char Setup : 1; unsigned char Template : 1; unsigned char DefaultValsApplied : 1; unsigned char Other : 5;
    CDefClassBaseModel& operator=(const CDefClassBaseModel& o)
    {   // retail: the base call, two dwords, then each bit merged as x ^ ((x ^ y) & mask): only bits 0..2 change
        CResourceModel::operator=(o); PDefManager = o.PDefManager; GlobalIndex = o.GlobalIndex;
        Setup = o.Setup; Template = o.Template; DefaultValsApplied = o.DefaultValsApplied; return *this;
    }
};
struct CVectorMapModel { std::vector<std::pair<unsigned int, unsigned int> > v; bool Dirty; };
struct CParentDefClassBaseModel : CDefClassBaseModel { long InstantiationName; CVectorMapModel SubDefInfoMap; };
struct CCutsceneDefModel : CParentDefClassBaseModel {
    std::vector<std::string> Macro, SkipCond, Lights, LightScene, SetupCond, Sound, Answer0, Answer1;
    void Copy(const CParentDefClassBaseModel* other) { *this = *static_cast<const CCutsceneDefModel*>(other); }
};
int main()
{
    CCutsceneDefModel a, b;
    for (int i = 0; i < 6; ++i) { a.words[i] = 100 + i; b.words[i] = 0; }
    a.PDefManager = &a; a.GlobalIndex = 77; a.Setup = 1; a.Template = 0; a.DefaultValsApplied = 1; a.Other = 3;
    b.PDefManager = 0; b.GlobalIndex = 0; b.Setup = 0; b.Template = 1; b.DefaultValsApplied = 0; b.Other = 21;
    a.InstantiationName = 12; b.InstantiationName = -1;
    a.SubDefInfoMap.v.push_back(std::make_pair(5u, 6u)); a.SubDefInfoMap.Dirty = true; b.SubDefInfoMap.Dirty = false;
    a.Macro.push_back("m"); a.SkipCond.push_back("s"); a.Lights.push_back("l"); a.LightScene.push_back("ls");
    a.SetupCond.push_back("c"); a.Sound.push_back("snd"); a.Answer0.push_back("y"); a.Answer1.push_back("n"); a.Answer1.push_back("n2");
    b.Answer0.push_back("old"); b.Answer0.push_back("old2");
    b.Copy(&a);
    bool ok = b.words[0] == 100 && b.words[5] == 105 && b.PDefManager == &a && b.GlobalIndex == 77
        && b.Setup == 1 && b.Template == 0 && b.DefaultValsApplied == 1 && b.Other == 21
        && b.InstantiationName == 12 && b.SubDefInfoMap.v.size() == 1 && b.SubDefInfoMap.Dirty
        && b.Macro[0] == "m" && b.SetupCond[0] == "c" && b.Answer0.size() == 1 && b.Answer0[0] == "y" && b.Answer1.size() == 2;
    CCutsceneDefModel& r = (b = a);
    ok = ok && &r == &b;
    if (!ok) { std::printf("00f29ff0_TEST FAIL\n"); return 1; }
    std::printf("00f29ff0_TEST PASS\n");
    return 0;
}