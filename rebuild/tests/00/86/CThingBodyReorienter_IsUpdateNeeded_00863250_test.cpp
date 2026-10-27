// Behaviour test for 0x00863250 CThingBodyReorienter::IsUpdateNeeded() const: needed until BodyReorientationDone.
#include <cstdio>

class BodyReorienter {
public:
    bool BodyReorientationDone;
    bool ReorientingBody;
    bool IsUpdateNeeded() const { return !BodyReorientationDone; }
};

int main()
{
    BodyReorienter r;
    r.BodyReorientationDone = false; r.ReorientingBody = true;
    if (!r.IsUpdateNeeded()) { std::printf("00863250_TEST FAIL\n"); return 1; }
    r.BodyReorientationDone = true; r.ReorientingBody = false;
    if (r.IsUpdateNeeded()) { std::printf("00863250_TEST FAIL\n"); return 1; }
    std::printf("00863250_TEST PASS\n");
    return 0;
}