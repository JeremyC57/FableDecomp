# Landing nf249 (Account A, 2026-11-22): UnsheatheWeapon protected (PDB), 0x00890D80 onto the shared headers, the CEngine step-3 header opt-ins (the CEngine ctor / dtor pair HELD)

Agent NF249 (q12690, q12691 done; q12692 header done and both rows graded, rows held here; q12659 open). Hand-back:
`pending/2026-11-22_nf249_handback/` (rows/ carries the held 0x00B260B0 / 0x00B261D0 drafts; lk3.txt = the link that shows the clash).
- **q12690:** `?UnsheatheWeapon@CThingCreatureBase@@IAEXJ_N@Z` (protected; Ego_r / FableWin / Ego_d; bsim Ego_r 0x00540710 <-> 0x006648D0):
  self 0x006648D0 and binders 0x006649C0 / 0x00664A50 respelled in place on their local models; 0x00664A10 through NF247's
  `FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHERANGEDWEAPON`, now `protected: void UnsheatheWeapon(...); public:` on one line (line count
  kept). 0x006648D0 stays at one spelling, now the PDB one; table line respelled. equiv_check over 996 includers: only the macro's
  definer and the two known `__FILE__` rows differ.
- **q12691 0x00890D80** (CGameScriptInterface::EntityUnsheatheRangedWeapon): every local model -> the shared headers (CGameScriptInterface,
  CScriptThing, CThingCreatureBase, CTCCarrying, CWorld, CCombatManager, CGameEvent, CMainGameComponent); self leaf and four bindings
  onto their PDB spellings: 0x006A1620 3 -> 2 (0x00890250 remains, authoring), 0x00664A10 2 -> 1, 0x013B86A0 loses `g_MainGameComponent`.
  Two table lines deleted.
- **q12692 headers (add-only, no landed definer):** CEngine.h `TYPED_MEMBERS` (seven members typed, nested CEngine::CPreloadStats),
  CIEngine.h `INHERITED_VALIDATE` (slot 1 -> `?Validate@CInitBaseClass@@UBEXXZ` 0x0099A340), CIEngine.h `CTOR` (protected) + `INLINE_DTOR`,
  CEngineVideoOptions.h `CTOR`. equiv_check over 151 includers: 0 differ; pin_asserts / assert_names_check 0.
- **Recheck** (`logs/recheck.log`, landed tree): the 5 changed rows, the table witness 0x008B7BA0, 0x006A1620 and the HEAD CEngine rows
  0x00B260B0 / 0x00B261D0 / 0x00B2AB10 under the new headers (10 rows): 10 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.

## Coordinator rulings on NF249's questions
1. The three q12690 rows changed in place on their local models: accepted (standing in-place ruling); header ports queued as debt.
2. The one-line `protected: ...; public:` form in the opt-in: accepted (keeps every later line, and so every pinned assert, in place).
3. 0x00890C80 (EntityUnsheatheMeleeWeapon) with the 0x00890D80 recipe: queued.
4. 0x0122ED70 bound as `__real@0000000000000000`: accepted when the pair lands (as `g_fMinusOne` -> `__real@bf800000`); the other two
   stand-ins (`g_clearDouble`, `g_zeroPitch`) queued onto it.
5. 0x00890D80's byte-equal `??0CGameEvent@@QAE@J@Z` copy (LNK4006 v the strong owner 0x004159B0): accepted (the equal-copy ruling; the
   CGAMEEVENT_INLINE debt of hand3c3_b10).
6. **The CEngine pair 0x00B260B0 / 0x00B261D0: held (b).** 0x00B2AB10 (the `??_GCEngine` TU-emitter row) defines a strong scaffolding
   `CEngine::CEngine() { }`; with the ctor row on `??0CEngine@@QAE@XZ` the two strong definitions have different bodies (one new LNK4006
   in lk3.txt), so the link could take the wrong body. Option (a) (fold / re-point 0x00B2AB10 under the nf159 TU-emitter rule) is
   queued as the unblocker; the pair lands with it, with its table lines (four deletions, the `__real@0` add) from TABLE_CHANGES.tsv.
Rule slips (0x00890D80 graded before its lock was claimed; parts of the read-first list skimmed): noted.
