#include <cstdio>

#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE
#include "engine/CNavQuadTree.h"

int main()
{
    float v = CNavQuadTree::GetSmallestNodeSize();
    if (v == 0.5f)
        printf("00a76320_TEST PASS\n");
    else
        printf("FAIL %f\n", v);
    return 0;
}
