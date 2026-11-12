#ifndef FABLE_ENGINE_CARENAWAVEDEF_H
#define FABLE_ENGINE_CARENAWAVEDEF_H
// NEW header (agentFAM08). CArenaWaveDef : CSubDefClassBase -- one wave of an arena round (Ego_r.pdb record: NumWaveCreatures +0x28,
// vector<CArenaCreatureDef> Creatures +0x2C, ShortWave +0x3C with the 16-byte Dinkumware vector; retail STLport vector is 12 bytes,
// so ShortWave +0x38 and sizeof 0x3C, the element stride CArenaRoundDef's vector destructor 0x00F25B70 steps).
// Ego_r.pdb / FableWin.pdb: ??1CArenaWaveDef@@UAE@XZ, ??_GCArenaWaveDef@@UAEPAXI@Z, ?Copy@CArenaWaveDef@@UAEXPBVCDefClassBase@@@Z,
//   ?Transfer@CArenaWaveDef@@UAEXAAVCPersistContext@@@Z; retail vtable 0x012FB2F0 (no RTTI): slot 0 0x00F25CB0, slots 18 / 19 / 20
//   Transfer 0x00F261F0 / Copy 0x00F25F50 / GetSizeofClass 0x00F25CA0; default constructor 0x00F280E0 (reloc_bindings
//   ??0CArenaWaveDef@@QAE@XZ), copy constructor 0x00F25C60.
// Needs FABLE_RETAIL_DEF_CLASS_BASE (engine/CSubDefClassBase.h).
#include "engine/CSubDefClassBase.h"
#include "engine/CArenaCreatureDef.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CArenaWaveDef : public CSubDefClassBase {
public:
    CArenaWaveDef();
    CArenaWaveDef(const CArenaWaveDef&);
#ifndef FABLE_RETAIL_CARENAWAVEDEF_IMPLICIT_DTOR
    virtual ~CArenaWaveDef();
#endif
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    virtual void Transfer(CPersistContext&);
    long NumWaveCreatures;                                                        // +0x28
    std::vector<CArenaCreatureDef, std::allocator<CArenaCreatureDef> > Creatures; // +0x2c
    bool ShortWave;                                                               // +0x38
};
FABLE_STATIC_ASSERT(sizeof(CArenaWaveDef) == 0x3c);
#endif
