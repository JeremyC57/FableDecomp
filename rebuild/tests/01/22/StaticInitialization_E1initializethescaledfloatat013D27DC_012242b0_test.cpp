#include <stdio.h>
#ifndef _MSC_VER
#define __cdecl
#endif
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE
#include "engine/CNavQuadTree.h"
float g_UnprovenScaledFloat_013D27DC=CNavQuadTree::GetSmallestNodeSize()*0.9f;
static int calls;float __fastcall CNavQuadTree::GetSmallestNodeSize(){++calls;return 5.0f;}int main(){bool ok=calls==1&&g_UnprovenScaledFloat_013D27DC==4.5f;puts(ok?"012242b0_TEST PASS":"012242b0_TEST FAIL");return ok?0:1;}