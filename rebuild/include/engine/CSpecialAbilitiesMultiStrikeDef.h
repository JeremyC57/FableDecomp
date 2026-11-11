#ifndef FABLE_ENGINE_CSPECIALABILITIESMULTISTRIKEDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESMULTISTRIKEDEF_H
// New shared header (chunk 1024, row 0x007CD390 CTCMultiStrike::CTCMultiStrike): the retail CSpecialAbilitiesMultiStrikeDef, laid out
// as the sibling CSpecialAbilities*Def headers (CParentDefClassBase base, the first member at +0x3C): the 10 Ego_r.pdb members in order
// and spelling (ghidra_out/struct_layouts_egor.tsv, 180 bytes there, Sounds +0x40) with 12-byte STLport vectors (retail 0x9C). The ctor
// reads Sounds +0x3C (`mov edx,[ecx+eax*4+3Ch]`, 0x007CD620); the landed StartMultiHit 0x007CCB00 reads DelayTimeSecs at +0x6C.
// Needs FABLE_RETAIL_CDEFSTRINGTABLE and FABLE_RETAIL_CCHARSTRING (engine/CDefString.h).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesMultiStrikeDef : public CParentDefClassBase {
public:
    CDefString Sounds[4];                                                        // +0x3C
    CDefString Animations[4];                                                    // +0x4C
    float HighlightWidth;                                                        // +0x5C
    std::vector<long,std::allocator<long> > HighlightColor;                      // +0x60
    std::vector<float,std::allocator<float> > DelayTimeSecs;                     // +0x6C
    std::vector<long,std::allocator<long> > NumHits;                             // +0x78
    std::vector<float,std::allocator<float> > PerHitDamagerMultiplier;           // +0x84
    std::vector<CCharString,std::allocator<CCharString> > AttachmentPoint;       // +0x90
};
#endif
