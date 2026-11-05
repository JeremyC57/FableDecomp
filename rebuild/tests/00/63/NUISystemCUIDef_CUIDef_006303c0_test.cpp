// Behaviour test for NUISystem::CUIDef::CUIDef(const CUIDef&) (0x006303C0): a model of the row -- the copy constructor
// default-constructs every base and member, then assigns the def's fields one by one, so containers are deep copies and
// later edits to either def leave the other alone.
#include <stdio.h>
#include <string>
#include <vector>
struct UIStateDef { long GraphicIndex; float UpdateTime; };
struct UIDef {
    long Type;
    std::vector<UIStateDef> States;
    std::vector<long> Children;
    std::string MovieFilename;
    float Angle;
    bool PasswordBox;
    UIDef() : Type(0), Angle(0.0f), PasswordBox(false) { }
    UIDef(const UIDef& def) : Type(0), Angle(0.0f), PasswordBox(false)
    {
        Type = def.Type;
        States = def.States;
        Children = def.Children;
        Angle = def.Angle;
        PasswordBox = def.PasswordBox;
        MovieFilename = def.MovieFilename;
    }
};
int main()
{
    UIDef a;
    a.Type = 7; a.Angle = 1.5f; a.PasswordBox = true; a.MovieFilename = "intro.bik";
    UIStateDef s = { 3, 0.25f };
    a.States.push_back(s); a.Children.push_back(11); a.Children.push_back(12);
    UIDef b(a);
    a.Children.push_back(13); a.States[0].GraphicIndex = 9;
    int ok = b.Type == 7 && b.Angle == 1.5f && b.PasswordBox && b.MovieFilename == "intro.bik" && b.States.size() == 1 &&
             b.States[0].GraphicIndex == 3 && b.Children.size() == 2 && b.Children[1] == 12;
    printf(ok ? "006303c0_TEST PASS\n" : "006303c0_TEST FAIL\n");
    return ok ? 0 : 1;
}