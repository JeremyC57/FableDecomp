#ifndef FABLE_ENGINE_CREGIONSCRIPTDEF_H
#define FABLE_ENGINE_CREGIONSCRIPTDEF_H
// CRegionScriptDef (new, chunk Q1): a parent def (CParentDefClassBase, retail 0x39 bytes packed; own members aligned from +0x3C). Ego_r.pdb layout
// (struct_layouts_egor.tsv, 80 bytes, base 0x40): RandomVillagerMax, RandomGuardMax, RandomBanditMax, RegionDangerLevel; retail
// +0x3C..+0x48 (operator= 0x00F29F70 copies exactly those four longs after the base assignment 0x00431ED0), sizeof 0x4C.
// Name set (FableWin.pdb): ?Transfer@CRegionScriptDef@@UAEXAAVCPersistContext@@@Z, ?Copy@..UAEXPBVCDefClassBase@@@Z,
// ?Alloc@..SIPAVCDefClassBase@@XZ, ??_G; the copy assignment is the implicit one, out of line in retail at 0x00F29F70.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CRegionScriptDef.h needs the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
class CPersistContext;
class CRegionScriptDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    virtual void Transfer(CPersistContext&);                             // slot 18
    virtual void Copy(const CDefClassBase*);                             // slot 19 0x00F29F60
    virtual unsigned long GetSizeofClass();                              // slot 20
    CRegionScriptDef& operator=(const CRegionScriptDef&);                // 0x00F29F70 (the implicit assignment's retail body)
    long RandomVillagerMax;                                              // +0x3c
    long RandomGuardMax;                                                 // +0x40
    long RandomBanditMax;                                                // +0x44
    long RegionDangerLevel;                                              // +0x48
};
FABLE_STATIC_ASSERT(sizeof(CRegionScriptDef) == 0x4c);
#endif
