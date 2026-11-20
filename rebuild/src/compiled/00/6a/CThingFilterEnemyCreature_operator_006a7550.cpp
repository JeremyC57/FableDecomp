// CThingFilter_EnemyCreature::operator()(const CThing& thing) const -- the filter passes things that are enemies of its source thing
// Leaf: ??RCThingFilter_EnemyCreature@@QBE_NABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7550, 12 bytes: `mov edx,[ecx] ; mov ecx,[esp+4] ; call 0x0076D2F0 ; ret 4`.
// NF167 (2026-11-20, q4949 / q4988): self rename + port. Was a free stand-in `Cons_val(Allocator*, int, Pair*)` calling
//   `extern "C" @callee_76d2f0@8` under the propagated catalog label std::_Cons_val<...CParticleMorphs::CEntry...>.
// Identity (proven): (1) Ego_r.pdb 0x005A2F00 / FableWin.pdb 0x01F94340 ??RCThingFilter_EnemyCreature@@QBE_NABVCThing@@@Z, each followed
//   by the const-pointer overload (Ego_r 0x005A2F10 / FableWin 0x01F94360 ??RCThingFilter_EnemyCreature@@QBE_NPBVCThing@@@Z), as retail
//   0x006A7550 is followed by its byte-identical twin 0x006A7560 (not landed); (2) the body is AreThingsEnemies(thing, SourceThing) on the
//   filter's only member, Ego_r CThingFilter_EnemyCreature { CThing const& SourceThing; } (4 bytes, `mov edx,[ecx]` reads it); (3) the
//   landed CThingSearchTools::GetAllThingsInRoughArea<..., CFilter_Bind3<..., CThingFilter_EnemyCreature> > 0x007F4020 already binds
//   0x006A7550 as ??RCThingFilter_EnemyCreature@@QBE_NABVCThing@@@Z through engine/CThingFilter_EnemyCreature.h.
// Callee: CTCEnemy::AreThingsEnemies 0x0076D2F0 (Ego_r 0x0077B430 / FableWin 0x02606310 ?AreThingsEnemies@CTCEnemy@@SI_NABVCThing@@0@Z,
//   static __fastcall: ecx = the tested thing, edx = the source thing).
#include "fable_compiler.h"
#include "engine/CThingFilter_EnemyCreature.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES
#include "engine/CTCEnemy.h"   // CTCEnemy::AreThingsEnemies 0x0076D2F0 (NF167 opt-in)

bool CThingFilter_EnemyCreature::operator()(const CThing& thing) const
{
    return CTCEnemy::AreThingsEnemies(thing, SourceThing);
}
