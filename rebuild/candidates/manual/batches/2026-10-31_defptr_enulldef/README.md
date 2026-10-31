# CDefPointer(ENullDef) constructor landed (2026-10-31, owner request) — 6 PENDING_HEADER rows released

`rebuild/include/engine/CDefPointer.h` now declares the global `enum ENullDef { NULL_DEF }` and
`CDefPointer(ENullDef = NULL_DEF) : Object(0) {}`. Evidence: FableWin.pdb `??0?$CDefPointer@..@@QAE@W4ENullDef@@@Z` (332
instantiations) is the null constructor and neither PDB lists a `??0..@@QAE@XZ` default constructor although classes
default-construct CDefPointer members, so the enum parameter carries a default argument (inferred from that absence). The
enum's type name is the PDB's; the enumerator's name is in neither PDB name list, so NULL_DEF is a placeholder spelling
(naming queue). The body is `Object = 0` (retail `and/mov dword ptr [..], 0`).
Safety: every landed includer of the header (154 rows, `landed_includers.txt`) was rechecked with `vland.py --recheck-landed`
after the change: 154/154 MATCH / RELOCATION_MATCH + PASS (`landed_includers_recheck.log`). The one landed source that also
spells a local `ENullDef` (0x0042E204) uses its own CDefPointer model and does not include the header.
Released and landed (6, all RELOCATION_MATCH + PASS, tests green under g++ and VC7.1): 0x0071D940 CTCHeroMorph::AddTattoo,
0x00659C7E CTCVillage::AddCrateTasksFromAndTo, 0x0071D3B0 CTCHeroMorph::UpdateTattoos (sections 2b/2c, agents' tests),
0x00698A30 PlayCameraSoundThroughObservedThing (anonymous namespace), 0x007CF7E0 CTCTavernGameShoveHaPenny::NewRound,
0x00802D10 NPlayerTargeting::IsThingInTargetingRangeForTargetType (coordinator-written tests). Sources are the agents'
hdr/ copies with the local `enum ENullDef` line removed (now in the header).
Not released: 0x0044C6C2 CGameDefinitionManager ctor matches but models its four +0xD0..+0xDC def handles with a local
stand-in and Ego_r has no member names for them (CGameDefinitionManager is an empty struct in Ego_r) — sweep/naming item;
0x007805D0 / 0x008548B0 / 0x0087EE70 / 0x0083DB60 differ for other reasons (still in their sections' pending differ/).
Follow-up: the 69 landed files that spell the enumerator `ENullDef_DescriptiveNull` and the 6 that spell `NULL_DEF` in local
models migrate onto the header in migration wave 10 (NEXT.md item 0).
