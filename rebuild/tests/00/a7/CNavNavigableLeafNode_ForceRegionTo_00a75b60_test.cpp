// Behaviour test for 0x00A75B60 CNavNavigableLeafNode::ForceRegionTo (links the REAL row object).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REGION
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CNavNavigableLeafNode.h"
#include <stdio.h>
#include <string.h>
// The real class has only file / generator constructors: call the row on raw storage of the class's size with the member layout
// the header pins (PRegion +0x10), and check that no other byte changes.
struct SForceRegionCaller : CNavNavigableLeafNode { void Call(const CNavRegion* r) { ForceRegionTo(r); } };
static int g_fail;
#define CHECK(c) do { if (!(c)) { ++g_fail; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    static unsigned char buf[sizeof(CNavNavigableLeafNode) + 8];
    static unsigned char ref[sizeof(buf)];
    memset(buf, 0xA5, sizeof(buf)); memcpy(ref, buf, sizeof(buf));
    SForceRegionCaller* n = (SForceRegionCaller*)(void*)(buf + 4);
    const CNavRegion* r1 = (const CNavRegion*)(void*)0x12345678;
    n->Call(r1);
    CHECK(*(void**)(buf + 4 + 0x10) == (void*)r1);
    memcpy(ref + 4 + 0x10, buf + 4 + 0x10, 4);
    CHECK(memcmp(buf, ref, sizeof(buf)) == 0);
    n->Call(0);
    CHECK(*(void**)(buf + 4 + 0x10) == 0);
    memset(ref + 4 + 0x10, 0, 4);
    CHECK(memcmp(buf, ref, sizeof(buf)) == 0);
    if (g_fail == 0) printf("00a75b60_TEST PASS\n"); else printf("00a75b60_TEST FAIL (%d)\n", g_fail);
    return g_fail;
}