// Behaviour test for 0x0099A350 CInitBaseClass::Initialise(): sets Valid and returns true.
#include <cstdio>

class InitBaseClass {
public:
    bool Valid;
    InitBaseClass() : Valid(false) { }
    virtual ~InitBaseClass() { }
    bool IsValid() const { return Valid; }
    bool Initialise() { Valid = true; return true; }
};

int main()
{
    InitBaseClass c;
    if (c.IsValid()) { std::printf("0099a350_TEST FAIL\n"); return 1; }
    if (!c.Initialise() || !c.IsValid()) { std::printf("0099a350_TEST FAIL\n"); return 1; }
    if (!c.Initialise() || !c.IsValid()) { std::printf("0099a350_TEST FAIL\n"); return 1; }
    std::printf("0099a350_TEST PASS\n");
    return 0;
}