// Behaviour test for 0x00A77480 CNavQuadTree::JoinNavigationLayers (links the REAL row object). NF161 (q4782): rewritten for the
// shared-header row: the native layout is raw storage (TransitionPoints begin/end at +0x28 / +0x2c, 16-byte CNavigationTransitionPoint:
// Position +0, Layer1 +8, Layer2 +0xC; leaf nodes 0x20 bytes, Blocked +0xf, Neighbours +0x14). The private member is reached through a
// linker alias (thiscall = fastcall with a spare edx); the callees GetNodeAt and CSmallVector::push_back are recording stubs aliased the
// same way.
#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CallJoinNavigationLayers(void* self, int spare);
#pragma comment(linker, "/alternatename:@CallJoinNavigationLayers@8=?JoinNavigationLayers@CNavQuadTree@@AAEXXZ")
#pragma comment(linker, "/alternatename:?GetNodeAt@CNavQuadTree@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z=@StubGetNodeAt@16")
#pragma comment(linker, "/alternatename:?push_back@?$CSmallVector@PBVCANavQuadTreeNode@@$07@@QAEXABQBVCANavQuadTreeNode@@@Z=@StubPushBack@12")

static unsigned char gNodes[4][0x20];
static void* gLayerNodes[8];
static int gLookupCount;
struct LinkCall { const void* Vector; const void* Value; };
static LinkCall gLinks[8];
static int gLinkCount;

extern "C" void* __fastcall StubGetNodeAt(const void*, int, const void*, long layer)
{
    ++gLookupCount;
    if (layer < 0 || layer >= 8) return 0;
    return gLayerNodes[layer];
}

extern "C" void __fastcall StubPushBack(void* self, int, const void* const* value)
{
    if (gLinkCount < 8) { gLinks[gLinkCount].Vector = self; gLinks[gLinkCount].Value = *value; }
    ++gLinkCount;
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
    CallJoinNavigationLayers(tree, 0);
    if (gLookupCount != 0 || gLinkCount != 0) { printf("00a77480_TEST FAIL 1\n"); return 1; }

    points[0].Layer1 = 0; points[0].Layer2 = 1;
    points[1].Layer1 = 2; points[1].Layer2 = 7;   // layer 7 has no node
    points[2].Layer1 = 2; points[2].Layer2 = 3;   // node 2 is blocked
    for (int i = 0; i < 4; ++i) gLayerNodes[i] = gNodes[i];
    gNodes[2][0x0f] = 1;
    last = points + 3;
    memcpy(tree + 0x2c, &last, 4);
    CallJoinNavigationLayers(tree, 0);
    if (gLookupCount != 6) { printf("00a77480_TEST FAIL 2\n"); return 2; }
    if (gLinkCount != 2) { printf("00a77480_TEST FAIL 3\n"); return 3; }
    if (gLinks[0].Vector != gNodes[0] + 0x14 || gLinks[0].Value != gNodes[1]) { printf("00a77480_TEST FAIL 4\n"); return 4; }
    if (gLinks[1].Vector != gNodes[1] + 0x14 || gLinks[1].Value != gNodes[0]) { printf("00a77480_TEST FAIL 5\n"); return 5; }
    printf("NAV_QUAD_TREE_JOIN_NAVIGATION_LAYERS_TEST PASS\n");
    return 0;
}
