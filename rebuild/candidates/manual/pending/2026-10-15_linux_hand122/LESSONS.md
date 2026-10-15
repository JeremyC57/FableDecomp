# Chunk 122 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A CCharString local declared AFTER an `if` whose condition inlines GetInterface shares the lookup key's frame
  word only when it sits in its OWN block (`{ CCharString slot("..", -1); DestroyItem(slot); }`); at function scope
  the frame grows to `sub esp,8` (0x00944690).
- An uninitialised `long` filled through a pointer lands in the dead argument slot only as the local of an INLINE
  helper that returns it (`inline long ParseLong(ctx, parser) { long v; TransferIn(&ctx, &parser, &v); return v; }`);
  a body local takes a frame word, an inlined by-value parameter adds a self-copy (0x005DC3C0).
- A by-value class result consumed as `member = Call();` reads through the returned pointer in eax when it is an
  unnamed temporary (assign into a reference view of the member); a named local re-reads the frame and can even
  move `this` to another callee-saved register (0x005DBDA0).
- `return !Pred();` widens to `sbb eax,eax ; inc eax`; `return Pred() ? false : true;` keeps retail's byte
  `neg al ; sbb al,al ; inc al` (0x009635C0). `return result == 2 ? true : false;` after an inner block gives the
  post-destructor `cmp ; sete al` without `xor eax,eax` (0x009454C0).
- Retail's "false tail right after the second test" layout needs two separate `if (!x) return false;` statements;
  `!a || !b` and `a && b { ... }` both move the false tail to the end (0x00870D70).
- An argument that is evaluated before an object's zero-constant stores (`C3DVector(0,0,0)` passed by const
  reference to a virtual) must be a NAMED local; the unnamed temporary lets VC hoist the object-pointer load above
  the stores (0x009685C0).
- An embedded action member whose position is copied with direct `[edi+N]` stores has three SCALAR float members
  (a C3DVector member copies through `lea ecx,[edi+0Ch]`) (0x00713180); a constructor-body assignment of a float
  sinks its immediate store below the argument pushes, an init-list entry does not (0x009395F0).
- A CScriptThing passed BY VALUE keeps both the root and the CScriptThing vtable stores in the argument area; its
  counted pointer must be a member with its own inline copy constructor (re-addressed from `this` with the info
  word in ecx) -- a flat Data/Info copy reuses the cached member-address register (0x00E33720).
- A thing-search filter pair `CFilter_Bind2<A, B>` holds two const references; the empty B filter lands in the
  dead high byte of the lowest frame word (`lea edx,[esp+7]`) only as a TEMPORARY in the Bind2 construction
  (0x00964360).
- An inline intermediate-base constructor taking an argument computed by a virtual call (CSpawnedFuncBase(name))
  must be `__forceinline`, else VC emits it out of line (0x00CE1110).
- The combat-action temporaries behind AIAction_*PerformActionAndWait are 0x114 bytes (CCombatActionBase) or
  0x118 (with one extra word: CCreatureAction_RiseUp) -- size the model from the frame (0x0094DBC0, 0x0096E610).
- Retail STLport `vector::reserve` (`_M_allocate_and_copy` + `_M_clear`, else `_M_end_of_storage.allocate(n)`,
  then `_M_set`) reproduces in a frame-pointer size TU with `optimize("y",off)` + `optimize("s",on)` (0x005C2013).
- GSI virtual slots are named by `rtti-vtables.tsv` (CGameScriptInterface 0x01260F0C) joined with
  `ghidra_out/gamescriptinterface_catalog.tsv`: 70 GetHero, 360 SetThingHasInformation, 599
  EntitySetThingAsAllyOfThing, 844 SetIsPushableByHero (0x00E33720).

Review at landing:
- Rows that needed more than 4 grades: 00945a90 (5: the inline-helper probe first compiled without inlining) and
  00945b10 (5 including one SRC_FAIL compile); both are parked.
- Local models next to an existing engine header: 0051a110 (CRegion.h has 16-byte PDB vectors; retail 12),
  00a763f0 (CANavQuadTreeNode.h is data-only, the body needs virtual slot 5), 005dc3c0 (CAnimComponentFlags.h is
  data-only, the body calls virtual slot 8), 009635c0 (modelled over CAIStateGroup_VersusEnemyBase.h instead of the
  flat CAIStateGroup_VersusScorpionKing.h, whose +0x14 is a pad). The other header rows use `_Methods` shells over
  the PDB structs with PBrain read from the flattened base bytes.
- Catalog/label changes the lander must accept: 0051a110 -> CRegion::~CRegion (leaf `??1CRegion`), 00717bc0 ->
  NTCScriptedControl::CActionOverheadTalkToThing ctor (leaf `??0CActionOverheadTalkToThing`), 009428a0 ->
  CAIStateGroup_WorkAtSchool::Teach (leaf `Teach`), 005c2013 -> CRetailVector20::reserve (leaf
  `?reserve@CRetailVector20`). Rows whose catalog name is mangled carry `// Leaf:` lines (009685c0, 005dbda0,
  005dc3c0, 009635c0).
- Already-landed source worth a look: rebuild/src/compiled/00/5c/CActiveFile_OnReadFinished_005c1f1f.cpp is an
  "AutoTiny" __fastcall stub under a propagated label -- 0x005C1F1F is the STLport vector `_M_clear` called by
  0x005C2013's reserve. rebuild/src/compiled/00/40/CStringParser_CStringParser_00404720.cpp casts raw vtable
  addresses and calls the base constructor through `extern "C" __fastcall` stand-ins (not a C++ class model).
- No catalog row of this chunk holds several functions.
