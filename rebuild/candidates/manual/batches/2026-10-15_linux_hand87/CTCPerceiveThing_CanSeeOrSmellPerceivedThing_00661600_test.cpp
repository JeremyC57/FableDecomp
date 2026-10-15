#include <cstdio>
struct CTCPerceiveThing {
    bool LineOfSight; bool Smelt;
    bool CanSeeOrSmellPerceivedThing() const { return LineOfSight || Smelt; }
};
int main()
{
    CTCPerceiveThing p;
    p.LineOfSight = false; p.Smelt = false; bool none = p.CanSeeOrSmellPerceivedThing();
    p.LineOfSight = true; bool seen = p.CanSeeOrSmellPerceivedThing();
    p.LineOfSight = false; p.Smelt = true; bool smelt = p.CanSeeOrSmellPerceivedThing();
    if (none || !seen || !smelt) { std::printf("00661600_TEST FAIL\n"); return 1; }
    std::printf("00661600_TEST PASS\n");
    return 0;
}
