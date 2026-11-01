# Chunk 693 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- zlib `local` functions in a byte-match row: compile the TU as C (`// Flags: /TC`). In C++ `d_code(dist)`'s conditional
  over two `const uch` lvalues is an lvalue, and VC7.1 merges the arms before one byte load and keeps a dead `lea` for
  `.Freq++` (922v909 -> match, deflate_fast 0x00C013C0).
- /GS buffer placement: a `char id[5] = "BREC";` initialised from the literal is the /GS buffer directly under the cookie.
  memcpy from a global into the same array placed it below the class locals and shifted the frame by 8 (0x009CDF50).
- Implicit conversion temporary order: `Game->SetQuestAsFailed(Game->GetActiveQuestName(), false, L"", true)` (implicit
  `L""` -> const CWideString&) builds the temporary BEFORE Game's vtable load. An explicit `CWideString(L"")` loads Game first
  and reshuffles registers through the whole body (0x00D89E60).
- Mixed inline / out-of-line ~CScriptThing WITHOUT `#pragma inline_depth`: engine/CScriptThing.h plus a plain
  `inline CScriptThing::~CScriptThing() { }`. VC7.1 then expands it at the first return only and calls 0x004AA840 at every
  later exit, exactly retail's split. The landed CUndeadFellow / LookAtMe1 models used pragma regions for the same split
  (0x00EF8D30 matched on its own).
- A materialised `bl` that holds the SKIP condition (`test bl,bl ; jne <shared NewScriptFrame>`) is an early
  `if (!A || B) { Game->NewScriptFrame(); continue; }` in a `while (!IsActiveThreadTerminating())` loop. The positive
  `if (A && !B) {...}` materialises the inverse byte and moves the zero / pointer registers (85 lines -> match, 0x00EF8D30).
  Turning the second test into a `continue` too breaks it (+16 bytes): keep it a nested if.
- libpk C / MMX Process rows repeat the same-TU static mixer bodies above Process. VC7.1's inline budget then reproduces
  retail's "inline at the first N sites" by itself (0x00C6CC60: MixDelayTap inlined twice).
- Two RECT locals: VC7.1 puts the first-used RECT at frame +0 whatever the declaration order, scope or names (12 probes);
  `tagRECT rects[2]` gives retail's frame (0x0099D8C0, still parked on arithmetic order).
- A REPAIR row without a `// Leaf:` line grades the largest function in the object (here an out-of-line base ctor), so a
  landed source can "differ" only because of its missing Leaf (0x00BC9C30).

Review at landing:
- Parked 0x00C6E8F0 / 0x00C6DBA0 / 0x0099D8C0 (12 / 14 / 19 probes). The two mixers share one register-allocation residue.
- All four quest-script rows are on the shared headers (CScriptThing, CTimer, CIsThingAlive, CPredicateBase,
  CEntityScriptBase). Relocations are masked, so a callee declared in the wrong namespace still grades RELOCATION_MATCH. An
  early 0x00EF8D30 draft declared IsDistanceBetweenThingsOver inside namespace NScript; the PDB symbol is global
  (?IsDistanceBetweenThingsOver@@YI_NABVCScriptThing@@0M@Z). Check callee manglings with objdump -t, not the grade.
- Already-landed sources with errors: CEngineSubPrimitiveSpriteTrail 0x00BC9C30 and CBankFile 0x009CDF50 (replace by hand).
  Landed callee spellings for 0x009CBFE0 / 0x009CDA70 / 0x00994400 / 0x00994480 and the bare "WardenKeyMiniGame" Leaf of
  0x00DDE230 are in NAME_FIXES.tsv.
- The landed CUndeadFellow::Main 0x00EF87D0 and CLookAtMe1::Main 0x00EF5CB0 rely on `#pragma inline_depth` regions. The
  0x00EF8D30 lesson above (plain inline header dtor) is the pragma-free re-model to try on them.
- g_pAudioMaster spelling (struct v class CAudioMaster) differs across landed libpk rows: coordinator decision.
- 0x009CDF50 models CAFile / CDiskFileWin32 / CDataOutputStream / CFileDataOutputStream / CBankFile locally: their engine
  headers are flat (header debt).
