#ifndef FABLE_ENGINE_CARENACREATUREDEF_H
#define FABLE_ENGINE_CARENACREATUREDEF_H
// NEW header (agentFAM08). CArenaCreatureDef : CSubDefClassBase -- one creature entry of an arena wave (Ego_r.pdb record, sizeof 0x38:
// CreatureType +0x28, NumCreatures +0x2C, HUDType +0x30, DeathScore +0x34; identical in retail, no STL members).
// Ego_r.pdb / FableWin.pdb: ??1CArenaCreatureDef@@UAE@XZ, ?Transfer@CArenaCreatureDef@@UAEXAAVCPersistContext@@@Z,
//   ?Copy@CArenaCreatureDef@@UAEXPBVCDefClassBase@@@Z; retail vtable 0x012FB358 (no RTTI): slot 0 the deleting destructor 0x00F25E20,
//   slots 18 / 19 / 20 Transfer 0x00F25E70 / Copy 0x00F25F00 / GetSizeofClass 0x00F25E10; copy constructor 0x00F25DD0
//   (reloc_bindings ??0CArenaCreatureDef@@QAE@ABV0@@Z), default constructor 0x00F263D0.
// Needs FABLE_RETAIL_DEF_CLASS_BASE (engine/CSubDefClassBase.h).
#include "engine/CSubDefClassBase.h"
#include "fable_string.h"
class CArenaCreatureDef : public CSubDefClassBase {
public:
    CArenaCreatureDef();
    CArenaCreatureDef(const CArenaCreatureDef&);
#ifndef FABLE_RETAIL_CARENACREATUREDEF_IMPLICIT_DTOR
    virtual ~CArenaCreatureDef();
#endif
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    virtual void Transfer(CPersistContext&);
    CCharString CreatureType;                                            // +0x28
    long NumCreatures;                                                   // +0x2c
    CCharString HUDType;                                                 // +0x30
    long DeathScore;                                                     // +0x34
};
FABLE_STATIC_ASSERT(sizeof(CArenaCreatureDef) == 0x38);
#endif
