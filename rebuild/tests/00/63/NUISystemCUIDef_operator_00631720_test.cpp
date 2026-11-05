// Behaviour test for NUISystem::CUIDef::operator=(const CUIDef&) (0x00631720): a model of the row -- the implicit
// assignment copies the parent-def base (instantiation name, sub-def map) and then every member in declaration order,
// containers as deep copies, so later edits to either def leave the other alone; Copy (slot 19) forwards to it.
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
struct SubDefInfo { long A; long B; };
struct ParentDef { long GlobalIndex; long InstantiationName; std::vector<std::pair<unsigned long, SubDefInfo> > SubDefInfoMap; bool Dirty; };
struct UIStateDef { long GraphicIndex; float UpdateTime; };
struct UIDef : ParentDef {
    long Type;
    std::vector<UIStateDef> States;
    std::map<int, long> Sprites;
    std::vector<long> Children;
    float Angle;
    long ActionOnLeftHeld;
    std::map<unsigned long, std::string> ActionMap;
    std::string MovieFilename;
    std::vector<float> SwappingTimes;
    long Sprite2DFlag;
    void Copy(const ParentDef* def) { *this = *static_cast<const UIDef*>(def); }
};
int main()
{
    UIDef a, b;
    a.GlobalIndex = 4; a.InstantiationName = 21; a.Dirty = true;
    SubDefInfo info = { 1, 2 };
    a.SubDefInfoMap.push_back(std::make_pair(5ul, info));
    a.Type = 7; a.Angle = 1.5f; a.ActionOnLeftHeld = 33; a.Sprite2DFlag = 2;
    UIStateDef s = { 3, 0.25f };
    a.States.push_back(s); a.Sprites[0] = 40; a.Children.push_back(11); a.Children.push_back(12);
    a.ActionMap[9] = "jump"; a.MovieFilename = "intro.bik"; a.SwappingTimes.push_back(0.5f);
    b.Type = 1; b.Children.push_back(99);
    b.Copy(&a);
    a.Children.push_back(13); a.States[0].GraphicIndex = 9; a.ActionMap[9] = "run"; a.SubDefInfoMap.clear();
    int ok = b.GlobalIndex == 4 && b.InstantiationName == 21 && b.Dirty && b.SubDefInfoMap.size() == 1 &&
             b.SubDefInfoMap[0].second.B == 2 && b.Type == 7 && b.Angle == 1.5f && b.ActionOnLeftHeld == 33 &&
             b.Sprite2DFlag == 2 && b.States.size() == 1 && b.States[0].GraphicIndex == 3 && b.Sprites[0] == 40 &&
             b.Children.size() == 2 && b.Children[1] == 12 && b.ActionMap[9] == "jump" &&
             b.MovieFilename == "intro.bik" && b.SwappingTimes.size() == 1;
    printf(ok ? "00631720_TEST PASS\n" : "00631720_TEST FAIL\n");
    return ok ? 0 : 1;
}