#include <stdio.h>
#include <math.h>
#include "rebuild_abi.h"
#include "engine/GFCos.h"
namespace { const float COS_45 = (float)GFCos(45.0f/360.0f); }
int main() { if (fabs(COS_45-0.70710678)>0.000001) return 1; puts("0121ee10_TEST PASS"); return 0; }
