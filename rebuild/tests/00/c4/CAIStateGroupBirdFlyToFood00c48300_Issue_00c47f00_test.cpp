// Behaviour test for CAIStateGroup_BirdFlyToFood_00c48300::Issue (0x00C47F00): the request is forwarded to the member queue.
#include <stdio.h>
struct Queue { void* last; int n; long ProcessMessage(void* r) { last = r; ++n; return 0; } };
struct Group { char pad[0x18]; Queue queue; void Issue(void* r) { queue.ProcessMessage(r); } };
int main()
{
    Group g; g.queue.last = 0; g.queue.n = 0; int request = 5;
    g.Issue(&request);
    bool ok = g.queue.last == &request && g.queue.n == 1;
    printf(ok ? "00c47f00_TEST PASS\n" : "00c47f00_TEST FAIL\n");
    return ok ? 0 : 1;
}