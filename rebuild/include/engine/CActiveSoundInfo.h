#ifndef FABLE_ENGINE_CACTIVESOUNDINFO_H
#define FABLE_ENGINE_CACTIVESOUNDINFO_H
// New header (chunk 1024, row 0x006E2ED0): RTTI .?AVCActiveSoundInfo@@ (vtable 0x0129E674, rebuild/manifest/rtti-vtables.tsv), the
// 0x10-byte record CASoundEmitter::GetActiveSampleInfo (LH body 0x00AAC5E0) appends: CBaseClass() 0x0099A2D0, the vtable, then three
// words (the sample index, the bank and the playing sample id read off the active sound). Member names descriptive (no PDB record).
#include "engine/CBaseClass.h"
class CASoundBank;
#ifdef FABLE_RETAIL_CACTIVESOUNDINFO_LH_CTOR
// hand4e opt-in (row 0x00AAC5E0 CSoundEmitterLH::GetActiveSampleInfo): the record's three-argument constructor (out-of-line copy 0x00AAB810:
// first argument -> +0x08, second -> +0x04, third -> +0x0C, `ret 0Ch`), which the LH body expands, and the third word's real type: it is
// copied from CSoundEmitterLH::CActiveSound +0x10, the float start time CActiveSound's constructor 0x00AAB910 receives as its third
// argument (caller 0x00AAC26F..0x00AAC275: GFGetTime() stored with `fstp dword ptr [esp]`). Names descriptive (no PDB covers the module).
class CActiveSoundInfo : public CBaseClass {
public:
    CActiveSoundInfo(const CASoundBank* pbank, long sample_index, float start_time)
        : SampleIndex(sample_index), PBank(pbank), StartTime(start_time) { }
    long               SampleIndex;                                      // +0x04
    const CASoundBank* PBank;                                            // +0x08
    float              StartTime;                                        // +0x0C
};
FABLE_STATIC_ASSERT_AT(1014, sizeof(CActiveSoundInfo) == 0x10);
#else
class CActiveSoundInfo : public CBaseClass {
public:
    long               SampleIndex;                                      // +0x04
    const CASoundBank* PBank;                                            // +0x08
    unsigned long      SampleID;                                         // +0x0C
};
FABLE_STATIC_ASSERT_AT(14, sizeof(CActiveSoundInfo) == 0x10);
#endif
#endif
