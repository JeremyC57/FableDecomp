# Chunk Q7 lessons (GOTCHAS-ready one-liners)

- (chunk Q7) Redundant float parentheses are a scheduling lever on one-slot x87 residues: VC7.1 keeps a no-code PAREN tuple for
  them, which shifts the list scheduler by a cycle. Four matches: `(GetScreenWidth() * 0.5f) - scale.X * K` moves the float-arg
  `push ecx` below the X `fsubp` (0x0047E651 / 0x0047E9AE / 0x0047F30F; 0x0047D296 likewise fixed that slot), `axis *= (1.0f / height)`
  lets the pos-argument load go above the last normalised-axis fstp (0x00697490), and `float radius = (PInterface->GetRadius(false));`
  sinks the radius fstp below two integer loads (0x00A1CC30). Put the parentheses in the row body; parentheses inside inline
  helpers (GetXYLength, operator-) did not move anything in 0x0095B9E0 / 0x008F3310. Try it before any other x87-schedule lever.
- (chunk Q7) The PAREN lever does not change which operand c2 loads first (fld mem vs fld st(i)): 0x00BCF040, 0x00B9D730, 0x00A1BA10,
  0x006AD700 stayed put under every parenthesisation and (double) cast tried.
- (chunk Q7) A running sum (`d = a.Y*b.Y; d += a.Z*b.Z; d += a.X*b.X;`) fixed a dot product's term order that no single-expression
  spelling reached (0x006AD700, as GOTCHAS 2158).
- (chunk Q7) 0x0086BFB0 CRandomAppearanceMorph::TransferBinaryIn: STLport resize(n)'s default-element temporary -- retail's erase arm
  jumps past the temporary's free test (it knows the temporary is untouched) only when vector::_M_fill_insert's body is visible
  (FABLE_RETAIL_STLP_VECTOR, out of line): then the temporary is not captured and the opaque erase call cannot change it. The
  local-model draft (declaration-only _M_fill_insert) could never get there; the shared CRandomAppearanceMorph / CArray /
  CVectorMap / CMemoryDataInputStream headers matched first time.
- (chunk Q7) Parked drafts in prior/ were not always each row's latest attempt: several rows had later, closer drafts in
  rebuild/candidates/manual/pending/*/differ (hand649/755/759/762, parked_retry2_*). Check `find pending -path '*differ*' -name
  '<addr>.cpp'` and start from the newest (00697490's shared-header hand762 draft matched with the same lever as the local one).
- (chunk Q7) 0x0047E9AE: the hero path calls IS_THING_ALIVE<CThingPlayerCreature*> 0x0043DBD0 on `CPlayer::GetPControlledCreature()`;
  the shared engine/ThingAlive.h body under FABLE_RETAIL_IS_THING_ALIVE_NOINLINE (needs rebuild_abi.h for FABLE_FASTCALL) keeps the
  hero in edi across the call as retail; declaration-only it reloads from the frame (861v860).
- (chunk Q7) engine/CDefString.h #errors without FABLE_RETAIL_CDEFSTRINGTABLE (+ FABLE_RETAIL_CCHARSTRING), and an #error makes cl
  spin under Wine until the 240 s timeout with no output: define both before including CRandomAppearanceMorph.h.
- (chunk Q7) NUISystem dialog rows: GetInstance 0x0041E5F2 returns the CObservable part; +0x14 is RemoveObserver (slot 5), +0x24
  AddConcurrentExclusiveObserver (slot 9) per the landed 0x0047D0D5 model, and the dialog's CObserver slot 6 (+0x18, no argument)
  is ObserveAllEvents (Ego_r name set). The parked drafts called them AddObserver / SetFocus / TakeFocus_slot6.

## Rework onto shared headers (late Q7)
- The shared NUISystem headers already carry every slot these dialog rows need, under PDB names: CComponent FindChild 3,
  ChangePosition 31, Initialise 43, ChangeState(unsigned long) 48, AddActionOnLeftUnclicked 80, GetTargetStateIndex 87; CYesNo
  SetYesCallBack / SetNoCallBack 142/143 (FABLE_RETAIL_CYESNO_CALLBACKS); CText::SetValue 145/144; CObservable RemoveObserver 5 /
  AddConcurrentExclusiveObserver 9. Check them before inventing slot names.
- Rewriting a byte-matched local-model row onto shared headers kept the match on the first clean compile for all four rows: the
  opt-ins (CCOUNTEDPOINTER_CALLED_CTOR + RELEASEDATA_DECL_ONLY, STLP_VECTOR_PUSHBACK_NOINLINE, CTHING_GETTC_EXPANDED, CGAMEEVENT_INLINE)
  reproduce what the local models did. Copy the define block of a landed row that calls the same callees (0x0058AF10, 0x005BE284).
- A header `#error` makes cl spin under Wine until the grade times out with an empty result: run `g++ -E -D_MSC_VER=1310` with the
  overlay and repo include paths first and grep for `#error`.
- Overlay headers go stale when other agents land additions: re-diff against the repo just before finishing.
