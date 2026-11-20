# NF107 overlay (round 10): opt-ins only

Every header here is the CURRENT repo header (base copy in ../orig/engine/, byte-equal to rebuild/include at hand-back) plus `#ifdef`
opt-ins. Without the new macros each header is token-identical to the repo copy (the additions are whole `#ifdef` / `#elif` blocks; no
existing line changed). Checked against the NF105 / NF108 overlays: neither touches these headers (NF105: the CFloat* reverb headers only;
NF108: no overlay), so no member is declared twice.

| header | macro | what it adds | rows |
|---|---|---|---|
| engine/CThingSearchTools.h | FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_THISEND | inside BESTROUGH_BODY: the map loop's end read from the member (`it!=AreaMaps.end()`, as ROUGHAREA_THISEND); with ROUGHAREA_CELL_BYTEOFFSET also defined, an `#elif` cell branch with the byte-offset expression of ROUGHAREA_CELL_BYTEOFFSET (FITTED, as that opt-in) | 0x007F7340, 0x007F70D0, 0x00937420 |
| engine/CThingMarker.h | FABLE_RETAIL_CTHINGMARKER_STATTYPEFLAGS | `static unsigned long __fastcall StatGetTypeFlags() { return 0x8007; }` (FableWin.pdb ?StatGetTypeFlags@CThingMarker@@SIKXZ; retail 0x00851490 tests 0x8007) | 0x00851490 |
| engine/CTCDExperienceOrb.h | FABLE_RETAIL_CTCDEXPERIENCEORB_STATGETDRIVERTYPE | `static unsigned long __fastcall StatGetDriverType(){return 26;}` (FableWin.pdb ?StatGetDriverType@CTCDExperienceOrb@@SIKXZ; retail 0x0077ADF0 `push 1Ah ; call [edx+70h]`) | 0x0077ADF0 |
| engine/CTCCarrying.h | FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON | declaration `static bool __fastcall IsThingCarryingMeleeWeapon(const CThing&);` (Ego_r.pdb 0x006C6040 ?IsThingCarryingMeleeWeapon@CTCCarrying@@SI_NABVCThing@@@Z; retail 0x006A15A0, called by 0x008E4100) | 0x008E4100 |
| engine/CThingDeadCreature.h | FABLE_RETAIL_CTHINGDEADCREATURE_STATTYPEFLAGS | `static unsigned long __fastcall StatGetTypeFlags() { return 0x87; }` (FableWin.pdb ?StatGetTypeFlags@CThingDeadCreature@@SIKXZ; retail tests 0x87) | 0x009595C0, 0x00937080 |

Ownership: the brief gives NF107 engine/CThingSearchTools.h, NThingMap_CThingMap.h, CTCWeapon.h and CThingDeadCreature.h this round.
CThingMarker.h, CTCDExperienceOrb.h and CTCCarrying.h are NOT in that list: each takes one additive member-specific opt-in (a declaration
or a one-line static body). The coordinator should confirm no other overlay of this landing touches them (NF105 / NF108 do not).
NThingMap_CThingMap.h and CTCWeapon.h: not changed.

Static asserts: pin_asserts.py (rebuild/include vs hdr) per header: CThingSearchTools.h pinned 0 (the asserts below the new lines are
already FABLE_STATIC_ASSERT_AT with fixed tags); CThingMarker.h pinned 1 (its `FABLE_STATIC_ASSERT(sizeof(CThingMarker)==0xd0)` became the
FABLE_STATIC_ASSERT_AT form with the repo line tag, the only assert change); CTCDExperienceOrb.h / CTCCarrying.h / CThingDeadCreature.h
pinned 0.

Header users (landed includers in rebuild/src/compiled; none defines any of the five new macros, so none changes):
CThingSearchTools.h 333, CThingMarker.h 23, CTCDExperienceOrb.h 3, CTCCarrying.h 86, CThingDeadCreature.h 19.
Account B's landed 0x0063F4B0 compiled with this overlay: RELOCATION_MATCH (q.py, work/q_0063f4b0_overlay.log; not in rows/).
