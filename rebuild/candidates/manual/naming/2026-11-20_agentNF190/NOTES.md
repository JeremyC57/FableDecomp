# NF190 notes (round 10, script interface / defs / quest scripts, second pass)

Handed back at the q6277 entry boundary (context). Counts come from APPLIED.tsv / STATUS.tsv: 64 entries = 9 done, 11 done-already,
1 rejected, 9 blocked, 1 deferred (q6419, held), 33 open (q6291 onward, not started). Rows changed: 13 (rows/), all RELOCATION_MATCH;
recheck_final.log is one vland run over the 13.

## Families / decisions
- **q4620 / q4621 (resource-handle ctors).** Census by source grep of every compiled row that declares `Construct()` on the two handle
  classes (the table keeps one witness): 0x00DBEB20, 0x00DDD030, 0x00E0AD40, 0x00E1CFD0, 0x00DC8C80. Fix = in-place respell of the
  row-local class member `void Construct();` to the constructor declaration (same class token, no new class; brief "in-place respelling
  inside a local model the landed row already had"); objdump defined-symbol sets equal to HEAD for all five. 0x006E7B60 now has one
  spelling. 0x007E72A0 keeps one stand-in: 0x00DDD030 `?Construct@CScriptedThingStorage@@QAEXXZ`. Retyping its hero onto the row's
  local CScriptGameResourceObjectScriptedThingBase grades RELOCATION_MATCH (work/00ddd030_f2_held.cpp) but then the row binds, at a retail
  site, its COMDAT copy of that ctor (defined at HEAD already: the scripted handle's inline ctor), whose bytes differ from owner 0x007E72A0
  (vptr store scheduled after the PImp stores): held by the pick-any rule. The pre-existing copy itself is debt (also: the owner row is a
  strong definition, so that COMDAT is an LNK2005 pair at HEAD).
  - The five rows keep their local models (SHARED_HEADER_AVAILABLE warnings are all pre-existing at HEAD).
- **Def scalar-deleting destructors (q4838 q4841 q4844 q4847 q4849): coordinator decision needed.** Identities all proven (vtable slot 0
  next to the landed Copy / GetSizeofClass slots 19 / 20 + PDB name sets). The K050 emitter (inline ctor + `new`, as landed 0x00F25B40 /
  0x00F26F40) newly defines the base-ctor COMDATs:
  - CSubDefClassBase classes (CArenaWaveDef, CFeatDef, CHeroSoulsCreatureDef): `??0CSubDefClassBase@@QAE@XZ` from the
    FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE body (18 B, CDefClassBase ctor called) vs owner 0x0044C0C0 (29 B, expanded). The same copy is
    already carried by 0x00F280E0 (the CArenaWaveDef ctor row) and 7 other CTOR_INLINE rows. 0x00F25CB0 draft: work/00f25cb0_blocked.cpp
    (RELOCATION_MATCH; its other new copies are identical to their owners: ??0CArenaWaveDef 0x00F280E0, ~vector<CArenaCreatureDef>
    0x00F25CE0, _STLP_alloc_proxy ctor 0x00F285C0).
  - CRegionScriptDef (CParentDefClassBase): work/00f29ce0_blocked.cpp RELOCATION_MATCH, but ??0CRegionScriptDef / ??0CParentDefClassBase
    copies differ from 0x00F29C10 / 0x00430370.
  - CScriptDef: the ctor row 0x00F28F90 is a 1014-line local model: blocked port.
  - Options for the coordinator: accept a copy identical to the copies already landed (not to the address owner), or a header
    opt-in that makes the base ctor declaration-only plus an emitter that does not compile the derived ctor (none found: VC7.1 emits the
    vtable, hence ??_G, only where a ctor is compiled, and emits every referenced inline as a COMDAT).
- **q6224 (0x00DFDE30 vector<CScriptThing> copy ctor).** NF123's idea works for the grade: an overlay opt-in making
  `_Vector_base(size_t, const allocator&)` noinline (patch kept in work/blocked_hdr/, not in hdr/) + `#pragma inline_depth(1)` grades
  RELOCATION_MATCH. But the pragma state at TU end also governs the pick-any `_Construct<CScriptThing,CScriptThing>` copy (37 B, copy ctor
  called) vs owner 0x008AC120 (29 B, expanded); an explicit instantiation before the pragma does not help. Next form (not tried, budget):
  default depth with both `_Vector_base(size_t, alloc)` and the FASTCALL `_Construct` noinline through header opt-ins.
- **q6263 (0x00DE1770).** The port recipe that matched 0x00D04770 / 0x00DD9900 / 0x00D043A0 does not compile here: from the statement
  after `EnteredNewRegion = true;` VC7.1 loses the member scope (C2227 / C3861 "Game: identifier not found"); the preprocessed text is
  clean. Something in engine/CQ_RansomVictimScript.h (nested classes, by-value ScriptedThingBase members) is the suspect. Draft kept.
- **q5109.** The PROPOSAL leaf (fastcall, 5 args) contradicts the landed self row (stdcall, 4 args), which already carries the proposed
  descriptive name; libpk vendor code gets light naming only. Rejected; the manifest's propagated LHAudioBank_ReadMurderTwistData is in
  NAME_FIXES. Existing split left: binder 0x00C174A0 binds 0x00C16F00 through its own static noinline helper `LoadLookupTableBank`.

## Self renames with no landed referencer (exe scan REFS empty or only the binder already on the new spelling)
0x00F27180, 0x00E50E40, 0x00E50100, 0x00DD9900, 0x00D04770, 0x00D043A0, 0x00DA3F40, and the leaf-only parameter retype of 0x0089B110
(vtable-only reference). Each binder (0x00DD2CE0, 0x00D038E0, 0x00DA34B0 [NF186 lock, untouched], 0x00DE0850, 0x00EDC450, 0x00CFE030) already
binds the PDB spelling (reloc_bindings witness rows); none was edited.

## Evidence re-checked vs taken from the queue
- 0x0089B110: both PDB name sets hold ?StartMovieSequence@CGameScriptInterface@@UAEXABVCCharString@@AAVCScriptGameResourceObjectMovieBase@@@Z
  (the self-signature rule's two sources, with engine/CGameScriptInterface.h slot 370); rtti_bases.tsv: CScriptGameResourceObjectMovie :
  CScriptGameResourceObjectMovieBase (licenses MovieResource on the shared base); callers.py: no landed binder (vtable only).
- 0x00E50100: Fable.exe holds the address only at 0x00E4FB7F (the immediate of the store at 0x00E4FB7C). 0x00E50E40: rel32 calls at
  0x00E50A96 / 0x00E50DB8; 0x00E50650 at 0x00E4FEEE / 0x00E50B71 (scanned). The FSE Lua-port detail is queue evidence, not re-checked.
- q5109 body strings: queue evidence, not re-checked (marked in NAME_FIXES).

## Binding changes (unfiltered binds.py, HEAD object vs new, the 8 ported rows)
Only 0x0089B110 (0x008AB970), 0x00DD9900 (0x00DD9A50) and 0x00F27180 (0x00F27190) change a callee spelling; all three are in RENAMES
section 2. The other five ported rows bind exactly what HEAD bound. recheck_final.log WARN RELOC_NAME lines (16) are all pre-existing
stand-ins unchanged from HEAD: the Destroy() stand-ins for the handle dtors 0x007E74D0 / 0x006E7B80 in the q4620 family (follow-up: the
dtors cannot be respelled in place, since a declared dtor runs automatically at scope exit; needs the shared-header port), the
0x00DDD030 CCutsceneCastMap / CScriptedThingStorage / CBaseObjectDtor stand-ins, and 0x0089B110 MovieResource / g_MovieTransitionState.

## Leaf lines completed (not renames)
0x00DBEB20, 0x00E1CFD0, 0x00DC8C80 had short Leaf lines; they now carry the full mangled self leaf, which is the symbol the HEAD object
already defined (no self rename; the gate should read them as unchanged).

## Header opt-ins (hdr/README.md)
- engine/CHeroSoulsCreatureDef.h: FABLE_RETAIL_CHEROSOULSCREATUREDEF_COPY, _OPERATOR_ASSIGN (row 0x00F27180).
- engine/CV_BeardyBaldyScript.h: FABLE_RETAIL_CV_BEARDYBALDYSCRIPT_GOTALKTOBARBER, _WATCHFORQUESTFINISHED (rows 0x00E50E40 / 0x00E50100).

## Stand-ins / descriptive names
- No new made-up names. 0x0089B110 keeps the existing stand-in method class `CGameScriptInterfaceMethods` (its parameter now the shared
  CScriptGameResourceObjectMovieBase); its header-less `MovieResource` (RTTI CScriptGameResourceObjectMovie, no shared header) keeps its token
  and now sits on the shared base (narrow rule). No stand-in was cast to a real type.
- 0x00D043A0: the +0x4D flag is renamed from ActivatedBanditRaid (CQ_BanditCampScript record) to CQ_BanditCampBossBattleScript::Gate3Open
  (Ego_r +97 in both records).

## Flags lines
None added. No row needed flags beyond its catalog CompilerFlags (all empty).

## Funclets
No EH funclet includes any changed parent (grep of rebuild/src/compiled for the parents' file names). Self renames move these parents'
files at landing: 0x00F27180, 0x00E50E40, 0x00E50100, 0x00DD9900, 0x00D04770, 0x00D043A0, 0x00DA3F40.

## Rule slips
- Three Bash calls exited non-zero (the first `ls && wc` in the agent dir, a `cp` loop with a wrong path join, and an `ls` of the other
  agents' stlport overlays that did not exist).
- I compiled 16 owner rows read-only into my own pO directory for the pick-any comparisons before reading their locks. Read afterwards:
  none is acctB or another agent's (0x007E72A0 NF190; 0x008AC120 released; the rest have no lock). None was edited.
- q6419 (0x00E15500) is recorded deferred from HELD.txt (pending landing in the NF186/NF187 hand-backs); its lock file now reads
  `released`, so the coordinator may re-queue it once that landing is in.
- Forms on 0x00DFDE30 counted as NF123's 2 + my 2 (one past 3 if counted across agents; the second was the pick-any fix attempt).
- 0x00D043A0 forms 1-2 differed only because my port script dropped the `inline long CTimer::GetTime()` body (filter matched
  `inline CTimer` only); form 3 restored it.
- One Bash call printed a large reloc_bindings dump (an awk filter without the address variable): output only, no file change.
