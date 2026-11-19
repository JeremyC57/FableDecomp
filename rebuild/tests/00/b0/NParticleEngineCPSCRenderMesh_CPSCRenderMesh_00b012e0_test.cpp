// Behaviour test for NParticleEngine::CPSCRenderMesh::CPSCRenderMesh (0x00B012E0): the row's packed database defaults on a
// stand-alone copy of its bit-field block (32-bit units, as VC7.1 lays out `unsigned long`) (+0x2c..+0x44), each float default quantised through the shared GFPackFloat (round to
// two decimals, map [min, max] onto [0, 2^bits - 1], round to nearest even: retail asm under VC7.1, the FABLE_PORTABLE twin
// elsewhere), the bools packed beside them; the five dwords are checked whole.
#ifndef _MSC_VER
#define FABLE_PORTABLE
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>
#include <new>
#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
#include "engine/GFPackFloat.h"

struct Block {
    unsigned int BlendMode : 3;  unsigned int TrailBlendMode : 3; unsigned int BlendOp : 3; unsigned int TrailBlendOp : 3;
    unsigned int FadeInEndInteger : 10; unsigned int FadeOutBeginInteger : 10;
    unsigned int TrailLengthInteger : 12; unsigned int FlickerMinAlphaInteger : 10; unsigned int FlickerMinSizeInteger : 10;
    unsigned int StartRenderSizeX : 11; unsigned int StartRenderSizeY : 11;
    unsigned int StartRenderSizeZ : 11; unsigned int CentredOnPos : 1; unsigned int AlphaFadeEnable : 1; unsigned int EndRenderSizeX : 11;
    unsigned int EndRenderSizeY : 11; unsigned int EndRenderSizeZ : 11; unsigned int SizeFadeEnable : 1; unsigned int FlickerEnable : 1;
    unsigned int TrailUseStartColour : 1; unsigned int TrailUseMidColour : 1; unsigned int TrailUseEndColour : 1;
    unsigned int UseRenderSizeParam : 1;
    unsigned int FlickerBias : 8; unsigned int FlickerSpeed : 12; unsigned int AlphaFadeMinimum : 7; unsigned int UseStartColour : 1;
    unsigned int SizeFadeMinimum : 7; unsigned int UseMidColour : 1; unsigned int TrailWidth : 10; unsigned int UseEndColour : 1;
    Block()
        : FadeInEndInteger(0), FadeOutBeginInteger(1000),
          TrailLengthInteger(1000), FlickerMinAlphaInteger(0), FlickerMinSizeInteger(0),
          StartRenderSizeX(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)), StartRenderSizeY(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)),
          StartRenderSizeZ(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)), CentredOnPos(0), AlphaFadeEnable(0),
          EndRenderSizeX(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)),
          EndRenderSizeY(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)), EndRenderSizeZ(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)),
          SizeFadeEnable(0), FlickerEnable(0), TrailUseStartColour(1), TrailUseMidColour(0), TrailUseEndColour(0), UseRenderSizeParam(0),
          FlickerBias(GFPackFloat(0.0f, 8, -1.0f, 1.0f, 2)), FlickerSpeed(GFPackFloat(15.0f, 12, 0.0f, 30.0f, 2)),
          AlphaFadeMinimum(GFPackFloat(0.0f, 7, 0.0f, 1.0f, 2)), UseStartColour(1),
          SizeFadeMinimum(GFPackFloat(0.0f, 7, 0.0f, 1.0f, 2)), UseMidColour(0), TrailWidth(GFPackFloat(0.5f, 10, 0.0f, 10.0f, 2)), UseEndColour(0)
    {
        BlendMode = 2;          // GetParticleEmitterManager().GetDefaultBlendMode()
        BlendOp = 0;            // GetDefaultBlendOp()
        TrailBlendMode = 2;
        TrailBlendOp = 0;
    }
};

int main()
{
    int fails = 0;
    unsigned char raw[sizeof(Block)];
    memset(raw, 0xff, sizeof raw);                       // every unnamed bit starts set: each field must be written
    Block* b = new (raw) Block;
    unsigned int w[7];
    memcpy(w, b, sizeof w);
    if (sizeof(Block) != 28) ++fails;
    if (GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2) != 102) ++fails;              // 1/20 * 2047 = 102.35
    if (GFPackFloat(0.0f, 8, -1.0f, 1.0f, 2) != 128) ++fails;               // 127.5: ties to even
    if (GFPackFloat(15.0f, 12, 0.0f, 30.0f, 2) != 2048) ++fails;            // 2047.5: ties to even
    if (GFPackFloat(0.5f, 10, 0.0f, 10.0f, 2) != 51) ++fails;               // 51.15
    if (w[0] != (2u | (2u << 3) | (1000u << 22))) ++fails;                  // +0x2c blend modes, fade-out 1000
    if (w[1] != 1000u) ++fails; // +0x30 trail length 1000, minima 0
    if (w[2] != (102u | (102u << 11) | (w[2] & 0xFFC00000u))) ++fails;      // +0x34 start X / Y (bits 22.. unused)
    if (w[3] != (102u | (102u << 13) | (w[3] & 0xFF000000u))) ++fails;      // +0x38 Z, CentredOnPos 0, AlphaFadeEnable 0, end X
    if ((w[3] & 0x1800u) != 0) ++fails;
    if ((w[4] & 0x0FFFFFFFu) != (102u | (102u << 11) | (1u << 24))) ++fails; // +0x3c end Y / Z, only TrailUseStartColour set
    if ((w[5] & 0x0FFFFFFFu) != (128u | (2048u << 8) | (1u << 27))) ++fails; // +0x40 bias, speed, alpha min 0, UseStartColour
    if ((w[6] & 0x7FFFFu) != (51u << 8)) ++fails;                            // +0x44 size min 0, mid 0, trail width 51, end 0
    printf(fails ? "00b012e0_TEST FAIL\n" : "00b012e0_TEST PASS\n");
    return fails ? 1 : 0;
}