#ifndef FABLE_ENGINE_CSOUNDTHEMEDEF_H
#define FABLE_ENGINE_CSOUNDTHEMEDEF_H
// Retail opt-in (Codex pass 22): Ego_r and FableWin CSoundThemeDef members; retail 0x006B174A..0x006B1890.
#ifdef FABLE_RETAIL_CSOUNDTHEMEDEF_PASS22
#include "engine/CParentDefClassBase.h"
#include "engine/CRGBColour.h"
#include "stlport/fable_map.h"
class CSoundThemeDef : public CParentDefClassBase {
public:
    CRGBColour EditColour;                        // +0x3c
    long AtmosIndex;                              // +0x40
    long MusicSet;                                // +0x44
    float Gain;                                   // +0x48
    float Range;                                  // +0x4c
    float Granularity;                            // +0x50
    std::multimap<CCharString,long> AtmosGroupMap;  // +0x54
    bool PlayOnlyWhenInside;                      // +0x60
    bool PlayQuietlyWhenInside;                   // +0x61
    bool PlayOnlyWhenOutside;                     // +0x62
    bool PlayQuietlyWhenOutside;                  // +0x63
};
FABLE_STATIC_ASSERT(offsetof(CSoundThemeDef, AtmosIndex) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CSoundThemeDef, AtmosGroupMap) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CSoundThemeDef, PlayOnlyWhenInside) == 0x60);
FABLE_STATIC_ASSERT(sizeof(CSoundThemeDef) == 0x64);
#endif
#endif
