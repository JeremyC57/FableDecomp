# NF112 notes (2026-11-20, round 10: PRIORITY slice 2 remainder, the 16 entries NF108 left)

Resumed once after a container restart (no hand-back files had been written then). Counts below come from APPLIED.tsv / STATUS.tsv.
Handed back early (context), as the brief asks.

**Entries (16): 1 done, 1 done-already, 1 rejected, 2 blocked, 0 deferred, 11 open.**
- done: q4769. done-already: q6230. rejected: q6210 (REJECTED.tsv).
- blocked: q5205 (family: 3 rows left, 0040f7e0 00816000 00816150), q6212 (1 Leaf-less old row left, 00644627).
- open: q4744 (self row ready and held, 11 binders need ports), q4975 q5002 q4454 q4525 q5080 q4645 q4447 (probe findings in
  APPLIED), q4449 q4524 q4452 (not started).

**Rows: 20 ported / respelled (rows/: 14 ports, 4 in-place respells, 2 self re-lands), 4 not-a-binder (0043f360 00542730 004bfb40 00bfc880: verified already right), 5 blocked** (STATUS.tsv: 0040f7e0 and 004c7a70 held self rows with
RELOCATION_MATCH drafts in work/; 00816000 after 2 forms; 00816150 family; 00644627 Leaf-less).
recheck_final.log: one vland --recheck-landed run over all 20 rows in rows/ (flags patched from `// Flags:` lines).

## Probe
work/probe.tsv: every landed REFS row of q5205 q6230 q4975 q4744 q6210 q6212 q5080 q4769 q5002 q4454 q4645 q4525 compiled once (144
rows) with the symbol it binds at the entry address. Used for every "already binds" statement in APPLIED.

## Per entry
- q6230: all 10 REFS rows (self included) already on the claim.
- q5205: claim holds (Ego_r + FableWin; header declares it). 005663c0 005dc8b0 005dca60 respelled in place inside their pre-existing
  local CPersistTraits templates (T* -> T&); 00686240's pre-existing same-TU copy of the owner text respelled in place and kept a
  strong definition (`// Fidelity: same-TU strong definition of 0x0040F7E0 (retail TU-mate)`, the nf96a wording; its symbol-map
  lookup is still the local Lookup(name, long*) model). These four leave the address with 2 spellings instead of 4 (no split created).
  The self row grades RELOCATION_MATCH on the shared headers (work/0040f7e0_ready.cpp, the header's
  FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY + a free scaffolding function returning its address) but is held for the family:
  00816000 / 00816150 (sphere / cylinder Init) give 0x0040F7E0 a stand-in body (`*value = ParseFloatItem_X(c, p)`) that cannot take
  the real name. The 00816000 port (work/d/00816000.cpp) is down to one 8-byte residue: the CPersistTraits<CDefString> temporary's
  DefaultVal constructor store before the opaque 0x007E3820 call; retail has none, as with the float temporary whose body is
  visible. So 0x007E3820's real body must be visible first, and its own row (007e3820) is a pointer-spelling local-model row
  (NAME_FIXES.tsv). 0x00816000's catalog name vfunc_4 is NHitLocations::CPhysicalPrimitiveSphere::Init (NAME_FIXES.tsv).
- q4769: all 12 callers already bind the claim; the two definition rows 0x0041CC14 / 0x0041CC2B re-landed as the header's inline
  static getters (FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS) emitted pick-any by a scaffolding function, `// Flags: /Os`.
  Data spellings: the ORIGINAL_DIMENSIONS block's private @@0 names (Ego_d.pdb PUB32 ?IsUsingRelativeCoords@...@@0_NA,
  ?BackBufferWidth@...@@0MA, ?DefaultBackBufferWidth@...@@0MA ...), the same as the landed 0x0052E470 / 0x0052E4D0 / 0x0054EF00. The
  data addresses were already split before this hand-back (exe scan + compile of every referencing row, work/probe2.tsv, rows
  outside the two self rows): 0x013B8768 @@0_NA x3 / @@2_NA x7 / ?g_bScreenSizeOverride@NUISystem@@3_NA x1; 0x013B876C @@0MA x2 /
  @@2MA x7 / ?g_OverrideScreenWidth@NUISystem@@3MA x1 / ?g_dimW@@3MA x1; 0x013B8770 the same with Height / g_dimH; 0x01375CD4
  ?DefaultBackBufferWidth@...@@0MA x2 / ?ScreenWidth@...@@2MA x6 / ?g_DisplayWidth@NUISystem@@3MA x1; 0x01375CD8 the Height
  twins. The self rows bound a further invented set (g_HeadAngVelMaxXY_*), so the hand-back removes spellings and creates none
  (RENAMES section 2 lists the six data respellings). The @@0 access rests on ONE source (Ego_d.pdb PUB32,
  ghidra_out/egod_carved_names.tsv); struct_layouts_egor.tsv records CUIScreenUnitConverter with no static members. Settling @@0 vs
  @@2 (and Default* vs Screen*) across the header's two opt-ins is a header item: needs header claim, unblocks the 7 INLINE rows.
- q6210: rejected as posed (vector<unsigned char> claim; the address is CArray<unsigned char>(unsigned int), see REJECTED.tsv). The
  last stand-in binder 00516d90 (CByteVector) ported onto engine/CArray.h like its twin 0x00516180; its own name is untouched.
- q6212: claim from the evidence column; 13 binders ported onto stlport/fable_algorithm.h (tools/fe.py transformer for the ten
  identical global-namespace SGI models, by hand for 0043b050 / 00645f8d / 0061b3c0). 00644627 is Leaf-less (left alone).
- q4744: self row ready (work/004c7a70_ready.cpp, RELOCATION_MATCH), held: 11 binders model the returned CMainGameComponent& as a
  CDisplayEngine* or a local CMainGameComponent and call members on it; each is a port onto engine/CMainGameComponent.h.

## RENAMES
Section 1: 0x0041CC14 / 0x0041CC2B (leaf only; the catalog names were already right). Section 2: the q5205 / q6212 / q6210
binder respellings (18 lines) plus the six data respellings of 0x0041CC14 / 0x0041CC2B (24 lines). No section-1 rename for 0x0040F7E0 or 0x004C7A70 (held).

## Flags lines (the coordinator must carry them into CompilerFlags)
- 0041cc14, 0041cc2b: `// Flags: /Os` (retail `cmp byte ptr [13B8768h],0`; without it DIFFER(23v23) `mov al ; test al`; the old rows
  had `#pragma optimize("s",on)`).

## Descriptive names
- q6212: the STLport std::for_each<CEnginePrimitiveHandle*, std::mem_fun_ref_t<void, CEnginePrimitiveHandle> > spelling is
  descriptive-by-library (FableWin.pdb has the Dinkumware-iterator instance of the same template; STLport's iterator is the pointer).
- Scaffolding helper names (anonymous namespace, never called): TransferInReference (held draft), GetBackBufferWidthReference,
  GetBackBufferHeightReference.
- No other made-up names.

## Coordinator decisions / pre-existing items
- 00686240 keeps a STRONG same-TU definition of ?TransferIn@?$CPersistTraits@M@@QAEXAAVCPersistContext@@AAVCStringParser@@AAM@Z, the
  symbol 0x009E81E0 (and the held 0040f7e0 draft) emit as a pick-any COMDAT. The nf96a form is used, but its condition (a register
  live across the call) was not measured here: the row's reason for the visible body is frame homing. Coordinator decision item.
- Pre-existing local models in handed-back rows (verifier WARN SHARED_HEADER_AVAILABLE, not added by me): 005dc8b0 / 005dca60 keep a
  local `class CStringParser : public CBaseClass` (base list), 005cac8d / 0061b3c0 local CTCInventoryWeapons / CTCInventoryQuests /
  CTCInventoryBase / CFadeSettingsObject with base lists, 00686240 local CPersistContext / CStringParser / CSymbolMap, and the
  CEnginePrimitiveHandle / CPersistTraits / CAFile models listed in recheck_final.log. If the new base-list rejection applies to
  pre-existing models, 005dc8b0 / 005dca60 / 005cac8d / 0061b3c0 need a port before they land. The two WARN RELOC_NAME lines
  (0061b3c0 -> 0x005BC4D8, 00645f8d -> 0x0065533C) are catalog labels of callees this hand-back does not touch.
- Locks: 00516d90 and 0043b050 read `released`; I took them (owner NF112).

## Header overlay
None (hdr/README.md). No opt-in added; all rows compile on the repo's rebuild/include.

## EH funclets
No landed funclet in rebuild/src/compiled/01/21 includes any changed row file (grep by file name).

## Rule slips
- My first q.py grade (0040f7e0) ran while my background probe was still compiling: two compiles at once, once.
- 00816000: two grades spun cl at 100% CPU on a header #error (engine/CDefString.h needs FABLE_RETAIL_CDEFSTRINGTABLE) because the
  precheck was not wired into my grade script; both were killed by timeout / by hand. tools/g.sh now runs a cpp precheck first.
- 00686240 was graded once in a form that made its same-TU copy `inline` (pick-any); reverted to the strong copy (nf96a) on advice.
