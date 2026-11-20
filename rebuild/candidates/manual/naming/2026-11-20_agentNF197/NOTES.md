# NF197 notes (2026-11-20, round 10)

## Per entry

- q4860: self row 0x00E2AB30 is locked NF194 (hand-back pending landing); deferred, nothing touched.
- q5052 (~CBaseObject, self right): 0x00687730 in-place access fix of its HEAD-local CBaseObject (dtor under `protected:` -> MAE).
  0x00687210 re-ported from the generated `CAggregate_00687210` onto engine/CMessageEvent.h as the implicit `~CMessageEvent`
  (nf86 scaffolding: a qualified explicit destructor call in an anonymous-namespace helper; a user-written empty body stores the
  vptr, retail does not). This is a self rename of a row outside my entries (catalog `_unattributed::~CAggregate_00687210` ->
  `CMessageEvent::~CMessageEvent`): both PDBs carry ??1CMessageEvent@@UAE@XZ, the vtable 0x0125BE8C slot 0 expands the same
  body, and the exe scan finds no referrer of 0x00687210. The 0x00687730 copy of ??1CMessageEvent@@UAE@XZ (HEAD, pick-any) is
  byte-identical to the new owner (cmpbody). Census of the rest in APPLIED (script rows: whole-row ports).
- q5064 (WriteSLONG, self right): 0x005DB1E0 re-ported onto engine/CAnimComponentFlags.h (leaf-only self rename, catalog already
  right); 0x00614620 in place (its HEAD-local CDataOutputStream class gains WriteSLONG). 3 junk stand-in rows blocked.
- q5138 (ToC2DBoxI): self rename ToIntBox -> ToC2DBoxI (no landed row binds ToIntBox). 0x00730F90 stays on ?ToInteger@C2DBox
  (2 forms DIFFER 290v292 on the header centre ctor; the pre-existing split narrows from 3 spellings to 2).
- q6196 (~CUniqueVertex): self rename CTwoLongSets -> CUniqueVertex on the shared header. The new pick-any COMDAT
  ??1?$_Rb_tree@KKU?$_Identity@K@std@@U?$less@K@2@V?$allocator@K@2@@std@@QAE@XZ has a table line to 0x00517850 (made by the stand-in
  binder 0x0051C920). Ownership by body (advisor nf181 rule): 0x00517850 erases through 0x004A9CB0, while set<unsigned long>'s
  _M_erase is 0x004CF810 (this row's body, Ego_r CIndexList set<unsigned long>, binder 0x00AE67D0). So 0x00517850 is another
  tree instance's destructor and that table line is wrong: queued (NAME_FIXES).
- q6197: undecided set vs _Rb_tree copy-ctor spelling (see APPLIED); needs a coordinator decision. 0x00513700 vs 0x00AD3A10 are two 4-byte-key tree copy ctors (_M_copy 0x005114F0 vs 0x00AD3230).
- q6201: Remove -> Kill (FableWin.pdb only + body + sole caller): in place, the row keeps its HEAD-local std / COptimisedVertex
  models (the shared COptimisedVertex has PairContractions as raw bytes, so the row cannot move onto it yet). Pre-existing: the
  row's local std models bind 0x004CF810 with a `V?$less` class key (STLport is `U?$less`), a fourth spelling there; unchanged.
- q6209: self signature respelled to the PDB reference form (both PDBs); also its 0x00993E80 call WriteUShort -> WriteUWORD
  (engine/CDataOutputStream.h spelling); 9 other rows still bind WriteUShort / WriteUSHORT (not touched).
- q6243: header opt-in FABLE_RETAIL_C3DCLOTHPRIMITIVE_VIRTUAL_DTOR. The Reset row 0x00A94DF0 now instantiates DeleteData pick-any:
  byte-identical to the new owner 0x00A93A60 (cmpbody). The queue cited Ego_r for DeleteData; the name set has it in FableWin.pdb
  0x03306EA0 (corrected in the row).
- q6284: GFWrapLimit<long,int,int> self row re-authored as an explicit instantiation of 0x004963A0's template text (no shared
  header has GFWrapLimit); the COMDAT is LINK_ONCE_DISCARD and byte-identical to 0x004963A0's noinline copy. Test rewritten: it
  included the row file by path and called the old name.
- q6288..q6290: header opt-in FABLE_RETAIL_CFRONTENDGAMECOMPONENT_GETSAVENAMES. 0x00495FE0 stays a HEAD-local model of
  CFrontendGameComponent (in-place respell: member names, two static members, the private EGameMode SetMode); its new empty
  local `enum EGameMode { };` matches landed 0x004963A0 (no header has EGameMode). 0x00496070's local STLport vector stand-ins
  (CWideStringAllocator / ...Vector, no shared header classes) were replaced by stlport/fable_containers.h; its pick-any
  ~vector<CWideString> is byte-identical to owner 0x00412130.
- q6296: claim rejected (HasInputFocus is in no PDB; both say IsApplicationInputFocus, the self Leaf). Six binders respelled in
  their local models. **Needs header claim:** engine/CSystemManager.h FABLE_RETAIL_CSYSTEMMANAGER_FOCUS declares `bool HasInputFocus();`
  and engine/CJoystickDX.h's inline UpdateAcquired calls it; renaming both to IsApplicationInputFocus unblocks 0x00AB6E40.
- q6437/q6442: header opt-ins FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFADEOUT and
  FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_DELETERADIALBLURENTRY. 0x00890150 is re-authored on the landed sibling 0x00890200's text,
  which reaches the interpolator through `(const SelfOverlay*)this` overlay structs (a cast of `this` to header-less overlay
  structs; precedent 0x00890200, reported here as asked). The 0x006B82E0 test reinterpret_casts a static buffer to the real
  interpolator class (test only).
- q6197: needs a coordinator decision (set vs _Rb_tree copy-ctor spelling; see APPLIED).
- q6250: the block-scope id is not settleable from the PDBs (23 vs 50).

## Self renames of rows outside my entries (both PDBs or PDB + body; exe scan: no landed referrer of the old symbol)
- 0x00687210 `_unattributed::~CAggregate_00687210` -> `CMessageEvent::~CMessageEvent` (class + leaf).
- 0x005DB1E0 leaf-only `?TransferBinaryOut@S...` -> `?TransferBinaryOut@CAnimComponentFlags@@UBEXAAVCDataOutputStream@@@Z`.
- 0x00A94DF0 `CCountedPointer_00a93a60::Reset` -> `CCountedPointer<C3DClothPrimitive>::Reset`.
- 0x00890150 leaf-only -> `?RadialBlurFadeOut@CGameScriptInterface@@UBEXMPAX@Z`.
- 0x00495FE0 / 0x00AAD150 / 0x00496EB0 / 0x00495830 / 0x009E41D0 / 0x006B82E0 / 0x00A93A60 / 0x00AD4A70 / 0x00A55A30 / 0x00AD71B0 are
  entry self rows (RENAMES section 1). Catalog Source file names are unchanged (no row file renamed); no EH funclet or other row
  includes any changed row file (grep of src/compiled + tests; coordinator/funclet_includes_check.py is not in the tree).

## Descriptive names
- None added. All new spellings are PDB symbols (or the STLport header spelling for the 0x004CF810 _M_erase / tree members).

## Flags lines
- None added or changed.

## Tests rewritten
- 0x00A55A30 (on the shared box headers), 0x00AD71B0 (method name in place), 0x00496EB0 (declares the template, links the row),
  0x006B82E0 (on the shared interpolator header; stubbed DeleteRadialBlurEntry). PassPatterns kept.

## Queued debt (found, not changed)
- reloc_bindings.tsv line `??1?$_Rb_tree@KKU?$_Identity@K@std@@U?$less@K@2@V?$allocator@K@2@@std@@QAE@XZ 00517850 0051c920` is wrong:
  0x00517850's tree dtor erases through 0x004A9CB0, set<unsigned long>'s _M_erase is 0x004CF810 (see q6196). Its real tree type is
  unproven here (moved out of NAME_FIXES: no correct symbol to give).
- Landed 0x00A91640 (C3DPrimitive2::InitialiseFromMeshFileData, FABLE_RETAIL_C3DCLOTHPRIMITIVE_CTOR) defines a pick-any copy of
  ?DeleteData@?$CCountedPointer@VC3DClothPrimitive@@@@SIXPAX@Z that differs from the new owner 0x00A93A60 (23 v 11 bytes, cmpbody):
  pre-existing at HEAD (the address had no owner spelling before). Converging needs the _CTOR block to declare the destructor
  virtual (header decision).
- reloc_bindings.tsv has no line for ?IsApplicationInputFocus@CSystemManager@@QAE_NXZ (one witness per symbol; 0x00AB4830 binds it);
  the landing's rebind adds it.

## New local models (coordinator decision asked)
- 0x00890150 now defines `Field18Overlay` / `Field04Overlay` / `SelfOverlay` (header-less overlay structs for the world chain, copied
  from the landed sibling 0x00890200, no shared base) and casts `this` to `const SelfOverlay*`. The brief's three-condition rule
  (base from a shared header) is not met as written. The row can be dropped alone: the 0x006B82E0 rename does not depend on it.

## vland WARN lines in recheck_final.log (all explained)
- SHARED_HEADER_AVAILABLE on 00495fe0 00496070 00614620 00687730 00aad150 00ab4910 00ab6100 00ab6240 00ab6450 00ab69f0 00ab6d60:
  local models present at HEAD (selfcheck: no new local class).
- STANDIN_LEAF / RELOC_NAME against catalog labels on 00495830 00495fe0 00687210 006b82e0 00890150 00a94df0: label lag, the RENAMES
  section 1 lines fix the labels.
- RELOC_NAME on 00496070 (GetDefaultProfileName), 00ab6240 (CKeyboardDX::EmptyEventStore, SetAsKeyboardKeyPressed, __chkstk) and
  00ab69f0 (UpdateEvents): other callees' labels, unchanged from HEAD.

## Extra locks claimed (outside rows.txt)
- 00730f90 00890150 00ab4910 00ab6240 00ab6450 (was `released`) 00ab69f0 00ab6d60.

## Decisions for the coordinator
1. q6197: which STLport spelling (set:: or _Rb_tree:: copy constructor) 0x00AD3A10 takes.
2. Self renames at 0x00A55A30, 0x00AD4A70 and 0x006B82E0 leave one or two referrers on stand-in spellings (0x00730F90; 0x00AD5C30 /
   0x00AD7E80; 0x0079C750 / 0x007AB190). Each relies on "a self rename lands if no landed row binds the OLD self leaf" (none does;
   the address's spelling count shrinks). The brief's stricter sentence ("every REFS row must end on the new spelling") would hold them.
3. 0x00687210: a class-level self rename (`_unattributed::~CAggregate_00687210` -> `CMessageEvent::~CMessageEvent`) of a row outside
   my entries, done to move a q5052 binder.
4. Header claim: rename `HasInputFocus` -> `IsApplicationInputFocus` in engine/CSystemManager.h (FOCUS block) and engine/CJoystickDX.h
   (UpdateAcquired body); unblocks 0x00AB6E40.
5. 0x00890150's overlay structs (above).

## Rule slips
- GOTCHAS.md and EXTRA_RULES.md were read by section and grep (recent sections in full), not end to end; NF92 NOTES.md, ASM_RULES.md and
  the land1119 batch READMEs were not read.
- The 0x00730F90 form-1 grade ran four times (output plumbing while viewing the diff); its grades_used of 2 counts forms, not compiles.
- Two Bash calls exited non-zero (a `cat` of a missing lock file; a header listing ending in a failed `ls`).
