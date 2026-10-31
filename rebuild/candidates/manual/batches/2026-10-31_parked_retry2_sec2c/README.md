# Parked-row retry sweep, wave 2 section 2c (2026-10-31): 2 of 85 rows landed, 39 improved

Section 2c = the 85 held section-2 rows (retail 0x00647575-0x007805D0, the larger 300-1900 byte bodies) that section 2b never
reached before its lane limit. All 85 attempted (1h50m of lane time). Landed 2: 0x006CF3B0
`CTCWife::InitWifeAtEndOfMarriageFrescoStartFadeIn` (C++; `ParentThing` read through an inline const accessor; Ego_r callee spellings
CTCBase::GetPlayerManager, CPlayer::GetPControlledCreature() const, CTCFollowed/CTCRegionFollower::RemoveFollower(CThing&)) and
0x00747DA0 `CTCStealth::FrameUpdate` (`retail_asm`: the engine's inline CeilToLong FPU block at 0x0074803F `fstp/fld/fadd/fistp dword`,
bytes verified; separate retail_asm_payload.json / retail_asm_oracle.tsv). Both RELOCATION_MATCH + PASS, no RELOC_IDENTITY.
Two PENDING_HEADER rows (0x00659C7E, 0x0071D3B0) are RELOCATION_MATCH with the proposed `CDefPointer(ENullDef)` ctor (hdr tests
pass) and land with migration wave 10. 39 improved in `../../pending/2026-10-31_parked_retry2_sec2c/differ/` (47 files; 13 skeleton
rows of 1.1-1.9 KB got head drafts only — 0x006E5A00 is the best next target: its "lane gap" is the same CeilToLong block at
0x006E5E43 / 0x006E5F4B), 14 header-moved sources in `hdr/`. 42 held with residues in STATUS.tsv (frame-slot packing into dead
argument slots, register rotation/scheduling, a compiled-out type check `xor al,al ; jne`, one level of dtor inlining,
frame-block copy temporaries).
Levers (matched / improved): inline accessor for a member used as an argument 1 / 1; retail_asm ceiling helper 1 / 1; shared def
model + same-TU GetSubDef bodies 0 (+2 pending header) / 4; same-TU callee bodies or stand-ins 0 / 3; named locals, copies or
const& 0 / 8; inline bool getter 0 / 1; STLport in-place iterator / templated GetTC 0 / 2; structural/signature/matrix fixes to
parked drafts 0 / 5; post-increment 0 / 1; head drafts of skeletons 0 / 13.
NAME_FIXES.tsv: 2 rows with wide reach — 0x0049D870 is `CWorld::GetFrame` (`?GetFrame@CWorld@@SIJXZ`; 54 landed files say
GetCurrentFrame, 9 say GetWorldFrame) and 0x00487DC0 is `CPlayer::GetPControlledCreature() const` (landed 0x0089ED80 and the catalog
say GetHeroCreature) — plus 4 review rows in the queue (0x006D6570 two names; 0x007122C0 / CScriptTask+0x18 unproven; the
0x006CF3B0 inline accessor + engine/CPlayer.h gap; the CDefPointer header gap).
Review notes: 0x0071D3B0 still uses a GrowVectorStorage stand-in; 0x006FDA20 / 0x0077DC40 / 0x007070D0 (differ) need real callee
bodies for 0x006FE7C0 and 0x00618100; 0x00747DA0 uses local CThing / CTCBase models (no engine/CThing.h; SHARED_HEADER_AVAILABLE
for CWorld, CTCBase); finish.py now loops over src/ only (retail_asm rows go through finish_rasm.py) and catalog-oracle rows need
an oracle_extra.tsv row (mkorc.py) first.
Coordinator: vland --land 1/1 + 1/1, bindings_multi_check 0.
