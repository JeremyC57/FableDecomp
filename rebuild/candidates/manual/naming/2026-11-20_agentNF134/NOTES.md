# NF134 notes (2026-11-20, round 10, PRIORITY slice 9 third pass)

Hand-back on context. Counts come from APPLIED.tsv / STATUS.tsv / recheck_final.log.
- Entries (9, incl. q6180 / q6181 added by the coordinator): done 1 (q4650); done except 0x009B08C0 2 (q6180, q6181; 0x009B08C0 stays
  with NF133); blocked 1 (q6182); open 5 (q5021 partial: 2 rows left; q4993, q4994 / q5140, q5030 not started). Rejected 0, deferred 0.
- Rows: 14 ported and handed back in rows/ (all RELOCATION_MATCH + PASS: recheck_final.log, one vland run, 14 of 14). Blocked 4 (STATUS):
  0x00956FE0, 0x009551A0 (q5021 ports not attempted), 0x009FF590 (q6182 port), 0x00994930 (self rename waits). No test rewritten.

## Descriptive names
- No new made-up names. Every name applied is a PDB spelling (Ego_r.pdb + FableWin.pdb), or the spelling a landed self row already defines
  (Beetle ctors, NAME_FIXES).

## q4650 (done)
- 0x00B5E700: row-local CShaderEnableState / CShaderRenderManager / CVertexShaderConstantLayout / CArray models dropped for
  engine/CShaderRenderManager.h + the shared CScopedShadersEnable / CScopedVertexShaderConstantLayout (as the landed twins 0x00B2B180 / 0x00B5E0F0).
  Other callee spellings moved with it onto spellings already in each address set (0x013BC470 SingletonInstance, Enable/Disable shaders,
  ApplyPixelShader ABV, ResetWorldTransform QAE). The ??0/??1 CScopedShadersEnable COMDATs are pick-any (as in every includer of the header).
  The members-mode header form (no casts) fails C2248: the self is the _Methods stand-in and the retail members are private (NAME_FIXES 0x00B5E700).
- 0x0098B5E0: self re-authored on engine/CShaderRenderManager.h with FABLE_RETAIL_CSHADERCONSTANTRANGE_IMPLICIT_COPY, FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
  and the new overlay opt-in FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT_GETLIGHTARRAY_INLINE. Leaf-only rename (catalog already right). Its callees
  moved off View stand-ins onto PDB spellings (RENAMES s2); the spelling count of each address does not grow (0x0098B420 AAE vs its self row's
  QAE and 0x00987D70 vs its CPointerPair placeholder self row are NAME_FIXES). After this hand-back all 17 REFS rows bind the PDB spelling.
- The test of 0x0098B5E0 is a standalone verbatim copy of the old stand-in body: unchanged, still PASS.

## q5021 (partial)
- In place on the row-local CAIStateGroupBase (declaration moved from the derived stand-in, protected, const&, EAIActionReturn from
  engine/fable_ai_action_enums.h; temporaries passed by reference): 0x00967A20, 0x0093A5C0, 0x0093A8C0, 0x00957A20, 0x00956380 (its own local
  EAIActionReturn line moved above the base unchanged); 0x00951B20 access only.
- Ported onto the shared headers (template landed 0x009208E0): 0x00920730, 0x0091F0D0, 0x009211D0 (N_0062e900 namespaced stand-in gone;
  CAIStateGroupBase.h PERFORMACTION + CCombatActionBase.h). 0x00920730's action is really CCreatureAction_UndeadReturnToEarth (0x008EC6A0 catalog
  + landed binder 0x0074ACE0); its stand-in had called it GoUnderground (pointer form).
- Leaf-only self renames outside the entries (both PDBs agree, exe scan: no landed referrer): 0x009211D0, 0x00920730, 0x0091F0D0 Update UAE -> MAE
  (protected virtual). 0x009211D0 now uses the shared engine/CAIStateGroup_BeetleToggleHover.h (its row-local struct is gone).
- Local models kept (decision for the coordinator): 0x00920730 / 0x0091F0D0 keep their HEAD-defined derived state-group class (same token; base
  token CAIStateGroupBase now resolves to the shared header instead of the row-local model). No header exists for CAIStateGroup_CreatureGoUnderground;
  CAIStateGroup_CreatureRangedCombatUseWillShield.h is only a flattened generated blob (no retail mode). 0x009211D0 keeps its HEAD-defined
  CCreatureAction_BeetleLand / _BeetleTakeOff (no header exists), base changed from the namespaced stand-in to the shared CCombatActionBase (RTTI);
  this emits the implicit pick-any ??1CCreatureAction_BeetleLand@@UAE@XZ / ??1CCreatureAction_BeetleTakeOff@@UAE@XZ. Same pattern as landed 0x009208E0.
- Left: 0x00956FE0 (same port as 0x009211D0; BriarRoseForcePush has no header, ShotFarInto has one), 0x009551A0 (Retail-suffixed stand-in chain onto
  CCombatActionBase.h / ThrowShotObject.h / PlayCombatAnimation.h). Not attempted (context).

## q6180 / q6181 (done except 0x009B08C0)
- 0x009D0380 / 0x009D4C30 ported onto the shared engine/CAFile.h / CDiskFileWin32.h (DEFAULTCTOR) / CDataInputStream.h / CFileDataInputStream.h,
  no local re-base (coordinator acceptance test). Defined-symbol diff, new object vs HEAD object (objdump -t): new only ??0CAFile@@QAE@XZ,
  ??1CAFile@@UAE@XZ, ??_7CAFile@@6B@, ??_GCAFile@@UAEPAXI@Z, ?GetLength / ?GetPosition@CDiskFileWin32@@UBEJXZ, all LINK_ONCE_DISCARD COMDATs from the
  shared headers; the landed shared-header row 0x00B3C5D0 defines exactly the same CAFile / CDiskFileWin32 set (compiled to check). Dropped:
  ?IsValidPosition@CDataInputStream (0x009D0380). The only non-COMDAT symbol in each object is its own leaf.

## q6182 (blocked) -- decision for the coordinator
- Ported 0x00A62120 (PAXK -> PBXK, in place). With nf131 landed, the graded REFS binders are on PBXK except 0x009FF590 (blocked: port onto
  engine/CGraphicDataBank.h, see STATUS) and old no-Leaf rows on candidates headers (checked by their #include lines): 0x00BDC510 includes
  candidates/landscape_map_update_foreground_compiled.h (declares (void*, unsigned long): PAXK); 0x00BE8920 includes
  candidates/landscape_background_patch_compressed.h ((unsigned char*, unsigned long): PAEK); 0x00BE7D70 includes
  candidates/landscape_background_patch_load_compiled.h, which declares no CMemoryDataInputStream constructor (its spelling is unverified);
  0x00BF3B60 includes candidates/landscape_patch_load_foreground_compiled.h (PBXK). "On PBXK" for the other binders rests on a source
  re-scan after the nf131 landing (src.py -g over REFS: every declaration is (const void*, unsigned long); the (void*, long) hits are comments)
  plus NF131's probe. Those rows DIFFER under q.py by design (nf127 probe), so a candidates-header opt-in cannot be graded.
- Question: may the 0x00994930 self rename (PBEJ -> PBXK, NF131 draft RELOCATION_MATCH) land while those old rows keep their spellings? It only
  removes a spelling (PBEJ: no landed binder left after nf131); it cannot meet "every REFS row on the new spelling" this round.

## Header opt-ins (hdr/README.md)
- engine/CVertexShaderConstantLayout.h `FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT_GETLIGHTARRAY_INLINE` (0x0098B5E0). No shared-header member renamed
  (no "needs header claim" item).

## Flags lines
- None added.

## Funclets
- No EH funclet includes any changed row: a grep of rebuild/src for all 14 file names finds only the rows themselves (and one symbol name,
  not an include, in 0x00957B80). No file moves (all self renames are leaf-only).

## Defined-symbol diffs (objdump -t, new object vs HEAD object; work/defs_diff.txt, work/q4650/dH.txt / dS.txt)
- 0x00A62120, 0x00951B20, 0x00967A20, 0x0093A5C0, 0x0093A8C0, 0x00957A20, 0x00956380: empty.
- 0x009D0380 / 0x009D4C30: see q6180 (shared-header CAFile / CDiskFileWin32 COMDATs, the set landed 0x00B3C5D0 defines).
- 0x00920730 / 0x0091F0D0 / 0x009211D0: new = the renamed leaf, the shared-header inline GetCreature pair, and the action temporaries' implicit
  destructors now virtual (??1CCreatureAction_UndeadReturnToEarth / _CastPhysicalShieldSpell / _BeetleLand / _BeetleTakeOff@@UAE@XZ; at HEAD the
  same objects defined the non-virtual QAE forms). No reloc_bindings line, catalog row or landed source defines or binds any of them (grep);
  all pick-any COMDATs. No symbol naming the local state-group class beyond the leaf.
- 0x0098B5E0: new = the leaf; the CShaderConstantRange / CBaseClass implicit set (??0 copy / ??1 / ??_7 / ??_G), exactly what landed 0x00BCFB00
  (same IMPLICIT_COPY opt-in) defines (compiled to check); ?GetLightArray@CVertexShaderConstantLayout (the overlay inline; no catalog row or binding);
  the STLport vector<C4DVector> instantiations (resize, erase, begin/end/size, __copy, _Destroy ...) as pick-any COMDATs: resize is 0x0098DA90's
  explicit instantiation (also pick-any), the same out-of-line pattern as landed 0x00A62120. HEAD old leaf was ...SetLayoutView@@QAEXH@Z (int).
- 0x00B5E700: new = CScopedShadersEnable / CScopedVertexShaderConstantLayout / Get / Set*ShadersEnable / PeekVertexShaderConstantLayout and the
  CArray<CVertexShaderConstant> accessors, all shared-header inlines; dropped the local saver / CShaderEnableState inlines.
- 0x0098B5E0 keeps one cast: `const_cast<CVertexShaderConstantLayout*>(VertexShaderConstantLayouts[index])`; both sides are PDB types
  (CArray<const CVertexShaderConstantLayout*> element vs the non-const CurrentVSConstantLayout member), the nf105 source-faithful case.

## Locks
- Claimed: 0x00B5E700, 0x00A62120 (both read `released`), 0x00920730, 0x009211D0, 0x0093A8C0, 0x00957A20 (q5021), 0x009D0380 / 0x009D4C30 (given by
  the coordinator). Claimed but not changed: none beyond those listed blocked in STATUS.

## Decisions for the coordinator
1. q6182: the 0x00994930 self rename against the old no-Leaf candidates-header rows (above).
2. 0x00920730 / 0x0091F0D0: their HEAD-defined local state-group class now sits on the shared CAIStateGroupBase (same tokens; no header for
   CreatureGoUnderground, only a flattened blob for CreatureRangedCombatUseWillShield).
3. 0x009211D0: the row-local Beetle action classes now on the shared CCombatActionBase, emitting the virtual implicit destructors listed above.

## Rule slips
- One grade of 0x00B5E700 (members-mode variant) failed with C2248 (access), counted; no other failed compile.
- Order: q5021 was taken before q4994 / q4993 / q5030 on advice (cheaper per row); those three entries are untouched.
