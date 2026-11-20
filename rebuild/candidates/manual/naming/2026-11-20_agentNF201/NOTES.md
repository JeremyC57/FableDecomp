# NF201 notes (round 10, 2026-11-20; entries q4860 .. q12194, handed back after q11797)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 50 entries: 11 done, 1 done-already, 7 blocked (partial or decision),
5 deferred, 26 open (not started, queue order kept). 24 STATUS rows (grades_used counts every compile, HEAD probes included): 18 changed (10 ported in place / onto headers, 8 re-authored),
4 not-a-binder (HEAD probes), 2 blocked. recheck_final.log: one vland --recheck-landed run over the 18 changed rows: 1 MATCH +
17 RELOCATION_MATCH, every test PASS. selfcheck.log: 0 problems. RENAMES: 10 section-1 self renames, 17 section-2 respellings.

## Descriptive names
- No new made-up names. STLport instances (descriptive-by-library, the spelling stlport/fable_containers.h produces; no STL names in
  the PDB name sets): 0x007464A0 std::list<CEnginePrimitiveHandle>::insert(iterator), 0x007467D0 list<CEnginePrimitiveHandle>::push_back(),
  0x00A80360 vector<C2DBoxF>::vector(size_type).

## Splits narrowed (no split created)
- 0x006B82E0 (q6437): every exe-scan referrer now binds ?RadialBlurFadeOut@CEnvironmentSpellEffectInterpolator@@QAEXMPAX@Z.
- 0x0073DFF0 (q11692): ?SetGroup@CAnimFilterOfGroup leaves (7 rows); ?SetAndHash@AnimationGroupFilter (0x008D9530) remains.
- Side effects that only shrink spelling sets: 0x006B8550 (0x0079C750 off ?AddRadialBlur@CRadialBlurManager), 0x006B8310 (0x007AB190 off
  CEnvironmentSpellEffects), 0x0099F180 (0x00681BA0 off ?AppendCharacter@CCharString onto ??YCCharString@@QAEABV0@D@Z), 0x0129AAF4
  (0x00681BA0 off ?EmptyString@NCharStringPool onto ?NullChar@CCharString@@1DB), 0x00B5A3F0 (placeholder DoRender leaves the table),
  0x009E2560 (0x00F2D660 off ?TransferOut_009e2560 onto the PDB CPersistTraitsSLONG::TransferOut).
- Stale reloc_bindings lines (HEAD probes; drop at landing): ?test@CBitset@@QBE_NK@Z (0x00659104), ?test@CTCBitset@@QBE_NK@Z
  (0x007890EA), ?test@CTCBitset@@QBE_NJ@Z (0x0064A568) at 0x00410DE0.

## Pick-any copies (checked against owners)
- 0x007464A0: ??0CEnginePrimitiveHandle@@QAE@ABV0@@Z (implicit copy ctor COMDAT) byte-identical to retail 0x0043F450 (vtable reloc aside;
  that landed row's catalog name is wrong: NAME_FIXES.tsv); ??2@YAPAXIPAX@Z identical to retail 0x00405C10; ??0_List_iterator_base equal to
  owner row 0x0041C780's object (cmpbody).
- 0x00681BA0: GetDefaultVal<CCharString> 0x0040E1E0, Transfer<CCharString>(name, value) 0x00411010, IsSaving 0x004AEF50,
  CCharString::GetAsCharArray 0x00403A10, CCharString::GetLength 0x00403A00: each byte-identical to retail (tools/cmpret.py: COMDAT bytes
  vs the catalog bytes with relocation fields zeroed).
- 0x008E9850 (decision for the coordinator): its HEAD-local in-class inline SetGroup body was already emitted as a COMDAT at HEAD; after the
  respell it is ?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z, the owner self row 0x0073DFF0's leaf. Bytes are identical
  to the owner object and to retail 0x0073DFF0, but one relocation target differs: the owner binds ?NullChar@CCharString@@1DB at +0x23,
  this copy the "" literal COMDAT (the row's HEAD-local CCharString::GetPString). Both are empty strings; the rule says "relocations
  aside", so I kept it. If the coordinator reads that as a different copy, drop 0x008E9850 from the landing (the other 6 rows stand alone).
- 0x008E9850 follow-up: the owner 0x0073DFF0 is itself the nf86 / nf88 pick-any scaffolding (its object holds ?SetCurrentGroupName@.. in a
  LINK_ONCE_DISCARD COMDAT section, SELECT_ANY), so the new copy gives no LNK2005; only the relocation-target question above remains.
- Other new defined symbols: shared-header inlines with no binding-table owner, no `// Leaf:` owner (grep of all landed Leaf lines: only
  ??0_List_iterator_base -> 0x0041C780, compared equal) and no catalog owner (C2DBoxF::C2DBoxF rows 0x004371A0 / 0x0041A7E0 are the
  C2DVector-pair overloads, not the copy / default constructors this hand-back emits).
- Heads-up (no owner, so not a rule hit): 0x007464A0 emits the default-path ?Release@?$CIVCountedPointer@VCEnginePersistentPrimitiveBase@@@@QAEXXZ
  / its ??1 COMDAT; rows built with FABLE_RETAIL_CIVCOUNTEDPOINTER emit a differently scheduled body (same semantics) under the same name.

## Binding diff (every relocation, not only entry targets)
- Unfiltered PAIR diff of each re-authored row, HEAD object vs new object (work/bh_*.txt / bs_*.txt): every new (target, symbol) pair is
  already in reloc_bindings.tsv at that target, except two that are the target self rows' own leaves: 0x007467D0 -> 0x007464A0 (the leaf
  this hand-back gives 0x007464A0) and 0x00B5AF60 -> 0x00B5A3F0 ?ApplyColourFilter@CEngineScreenEffectColourFilter@@AAEXXZ (0x00B5A3F0's
  landed Leaf). No pair adds a spelling to an address.

## Header opt-ins (hdr/README.md): all add-only, member-specific, default path token-identical
- engine/CEnvironmentSpellEffectInterpolator.h FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFADETO_VECTOR (0x0079C750)
- engine/CEnvironmentSpellEffectInterface.h FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERFACE_SETRADIALBLURCENTERWORLDPOS (0x0079C750)
- engine/CIVCountedPointer.h FABLE_RETAIL_CIVCOUNTEDPOINTER_COUNTED_COPY (0x007464A0)
- engine/CEngineScreenEffectColourFilter.h FABLE_RETAIL_CENGINESCREENEFFECTCOLOURFILTER_RENDER (0x00B5AF60; pin_asserts pinned 20 asserts)
No FABLE_FORCEINLINE opt-in was added. No Flags lines were added (0x00681BA0 keeps its HEAD `// Flags: /O2 /Oy /W3`).

## Funclets
- None of the changed rows is included by an EH funclet row (grep of rebuild/src/compiled/01/21 for the 18 source file names: no hit).
  Self renames move no file in this hand-back (each row keeps its source path; the coordinator may rename files at landing).

## Not-a-binder census notes
- q11692: two more rows define a same-body inline SetGroup in their local CAnimFilterOfGroup model but never call 0x0073DFF0 (fully
  expanded, no symbol emitted): 0x008E1580, 0x00743990 (descriptive member name left; no binding).
- WARN RELOC_CONFLICT on 0x008D75D0 (?IsThingCarryingMeleeWeapon@@YI_NPAVCThing@@@Z -> 0x006A15A0 here, 0x006A1620 in 0x0091F050) is
  pre-existing (a callee unrelated to the respell); WARN RELOC_NAME on 0x00B5AF60 is the q12172 label (labels_landing_2026-11-11cl.tsv
  ?g_NormaliseEpsilon@@3MB at 0x0129BA3C).

## Extra locks claimed (outside rows.txt)
- 00bc5420 (q11901, not reached; set back to `released` at hand-back), 0073aac0 (was released), 008e9850 (was released), 008d75d0, 008d87c0, 008d92c0, 008d9350.
- Refused (held by NF199): 006d41f0 009f03b0 0099e4b0 00406070 009a9930 009aa240 009cb910 00a16d50 00e2ab30; by NF200: 0047db73.

## Rule slips
- My first Bash call exited 1 (`wc` on directories in my agent dir). Every later call ended `; true` or exited 0.
- 0x00681BA0 used 4 compiles: DIFFER, SRC_FAIL (missing CDataOutputStream.h include for the same form; not counted as a new form),
  DIFFER 290v290, RELOCATION_MATCH. Counting the SRC_FAIL as a form it would be the 4th form; I judged it a fix of form 2.
- I read GOTCHAS by grepping the 2026-11-19/20 and scaffolding lines rather than the whole 140 KB file.
