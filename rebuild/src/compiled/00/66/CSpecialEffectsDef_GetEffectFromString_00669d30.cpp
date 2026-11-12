// CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& effect) const -- hash the name with the engine CRC and look the
// hash up in SpecialEffectsMap.ContainmentMap; on a hit store the mapped effect and return true
// Leaf: ?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00669D30, 100 bytes, `ret 8` (178 call sites, e.g. 0x0066385F).
// NF28 (2026-11-12, queue land1011ei): was landed as CCRC::CalcAndLookup (owner unknown, local models). Identity: Ego_r.pdb 0x00526CA0 and
//   FableWin.pdb ?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z; the map read at +0x28 is Ego_r
//   CSpecialEffectsDef::SpecialEffectsMap (CSpecialEffectsStringMap { map<unsigned long,long> ContainmentMap }); the landed callers
//   0x006637D0 / 0x008B9E50 / 0x008C1F70 bind this spelling through engine/CSpecialEffectsDef.h.
// Model: the shared header's FABLE_RETAIL_SKYBEAM_ORIGINAL body (an inline __declspec(noinline) member: its COMDAT is emitted here by the
//   member-pointer reference below; the map find 0x006699C0 and CCRC::Calc 0x004014A0 stay calls).
#define FABLE_RETAIL_SKYBEAM_ORIGINAL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/CSpecialEffectsDef.h"

bool (CSpecialEffectsDef::* PGetEffectFromString)(const CCharString&, long&) const = &CSpecialEffectsDef::GetEffectFromString;
