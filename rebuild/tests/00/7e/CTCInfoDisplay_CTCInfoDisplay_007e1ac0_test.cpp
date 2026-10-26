// Behaviour test for CTCInfoDisplay::CTCInfoDisplay @ 0x007E1AC0: flags false, empty tags, zero radius and time.
#include <cstdio>
class CThing { public: long id; };
class CCharString { public: const char* text; CCharString() : text("") {} };
class CTCBase { public: CThing* ParentThing; CTCBase(CThing* owner) : ParentThing(owner) {} virtual ~CTCBase() {} };
struct CTCInfoDisplay : CTCBase {
    bool Forward, Backward;
    CCharString TextTag, TextTagBack;
    float Radius, DisplayTime;
    CTCInfoDisplay(CThing* owner) : CTCBase(owner), Forward(false), Backward(false), Radius(0.0f), DisplayTime(0.0f) {}
};
int main()
{
    CThing t; t.id = 2;
    CTCInfoDisplay d(&t);
    if (d.ParentThing != &t || d.Forward || d.Backward || d.TextTag.text[0] != 0 || d.TextTagBack.text[0] != 0 || d.Radius != 0.0f || d.DisplayTime != 0.0f)
    { std::printf("007e1ac0_TEST FAIL\n"); return 1; }
    std::printf("007e1ac0_TEST PASS\n");
    return 0;
}