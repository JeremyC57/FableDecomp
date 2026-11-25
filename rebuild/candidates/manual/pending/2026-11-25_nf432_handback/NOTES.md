# NF432 hand-back (2026-11-25): the NF430 identified ports

No repo write, no git command (naming_status.py was not run; see "Not checked"). Lane: VC7.1 RTM under Wine, `$S/wine/env.sh`.
Every gate ran HEAD and new at ONE LANDVERIFY_WORK path (`work/wd_<b>`, moved aside to `wd_<b>h` / `wd_<b>n`):
HEAD from `work/rsroot` (repo symlinks), new from `work/rs<b>` (repo symlinks + the batch's table applied to a COPY of
reloc_bindings.tsv + `hdr/engine/*.h` copied over rebuild/include/engine), FABLE_ROOT set per run (`tools/mkroots.sh`, `tools/runpair432.sh`).

## Batch n1 (ACCEPT, 7 rows): the NPlayerGui draw ctors + both CDrawBase virtuals
| address | old self | new self |
|---|---|---|
| 0x0064D2A9 / D335 / D351 / D389 / DA6E | `??0CDraw<X>@@QAE@PAVCPlayerGui@@PAX@Z` (local model) | `??0CDraw<X>@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@...@std@@@Z` |
| 0x00442760 | `?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ` | `?ClearPrimitives@CDrawBase@NPlayerGui@@UAEXXZ` (NF430 NAME_FIXES; RENAMES.tsv) |
| 0x00644627 | `?ClearPrimitivesIfNecessary@CDrawBase@@QAEXXZ` (local model, baked 0x41BE70, `Temp_helper` stand-in) | `?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ` |

Why 0x00644627 joined: the /FORCE link showed that respelling 0x00442760 alone leaves `?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ`
(slot 0 of every ported vftable, called by 0x00644D0E) with no definer, because its real address 0x00644627 was landed under a non-namespaced
local name. Porting it in the same batch makes both slots defined once (link: duplicates 0 -> 0, new selves 7/7 defined).

Sources per name (two each):
- ctors: Ego_r.pdb + FableWin.pdb publics `??0CDraw<X>@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector...`; the table already binds them (witness 0x0043B570).
- vftables 0x0125A538 / 574 / 580 / 598 / 5C4: retail RTTI COL `.?AVCDraw<X>@NPlayerGui@@` at va-4 + FableWin.pdb `??_7CDraw<X>@NPlayerGui@@6B@`.
- slot names: retail slots [0x00644627, 0x00442760] in every non-overriding draw vftable; FableWin.pdb CDrawBase vftable order
  (ClearPrimitivesIfNecessary, ClearPrimitives); FableWin bodies (Drawing test v empty); CDrawMiniMap overrides slot 1 with ClearPrimitives (0x00644660).
- class layout: struct_layouts_egor.tsv (44 bytes = the CDrawBase part only; retail 0x28) + FableWin field lists (ctor, Draw, compiler-generated dtor).

Headers (batches/n1/hdr/engine, HEADERS.tsv): 4 NEW class headers `NPlayerGui_CDraw{SpiritsHealth,FishingGame,SpiritScores,ScriptScreenText}.h`
(family file-name convention of NPlayerGui_CDrawDamage.h; FableWin members; size assert 0x28; nothing else includes them), and one add-only
opt-in `FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DECL_ONLY` in engine/CEnginePrimitiveHandle.h (orig/hdr_CEnginePrimitiveHandle.h): with the
inline Clear body, 0x00644627's `mem_fun_ref(&Clear)` emitted a pick-any Clear copy NEQ to 0x0041BE70 (44 v 57 bytes, reached); declaration-only
removes it (pk then 0 NEQ). CDrawReceivedItems uses the existing PASS23 class (note: it declares `~CDrawReceivedItems()`, FableWin says the
destructor is compiler-generated; no effect on this ctor).

Table (TABLE_CHANGES.tsv, apply_table --dry: replace 5, add 1, rewitness 1, delete 1, 0 misses, multi-target 1 -> 1):
5 `??_7CVtable_0125a5xx` -> `??_7CDraw<X>@NPlayerGui@@6B@`; add `?ClearPrimitives@CDrawBase@NPlayerGui@@UAEXXZ` 0x00442760;
rewitness `??0CDrawBase@@QAE@PAVCPlayerGui@@PAX@Z` 0x0064D2A9 -> 0x0064D2C5 (CDrawMoney still emits it: link census, 1 unresolved ref left);
delete `?Temp_helper@@YIXPAUTemp@@HHH@Z` (only 0x00644627 spelled it).

Gates (batches/n1/evidence):
- vland H/N: 7/7 grade kept (0x00644627 RELOCATION_MATCH both sides; its baked 0x41BE70 is now the Clear relocation), tests PASS, 0 LITERAL,
  warn kinds H {RELOC_NAME} -> N {} (with RENAMES applied to the payload / oracle name, as apply_renames does; without it STANDIN_LEAF on 0x00442760).
- name_gate --test (NG_TABLE = new table, overlay hdr/): 7/7 PASS (name_gate.tsv).
- slots (tools/vt432.py, new selves from selfmap): 5 vftables, 10 agree, 0 WRONG, 0 unknown; exact RTTI spellings.
- objsec (gate425): 0x00442760 ALL_EQ; ctors: leaf equal under the renames, the vftable COMDAT grows from the 1-slot placeholder to the 2 real
  slots (intended), .drectve +libcp (STLport include; NF318 Q1 / NF427 precedent); 0x00644627 is a port (leaf reloc count +1 = the former baked
  address) plus 4 NOADDR STLport COMDATs (mem_fun_ref, mem_fun_ref_t ctor, vector begin/end) OBJECT_EQ with landed 0x0064ED4F's copies (R1/R10).
- pk_all + pickany_vs_retail: 0 new NEQ, 0 reach flips (0x00442760 NEQ 1 -> 0; 0x00644627 copies 0 -> 4, all NOADDR, see above).
- /FORCE link with definers 0x0064CC65 0x0043F360 0x0041BE70 0x0064D2C5 0x00644D0E 0x00646567: duplicates 0 -> 0, LOST 0; control (duplicated
  object) reports 1 duplicate (linkgate_control.txt). Unresolved before and after: the PDB CDrawBase ctor (0x0064CC65's own self is a stand-in, NAME_FIXES).
- mutation: 7/7 mutants FAIL (ctor: second base argument replaced; 0x00442760: body writes Drawing; 0x00644627: test inverted).
- reserved_check (RESERVED_REFS=""): 0 / 0. hdrcheck: CEnginePrimitiveHandle.h same under default + every repo macro; 4 NEW.
- equiv_check copy v copy (old = cp rebuild/include, new = cp + hdr/): 0 of 57,160 rows differ; planted control (a typedef in the new
  CEnginePrimitiveHandle.h) flags 3 of 4 chosen includers (evidence/equiv_planted_control.tsv).

## HOLD
- hold_n1d 0x0064D3DA CDrawDamage ctor: one attempt; STLport list ctor pick-any copy NEQ to 0x00655B18 (RESULTS.tsv). Lock released.
- hold_d1 0x00C3C840 `??_GCCodaObjectBase@@UAEPAXI@Z` (Task 4 D3): every gate clean but name_gate FAILs on the pre-existing 15-spelling split at
  0x012B5868 (RULINGS_NEEDED R-NF432-1). Lock released.

## Batch g1 (ACCEPT, 4 rows): CScriptBase's default virtuals (found while measuring Task 3)
NF430's "wrong slots" on the 102 debt rows were mostly not the rows' fault: retail slots 1-4 of every script vftable that inherits them point at
0x00CBD4B0 / C0 / D0 / E0, whose landed selves were the shared gapscan stand-in `?Hook@T@@QAEXXZ` / `?Hook@T@@QAEXH@Z` (85 self rows share
that name). The emitted slot names (`?RegisterMain@CScriptBase@@MAEXXZ` ...) were right. g1 respells three selves:
RegisterMain / Main / Init (MAE, `ret`). OnPersist 0x00CBD4E0 (UAE) is HELD (batches/hold_g1o): the local CScriptBase models of
0x00DC3EA0 / 0x00DCC000 emit the same public symbol pick-any, so a strong definition is a new LNK4006 (NF382 b3). Sources: retail slots 1-4 of the CScriptBase vftable
0x012C1648 + FableWin.pdb ??_7CScriptBase@@6B@ slots 1-4 (read through fwpub.follow); engine/CScriptBase.h already declares them with that access.
Census for the link gate: 9 rows define a local CScriptBase with in-class virtual bodies (grep), compiled at HEAD: only 0x00DC3EA0 /
0x00DCC000 emit any of the four names (public UAE spellings). Gates: MATCH PASS both sides, objsec ALL_EQ 3/3, name_gate --test PASS,
pk 0/0, /FORCE link with those two as definers: duplicates 3 -> 1 (the shared Hook stand-in gone; the 1 left is a pre-existing ctor pair),
reserved 0, no table / header change (equiv n/a), mutation n/a (NF419 r1: leaf bytes identical, empty bodies). RENAMES.tsv moves the four
catalog entries from `_gapscan` / sub_<addr> to CScriptBase::<Name> (file names CScriptBase_<Name>_<addr>.cpp).
Effect on the debt rows (fixed vt432, NF430's own new objects; per t1a / t1b / t1c): agree 54 / 43 / 0 -> 143 / 115 / 113, unknown
168 / 175 / 137 -> 93 / 88 / 64, WRONG 193 / 131 / 178 -> 179 / 146 / 138 (evidence/vt_debt_t1*.txt). WRONG rises in t1b: slots that were
'unknown' (an emitted base name with no binding, retail pointing at an override) now resolve and show the missing override (NF419 r2). The rest are real model gaps (T3 nf432 family line).

## Task 3 (debt_header, 102 rows): not ported (context budget); shape measured
77 `Alloc` + 25 ctors. LOCAL_CLASS (62 fails) for script classes with no header, SHARED_HEADER_AVAILABLE (87) for those with one, in rows and
tests. After g1 the remaining WRONG slots are overrides the local models do not declare (e.g. CGlobal_TeleportToHeroGuildScript retail slots
1/2/3 = 0x00CDD5F0 / 0x00CDD6B0 / 0x00CDD5E0). Generator plan in T3.txt (family line). Existing headers such as CV_GuildMasterScript.h declare
the overrides but no constructor; the Alloc rows inline the ctor, so each needs a ctor opt-in. Do not edit engine/CGameScriptInterface.h (NF431).

## Task 2 (CEntityScriptBase): not attempted (context budget). The nf328 line on 0x00DEF6A0 is NOT resolved; CSP_UndeadTrigger waits.
Notes for the next agent in T3.txt (0x00EB2000 needs an add-only ctor opt-in in engine/CV_MurderTwistFinalScript.h, a manual ??_G compare,
and a census of every emitter of ??_7CVtable_012c3224@@6B@ before its delete).

## Task 4 (CCodaObjectBase, D3)
Settled spelling: `??_7CCodaObjectBase@@6B@` (EXTRA_RULES D3; CODAApiDbg.lib carries it; table binds it, witness 0x00C3E2A0). The one binder
that IS CCodaObjectBase (0x00C3C840, its deleting destructor) is ported (hold_d1, R-NF432-1). The other 13 are destructors of derived libpk /
CODA classes whose own names are R-NF430-1 holds; T3 lists them. Note the table line `??_7CVtable_012b5868@@6B@` is also emitted by 0x00C3F490
(object census), so it was rewitnessed, not deleted.

## Pilots for R-NF430-1 (findings only; nothing landed on them)
(a) FableWin.publics / Ego_r.publics / both types dumps carry NO CODA class: 0 `CCoda*`, 0 `CSal*`, 0 `CEAX*`; the 36 / 34 "PKC" hits are
    hash fragments inside `??_C@` string-literal names. The landed `??0CCoda3DSource@@QAE@XZ` (table, witness 0x00C3DFD0 CCodaContext::Create)
    comes from Codex pass 23 (pending/2026-11-15_codex_pass23/return/landed_evidence/012150a1/NAMING_EVIDENCE.md): CODAApi.lib CodaContext.obj
    relocations name the four constructors CCodaContext::Create calls; CODAApiDbg.lib gave class keys and signatures.
(b) Yes: `$S/sdk/gamecoda/tree/CODA/lib/Win32/CODAApiDbg.lib` (10.9 MB, 2004-10-04) next to CODAApi.lib, plus ToolkitDbg / CAGEDbg / OpenAL32Dbg.
    `nm` lists 77 distinct `??_7` names in CODAApiDbg.lib (CCoda2DSource, CCoda3DSource, CCodaContext, CCodaObject, CCodaObjectBase,
    CCodaSource, CSalBaseVoice, CSalBuffer, CSalMixer, CFloatLongReverb, ...). The slot-body match against it was not run. Nothing was copied.

## Locks
ACCEPT rows locked NF432: n1 (0x0064D2A9 0x0064D335 0x0064D351 0x0064D389 0x0064DA6E 0x00442760 0x00644627), g1 (0x00CBD4B0 C0 D0).
Released: 0x0064D3DA, 0x00C3C840, 0x00CBD4E0. T3_PORT_QUEUE lines resolved: none (0x00DEF6A0 nf328 untouched).

## Not checked
- naming_status.py tracker HEAD v new (not run: it needs a new tree with the moved sources; the brief warns about symlinked roots).
- Measured on 0x0043B570 only: its inline CDrawNPCName ctor (CTOR without VSLOTS) emits ??_7CDrawNPCName with the CDrawBase slots swapped;
  HEAD 1 WRONG + 1 unbound -> new 2 WRONG (existing debt made visible, NF419 r2; T3 line with the one-macro fix). Other includers without
  VSLOTS that emit a draw vftable were not censused.
- Tool gotcha (fixed here): vt430.py reads the FIRST relocation block of a section NAME, so an object with several `.rdata` vftable
  COMDATs checks every vftable against the first one's relocations. tools/vt432.py + coffload.py read each ??_7 COMDAT's own section
  (by index, offsets relative to the symbol). NF430's 151 / 120 / 178 WRONG were measured with the old tool; the fixed tool gives
  193 / 131 / 178 at HEAD. The n1 / d1 objects emit one vftable each (results unchanged).

## HEAD moved during the run
By the end, 5 reloc_bindings lines and 5 headers (CGameScriptInterface.h, CPlayerGui.h, CEnvironment.h, CEnvironmentSpellEffectInterpolator.h,
CThing.h) had changed in the repo (other landings). None touches a symbol, row source or header this hand-back uses (checked: every batch
row's HEAD source equals its orig/ copy; CEnginePrimitiveHandle.h equals orig/hdr_CEnginePrimitiveHandle.h). The equiv_check above used the
15:20 include tree; rerun it at landing.
