// Behaviour test for 0x00A77500 CNavQuadTree::VerifyTransitionPoints (links the REAL row object). NF161 (q4782): rewritten for the
// shared-header row: the native layout is raw storage (TransitionPoints begin/end at +0x28 / +0x2c, 16-byte CNavigationTransitionPoint:
// Position +0, Layer1 +8, Layer2 +0xC). The private member is reached through a linker alias (thiscall = fastcall with a spare edx), and
// the callee ?GetNodeAt@CNavQuadTree@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z is a recording stub aliased the same way.
#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CallVerifyTransitionPoints(void* self, int spare);
#pragma comment(linker, "/alternatename:@CallVerifyTransitionPoints@8=?VerifyTransitionPoints@CNavQuadTree@@ABEXXZ")
#pragma comment(linker, "/alternatename:?GetNodeAt@CNavQuadTree@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z=@StubGetNodeAt@16")

struct NavCall { const void* Self; const void* Position; long Layer; };
static NavCall gCalls[8];
static int gCallCount;

extern "C" const void* __fastcall StubGetNodeAt(const void* self, int, const void* position, long layer)
{
    if (gCallCount < 8) { gCalls[gCallCount].Self = self; gCalls[gCallCount].Position = position; gCalls[gCallCount].Layer = layer; }
    ++gCallCount;
    return 0;
}

struct Point { float X, Y; long Layer1, Layer2; };

int main()
{
    static unsigned char tree[0x88];
    Point points[3];
    memset(tree, 0, sizeof(tree));
    memset(points, 0, sizeof(points));
    Point* first = points;
    Point* last = points;
    memcpy(tree + 0x28, &first, 4);
    memcpy(tree + 0x2c, &last, 4);
    CallVerifyTransitionPoints(tree, 0);
    if (gCallCount != 0) { printf("00a77500_TEST FAIL 1\n"); return 1; }

    points[0].Layer1 = 2;  points[0].Layer2 = 5;
    points[1].Layer1 = -1; points[1].Layer2 = 9;
    points[2].Layer1 = 0;  points[2].Layer2 = 7;
    last = points + 3;
    memcpy(tree + 0x2c, &last, 4);
    CallVerifyTransitionPoints(tree, 0);
    static const long want[6] = { 2, 5, -1, 9, 0, 7 };
    if (gCallCount != 6) { printf("00a77500_TEST FAIL 2\n"); return 2; }
    for (int i = 0; i < 6; ++i)
        if (gCalls[i].Self != tree || gCalls[i].Position != &points[i / 2] || gCalls[i].Layer != want[i]) {
            printf("00a77500_TEST FAIL %d\n", 3 + i); return 3 + i; }
    printf("NAV_QUAD_TREE_VERIFY_TRANSITION_POINTS_TEST PASS\n");
    return 0;
}
