// Behaviour test for NParticleEngine::CPSCEmitterGeneric::CPSCEmitterGeneric (0x00B103F0): the row's packed database defaults on a
// stand-alone copy of its bit-field block (+0x30..+0x4c; the six 32-bit units +0x30..+0x44 are checked whole, the short / timer
// units by member, whose packing differs between compilers), every float default
// quantised through the shared GFPackFloat (round to two decimals, map [min, max] onto [0, 2^bits - 1], round to nearest even:
// retail asm under VC7.1, the FABLE_PORTABLE twin elsewhere); the dwords are checked whole against hand-computed values.
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
    unsigned int NoParticlesToStart : 10; unsigned int NoParticlesToStartRand : 10; unsigned int EmitterType : 2; unsigned int Solid : 1;
    unsigned int UseEmitterLifeSecs : 1; unsigned int UseEmitterTimelineSecs : 1; unsigned int OrientationXY : 1; unsigned int OrientationXZ : 1;
    unsigned int OrientationYZ : 1; unsigned int UseCustomDirection : 1; unsigned int UseOutwardDirection : 1; unsigned int UseParamDirection : 1;
    unsigned int OppositeDirection : 1;
    unsigned int AngularPerturbationInteger : 10; unsigned int CustomDirectionX : 8; unsigned int ParticlesPerSecond : 14;
    unsigned int EmitterSize : 10; unsigned int RadialBias : 10; unsigned int MinSpeed : 10; unsigned int UseForwardDirection : 1;
    unsigned int UseRandom2DDirection : 1;
    unsigned int CustomDirectionY : 8; unsigned int CustomDirectionZ : 8; unsigned int EmitterTimelineSecs : 15; unsigned int UseRandom3DDirection : 1;
    unsigned int EmitterLifeSecs : 15; unsigned int EmitterStartTime : 15;
    unsigned int MaxSpeed : 10; unsigned int NonUniformScalingX : 11; unsigned int NonUniformScalingY : 11;
    unsigned short NonUniformScalingZ : 11;
    unsigned int StartedTimer : 1; unsigned int FinishedGenerating : 1;
    Block()
        : NoParticlesToStart(0), NoParticlesToStartRand(0), EmitterType(0), Solid(1), UseEmitterLifeSecs(0), UseEmitterTimelineSecs(0),
          OrientationXY(1), OrientationXZ(0), OrientationYZ(0), UseCustomDirection(1), UseOutwardDirection(0), UseParamDirection(0),
          OppositeDirection(0),
          AngularPerturbationInteger(0), CustomDirectionX(GFPackFloat(0.0f, 8, -1.0f, 1.0f, 2)),
          ParticlesPerSecond(GFPackFloat(3.0f, 14, 0.0f, 100.0f, 2)),
          EmitterSize(GFPackFloat(1.0f, 10, 0.0f, 10.0f, 2)), RadialBias(GFPackFloat(0.0f, 10, -5.0f, 5.0f, 2)),
          MinSpeed(GFPackFloat(0.0f, 10, 0.0f, 10.0f, 2)), UseForwardDirection(0), UseRandom2DDirection(0),
          CustomDirectionY(GFPackFloat(0.0f, 8, -1.0f, 1.0f, 2)), CustomDirectionZ(GFPackFloat(1.0f, 8, -1.0f, 1.0f, 2)),
          EmitterTimelineSecs(GFPackFloat(30.0f, 15, 0.0f, 300.0f, 2)), UseRandom3DDirection(0),
          EmitterLifeSecs(GFPackFloat(10.0f, 15, 0.0f, 300.0f, 2)), EmitterStartTime(GFPackFloat(0.0f, 15, 0.0f, 300.0f, 2)),
          MaxSpeed(GFPackFloat(0.0f, 10, 0.0f, 10.0f, 2)), NonUniformScalingX(GFPackFloat(1.0f, 11, -10.0f, 10.0f, 2)),
          NonUniformScalingY(GFPackFloat(1.0f, 11, -10.0f, 10.0f, 2)),
          NonUniformScalingZ((unsigned short)GFPackFloat(1.0f, 11, -10.0f, 10.0f, 2)),
          StartedTimer(0), FinishedGenerating(0)
    {
    }
};

int main()
{
    int fails = 0;
    unsigned char raw[sizeof(Block)];
    memset(raw, 0xff, sizeof raw);                        // every unnamed bit starts set: each field must be written
    Block* b = new (raw) Block;
    unsigned int w[5];
    memcpy(w, b, sizeof w);
    if (w[0] != ((1u << 22) | (1u << 25) | (1u << 28))) ++fails;                        // +0x30 Solid, OrientationXY, UseCustomDirection
    if (w[1] != ((128u << 10) | (491u << 18))) ++fails;                                 // +0x34 dir X 127.5 -> 128 (even), 491.49 pps
    if ((w[2] & 0xFFFFFFFu) != (102u | (512u << 10))) ++fails;                           // +0x38 size 102.3, bias 511.5 -> 512, min speed 0
    if ((w[2] & 0x30000000u) != 0) ++fails;                                              // forward / random-2D directions cleared
    if (w[3] != (128u | (255u << 8) | (3277u << 16))) ++fails;                           // +0x3c dir Y / Z, 30 s timeline 3276.7, no random-3D
    if ((w[4] & 0x3FFFFFFFu) != 1092u) ++fails;                                          // +0x40 10 s life 1092.23, start time 0
    unsigned int w5; memcpy(&w5, raw + 0x14, 4);
    if (w5 != ((1126u << 10) | (1126u << 21))) ++fails;                                  // +0x44 max speed 0, scaling X / Y 1125.85
    if (b->NonUniformScalingZ != 1126) ++fails;                                          // +0x48 scaling Z
    if (b->StartedTimer != 0 || b->FinishedGenerating != 0) ++fails;                     // +0x4c timers cleared
    printf(fails ? "00b103f0_TEST FAIL\n" : "00b103f0_TEST PASS\n");
    return fails ? 1 : 0;
}
