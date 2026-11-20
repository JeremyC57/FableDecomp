# Landing nf190 (Account A, 2026-11-20): script interface / defs / quest script names, second pass (13 rows)

Agent NF190 (round 10) handed back early (context) at the q6277 boundary with 64 entries: 9 done, 11 done-already, 1 rejected,
9 blocked, 1 deferred and 33 open (q6291 onward). Its files are in `naming/2026-11-20_agentNF190/`, with the held Def ??_G draft
(`work/00f25cb0_blocked.cpp`) and the untried q6224 header patch (`work/blocked_hdr/`). The open entries, q6224 and q6419 (its row
lock has since been released) go back to the queue unclaimed (35 entries).

- 13 rows land, all RELOCATION_MATCH, every test PASS.
- In-place respells, no new class token, defined-symbol sets equal to HEAD: 0x00DBEB20, 0x00E0AD40, 0x00E1CFD0 and 0x00DC8C80 turn
  the local `Construct()` stand-in into the PDB `??0CScriptGameResourceObjectScriptedThingBase@@QAE@XZ` (q4621); 0x00DDD030 is respelled
  the same way.
- 0x0089B110 binds `CScriptGameResourceObjectMovieBase::SetImplementation`; its own leaf changes only in the parameter type (both PDBs),
  and the address has no landed binder.
- Self renames (7), each ported onto its shared class header; every referencing row already binds the new spelling, so no split:
  0x00F27180 (CHeroSoulsCreatureDef), 0x00E50E40, 0x00E50100, 0x00DD9900, 0x00D04770, 0x00D043A0, 0x00DA3F40.
- Headers (add-only, `orig` equals HEAD, default paths unchanged, no landed includer defines the macros; asserts pinned with
  FABLE_STATIC_ASSERT_AT, listed in `naming/.../hdr/README.md`):
  - engine/CHeroSoulsCreatureDef.h: FABLE_RETAIL_CHEROSOULSCREATUREDEF_COPY, _OPERATOR_ASSIGN (1 assert pinned).
  - engine/CV_BeardyBaldyScript.h: FABLE_RETAIL_CV_BEARDYBALDYSCRIPT_GOTALKTOBARBER, _WATCHFORQUESTFINISHED (2 pinned).
- Coordinator calls (existing rules, no new decision):
  - q4838 q4841 q4844 q4847 (Def scalar deleting destructors) stay held. Their emitter matches, but it newly defines base-ctor copies that
    differ from their owners: `??0CSubDefClassBase` 18 bytes against 0x0044C0C0's 29, and the CRegionScriptDef ctors against
    0x00F29C10 / 0x00430370. That 0x00F280E0 and 7 other landed rows already carry the same 18-byte copy is debt, not licence (advisor
    rule, nf183). The owners' port is queued.
  - q6224 (0x00DFDE30): the `inline_depth(1)` form grades RELOCATION_MATCH but compiles the copied `_Construct` at 37 bytes against owner
    0x008AC120's 29. It returns to the queue with the next form (both `_Vector_base(size_t, alloc)` and `_Construct` noinline through
    header opt-ins, default depth).
  - 0x00DDD030's remaining `CScriptedThingStorage::Construct` (q4621): its row-local ctor copy (present at HEAD) differs from 0x007E72A0;
    held for the shared-header port.
  - q6263 (0x00DE1770): the claim is proven (Ego_r + FableWin), but the self-row port fails VC7.1 C3861 "Game: identifier not found" on
    engine/CQ_RansomVictimScript.h while the recipe compiles in three siblings; header item queued.
- Rejected: q5109. The PROPOSAL's fastcall / 5-argument spelling contradicts the landed stdcall self row; the manifest label is fixed
  in NAME_FIXES.
- Pick-any (`logs/pickany_check.txt`): no row newly defines a copy; the only new symbols are the self leaves (`logs/symbol_diff.txt`).
- Link (`logs/link_landing.log`): 6 LNK2005 against 7 at HEAD (`logs/link_control_head.log`). The
  `?EndMission@CV_ChapelOfEvilScript@@QAEXXZ` clash is gone, and no new one appears.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 98 good, 0 bad;
  owner recheck good.
- tablecheck (`logs/tablecheck.txt`): 0x007E72A0 3 -> 2 and 0x006E7B60 2 -> 1; none grows. No out-of-image oracle line added.
