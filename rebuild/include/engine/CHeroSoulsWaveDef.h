#ifndef FABLE_ENGINE_CHEROSOULSWAVEDEF_H
#define FABLE_ENGINE_CHEROSOULSWAVEDEF_H
// Chunk 789 (new header). Ego_r.pdb CHeroSoulsWaveDef : CSubDefClassBase: NumWaveCreatures +0x28,
// std::vector<CHeroSoulsCreatureDef> Creatures +0x2c (ghidra_out/struct_layouts_egor.tsv; the PDB's 0x3c counts a 16-byte vector,
// retail STLport's is 12, so 0x38 like CHeroSoulsRoundDef). Retail: WatchForRoundsInArena 0x00D7FE90 steps Waves by 0x38 and reads
// +0x28 (creature-def count) and +0x2c (Creatures start). Members only.
#include "engine/CSubDefClassBase.h"
#include "engine/CHeroSoulsCreatureDef.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CHeroSoulsWaveDef : public CSubDefClassBase {
public:
    CHeroSoulsWaveDef();
    CHeroSoulsWaveDef(const CHeroSoulsWaveDef&);
    virtual ~CHeroSoulsWaveDef();
    long NumWaveCreatures;                                               // +0x28
    std::vector<CHeroSoulsCreatureDef, std::allocator<CHeroSoulsCreatureDef> > Creatures; // +0x2c
};
FABLE_STATIC_ASSERT(sizeof(CHeroSoulsWaveDef) == 0x38);
#endif
