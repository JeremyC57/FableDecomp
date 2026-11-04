// Behaviour test for NUISystem::CComponent::SetName (0x0052F0F0): the component's Name becomes a copy of the argument
// (CCharString::operator=); changing the argument afterwards does not change the name.
#include <cstdio>
#include <string>
struct Component {
    std::string Name;
    void SetName(const std::string& name) { Name = name; }
};
int main()
{
    Component c;
    c.Name = "Old";
    std::string n = "FrontEndMainMenu";
    c.SetName(n);
    n = "x";
    if (c.Name != "FrontEndMainMenu") { std::printf("0052f0f0_TEST FAIL\n"); return 1; }
    c.SetName(std::string());
    if (!c.Name.empty()) { std::printf("0052f0f0_TEST FAIL\n"); return 1; }
    std::printf("0052f0f0_TEST PASS\n");
    return 0;
}