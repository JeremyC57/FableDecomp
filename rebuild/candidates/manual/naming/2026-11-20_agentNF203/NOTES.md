# NF203 notes (round 10, 2026-11-20)

## q11724 0x005DCF70 (CAnimationSet::Remove port, blocked)
- Identity: CAnimationSet::Remove (NAME_FIXES). Ego_d.pdb holds no locals for it (ghidra_out/egod_*: no Remove@CAnimationSet line), so the
  loop wording is unsourced.
- Draft: work/r1/005dcf70_form1.cpp on fable_string.h + engine/CAnimationEntry.h + engine/CAnimationSet.h with a draft opt-in
  FABLE_RETAIL_CANIMATIONSET_REMOVE (work/r1/hdr/engine/CAnimationSet.h: declares Remove, an inline GetDefaultGroupName with a named
  CCharString local returned (retail copies: GetString 0x009D49B0 into one temporary, copy ctor 0x0099EC30, dtor 0x0099EAE0), and swaps the
  opaque DefaultGroup[4] for the Ego_r.pdb CDefString under the opt-in). Plan only: not handed back (no finished row proves it).
- Grades: form 1 erase(it++)/++it DIFFER(299v273); form 2 for(;;++it)+erase(it) DIFFER(288v273); form 3 named bool = 299 (size only).
  Two defects: (a) VC7.1 keeps it+1 as a second induction variable (frame 0x10 v 0xC); (b) destroy shape: retail stores the null only
  inside the null test (0x005DD062), the default ~CIVCountedPointer stores it on the common path. Next form (coordinator decision, over the
  3-form budget): define FABLE_RETAIL_CIVCOUNTEDPOINTER (Release clears inside its null test; as the landed same-TU __copy owner 0x005E18B0 /
  pair destroy 0x00454404 expand it), then re-check (a).
- Pick-any check for the eventual port: the object will emit a COMDAT copy of
  ??$__copy@PAU?$pair@KVCAnimationEntry@@@std@@PAU12@H@std@@YIPAU?$pair@KVCAnimationEntry@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
  only if the header's __copy is not declaration-only; its owner 0x005E18B0 is landed (00/5e/std_copy_005e18b0.cpp): the copy must pass
  cmpbody.py against that object or stay declaration-only (the 0x005E39E0 row declares it only).
- Test: the landed test is a standalone NErase005dcf70 host model; a port must rewrite it on the shared headers (GetAnim test pattern).

## q11902 0x00993B20 (ported) -- pick-any decision for the coordinator
- The port replaces the stand-in hierarchy with engine/CDataInputStream.h. Two compiler-generated COMDATs the HEAD object already defined
  change body: ??_GCDataStream@@UAEPAXI@Z and ??_GCDataInputStream@@UAEPAXI@Z now call the header's out-of-line ~CDataStream (0x00993B10)
  instead of expanding the stand-in's inline `virtual ~CDataStream() {}`. The new bodies are the header shape every landed
  CMemoryDataInputStream user of FABLE_RETAIL_CDATAINPUTSTREAM emits (about 40 rows); the HEAD shape matches owners 0x00996EA0 / 0x00996ED0
  (header-less local CDataStream rows). No reloc_bindings line binds either symbol. Not newly defined, so not the pick-any rule's trigger,
  but flagged: hold the row if the coordinator counts a changed body as a new copy.
- New pick-any copy ?GetPosition@CDataInputStream@@UBEKXZ: byte-identical to the owner 0x00405FA0 (8b4104 c3, by hand: the owner row defines
  only its stand-in ?GetPosition@CDataInputStream_Methods@@QAEKXZ).

## q12017 0x00696340 (done)
- In-place respell inside the row's own std::_Rb_tree model (member template + explicit member instantiation; the class
  instantiation line kept). objdump -t diff v HEAD: only the leaf. Descriptive-by-library (STLport D1 spelling).

## q12018 0x00696900 family (done)
- Three self renames of unreferenced COMDAT rows, all descriptive-by-library STLport spellings on stlport/fable_map.h:
  0x00696AF0 _Rb_tree::clear (was the July CTCCarryable_OnKill stand-in), 0x00696D10 ~_Rb_tree, 0x00696E20 ~map (both were generated
  CRetailTree_erase00696900 dtors). Tree-vs-map choice from the COMDAT run order (NOTES in each row header).
- vland prints WARN STANDIN_LEAF for 0x00696D10 / 0x00696E20 because the catalog still names CRetailTree_erase00696900: RENAMES s1 fixes it.
- Pick-any (coordinator rule update 2026-11-20 applied: bytes AND relocs): every copy compared with cmpbody (STATUS). Also re-checked
  0x00993B20's GetPosition copy under the new rule: neither the copy nor the owner 0x00405FA0 body has a relocation (8b4104 c3).
- Self renames move the rows' files (vland names them from the decorated name); no EH funclets include these rows.

## Not reached (open): q12020 onward in queue order (APPLIED `open`). Context handed back.
