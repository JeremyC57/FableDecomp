// Behaviour test for NParticleEngine::CPSCDecalRenderer::CPSCDecalRenderer (0x00B1DFD0): the row's defaults on a stand-alone copy of
// its member block (+0x0c..+0x48; 32-bit bit-field units as VC7.1 lays out `unsigned long`), every packed float quantised through the
// shared GFPackFloat (round to two decimals, map [min, max] onto [0, 2^bits - 1], round to nearest even: retail asm under VC7.1,
// the FABLE_PORTABLE twin elsewhere), then the manager defaults (alignment 0, blend mode 2, blend op 0) in the body.
#ifndef _MSC_VER
#define FABLE_PORTABLE
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>
#include <new>
#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
#include "engine/GFPackFloat.h"

struct Colour { unsigned char B, G, R, A; Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) { } };
struct Block {
    float DecalLifeSecs; int DecalBankIndex; Colour StartColour, MidColour, EndColour;
    unsigned int BlendMode : 3; unsigned int BlendOp : 3; unsigned int Flags : 10; unsigned int FadeInEndInteger : 10;
    unsigned int FadeOutBeginInteger : 10; unsigned int StartRenderSize : 11; unsigned int AlphaFadeMinimum : 7;
    unsigned int EndRenderSize : 11; unsigned int AnimationTimeSecs : 14; unsigned int SizeFadeMinimum : 7;
    unsigned int UseStartColour : 1; unsigned int UseMidColour : 1; unsigned int UseEndColour : 1; unsigned int FlickerBias : 8;
    unsigned int FlickerMinAlphaInteger : 10; unsigned int FlickerMinSizeInteger : 10;
    unsigned int FlickerSpeed : 12;
    unsigned char AlphaFadeEnable : 1; unsigned char SizeFadeEnable : 1; unsigned char ForceAnimationTime : 1; unsigned char FlickerEnable : 1;
    unsigned char PoolingEnable : 1; unsigned char PoolIncreaseAlphaEnable : 1; unsigned char PoolIncreaseSizeEnable : 1;
    unsigned char PoolIncreaseLifeEnable : 1; unsigned char PoolIncreaseFrameEnable : 1;
    float MaxPoolSize, MaxPoolAlpha, MaxPoolLife, PoolFrameIncreaseRate, StencilCubeWidth;
    Block()
        : DecalLifeSecs(1.0f), DecalBankIndex(0),
          StartColour(255, 255, 255, 255), MidColour(255, 255, 255, 255), EndColour(255, 255, 255, 255),
          FadeInEndInteger(0), FadeOutBeginInteger(1000),
          StartRenderSize(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)), AlphaFadeMinimum(GFPackFloat(0.0f, 7, 0.0f, 1.0f, 2)),
          EndRenderSize(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)), AnimationTimeSecs(GFPackFloat(1.0f, 14, 0.1f, 100.0f, 2)),
          SizeFadeMinimum(GFPackFloat(0.0f, 7, 0.0f, 1.0f, 2)),
          UseStartColour(1), UseMidColour(0), UseEndColour(0),
          FlickerBias(GFPackFloat(0.0f, 8, -1.0f, 1.0f, 2)), FlickerMinAlphaInteger(0), FlickerMinSizeInteger(0),
          FlickerSpeed(GFPackFloat(15.0f, 12, 0.0f, 30.0f, 2)),
          AlphaFadeEnable(1), SizeFadeEnable(1), ForceAnimationTime(0), FlickerEnable(0), PoolingEnable(1),
          PoolIncreaseAlphaEnable(0), PoolIncreaseSizeEnable(1), PoolIncreaseLifeEnable(1), PoolIncreaseFrameEnable(1),
          MaxPoolSize(5.0f), MaxPoolAlpha(1.0f), MaxPoolLife(5.0f), PoolFrameIncreaseRate(1.0f), StencilCubeWidth(0.2f)
    {
        Flags = 0;              // GetParticleEmitterManager().GetDefaultAlignmentMode()
        BlendMode = 2;          // GetDefaultBlendMode()
        BlendOp = 0;            // GetDefaultBlendOp()
    }
};

int main()
{
    int fails = 0;
    unsigned char raw[sizeof(Block)];
    memset(raw, 0xff, sizeof raw);                        // every unnamed bit starts set: each field must be written
    Block* b = new (raw) Block;
    unsigned int w[5];
    memcpy(w, &raw[20], sizeof w);                        // the bit-field units (+0x20 .. +0x30 in retail)
    if (b->DecalLifeSecs != 1.0f || b->DecalBankIndex != 0) ++fails;
    if (b->StartColour.R != 255 || b->EndColour.A != 255) ++fails;
    if ((w[0] & 0x3FFFFFFu) != 2u) ++fails;                                              // blend mode 2, op 0, flags 0, fade-in 0
    if ((w[1] & 0x0FFFFFFFu) != (1000u | (102u << 10))) ++fails;                         // fade-out 1000, start size 102, alpha min 0
    if (w[2] != (102u | (148u << 11))) ++fails;                                          // end size 102, 1 s on [0.1, 100] = 147.6, size min 0
    if ((w[3] & 0x7FFFFFFFu) != (1u | (128u << 3))) ++fails;                             // UseStartColour, bias 127.5 -> 128, minima 0
    if ((w[4] & 0xFFFu) != 2048u) ++fails;                                               // flicker speed 2047.5 -> 2048
    unsigned enables = b->AlphaFadeEnable | (b->SizeFadeEnable << 1) | (b->ForceAnimationTime << 2) | (b->FlickerEnable << 3)
        | (b->PoolingEnable << 4) | (b->PoolIncreaseAlphaEnable << 5) | (b->PoolIncreaseSizeEnable << 6)
        | (b->PoolIncreaseLifeEnable << 7) | (b->PoolIncreaseFrameEnable << 8);
    if (enables != 0x1D3u) ++fails;                                                      // retail +0x34 = 0xD3, +0x35 bit 0 (by member:
                                                                                         // the byte units pack differently in g++)
    if (b->MaxPoolSize != 5.0f || b->MaxPoolAlpha != 1.0f || b->MaxPoolLife != 5.0f || b->PoolFrameIncreaseRate != 1.0f
        || b->StencilCubeWidth != 0.2f) ++fails;
    printf(fails ? "00b1dfd0_TEST FAIL\n" : "00b1dfd0_TEST PASS\n");
    return fails ? 1 : 0;
}
