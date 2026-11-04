#ifndef FABLE_ENGINE_CTCNOISE_H
#define FABLE_ENGINE_CTCNOISE_H
// CTCNoise (new, chunk 765): RTTI .?AVCTCNoise@@, single base CTCBase at +0 (ghidra_out/rtti_bases.tsv). Ego_r.pdb members
// (struct_layouts_egor.tsv, 80 bytes, no STL container so the offsets are retail's): Definition (CTCNoiseDef, 52 bytes) +0x0C,
// LifeTimer +0x40, DistanceCanBeHeardFrom +0x44, Creator (CIntelligentPointer<const CThing>) +0x48; GetCreator 0x007498D0 reads
// +0x48. Name set: ?GetCreator@CTCNoise@@QBEPBVCThing@@XZ. Only the members and methods rows use are modelled (no virtual overrides).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
class CTCNoise : public CTCBase {
public:
    const CThing* GetCreator() const;                                    // 0x007498D0
protected:
    unsigned char Definition[0x34];                                      // +0x0c CTCNoiseDef
    long LifeTimer;                                                      // +0x40
    float DistanceCanBeHeardFrom;                                        // +0x44
    CIntelligentPointer<const CThing> Creator;                           // +0x48
};
#endif
