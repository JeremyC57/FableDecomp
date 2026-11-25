# 0x006C0B20 CTCBuyableHouse::FindPosToCreateNewVillager -- MATCHED (AUTH26 / AUTH26b, 2026-11-25)

Grade: RELOCATION_MATCH 368 B (also with LANDVERIFY_STRICT_HEADERS=1); test PASS (vt.py, linked to the row object); NAME_GATE PASS
(NAME_GATE.txt). Grades used: 2 (g0, g1). Source 006c0b20.cpp (= g1 + header comment), test 006c0b20_test.cpp. Flags: none extra.

## Route
- Old best DIFFER(383v368) was a full local model with `C3DVector*` and a stand-in GetAllThings. Not reused.
- FableWin.pdb's /Od body (0x024B1A30, 569 B, equal to Ego_d's code size) gave every callee's PDB name (fwsym.py over its ILT
  thunks) and the statement order; its byte ranges map one to one onto Ego_d's line sizes 937..958 (see LOG.md).
- g0: those statements on shared headers + the same-TU GetAllThings body (ALLTHINGSBODY) + HASTC_OUTOFLINE: RELOCATION_MATCH at once.
  The same-TU noinline body is what lets VC7.1 keep the vector start in ebx and re-read the finish word (no end_it slot).
- g1: g0's GetAllThings pick-any copy was 142 v 158 B. Added ALLTHINGSBODY_EGOD + CTHING_STATTYPEFLAGS +
  CTHING_SCANINCLUSION_ISALIVE (retail 0x006C1DD0: `not cl ; test cl,1` alive test, `xor dl,dl ; jne` folded type test, pushed pointer
  stored only on the push path). Copy now byte- and relocation-EQ with retail 0x006C1DD0 (retcmp); main body unchanged.
- Constant: `__real@43c80000` = 400.0f = retail [0x0125D824] (GFSqr(20) folded). The grader masks that relocation (mutant m1 with
  20.0f also grades RELOCATION_MATCH); the test kills it.

## Opt-ins
- NEW: FABLE_RETAIL_CTCBUYABLEHOUSE_FINDPOSTOCREATENEWVILLAGER in hdr/engine/CTCBuyableHouse.h (declaration only; hdr/README.md).
- Existing: CTCBUYABLEHOUSE, CTHINGSEARCHTOOLS(_ALLTHINGSBODY, _EGOD), CTHING_STATTYPEFLAGS, CTHING_SCANINCLUSION_ISALIVE,
  CTHINGFILTER_HASTC_OUTOFLINE, CWORLD, SCOPED_PTR, CWORLDMAP, PLAYER, CPLAYERMANAGER, CTHINGCREATUREBASE, CTHINGPLAYERCREATURE (+ bases).

## Names (two sources each)
| address | spelling | sources |
|---|---|---|
| 0x006C0B20 self | ?FindPosToCreateNewVillager@CTCBuyableHouse@@AAE_NAAVC3DVector@@@Z | Ego_r.publics.txt l.69382; FableWin.publics.txt l.131748 (+ Ego_d param ret_pos) |
| 0x00686D30 | ?GetWorldMap@CTCBase@@IAEAAVCWorldMap@@XZ | FableWin body (thunk 0x0181F8C1); CTCBase.h (Ego_r 0x006B2120); landed binding |
| 0x00686A70 | ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ | FableWin body (0x0179B85A); CTCBase.h (Ego_r 0x006B1E60 / FableWin 0x02190D40) |
| 0x00686D40 | ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ | FableWin body (0x01795EEB); CTCBase.h (Ego_r / FableWin) |
| 0x00449970 | ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ | FableWin body (0x017AE0E5); landed binding (gate agree_pdb) |
| 0x00487DC0 | ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ | FableWin body (0x017C1717); CPlayer.h (Ego_r 0x00493650), landed bindings |
| 0x004C73D0 | ?GetPos@CThing@@QBEABVC3DVector@@XZ | FableWin body (0x01829074); landed bindings (tracker PDB spelling) |
| 0x00864620 | ?IsAwareOfPosition@NCreatureAI@@YI_NPBVCThingCreatureBase@@ABVC3DVector@@@Z | FableWin body (0x0182E3F3); landed row 0x00864620's leaf (Ego_r) |
| 0x006C1DD0 | ??$GetAllThings@VCThing@@V?$vector@PAVCThing@@...@CThingSearchTools@@QBEX...@Z | FableWin.publics l.262516; Ego_r.publics l.136704 |
| vtable slot 16 | CWorldMap::GetMapNumberAt(const C3DVector&) const | FableWin body ([edx+40h] on GetWorldMap with GetPos); Ego_r S_CALLSITEINFO `long CWorldMap::(const C3DVector...)`; CWorldMap.h slot note |
| inline | CTCBase::GetParentThing, CWorld::PeekThingSearchTools, CThingFilter_HasTC / _InMap ctors, GFFilter_Bind2, GFWorld2DDistanceSmallerThan | FableWin body callees (fwsym.py) + the shared headers' FableWin notes |
| 0x00BFEA14 | free | CRT (agree_std) |
| enum | TCI_REGION_ENTRANCE (99) | ETCInterfaceType.h; FableWin `push 63h` into ??0CThingFilter_HasTC@@QAE@W4ETCInterfaceType@@@Z |

Ego_d locals used exactly: ret_pos, phero, entrances; inner block it, end_it (it = begin() first, end_it = end(), as FableWin).
No DESCRIPTIVE / LOCAL_OK lines. NAME_FIXES.tsv: 0x006C1DD0's landed stand-in leaf (CThingSearchToolsMethods / CResultVector) ->
the PDB spelling above (not applied; lock 006c1dd0 claimed AUTH26 for that respell if the coordinator wants it as a unit).

## Test (006c0b20_test.cpp)
Linked to the row object; fakes every external (CTCBase accessors, player chain, GetPos, PeekClassList over a hand-built STLport
list, the two filter operators, IsAwareOfPosition, a slot-16 world-map vtable). Private / protected members via explicit
instantiation (the landed 0x0075A7A0 test pattern). Scan bits found through CThing's own inline predicates and checked = retail
(+0x91: 1 / 4|8 / 20). Cases A-H: first unseen entrance wins and later ones are not asked; filter values (99, map, map number) and
HasTC-before-InMap; near hero (369 < 400, Z ignored) -> false; exactly 20 -> house test; aware of house -> false; null hero; scan
exclusions; 37 entrances (vector growth); empty list. Mutants m1 (compare not squared), m2 (null hero skips house test), m3 (walk on
past the first unseen entrance) all FAIL.

## Pick-any copies (all of the object's defined externals that reloc_bindings binds; retcmp, bytes + relocation targets)
GetAllThings<...> 0x006C1DD0 158 B, vector<CThing*>::_M_insert_overflow 0x00436AC0 216 B, vector<CThing*>::push_back 0x00436C80 50 B,
CThing::IsIncludedInThingScans 0x0048BC40 36 B (the ISALIVE text), _List_iterator_base ctor 0x0041C780 11 B / _M_incr 0x0041C7A0 7 B /
operator!= 0x0041C7C0 22 B, placement operator new 0x00405C10 5 B: all EQ, relocs EQ. The object's other 43 defined externals have no
binding (no landed owner).
