# e14 notes (egod IDENTIFY batch)

## 0x00442D50 -> NPlayerGui::CDrawDamage::`scalar deleting destructor' (RELOCATION_MATCH, 2 grades)
- Not an EH funclet: a plain `ret 4` thiscall, the non-virtual ??_G (flag bit 0 -> operator delete 0x00BFE9BC; returns this).
- Identity: Ego_d player_gui.obj ??_G run (Damage before HighlightingTargetingCursor 0x00442E60 landed, MeleeActionButtonEffects
  0x00443010, NPCName) + the +0x28 member dtor 0x0043FCF0 = reloc_bindings _List_base<SDamageValue>::~_List_base (Ego_r
  CDrawDamage::DamageValues) + body identical to the landed CCountedPointer<CDrawDamage>::DeleteData 0x00443840 minus the flag test.
- Match: shared headers only (CDrawBase HANDLES + IMPLICIT_DTOR opt-ins, NPlayerGui_CDrawDamage.h, CCountedPointer.h); the real retail
  explicit instantiation `template void CCountedPointer<NPlayerGui::CDrawDamage>::DeleteData(void*)` emits the ??_G COMDAT. No overlay.
## 0x004430F0 -> NPlayerGui::CDrawNPCName::`scalar deleting destructor' (RELOCATION_MATCH, 2 grades)
- Same recipe; +0x28 member dtor CWideString::~CWideString 0x0099B510 = Ego_r CDrawNPCName::Name; twin of DeleteData 0x00443B60.
## 0x0072AAF0 -> CTCSpecialAbilities::SetupMultiStrike (RELOCATION_MATCH, 2 grades)
- Identity: Ego_d order after SetupMultiArrow 0x0072AAA0 + Ego_r name table (0x007418C0 after SetupMultiArrow) + "CTCMultiStrike" string.
- Match: the landed twin SetupMultiArrow's text on shared headers with the landed UseSpecialAbility 0x0072CF40 opt-in set (no overlay).
  The old "AddTC receiver load" residue was a local-model artefact.
## 0x0051EA30 -> CThingManager::FillRenderListsWithAllEntities (RELOCATION_MATCH, 3 grades)
- Identity: Ego_d order + Ego_r name table (both directly before SetUpdateAndRenderAllFlag, retail 0x0051EA90 = sole caller).
- Match: shared headers + two NEW overlay opt-ins (hdr/README.md) for the inline PDB accessors; Ego_d locals c1/name/list/it/pthing in
  their scopes (incl. the plain block around `list`). First compiling grade matched: the draft's "this vs base in ecx" register residue was
  a local-model artefact. Callee 0x004C7590 = CThing::SetAsRenderedThisFrame(unsigned long) (reloc_bindings + CThing.h).
- `name` is an unused Ego_d local (fwsize: FableWin FillRender is 108 bytes longer at /Od: debug-only use). Fidelity line in the source.
## 0x006B5890 -> std::__lower_bound<pair<long,float>*, pair<long,float>, less<pair<long,float> >, int> (OPEN, DIFFER 76v76, 6 grades)
- Identity: reloc_bindings decorated name (landed caller 0x006B5A50) + Ego_r/FableWin Dinkumware twin _Lower_bound<pair<long,float>*,...>
  (Ego_r 0x006F1B30, environment.obj next to SetWeightedThemes). Real object environment.obj, not tc_mapwho.obj (ref egod_tu is the wrong TU).
- Residue (only): retail keeps val.second on the x87 stack across the loop (`fld [eax+4]` at 0x006B58A0 before the loop, `fcom [ecx+edx*8+4]`
  + `test ah,41h ; jne` at 0x006B58B3, `fstp st(0)` at exit 0x006B58D3); ours `fld [middle+4] ; fcomp [ebx+4] ; test ah,5 ; jp` per pass.
- Tried: shared STLport body + operator< (6 hunks, the old residue), /Oa, /G6, /G7, /Op (no change). Offline cl probes (tmp/x87/): VC7.1 RTM
  never hoists the float here -- not with val by value, a local float copy, a local pair copy, or `val.second > m->second` (that gives
  the retail `test ah,41h` but still per-iteration `fld`). So no source spelling found that enregisters it; looks like a compiler-version /
  codegen gap (QFE 4035 tried by an earlier pass: same). Coordinator: candidate for the x87 lane-gap list.
## 0x009D8650 -> MicroThread::MicroThreadSleep() (OPEN: asm-only, owner decision; 1 grade)
- Identity: Ego_d lib_microthread.obj order + FableWin / Ego_r name tables (Sleep() / Sleep(int) / MicroThreadReturn = retail 0x009D8650 /
  0x009D8680 / 0x009D8690) + landed caller SwitchToMicroThread 0x009D87F0. Ego_d /Od length 36 == retail 36 -> the original was naked asm.
- Not C++: it writes esp (`mov esp,[0x013CA898]` 0x009D865D, `mov [0x013CA898],eax` 0x009D8663) and fs:[0]. No intrinsic writes esp.
- Bytes proof: probe/asmhdr/engine/MicroThreadStackSwitch.h (a macro defining the naked function) + probe/f1.cpp = RELOCATION_MATCH.
  NOT handed back as a match: ASM_RULES 3.2 needs a semantically identical FABLE_PORTABLE twin (impossible for a stack switch; a portable
  port would need a different MicroThread design, e.g. fibers/ucontext) and the row would get its `naked` only through a macro (new pattern).
  Coordinator / owner: accept as retail_asm with a "no portable twin" exception, or keep parked.

## Follow-ups after review
- Rows 0x00442D50 / 0x004430F0: the same sources also grade RELOCATION_MATCH at the instantiated helpers' own addresses,
  CCountedPointer<CDrawDamage>::DeleteData 0x00443840 and CCountedPointer<CDrawNPCName>::DeleteData 0x00443B60 (both landed on LOCAL
  models): the coordinator can move those two landed rows to these shared-header sources.
- vland grades rows 0x00442D50 / 0x004430F0 by "largest function" (the leaf "`scalar deleting destructor'" names no symbol): the ??_G is
  78 bytes, the DeleteData in the same object 76 -- a 2-byte margin; the precedent row 0x00442E60 has the same name form.
- vland RELOC_NAME warn on 0x00442D50: 0x0043FCF0 is labelled `_global::StdList_Clear_0043fcf0`; reloc_bindings + the body say it is the
  STLport _List_base<CDrawDamage::SDamageValue> destructor (NAME_FIXES).
- 0x006B5890: Ego_d environment.obj confirms std::lower_bound<vector<pair<long,float>>::iterator, pair<long,float>> ->
  std::_Lower_bound<pair<long,float>*, pair<long,float>, int> with the plain std::operator< <long,float> (no custom comparator), so the
  source is right and the residue is codegen. Stopped (no further spelling / flag grades).
- 0x009D8650: the ASM_RULES rule-4 attempt table (30 C++ + 10 intrinsic spellings) was NOT done -- no C++ or intrinsic can write esp; it
  needs an owner waiver together with the missing FABLE_PORTABLE twin before the helper can be accepted.
- reloc_bindings already binds all four matched / both open rows' retail addresses to the spellings used here (no conflicting spellings).
