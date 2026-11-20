#include <cstdio>
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CENTRE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_ORIGINAL_FOUR_COORD_CTOR
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
C2DBoxI C2DBoxF::ToC2DBoxI() const { return C2DBoxI((long)TLX, (long)TLY, (long)BRX, (long)BRY); }   // the row's body
int main() { C2DBoxF f(1.9f, -2.9f, 3.1f, 4.5f); C2DBoxI b = f.ToC2DBoxI(); if (b.TLX != 1 || b.TLY != -2 || b.BRX != 3 || b.BRY != 4) { std::printf("00a55a30_TEST FAIL\n"); return 1; } std::printf("00a55a30_TEST PASS\n"); return 0; }
