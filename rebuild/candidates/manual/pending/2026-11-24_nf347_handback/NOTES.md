# NF347 hand-back: INDEX_DECL_ONLY pilot + pick-any debt groups (Account A naming queue, 2026-11-24)

Hand-back: `NOTES.md`, `STATUS.tsv`, `batches/b1/` (run dir, 3 rows, all ACCEPT), `evidence/`, `tools/` in `$S/w22/agentNF347/`; `work/` is scratch,
not part of it. No header changed, no table change, no repo write, no git. Every lock taken under NF347 was released at the end
(evidence/locks.log; no `nflocks/*/owner` reads NF347). Tools: tools/ (NF343's pk.py / objsec.py / pkdiff.py /
odis.py / rdis.py / nf.py retargeted to NF347, plus the new tools/droptest.py).

## Part 1: FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY pilot (T3 nf340 0x00CD3D2E)

Change per row: one `#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY` before the includes, under a comment block that starts
`// Fidelity: scaffolding, not retail (NF347 pilot, T3 nf340 0x00CD3D2E): ...` (the reason; 00d58490's also names 0x009AC700).

Measured per row (HEAD = repo source, new = rows/<addr>.cpp, both compiled at ONE path, work/g/<addr>):

| row | flags | grade H / new (pk.py and vland --recheck-landed) | objsec | COMDATs dropped | leaf-called NEQ copies removed | checkers H v new |
|---|---|---|---|---|---|---|
| 00ea59e0 | - | RELOCATION_MATCH / RELOCATION_MATCH, behaviour PASS | no section CHANGED (leaf identical) | 23 | ??A map<CCharString,STB> (0x00CD3D2E, 257 v 118) | no copy turned NEQ, no new NEQ |
| 00d58490 | - | same | same | 41 | the same, plus ??A map<CCharString,CCharString> (0x009AC700, 159 v 149) | same |
| 00e8ef80 | /d2inlT72 | same | same | 23 | ??A map<CCharString,STB> | same (the T72 flag did not react) |

The only external-symbol additions are the UND ??A symbols (one per removed operator[]). vland logs: evidence/vland_b1_head.log /
vland_b1_new.log (identical apart from the run). Precheck: clang PASS for 00ea59e0 / 00d58490 (sources + tests); 00e8ef80's source fails
clang at HEAD and new with the same single error (`specialization of member ~map does not specialize an instantiated member`, line 52 / 56),
so `cl /Zs` (with /d2inlT72) passed for both in an earlier call (NF305 Q2; evidence/pre_zs.log).

### Drop test (every dropped symbol, DEF and UND; evidence/droptest_<row>.tsv, drop_definers.tsv, drop_census_compiled.tsv)
1. **reloc_bindings lines witnessed by the row**: none names a dropped symbol (the rows witness only ?WatchForQuestFailureState /
   the CSpawnedFunc1 vftable (00ea59e0) and AddLogbookTutorialEntryPC (00d58490)). No rewitness needed.
2. **Landed objects that reference a dropped symbol while it is undefined there.** Scanned population: the 8 other 0x00CD3D2E emitters
   and the owner 0x00CD3D2E (NF343's HEAD objects of today, $S/w22/agentNF343/work/obj), plus three objects I compiled at HEAD: two of
   the 32 landed MAP_INDEX_DECL_ONLY rows that name STB (00d55e90, 00e8b020) and the 0x009AC700 owner; plus a source census (STB copy
   constructor declarations: 10 sources). Findings:
   - STB pair ctor `??0?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@...` is UND in the owner 0x00CD3D2E object;
     8 other emitters still define it.
   - STB copy ctor (0x00905330) is declared without a body in 0x00CD27C7 / 0x00CD3D2E / 0x00CD378D / 0x00CD36DF (source census); the
     landed owner row 0x00905330 defines the same symbol strongly (RELOCATION_MATCH), and other emitters keep EQ pick-any copies.
   - The two DECL_ONLY rows reference ??A map<CCharString,STB> UND (not a dropped symbol: the pilot rows reference it too now).
   - Every dropped STB-family DEF has at least one other definer among the 8 other emitters; every dropped map<CCharString,CCharString>
     DEF of 00d58490 is defined by the 0x009AC700 owner object. Not claimed beyond this population.
3. **EQ copies with a retail address that the row loses (fidelity loss, listed)**, identical in all three rows:
   - `??0CScriptGameResourceObjectScriptedThingBase@@QAE@ABV0@@Z` 0x00905330 (45 bytes, EQ): the landed owner row defines it
     (RELOCATION_MATCH, strong), and 0x00D13580 / 0x00CE7670 keep EQ pick-any copies.
   - `??8_Rb_tree_base_iterator@std@@QBE_NABU01@@Z` 0x0041C820 (16 bytes, EQ, table binding; the address's own landed row is
     CPixelFormat::IsEquivalent, another symbol): 0x00D13580 / 0x00CE7670 / 0x009AC700 keep EQ copies.
   Dropped NEQ copies with an address: pair ctor 0x00CD27C7, less<CCharString> 0x0042AAB3, ??A 0x00CD3D2E (and 0x009AC700 for 00d58490);
   with the addresses NF343 located: map::insert(it, v) v 0x00CD3CAE and tree lower_bound v _M_lower_bound 0x00CD28BC, both NEQ. The
   rest are NOADDR (unverified, mostly 4..13-byte tree accessors).

### Recommendation per row (measured)
- **00d58490: worth it.** Beyond 0x00CD3D2E it removes a NEQ competitor of 0x009AC700, whose landed owner defines the exact four-argument
  STLport symbol EQ (RELOCATION_MATCH; COMDAT selection 2 = select-any, read from its object), so the link's chance of keeping the retail body goes up.
- **00ea59e0, 00e8ef80: neutral on their own.** None of the 12 objects examined defines the four-argument `??A map<CCharString,STB>` symbol EQ:
  the owner row 0x00CD3D2E defines the three-argument stand-in spelling `??A?$map@...@U?$less@VCCharString@@@std@@@std@@` (checked with
  `syms` on its HEAD object), so the external
  reference still resolves to another row's NEQ pick-any copy. The image gains only when every emitter (about 74 by NF343's census)
  converts and the owner is respelled onto the STLport spelling (NAME_FIXES candidate, already noted by NF343).
- ACCEPT reading: a listed EQ-with-address loss does not hold the row when other landed objects still define that symbol EQ (the brief's
  hold test is dependency on this row's copy; none found).
- T3 note: `land_scripted --release-t3` will not edit the nf340 0x00CD3D2E line (column 2 is the copy, the rows sit in its reason text);
  the coordinator removes 00ea59e0 / 00d58490 / 00e8ef80 from that line's emitter list by hand, and 0x009AC700's line (if any) for 00d58490.
- COMDAT-set note: the standing "COMDAT sets SAME" rule (NF318 Q5) would hold these rows; the brief's pilot is what admits the drops.

## Part 2: pick-any debt groups (none fixed)
All rows /d2inlT-flagged; CompilerFlags untouched. Measurements in STATUS.tsv; the short version:
- **CCountedPointer<NUISystem::CComponent> family (0x00429C15 ctor, 0x00429C53 dtor, 0x00429C6F Reset; rows 00626110 / 00626750 /
  00605570): skipped.** Cause: retail's copies come from the size-optimised 0x0042xxxx TU (the landed owner rows set optimize("s", on);
  0x00429C53 is the 5-byte `jmp ReleaseData`, 0x00429C6F an /O1 body), while the emitters are /O2 speed TUs. The only existing opt-in that
  reaches the dtor shape, RELEASEDATA_DECL_ONLY, moves both leaves (00626110 DIFFER 1528v1596, 00626750 1526v1771). A fix needs new
  size-region explicit specializations in engine/CCountedPointer.h (the NF225 CFontBank / CMeshDataBank SIZE_BODY pattern) for ctor,
  dtor, Reset and probably ReleaseData: the kind of design rejected for group 1, so not attempted (decision 3).
- **0x00429F43 DeleteData<CAction>: candidate, pending the pilot ruling.** DELETEDATA_DECL_ONLY on 00626110 / 00626750 keeps the grade
  and the leaf, removes the DeleteData<CAction> (70 v 21) and ~CAction (57 v 162) NEQ copies, but also drops the leaf-referenced EQ
  DeleteData<CComponent> copy 0x00428B10 and ??_GCAction. The 0x00428B10 owner row defines the stand-in `@DeleteIfSet@4`, not this
  symbol, so the new UND resolves only to other rows' pick-any copies. The copy is removed, not made EQ: not worth it as measured.
- **0x005949C3 map<long,CComponent*>::operator[]: candidate for 00626750 only, pending the pilot ruling.** MAP_INDEX_DECL_ONLY keeps
  00626750's leaf and grade and drops 20 COMDATs (incl. the EQ ??8 0x0041C820); it moves 00626110's leaf (DIFFER 1596v1596). The new
  UND resolves to the landed owner 0x005949C3, which defines the exact four-argument symbol (size TU, explicit instantiation): worth it
  in the 00d58490 sense.
- **0x00B439C0 PeekLandscapeHeight: skipped (not compiled).** NF343's lead resolved: the owner row 0x00B439C0 (/d2inlT301, PEEK +
  GETMAP_ORIGINAL_PASS26, no PEEK_BODIES) and the EQ emitter 0x00B41770 do not use the header body at all; each defines
  PeekLandscapeHeight strongly in its own TU with the original body (world coordinates into GetMap, which
  subtracts the origin under GETMAP_ORIGINAL_PASS26). No header opt-in carries that body, and GETMAP_ORIGINAL_PASS26 alone on the
  default header body would subtract the origin twice (a behaviour change). A fix is a new header opt-in, and the measured PeekThemeId
  analogue moves the leaf (next item).
- **0x00B43720 PeekThemeId (00b40260): skipped.** The EQ emitter 0x00B412A0's existing pair PEEKTHEMEID_ORIGINAL_PASS27 +
  GETMAP_ORIGINAL_PASS26 makes the copy EQ (324) but the leaf DIFFERs 753v1037. 00b40bd0 not compiled: it also expands PeekThemeBlend
  with the default body, so GETMAP_ORIGINAL_PASS26 is unsound there.
- **0x00433530 vector<CCharString>::_M_insert_overflow (0076eb00 / 00eda8c0): skipped.** VECTOR_OVERFLOW_OUTOFLINE alone is a no-op (ALL_EQ);
  adding UNINIT_COPY_FORCEINLINE (the set the EQ emitters 0x004B4A10 / 0x00A2ED10 use, unflagged) gives 291 / 258 v 316, still NEQ, and
  resizes three other already-NEQ copies in 0076eb00. Control (evidence/one_R1_noflag_0076eb00.txt): the same R1 text compiled without
  /d2inlT65 makes the copy EQ 316 (the leaf then DIFFERs 1139v1080, as expected). So the copy needs UNINIT_COPY_FORCEINLINE plus no /d2inlT
  flag: a CompilerFlags change, NF344's lane, although DEBT_BY_COPY counts it as not flag-caused (its control dropped the flag without
  the opt-in). Owner 0x00433530: unflagged /O2 + UNINIT_COPY_FORCEINLINE, explicit instantiation.
- **0x005149F0 vector<unsigned short>::_M_fill_insert (0099ba70 / 0099bb80): skipped (near miss).** UNINITIALIZED_FILL_N_NOINLINE +
  FILL_NOINLINE + VECTOR_OVERFLOW_OUTOFLINE keep the leaf, turn the fill / uninitialized_fill_n copies into the retail __fastcall
  spellings, both EQ (0x0050E500, 0x005123A0; the HEAD __cdecl copies were NOADDR), and bring _M_fill_insert to 273 v 269, still NEQ:
  the register allocation differs from the first instruction (ebp / edi v ebx / esi). It also adds 2 and drops 2 COMDATs.
- 0x0075C040: not reached.

## Decisions needed
1. **Pilot verdict.** Do the three ACCEPT rows land under the pilot, with two EQ-with-address copies dropped per row
   (0x00905330, 0x0041C820; both still defined EQ by other landed objects) and 23 / 41 COMDATs dropped (NF318 Q5 would otherwise hold)?
   My recommendation: land 00d58490; for 00ea59e0 / 00e8ef80 the image gains nothing until the whole family moves and the owner
   0x00CD3D2E is respelled, so land them only as the start of that family move.
2. If the pilot is ruled in, the same opt-in family gives two more candidates from Part 2: MAP_INDEX_DECL_ONLY on 00626750 (0x005949C3;
   worth it, owner defines the exact symbol) and DELETEDATA_DECL_ONLY on 00626110 / 00626750 (0x00429F43; not worth it: it also drops
   the leaf-referenced EQ DeleteData<CComponent> 0x00428B10, whose owner row binds a stand-in name).
4. 0x00433530 belongs in NF344's flag lane (measured above): send it there?
3. The CComponent family (and 0x00B439C0 / 0x00B43720) need new header text: size-region specializations, or an original-body
   PeekLandscapeHeight opt-in. Are such designs wanted, given that the probes on the analogous existing opt-ins moved the leaves?

## Rule slips (self-reported)
- 0076eb00's landed test fails clang only in VC7.1 headers. I claimed its lock before its `cl /Zs` passed; the /Zs passed (11:03:54)
  before anything compiled it (its test was never compiled; only sources were). Likewise 0099bb80 / 0099ba70: their tests failed clang
  (`_assert`, VC7.1 iostream; D1) and I claimed the locks first; both tests passed `cl /Zs` afterwards, and their sources passed clang
  (0099bb80 with -fno-wchar, the VC7.1 default wchar_t) before their compiles.
- tools/droptest.py's "self_at_target" column reads self_symbols.tsv, which lacks some landed rows (shows `none` for 0x00CD27C7,
  0x0042AAB3); I checked those owners by catalog instead.
