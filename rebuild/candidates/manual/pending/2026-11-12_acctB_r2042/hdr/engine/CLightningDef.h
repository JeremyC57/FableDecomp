#ifndef FABLE_ENGINE_CLIGHTNINGDEF_H
#define FABLE_ENGINE_CLIGHTNINGDEF_H
// New shared header (w22 agent2042, row 0x007F2DC0 CTCLightningNode::FrameUpdate): the lightning definition (the generated flattened copy
// sits in engine/_quarantine/CLightningDef.h). RTTI .?AVCLightningDef@@ : CParentDefClassBase (ghidra_out/rtti_bases.tsv, vtable 0x01234C24);
// Ego_r.pdb / FableWin.pdb Alloc / Transfer / Copy. Members: Ego_r.pdb names and order; retail offsets 4 below Ego_r's (the 0x39-byte retail
// CParentDefClassBase): Range +0x3C (FrameUpdate 0x007F2E97 / 0x007F31E7), LifeTime +0x40 (0x007F2DE3 / 0x007F2E04), MaxNumberOfBranches
// +0x44 (0x007F2E59), NeedLOSToBranch +0x48, DamagePeriod +0x4C (0x007F3409), DamagePerCycle +0x50 (0x007F33DC / 0x007F3458), NodeDefIndex
// +0x54 (0x007F2E50 / 0x007F3002); sizeof 0x58. Needs FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
class CPersistContext;
class CDefClassBase;

class CLightningDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext&);                     // ?Transfer@CLightningDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase*);                     // ?Copy@CLightningDef@@UAEXPBVCDefClassBase@@@Z
    float Range;                                                 // +0x3c
    float LifeTime;                                              // +0x40
    long  MaxNumberOfBranches;                                   // +0x44
    bool  NeedLOSToBranch;                                       // +0x48
    float DamagePeriod;                                          // +0x4c
    float DamagePerCycle;                                        // +0x50
    long  NodeDefIndex;                                          // +0x54
};
FABLE_STATIC_ASSERT(offsetof(CLightningDef, Range) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CLightningDef, DamagePeriod) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CLightningDef, NodeDefIndex) == 0x54);
FABLE_STATIC_ASSERT(sizeof(CLightningDef) == 0x58);
#endif
