# NF278 notes (2026-11-22, round 33b)

Base: the repo working tree at session start (landings up to nf276). NF277 runs alongside me (0x00BFE900 / the landscape host-model tests):
none of its rows touched; its hdr/ was empty. Every row I compiled was `released` or had no lock directory before I claimed it with
tools/lock.sh (work/locks.log). None is in codex_pass28 RESERVED.tsv (0 hits for every address).
Reading list: BRIEF.md (NF278), NF275 / NF273 BRIEFs, NF214_BRIEF.md, ASM_RULES.md, EXTRA_RULES.md, COORDINATION.md, GOTCHAS.md (all 503
lines, before the first compile), the nf270 / nf273 / nf275 landing READMEs, the NF275 hand-back (NOTES, LESSONS, NAME_FIXES, HDR_README).
No header changed (hdr/ and orig/ empty): no equiv_check needed.

## Entry 1: the CMainGameComponent slot self rows -- 4 of 5 done; 0x00416E78 open

### Claims (two sources each)
| Address | Leaf now | Source 1 | Source 2 | Was |
|---|---|---|---|---|
| 0x00415E83 | ?OnPreDeviceReset@CMainGameComponent@@UAEXXZ | Ego_r.pdb 0x00415023 + FableWin.pdb 0x018A169C | retail vtable 0x0122F174 slot 0 (tools/rvt.py); body `ret` | ?Hook@T@@QAEXXZ |
| 0x004197A0 | ?SetQuit@CMainGameComponent@@UAEXXZ | FableWin.pdb 0x018A11F0 | vtable 0x0122F180 slot 4; body `mov byte ptr [ecx+8],1 ; ret` = Quit +8 | ?SetQuit@T@@QAEXXZ |
| 0x004197B0 | ?IsEditorActive@CMainGameComponent@@UBE_NXZ | FableWin.pdb 0x018A1210 | vtable 0x0122F180 slot 9; body `xor al,al ; ret` | ?IsEditorActive@T@@QAE_NXZ |
| 0x004197B3 | ??_GCMainGameComponent@@UAEPAXI@Z | Ego_r.pdb 0x00418362 + FableWin.pdb 0x018A1307 | vtable 0x0122F180 slot 0; body calls 0x00418B7A then operator delete 0x00BFE9BC | ?ScalarDeletingDestructor@CMainGameComponent@@QAEPAXI@Z |

### Rows
- 0x00415E83 / 0x004197A0 / 0x004197B0: each drops its stand-in `struct T` for engine/CMainGameComponent.h (default path / the landed
  `FABLE_RETAIL_CMAINGAMECOMPONENT_INIT_SETQUIT` / the landed `FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR`), body unchanged. MATCH first form; each
  object defines only its leaf (symbol diff: old stand-in out, PDB leaf in; no relocations, no other COMDAT). HEAD tests kept (placeholders).
- 0x004197B3: VC7.1 emits ??_G only beside the vtables, i.e. in the ctor or dtor TU (both own rows; CMainGameComponent has no inline ctor or
  dtor, so the nf86 scaffolding cannot reach it). So the row is folded under an emitter's TU, the landed 0x00B2AB10 ??_GCEngine pattern
  (landing nf251 ruling 1): its source has no code and includes `../src/compiled/00/41/CMainGameComponent_CMainGameComponent_00418b7a.cpp`
  (THE DTOR'S FILE, BY PATH: includer greps must follow it; a rename of 0x00418B7A's file breaks this row).
  - Form 1, fold under the ctor TU 0x00418DCA (work/alt/004197b3_ctorfold.cpp): RELOCATION_MATCH, but pk finds one NEQ copy,
    ??1?$CCountedPointer@VCGraphicDataBank@@@@QAE@XZ v 0x004194B5 (44 v 44; NF275 debt in 0x00418DCA) that the fold would define a second
    time (evidence/pk_004197b3_ctorfold.tsv).
  - Form 2 (final), fold under the dtor TU 0x00418B7A, the ??_G's only callee's TU: RELOCATION_MATCH; the object's 12 function COMDATs are
    SAME as the HEAD dtor object's (evidence/cmpobj_004197b3_v_00418b7a.txt); pk: every copy EQ (the CGraphicDataBank one, 5 = 5 v 0x004194B5)
    or NO-OWNER (evidence/pk_004197b3.tsv). Leaf relocation: 0x00418B7A ?Dtor@CMainGameComponent@@QAEXXZ (stand-in) -> ??1CMainGameComponent@@UAE@XZ.
  - The fold object is the dtor TU compiled twice (as nf251's). The link of the FINAL (dtor-fold) form was not run; evidence/lk_e1.txt is the
    ctor-fold form (one LNK4006 on ??0CMainGameComponent, identical body). The dtor fold should give the same shape on ??1CMainGameComponent
    (its 12 COMDATs are SAME as the dtor object's); counted one object per TU, 0 new duplicates expected. It inherits 0x00418B7A's local models
    (that row's SHARED_HEADER_AVAILABLE debt); when 0x00418B7A is ported, this row follows with no edit.
- vland --recheck-landed (evidence/vl_e1.log; the four rows + both emitters): 3 MATCH + 3 RELOCATION_MATCH, 6 PASS, 0 RELOC_CONFLICT. WARNs:
  0x004197B3 STANDIN_LEAF (catalog spells the name "`scalar_deleting_destructor'"; the leaf is the compiler's ??_G; the nf251 recheck log has the
  same WARN for 0x00B23770 ??_GCIEngine), and the
  emitters' HEAD-present RELOC_NAME / SHARED_HEADER_AVAILABLE lines.

### Census (spellings per address, before -> after; exe scan: tools absrefs / callers)
- 0x00415E83, 0x004197A0, 0x004197B0, 0x004197B3: referenced only from their vtable slot (no E8/E9 caller, one .rdata word each). Both HEAD
  emitter objects (compiled, evidence/vt_head_*.txt) bind slot 0 ??_ECMainGameComponent@@UAEPAXI@Z (the weak alias VC7.1 emits for ??_G; both
  also define ??_G pick-any) and slots 4 / 9 / OnPre exactly the new leaves. So each address 2 -> 1 (the stand-in self leaf gone).
- 0x00418B7A: its only caller is 0x004197B3 (retail 0x004197B6); {??1 self, ?Dtor stand-in} 2 -> 1.
- Old names: ?Hook@T@@QAEXXZ / ?SetQuit@T@@QAEXXZ / ?IsEditorActive@T@@QAE_NXZ are also defined by unrelated gapscan rows (byte-twin
  stand-ins at other addresses); my rows stop defining them.
- New names: no landed row (src/compiled) defines CMainGameComponent::OnPreDeviceReset / SetQuit / IsEditorActive besides mine (out-of-class
  definitions grepped; the rows that define `class CMainGameComponent` locally were grepped for in-class bodies: only calls found); tests that
  define them are link stubs of their own test executables (e.g. 0x00418B7A's test). ??_GCMainGameComponent is compiler-generated, pick-any
  in the two emitters and the fold (all SAME).
- Link (evidence/lk_e1.txt: HEAD emitters + the new objects, LINK unset; the fold object there is the ctor-fold form): OnPreDeviceReset /
  SetQuit / IsEditorActive resolve (unresolved at HEAD: their self rows defined only T:: stand-ins); 0 duplicates without the fold object;
  the fold adds the expected identical-body LNK4006. Not rerun for the dtor-fold form.
- Table: no change (no table line targets these addresses or 0x00418B7A).

### 0x00416E78 GetInputs -- open (not started; a port, not a respell)
Its local model is a non-virtual public `GetInputs` on a `_pad000[0x20]` CMainGameComponent, so MAE needs the shared header (an in-place
`virtual` would add a vptr and shift every member). Port map found (for the next agent):
- `this`: engine/CMainGameComponent.h with EDITOR (MAE slot 6), PPLAYERINTERFACE (+0x20 scoped_ptr<CGamePlayerInterface>), EVENTPACKAGES or
  SMART_POINTER_ACCESSORS (+0x24 scoped_ptr<CWorld>, PeekWorldChecksum), CMAINGAMECOMPONENT + ADDGAMEEVENT + EVENTISSYSTEMEVENT; the local names
  map to Ego_r members: "InputChannel" +0x48 = CurrentServerFrame, "Frame" +0x1613C = WorldSeed, "Loading" +0x16170 = LoadingEventPackages.
- ProcessEvent 0x00415FE0 has no declaration in the header: needs an opt-in with the PDB IAE spelling (table: IAE witness 0x00416670, QAE
  only from this row: NAME_FIXES). EventIsSystemEvent likewise binds QAE here against the header's IAE (NAME_FIXES).
- The camera owner at +0x24 is CWorld: PCurrentGameCamera +0x34 (engine/CWorld.h, CAIGameCameraBase*), slot 1 the camera fetch: needs
  engine/CWorld.h + CAIGameCameraBase.h; CCamera / CChecksum / CGameEvent / CInputManager / CSystemManager / CGamePlayerInterface all have
  shared headers (the row models each locally). 0x004457F0 is bound as ?BeginEventPoll@CGamePlayerInterface@@QAEXXZ against a self Leaf
  ?CXMVPlaybackInfo@@... (NAME_FIXES, unsettled). The row is /GS under optimize("s")/("y",off).
Slot 6 stays {MAE (emitters), QAE (self)}: 2 spellings, as at HEAD.

## Entry 2: the `_M_erase` split at 0x004507A1 -- measured, not applied (needs a ruling)
- Census of all eight rows with the 3-argument local `_Rb_tree<K, V, Compare>` model (HEAD objects compiled, all RELOCATION_MATCH): only the
  brief's four bind or define the 3-arg `_M_erase`: 0x00ADC190 / 0x00ADC250 / 0x00ADC4E0 bind it from the leaf (retail calls 0x004507A1 at
  0x00ADC1BA / 0x00ADC289 / 0x00ADC52B) and from their ~CVertexGroupBlock copy; 0x00ADCB90 only from the copy. All four also DEFINE a pick-any
  3-arg `_M_erase` copy (51 bytes). 0x00ADCC80 / 0x00ADC3B0 / 0x00ADC330 / 0x00ADC440 neither define nor reference _M_erase or the dtor.
- pk (evidence/pk_e2_head.tsv, with the 3-arg spelling owned by 0x004507A1): each 3-arg copy NEQ:bytes (51 v 51) against retail; each
  ~CVertexGroupBlock copy EQ against 0x00A96DA0 once the 3-arg call is read as 0x004507A1.
- Zero-compile comparison (tools/cmpsym.py, evidence/cmp_merase_3v5.txt): with the 3-arg name renamed to the fable_map.h 5-arg name, each of
  the four 3-arg `_M_erase` copies is byte- AND relocation-identical to the 5-arg `_M_erase` copy of 0x00A96DA0's HEAD object (which is
  itself NEQ:bytes 51 v 51 against 0x004507A1: HEAD debt shared by every fable_map.h user, NF275 pk_e2), and the ~CVertexGroupBlock copies
  of 0x00ADC190 / 0x00ADC250 are identical to the owner's including relocations. ONLY those two symbols were compared. An arity respell
  also renames the local `clear` (owner 0x00451046 per nf270, where fable_map.h copies are NEQ), `~_Rb_tree`, `~map` (map's arity too),
  `_M_root` / `_M_leftmost` / `_M_rightmost`, `_S_left` / `_S_right` and `_Rb_tree_base` copies: UNMEASURED (objects cleaned).
- Why not applied:
  (a) the in-place arity respell (local `_Rb_tree` to five parameters) needs a `_Select1st` class token the rows do not define at HEAD
      (fable_map.h owns it), and each object would define symbols naming the respelled class beyond its leaf (`_M_erase`, `clear`,
      `~_Rb_tree`, `~map` copies, some of which fable_map.h users also define): it fails both conditions of the 2026-11-20 in-place rule;
  (b) the alternative, a port onto stlport/fable_containers.h + fable_map.h + engine/CVertexGroupBlock.h (MEMBERS, DTOR_INLINE, as
      0x00A96DA0 / 0x00AD9F10), was not attempted (context budget): the leaves must keep retail's partly inlined `_M_erase` top level, and
      three are leaf-bound to the newly owned 5-arg `_M_erase` whose copy is NEQ v 0x004507A1 (a held shape under the p25 rule unless the
      byte-identity above is accepted).
- Decision needed: whether (a) may be attempted as a scoped exception (measured so far: `_M_erase` and `~CVertexGroupBlock` copies identical
  to the landed owner's; the other renamed copies unmeasured), or the four rows go to a port (b) (queue), or stay as debt.

## Entry 3: 0x00ADCE60 -- open (not started; context)

## Whole-pass checks
- recheck_final.log: one vland --recheck-landed run over the four changed rows (no overlay), rerun after a comment-only fix to 0x004197B3 (slot-0 ??_E wording, caller direction; clang precheck first): 2 MATCH + 1 MATCH + 1 RELOCATION_MATCH, 4 PASS, 0 RELOC_CONFLICT.
- New symbol names: the four leaves only (census above); no new macro, header, helper or descriptive name. Flags lines: none added.
- Funclets / folds: no landed file includes the four rows' files; 0x004197B3 now includes 0x00418B7A's file (above).
- Locks: 0x00415E83 / 0x004197A0 / 0x004197B0 / 0x004197B3 stay NF278 (rows/); every other row I claimed (0x00416E78, the controls
  0x00418DCA / 0x00418B7A, the eight entry-2 rows, 0x00A96DA0) is released.
- Nothing written in the repo; no git; no sleep. Grades and vland runs launched with `&` under `timeout 900` and checked in later calls;
  every NEW source was clang-prechecked before its grade (the fold row: each form). Exceptions: see Rule slips.
- Tools added: tools/cmpsym.py (+ cmpobj_lib.py), one-symbol cross-object comparison with a rename map.

## Rule slips
- The entry-1 link (tools/lnk2.py, two link sets in one call) ran in the foreground (under `timeout 300`), which the brief forbids.
- The HEAD-source grades (every pH / SKIPS control run: 0x00418DCA, 0x00418B7A, the eight entry-2 rows, 0x00A96DA0) and the landed emitters and
  HEAD tests compiled by the two vland runs had no clang precheck of their own.
- One Bash call exited 1 (a `cat` of test files that live under rebuild/tests, not src/compiled).

## Decisions for the coordinator
1. Land 0x00415E83 / 0x004197A0 / 0x004197B0 (header ports, MATCH) and 0x004197B3 (fold under the dtor TU 0x00418B7A, RELOCATION_MATCH,
   nf251 pattern). Note the dtor fold compiles 0x00418B7A's LOCAL models, so it is not "onto the shared header" as the brief worded it; the
   alternative is the ctor-TU fold (work/alt/004197b3_ctorfold.cpp, shared headers) with its one NEQ copy (~CCountedPointer<CGraphicDataBank>). RENAMES section 2: the
   ??_G's dtor binding ?Dtor stand-in -> ??1. TABLE: none. Catalog: 0x004197B3's name stays; STANDIN_LEAF WARN as 0x00B2AB10.
2. 0x00416E78: queue as a port (map in NOTES entry 1); NAME_FIXES ProcessEvent / EventIsSystemEvent / 0x004457F0.
3. Entry 2: rule on (a) an in-place 5-arg respell (rule exception; `_M_erase` / dtor copies measured identical, the other renamed copies
   unmeasured), or (b) the shared-header port, for the four rows.
4. Entry 3 (0x00ADCE60) not started.
