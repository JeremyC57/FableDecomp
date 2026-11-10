#ifndef FABLE_ENGINE_CHEROSUITDEF_H
#define FABLE_ENGINE_CHEROSUITDEF_H
#include "engine/CSubDefClassBase.h"
#include "engine/EClothingSuitPart.h"
#include "stlport/fable_map.h"
#ifdef FABLE_RETAIL_CHEROSUITDEF_STATGETTYPENAME
// Per-TU opt-in (agentCLO 2026-11-10, row 0x005B8743): FableWin.pdb ?StatGetTypeName@CHeroSuitDef@@SI?AVCCharString@@XZ, inline in
// the release build (no Ego_r copy); the "CHeroSuitDef" literal the landed GetSubDef<CHeroSuitDef> 0x005935C4 passes to GetPSubDefInfo.
#include "fable_string.h"
class CHeroSuitDef:public CSubDefClassBase { public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CHeroSuitDef", -1); }
    std::map<EClothingSuitPart,long> SuitParts; };
#else
class CHeroSuitDef:public CSubDefClassBase { public:std::map<EClothingSuitPart,long> SuitParts; };
#endif
FABLE_STATIC_ASSERT(offsetof(CHeroSuitDef,SuitParts)==0x28);
#endif
