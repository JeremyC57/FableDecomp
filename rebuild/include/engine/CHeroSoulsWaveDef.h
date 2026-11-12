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
// Opt-in FABLE_RETAIL_CHEROSOULSWAVEDEF_IMPLICIT_DTOR (agentFAM07, row 0x00F26F40 ??_GCHeroSoulsWaveDef@@UAEPAXI@Z): no user-declared destructor. The retail
// scalar deleting destructor (vtable 0x012FB498 slot 0, the vtable the ctor 0x00F28BA0 stores) runs the member vector's destructor and
// then stores the CDefPointeeBase vptr 0x01230BA0 before ~CResource: it stores no vtable of its own, which only the
// compiler-generated destructor does (a user-declared one stores it first; family K050).
class CHeroSoulsWaveDef : public CSubDefClassBase {
public:
    CHeroSoulsWaveDef();
    CHeroSoulsWaveDef(const CHeroSoulsWaveDef&);
#ifndef FABLE_RETAIL_CHEROSOULSWAVEDEF_IMPLICIT_DTOR
    virtual ~CHeroSoulsWaveDef();
#endif
#ifdef FABLE_RETAIL_CHEROSOULSWAVEDEF_DEF_VIRTUALS
    // Opt-in (agentFAM07): the def-class virtuals of vtable 0x012FB498, Ego_r.pdb ?Copy@CHeroSoulsWaveDef@@UAEXPBVCDefClassBase@@@Z,
    // ?GetSizeofClass@CHeroSoulsWaveDef@@UAEKXZ, ?Transfer@CHeroSoulsWaveDef@@UAEXAAVCPersistContext@@@Z (without them the class is abstract).
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    virtual void Transfer(CPersistContext&);
#endif
    long NumWaveCreatures;                                               // +0x28
    std::vector<CHeroSoulsCreatureDef, std::allocator<CHeroSoulsCreatureDef> > Creatures; // +0x2c
};
FABLE_STATIC_ASSERT(sizeof(CHeroSoulsWaveDef) == 0x38);
#endif
