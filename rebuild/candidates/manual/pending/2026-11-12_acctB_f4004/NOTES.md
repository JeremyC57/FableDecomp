# Hand chunk 4004 (CScriptDef::Transfer, CScriptDef::operator=) -- working notes (kept current for a hand-over)

## Row 0x00F2D6E0 CScriptDef::Transfer (23,755 bytes: `push ecx` at 0x00F2D6E0 .. `ret 4` at 0x00F333A8; next COMDAT 0x00F333B0)

### Map (tools/map.py -> map/events.txt, map/units.txt; from dis_all.txt)
- 944 transfer statements, one per member of CScriptDef in Ego_r member order with 17 order breaks (tools/chk.py: every name
  string equals the member name at that offset in rebuild/include/engine/CScriptDef.h; 943 members, all transferred).
- kinds: L = long (549), F = float (285), D = CDefString (45), S = CCharString (15), VS / VF / VL / VX = TransferVector of
  CCharString (14) / float (14) / long (5) / CDefString (12), 5 TransferVectorOfSubComponents (CConversation 0x00F334E0,
  CFireHeartPatternDef 0x00F33B90, CFeatDef 0x00F34330, CArenaRoundDef 0x00F349F0, CHeroSoulsRoundDef 0x00F351F0).
- The source statement is always `context.Transfer("Name", Name);` (the two-argument inline
  `Transfer(name, value) { Transfer(name, value, GetDefaultVal<T>()); }`), `context.TransferVector("Name", Name);` or
  `context.TransferVectorOfSubComponents("Name", Name);`.
- INLINE SPLIT (the whole difficulty of the row): statements 1..451 have the two-argument Transfer EXPANDED
  (`call GetDefaultVal<T>` + `call Transfer<T>(name, value, default)`; kinds L3 / F3 / D3 / S3 in the map), statements
  452..944 CALL the two-argument Transfer (0x004110B0 long, 0x004107E0 float, 0x007E3AA0 CDefString, 0x00411010 CCharString).
  Inside the expanded part GetDefaultVal<T> is called out of line everywhere except statements 448 (TempleActivationAge,
  0x00F30E97: `xor ebx,ebx` .. `mov [esp+20h],ebx`) and 451 (TempleAcolytePrayerTimer, 0x00F30EF7), both long; 449 / 450
  (float) still call GetDefaultVal<float>.

### What decides the split (measured; tools/inl.sh + tools/inl/inl3.py hook c2, tools/sim.py replays it)
- c2 has TWO inline decision rules (c2.dll 0x1078CF54): callees it has ALREADY compiled in this TU carry an inline-info record
  (sym+0x99) and go through the cost rule (0x1078D1AA, MATCHING_PLAYBOOK "regime 0"); callees NOT yet compiled go through the
  BUDGET rule at 0x107284A3: refused if G2 [0x10882B5C] > 35000; else accepted if budget >= w or w <= 40, where w is the
  callee's c1 size word (sym+0x6D, read from the IL). G2 starts at the caller's own w (13231 for this row = 944 x 14 + 15) and
  grows by w of every accepted callee; the caller's budget R starts at min(35000, max(1000, 2 x w)) = 26462 and loses w of
  every accepted callee with w > 40. A nested pass (the callee's own call sites) gets floor(R / remaining top-level sites);
  what it consumed leaves R and is added to G2 a second time. A body that is accepted but later fails to expand (switch
  bodies: TransferVector, TransferVectorOfSubComponents) keeps its G2 / R effect, nested accepted callees included.
- c2 compiles functions in the order c1 emits them, and c1 emits an INLINE VIRTUAL that is only needed by the vtable BEFORE the
  templates it is first to use (probe/o1..o5.cpp: `[ctor] [templates of plain functions] [inline virtual] [its templates]`),
  while a non-inline member gets its templates first. Retail's layout has exactly that order: CScriptDef::Transfer 0x00F2D6E0,
  then TransferVector<CDefString> 0x00F333B0, the CDefString wrapper virtuals, TransferVectorOfSubComponents<CConversation>
  0x00F334E0 ... (and CFeatDef::Transfer 0x00F2D330 followed by Transfer<EFeatAttackType> 0x00F2D4D0). So retail's
  CScriptDef::Transfer is an inline virtual (defined in the class / a header) and its Transfer<T>(name, value) callees are
  decided by the BUDGET rule. With a non-inline row every site is expanded (d01 / d02: 30307 / 25820 bytes).
- Reproduction so far: row declared `inline`, emitted through the vtable by `CScriptDef::CScriptDef() {}` (the device the
  landed destructor row 0x00F29530 uses), GetDefaultVal<T> body visible (overlay opt-in), CPersistTraits with user ctor AND
  user destructor (both PDBs name ??0 / ??1?$CPersistTraits@J@ / @M@ / @VCDefString@@), TransferVector<T> named body visible
  (overlay opt-in; the call sites are budget candidates) -> probe f1: first 3431 instructions exact, cut at statement 394,
  no inlined default. Measured c1 sizes (tools/w6.sh probes): Transfer2 long / float 38, CDefString 34, CCharString 48;
  GetDefaultVal 48 (CCharString 64) with ctor 22 + dtor 14 nested; TransferVector<T> 151 with nested clear 35, end 19, begin 19,
  wrapper ctor 39 + base ctor 22 (x2); TransferVectorOfSubComponents 268 (+250 nested).
- tools/sim.py reproduces the f1 trace exactly (G2 and R at all 944 top-level decisions). With these MEASURED sizes it gives
  retail's exact split (cut 452, inlined defaults [448, 451]) under two further conditions:
  (1) the 8 TransferVector<CCharString> sites before the cut have no budget effect (callee already compiled -> cost rule);
  (2) GetDefaultVal<float> is not expandable at share 48 (w >= 49, or refused for another reason): sim `skip=VS gF=49`.
  Without (1) the cut comes at 394; without (2) the float defaults at 447 / 449 are inlined and the cut moves to 450.

### Result (2026-11-12): RELOCATION_MATCH, test PASS under g++ and VC7.1
- src/00f2d6e0.cpp (generated: `python3 tools/gen.py src/00f2d6e0.cpp --head work/head_final.inc --tail work/tail_final.inc`):
  q.py / gall.sh RELOCATION_MATCH, no flags; all 6409 instructions equal, 965 relocation symbols pair one to one with 965
  retail targets (tools/rel.py fin --all: "inconsistencies 0"); read-only vland RELOCATION_MATCH PASS with three RELOC_NAME
  warnings (labels at 0x0040E160 / 0x0040E250 / 0x007E37C0, see NAME_FIXES.tsv).
- The two conditions sim.py predicted, as source:
  (1) f2: `template void CPersistContext::TransferVector<CCharString>(...)` (explicit instantiation) ahead of the row -> the
      instance is compiled before the row, its sites use the cost rule -> cut moves 394 -> 450 (23749 bytes, 3896 exact).
  (2) f5: CPersistTraits<float> as an explicit CLASS specialisation (PDB: its TransferBinaryOut / TransferOut take `const
      float&`) whose DefaultVal is `const float` -> GetDefaultVal<float> c1 size 49 -> refused at share 48 -> exact.
  Failed alternatives for (2): explicit instantiation of GetDefaultVal<float> (f3, 21950) or of Transfer<float>(name, value)
  (f4, 25096).
- FITTED, NOT PROVEN (say so at landing):
  a. the `const` on CPersistTraits<float>::DefaultVal (only measured shape with size word 49; PDB dumps carry no qualifiers);
  b. the explicit instantiation of TransferVector<CCharString> (stands for "retail's TU compiled 0x0049B8D0 before this
     function"; which retail function did is unknown -- nothing in 0x00F20000..0x00F40000 besides this row calls it);
  c. the `CScriptDef::CScriptDef() {}` vtable trigger (precedent: landed 0x00F29530). Retail's constructor 0x00F28F90 is in
     the same TU, but nothing proves the row's COMDAT came from that TU's copy;
  d. CPersistContext_TransferableVector<T>'s four overrides and GFSerialiseVectorBinaryIn / Out are declared only (header debt).
- Compiles: 18 whole-function compiles before the final one (19 lines' worth in grades.log with the q.py grade) (d01, d02, e1..e8, e7 with three debug-info flags, f1..f5) + the
  q.py grade; 5 emission-order mini probes (o1..o5, o1 also with /Gy); 9 size-word probes (w1..w10 without w6); 5 c2
  inline-decision traces of whole-function builds (d01, d02, d02x, e7, f1).

### Names
- PDB (two sources: Ego_r.pdb / FableWin.pdb symbol + the object's relocation pairing at retail's address): every callee in
  the source header comment. Descriptive (found nowhere): none in this row.
- Overlay header names: CPersistContext_TransferableVector<T> (RTTI .?AV?$CPersistContext_TransferableVector@J@@ at vtable
  0x0124F33C + ghidra_out/struct_layouts_egor.tsv), member TransVector +4 (Ego_r.pdb layout; same name as the sibling
  CPersistContext_TransferableVectorOfSubComponents).
  TransferVectorSaveText 0x009BAC80 / TransferVectorLoadText 0x009BB110: both PDBs, protected.
  GFSerialiseVectorBinaryIn / Out: PDB names (global function templates); parameter spelling from the landed 0x00F333B0 row.

### Test (test/00f2d6e0.cpp, generated by tools/mktest.py)
- Function under test: the row's 944 statements copied from src. Model: recording CPersistContext (two- and three-argument
  Transfer<T>, GetDefaultVal through a traits local, TransferVector, TransferVectorOfSubComponents; clear / save / load modes),
  CScriptDef model generated from engine/CScriptDef.h (943 members, header order and types).
- Expected table from RETAIL (map/events.txt): key string retail pushes, kind of the retail callee, member that the header
  places at the retail offset (looked up by offset, not by key). Checks: 944 events; key, member address, kind per statement;
  943 distinct members; statements 478 / 479 both TemplePrayerFactorHighest; all 943 members at their type default after clear
  mode (member visitor generated from the header, independent of the row); save / load round trip restores all 943.
- tools/mut.py: 12 single-edit mutations, 12 fail the test (swap, delete, key typo, wrong member of the same type, dropped
  duplicate, repeated last statement, moved statement, wrong vector member, copied statement, key case, swapped sub-component
  statements, and one model edit: a long member typed float).

## Row 0x00F2A250 CScriptDef::operator= (11,711 bytes) -- NOT STARTED (context budget; no source written)
- Outline from map/dis_opeq.txt: compiler-generated member-wise assignment, `ret 4`, ends `mov eax,esi` near 0x00F2D00C.
  Base operator= call 0x00431F60, bit-field copies at +0x24, CVectorMap copy 0x00431F90, 15 x 0x0099EFB0 (CCharString
  assignment), vector assignments 14 x 0x0045EF2D, 14 x 0x00432EE9, 12 x 0x0046078C, 5 x 0x00454886, one each 0x00F2D010 /
  0x00F26C30 / 0x00F25980 / 0x00E54AA0 / 0x00D34D40 (the five sub-component vectors); scalar runs are copied as dwords.
- Twins to start from: rebuild/src/compiled/00/f2/CFeatDef_operator_00f2d200.cpp, CCutsceneDef_operator_00f2a000.cpp,
  CArenaWaveDef_operator_00f25f60.cpp. Expected source: an implicit operator= forced out of line (the header already declares
  `CScriptDef& operator=(const CScriptDef&);` -- an opt-in that drops that declaration will be needed so the compiler writes
  it). tools/cfg.py already takes ROW=00f2a250 (cc.py / pd.py / rel.py follow it).
