// Behaviour test for NUISystem::CUIMiscThingsDef::operator=(const CUIMiscThingsDef&) (0x004321F0): a model of the row -- the
// implicit assignment copies the resource base, PDefManager, GlobalIndex and the three flag bits, the parent-def members
// (instantiation name, sub-def map), then every misc-things member in declaration order (wide / char strings, both maps,
// scalars, 2D vectors) as deep copies; Copy (slot 19) forwards to it.
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
struct SubDefInfo { long DefIndex; long OriginalParentDefIndex; };
struct Resource { virtual ~Resource() { } unsigned long IVRefCount; unsigned long LastUsedFrame; };
struct DefClassBase : Resource {
    void* PDefManager; unsigned long GlobalIndex;
    unsigned char Setup : 1; unsigned char Template : 1; unsigned char DefaultValsApplied : 1;
};
struct ParentDef : DefClassBase { long InstantiationName; std::vector<std::pair<unsigned long, SubDefInfo> > SubDefInfoMap; bool Dirty; };
struct Vec2 { float X, Y; };
struct UIMiscThingsDef : ParentDef {
    std::wstring SpaceSeparator;
    std::wstring Minus;
    unsigned long CoreGraphic;
    Vec2 RingCenter;
    float WorldMapWidth;
    std::map<unsigned long, unsigned long> MapPaths;
    std::string SoundUpDown;
    std::wstring FrontEndMusic;
    long QuestStartScreenMusic;
    std::map<std::string, long> MiniMapGraphics;
    float MinimumFilterColor;
    void Copy(const DefClassBase* def) { *this = *static_cast<const UIMiscThingsDef*>(def); }
};
int main()
{
    UIMiscThingsDef a, b;
    a.IVRefCount = 3; a.LastUsedFrame = 77; a.PDefManager = &b; a.GlobalIndex = 9;
    a.Setup = 1; a.Template = 0; a.DefaultValsApplied = 1;
    a.InstantiationName = 21; a.Dirty = true;
    SubDefInfo info = { 1, 2 };
    a.SubDefInfoMap.push_back(std::make_pair(5ul, info));
    a.SpaceSeparator = L" "; a.Minus = L"-"; a.CoreGraphic = 400; a.RingCenter.X = 0.5f; a.RingCenter.Y = 0.25f;
    a.WorldMapWidth = 640.0f; a.MapPaths[1] = 10; a.MapPaths[2] = 20; a.SoundUpDown = "UI_UPDOWN";
    a.FrontEndMusic = L"FE"; a.QuestStartScreenMusic = 3; a.MiniMapGraphics["HERO"] = 12; a.MinimumFilterColor = 0.1f;
    b.Setup = 0; b.Template = 1; b.DefaultValsApplied = 0; b.MapPaths[7] = 70; b.Minus = L"x";
    b.Copy(&a);
    a.MapPaths[1] = 11; a.SoundUpDown = "changed"; a.SubDefInfoMap.clear(); a.MiniMapGraphics["HERO"] = 0;
    int ok = b.IVRefCount == 3 && b.LastUsedFrame == 77 && b.PDefManager == &b && b.GlobalIndex == 9 &&
             b.Setup == 1 && b.Template == 0 && b.DefaultValsApplied == 1 && b.InstantiationName == 21 && b.Dirty &&
             b.SubDefInfoMap.size() == 1 && b.SubDefInfoMap[0].second.OriginalParentDefIndex == 2 &&
             b.SpaceSeparator == L" " && b.Minus == L"-" && b.CoreGraphic == 400 && b.RingCenter.Y == 0.25f &&
             b.WorldMapWidth == 640.0f && b.MapPaths.size() == 2 && b.MapPaths[1] == 10 && b.SoundUpDown == "UI_UPDOWN" &&
             b.FrontEndMusic == L"FE" && b.QuestStartScreenMusic == 3 && b.MiniMapGraphics["HERO"] == 12 &&
             b.MinimumFilterColor == 0.1f;
    printf(ok ? "004321f0_TEST PASS\n" : "004321f0_TEST FAIL\n");
    return ok ? 0 : 1;
}