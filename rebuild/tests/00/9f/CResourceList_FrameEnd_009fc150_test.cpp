// Test for 0x009FC150 CResourceList::FrameEnd: advances the frame counter and clears the per-frame unload count.
#include <stdio.h>
class CResourceList {
public:
    void FrameEnd() { ++CurrentFrame; UnloadedThisFrame = 0; }
    unsigned int CurrentFrame; unsigned int UnloadedThisFrame; unsigned int Other;
};
int main()
{
    CResourceList r; r.CurrentFrame = 41; r.UnloadedThisFrame = 7; r.Other = 5;
    r.FrameEnd();
    bool ok = r.CurrentFrame == 42 && r.UnloadedThisFrame == 0 && r.Other == 5;
    puts(ok ? "009fc150_TEST PASS" : "009fc150_TEST FAIL");
    return ok ? 0 : 1;
}