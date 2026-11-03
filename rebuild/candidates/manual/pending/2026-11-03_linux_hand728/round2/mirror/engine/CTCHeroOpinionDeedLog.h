#ifndef FABLE_ENGINE_CTCHEROOPINIONDEEDLOG_H
#define FABLE_ENGINE_CTCHEROOPINIONDEEDLOG_H
// CTCHeroOpinionDeedLog (chunk 728 round 2, row 0x006D0830 CTCWife::GlobalUpdate): TCI_HERO_OPINION_DEED_LOG (81; GlobalUpdate tests
// bit 81 of UsingComponents, `test dword [hero+28h],20000h`). The member GlobalUpdate calls out of line: Ego_r.pdb / FableWin.pdb
// ?PostDeed@CTCHeroOpinionDeedLog@@QAEJW4EOpinionDeedType@@ABVC3DVector@@PBVCThing@@PAVCThingAICreature@@W4EOpinionPostDeedType@@
// _N5PBVCFaction@@5J@Z (0x006C6990). Needs FABLE_RETAIL_CTCBASE. Layout past CTCBase opaque (not used).
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EOpinionDeedType.h"
class C3DVector;
class CThing;
class CThingAICreature;
class CFaction;
enum EOpinionPostDeedType;
class CTCHeroOpinionDeedLog : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_OPINION_DEED_LOG; }
    long PostDeed(EOpinionDeedType type, const C3DVector& pos, const CThing* originator, CThingAICreature* victim,
                  EOpinionPostDeedType postType, bool, bool, const CFaction* faction, bool, long);   // 0x006C6990
};
#endif
