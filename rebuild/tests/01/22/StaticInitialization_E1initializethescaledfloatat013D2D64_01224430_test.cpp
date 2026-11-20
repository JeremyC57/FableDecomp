#include <stdio.h>
#ifndef _MSC_VER
#define __cdecl
#endif
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE
#include "engine/CNavQuadTree.h"
float g_UnprovenScaledFloat_013D2D64=CNavQuadTree::GetSmallestNodeSize()*0.9f;
static int calls;float __fastcall CNavQuadTree::GetSmallestNodeSize(){++calls;return 5.0f;}int main(){bool ok=calls==1&&g_UnprovenScaledFloat_013D2D64==4.5f;puts(ok?"01224430_TEST PASS":"01224430_TEST FAIL");return ok?0:1;}