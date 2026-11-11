#ifndef FABLE_ENGINE_CACTIVESOUNDINFO_H
#define FABLE_ENGINE_CACTIVESOUNDINFO_H
// New header (chunk 1024, row 0x006E2ED0): RTTI .?AVCActiveSoundInfo@@ (vtable 0x0129E674, rebuild/manifest/rtti-vtables.tsv), the
// 0x10-byte record CASoundEmitter::GetActiveSampleInfo (LH body 0x00AAC5E0) appends: CBaseClass() 0x0099A2D0, the vtable, then three
// words (the sample index, the bank and the playing sample id read off the active sound). Member names descriptive (no PDB record).
#include "engine/CBaseClass.h"
class CASoundBank;
class CActiveSoundInfo : public CBaseClass {
public:
    long               SampleIndex;                                      // +0x04
    const CASoundBank* PBank;                                            // +0x08
    unsigned long      SampleID;                                         // +0x0C
};
FABLE_STATIC_ASSERT(sizeof(CActiveSoundInfo) == 0x10);
#endif
