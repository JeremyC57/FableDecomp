# Chunk H40i (ninth pass over the 23 long parked rows) -- notes

Grade with `FABLE_HDR_OVERLAY=$A/hdr` (hdr/README.md top section). 10 grades in ATTEMPTS.log; no row matched; no src/ rows.

## 0x00507C30 CWorldMap::LoadFromFile (?LoadFromFile@CWorldMap@@UAEXAAVCAFile@@J@Z) -- DIFFER(7661v7661), 2 normalised lines
- Size now matches retail (7677 -> 7661). differ/00507c30.cpp = the best grade (507_2).
- Lever 1 (507_1): the unused local that retail constructs after the file read and destroys at exit is a **CParsedItem** (Ego_r class,
  12 bytes, CCharString String at +8), not a CCharString: retail `lea ecx,[esp+160h]; call 0x0099E4B0` (0x00507DFE, dtor 0x0050953A)
  addresses String at +8; the two unreferenced retail frame words +0x158 / +0x15C are its Type / union. A 12-byte node sorts after every
  4-byte slot, which gives retail's frame 0x1BC (was 0x1B4) and repacks the post-if temporaries. Uses HEAD's landed FABLE_RETAIL_CPARSEDITEM.
- Lever 2 (507_2): CurrentUniqueID is written through FableWin's inline `CThingManager::SetCurrentThingUID(unsigned __int64)`
  (?SetCurrentThingUID@CThingManager@@QAEX_K@Z; stores at 0x00507E87 / 0x00508217): hdr opt-in FABLE_RETAIL_CTHINGMANAGER_SETCURRENTTHINGUID.
  The direct member store scheduled the loop-entry lea/store differently.
- Residue: the loop-exit reload order at 0x00509395: retail ebp, edi, ebx (file, buffer-data CSE, this); ours ebp, ebx, edi.
  MEASURED with c2prio: the reloads come out in ascending order of the split candidates' FINAL ids; the split pass walks parents in
  ascending original id and takes ids from a LIFO free list. Ours: file 65 < this 67 < local 71 (66 = live `length`). Retail needs the
  local between file and this. Probed (no grade, all inert, local stays 63 / 68 / 71): bufferSize / end / source locals, split
  `char* text;`, `char* const`, `parser(&buffer[0])`, size-copy variants, ternary, text declared at top / before the length test.
  Graded dead ends: no named text (507_3, 507_5: ebp/ebx swap, 50 lines), buffer.begin() (507_4 inert), `long size = length` (507_6:
  order edi/ebp/ebx). Next idea: change the ORIGINAL creation order of the buffer-data web (e.g. where the CCharString/vector buffer is
  constructed relative to `this`'s first use, or an inline accessor that touches buffer before the LevelLoader `new`), checked by c2prio
  `--func 'LoadFromFile@CWorldMap'` before grading.
- Follow-up probes (c2prio, no grade; work/i/pv_n..pv_s): `&buffer.front()`, `&*buffer.begin()`, a reference onto the vector's
  start, `std::vector<char>` for CArray, an extra scope round the buffer: the local lands at final id 71, 2 or 5, never between
  file and this. MEASURED mechanism (507_2.trace): three split passes; pass 1 hands fresh piece ids in ascending candidate id
  (63 mapX .. 65 file, 67 this .. 71 local -> #294..#301), pass 2 renumbers from a LIFO free list (order reversed), pass 3 again
  (reversed back), so the reload order is the ascending order of the INITIAL candidate ids. file / length / this are always
  65 / 66 / 67 (adjacent params), so no source position of the local can put it between file and this: retail must differ in
  which candidates the passes split (which pieces exist), not in declaration order. Next: compare c2's per-pass split lists
  (trace "split pass over ids") against what retail's code implies for `this` (its esi piece #104 and the three ebx pieces).
- Header debt still in the row: row-local CGenericException, NProfileTimer, GetLevelsDirectory, the mode flags, the CBasicString
  Compare / operator== template bodies; ReadAsStringUntilWhitespace stands in for 0x009BA390.

## 0x0078FD60 CTCDivineWrath::FrameUpdate -- SRC_FAIL, not graded
- HEAD's engine/CTCDivineWrath.h layout is already retail's (CIntelligentPointer pack(1); CWillButtonHelperTimer 0x34; PRA +0x76,
  map +0x78): no layout opt-in needed.
- Compile probe (work/dw/d1.cpp with FinalCleanup 0x0078FAF0's opt-in set; landed precedent
  rebuild/src/compiled/00/78/CTCDivineWrath_FinalCleanup_0078faf0.cpp) gives >100 errors, all missing declarations / accessor spellings.
- Callee bindings proven (LABELS.tsv; the differ header lines list the rest): CTCWillResponse::IsThing* static __fastcall overloads;
  the three CCreatureAction_*DivineWrathSpell ctors; CHitParameters 9-arg ctor 0x00789238; AddDamageWithAttribute 0x00868CA0
  (DAMAGE_DIVINE_WRATH = 9); ModifyStaminaBy 0x0057B1DB; first-strike search = std::list<CThingObject*> with
  CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell> (built / destroyed out of line 0x007926C0 /
  0x00792240; operator() 0x005A4D70); CreateCameraShake 0x007F1B70 is __fastcall with edx = strength; PeekThingSearchTools 0x00686D90.
- Continued after the coordinator's follow-up (compile probes work/dw/d2.cpp, no grade): the first ~40 errors closed with the hdr
  opt-ins of hdr/README.md ("Added later in H40i"): the three actions modelled as 0x114 CCombatActionBase + ctor (destroyed by
  ~CCombatActionBase 0x0062E900), SetPItemRemoved 0x0072D360 (NAME_FIXES), the inline timer state read, new CTCWillResponse.h /
  CThingFilter_CanBeHitByEpicSpell.h / CTCDRumble.h (0x007F1B70 = CTCDRumble::Create, NAME_FIXES), and the row switched to
  FABLE_RETAIL_CTHING_GETTC_EXPANDED (0x0078FE33..0x0078FE5E is exactly that LowerBound shape). Still >100 errors; the remaining
  list is line 3 of differ/0078fd60.cpp (C3DVector / CThing / CHitParameters / events / experience / nymph / mode-manager
  accessors, the GetAllThingsInRoughArea declaration branch, SetAsGameStatePersisted's inline body, and the killed-creature tail
  0x0079136D.. / 0x00791CB6.., still pseudo-code). Not graded: a grade needs a compiling TU.

## Baseline regrades only (1 grade each, unchanged)
- 0x00C4F600 (CEAXManager::SetEAXProperty; catalog name wrong, see NAME_FIXES): DIFFER(9204v9204), 83 nd lines, ~70 of them
  jump-table data masking; real residue = the ROOM / ROOMHF / ROOMLF data-reload register rotation (retail edx, eax, ecx; ours ecx, edx, eax).
- 0x00D0F640 CheckForSecondAreaMassacre: DIFFER(2131v2101), 108 lines.
- 0x00E50FD0 CBB_BeardyBaldyMan::Main: DIFFER(9850v9885), ~99 lines (const 0 v vtable register in ebp).

## Not reached
All other rows (STATUS.tsv): drafts untouched.
