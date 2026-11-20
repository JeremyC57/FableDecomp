# NF194 notes (round 10, script interface / defs / quest script names, third pass)

Handed back after q11915 (context; q11955 not started). Counts from APPLIED.tsv: 50 entries = 18 done, 5 done-already,
1 rejected, 7 blocked, 19 open (q11955 onward, not started). Rows: see STATUS.tsv; rows/ holds every changed row; recheck_final.log
is one vland run over all of them. q6224 (named in the task message) is in neither ENTRIES.tsv nor rows.txt: not worked.

## Families / decisions
- **q5992 ShareData (0x00CE1040), 35 rows.** Every stand-in binder modelled CCountedPointer / CCPPointerInfo in its own namespace
  (EntityScriptBinding_<addr>, HenchmenBinding, ArenaBinding, ExternalRetry_00ce0ef0). Transformer work/sd/xf.py: delete both local
  models, include engine/CCountedPointer.h with CALLED_CTOR + VISIBLE_CALLED_DTOR + the new SHAREDATA_DECL_ONLY (0x00CE0EF0 by hand:
  DTOR_DECL_ONLY, and its namespaced CEntityScriptBase stand-in replaced by engine/CEntityScriptBase.h). Side effects, all listed in
  RENAMES section 2: ~CCountedPointer<CEntityScriptBase> 0x00CE1000 moves from 35 stand-ins to the PDB spelling (pick-any COMDAT
  byte- and reloc-identical to owner row 0x00CE1000 in all 34 objects that emit it; cmpbody); the (T*) ctor 0x00CE10B0 moves from
  one per-row stand-in spelling to another per-row stand-in (::CCountedPointer<NS::CActiveEntityScriptBase>; count-neutral).
  Self leaves: the hidden-result type changes namespace, so each row's leaf changes stand-in -> stand-in (RENAMES s1, leaf-only,
  catalog names unchanged; no landed row binds these vtable-only addresses). Tests left as they were (standalone host models; PASS).
  The 35 rows keep their pre-existing local CActiveEntityScriptBase / CEntityScriptBinding / CQ_* models (SHARED_HEADER_AVAILABLE
  warnings pre-existing; the full port to the 0x00CF66E0 form is a separate item).
- **q6466 / q6518 FadeOutAndKillEntity (0x00CBE716), 13 rows.** work/fk/fx.py: the (thing, bool, float, bool) declaration respelled
  in place to the PDB order (0x00E4A280 / 0x00F04CC0 include engine/fable_script_fade.h instead, CScriptThing is shared there) and every
  call's 2nd / 3rd arguments swapped (same machine code: dl = the bool). 0x00E7E640 only declared it (never called): declaration fixed.
- **q5216 / q12134 / q12135 cast map (0x00CDBFB0 / 0x00CDBF70): blocked family.** NF178's analysis re-read: 0x00D246C0 / 0x00DDD030 destroy
  CCutsceneCastMap views by hand at early returns; a real map object cannot reproduce that until the handles are real objects
  (port onto the ScriptedThingBase / MovieBase headers first). 0x00DC21F0 already at 3 failed forms (NF178). 0 forms spent here.
- **q6386 pGlobalScriptDef: rejected claim + one held row.** PDB spelling is the header's (global, const). Only 0x00EE2A80 still binds
  ?pGlobalScriptDef@@3PAVCScriptDef@@A; its port (work/00ee2a80_held.cpp, fable_string.h + engine/fable_script_def_global.h, Compare as
  the PDB template specialization) grades RELOCATION_MATCH but newly defines the 20-byte ??8?$CBasicString@D@@QBE_NPBD@Z pick-any copy
  vs the 88-byte owner 0x004115A0: held under the pick-any rule. COORDINATOR DECISION: accept it as the existing TEXT_EQ_INLINE debt
  (the same copy is in landed rows such as 0x00D13580), or add an expanded-Compare body opt-in for ??8 first.
- **q11718 / q12068 GetSubDef<CSpecialEffectsDef>: blocked family, not attempted** (33 binders with local def models; NF170 census reused).

## Self renames (RENAMES s1) and their referencers
0x00F0D470, 0x00D15140, 0x00D360E0, 0x0088B7E0, 0x00EA3490, 0x00E0C350, 0x00D1CAC0, 0x00E0A510, 0x0089C100 (+ caller 0x008A7EC0 in
the same hand-back), and 0x00D3CBD0 (catalog module only; leaf already right). Every referencing landed row already bound the new
spelling (reloc_bindings witnesses + REFS census), so each rename narrows a split. 0x00D15140's old leaf ??0CMainGameComponentInit@@QAE@XZ
was also the real CMainGameComponentInit ctor 0x004131A0's symbol (bound by 0x0042EA8F): the rename removes that clash.
Funclets: no EH funclet or other file includes any of the 61 changed rows (grep of rebuild/src/compiled and rebuild/tests for every
changed row's file name, run over all 61). Self renames move these files at
landing: 0x00F0D470, 0x00D15140, 0x00D360E0, 0x0088B7E0, 0x00EA3490, 0x00E0C350, 0x00D1CAC0, 0x00E0A510, and 0x00D3CBD0 (catalog
module changes to NScript::CQ_GuildTrainingScript). 0x00EA3490: rebuild/build_candidates.ps1 still names its module CV_BeardyBaldyScript
while auto-re-candidates.tsv already says NScript::CV_LostTraderScript::WatchForQuestFailure.
Tests of the self-renamed rows were left unchanged (standalone stand-in host models; all PASS in recheck_final.log); only 0x00F0D470's
was read in full.

## Pick-any checks (defined-symbol diff vs HEAD object; tabdef.py / cmpbody.py / cmporc.py)
- 0x00F0D470: new STL COMDATs for vector<CCharString>; the two with table lines equal retail bytes (0x00414DE0 ctor 15 B, 0x00414EA0 dtor
  49 B; 0x00414DE0's landed row is a CIndexBuffer stand-in, not an owner, so compared to the oracle); the other 13 (allocator,
  _STLP_alloc_proxy, _Vector_base, _Destroy / __destroy / __destroy_aux, __false_type, ??_GCCharString, deallocate) have no table line
  and no landed Leaf owner (grep of every one).
- 0x00E0A510: new ??4CTimer@@QAEAAV0@J@Z (no owner row / no table line; retail always expands it).
- q5992 rows: ~CCountedPointer<CEntityScriptBase> identical to 0x00CE1000; copy ctor ??0?$CCountedPointer@VCEntityScriptBase@@@@QAE@ABV0@@Z
  has no owner.
- All other changed rows: symbol diff = self leaf only, or none.

## Header opt-ins (hdr/README.md)
- engine/CCountedPointer.h: FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY (35 q5992 rows).

## Needs header claim
- engine/CV_SwordInTheStoneScript.h: the DEFAULT path declares WatchForDeathDuringSwordPull() with no parameter; the PDB form
  (CScriptThing by value) is only under FABLE_RETAIL_CV_SWORDINTHESTONESCRIPT_ENTITIES. No landed row binds the wrong form (q6464).

## Stand-ins / descriptive names
- No new made-up names. 0x00D1CAC0's descriptive WatchForAllLeaderChargeGruntsKilled replaced by the PDB name. 0x0089C100 / 0x008A7EC0,
  0x00412F90, 0x005DCB00 and 0x00D1CAC0 were respelled in place inside the row-local models they already had (no new class token).
- No stand-in was cast to a real type.

## Flags lines
None added.

## Stale reloc_bindings lines (NAME_FIXES)
?g_PlayAnimationDefaultFlag@@3_NA (0x00D08790) and ?g_bScriptAnimationFlag@@3_NA (0x00D7F460): both objects bind the PDB spelling.

## Rule slips
- Two Bash calls failed: a `wc` over the NF190 directory (exit 1), and a pipeline whose file list came back empty ran `cat` on stdin and
  hung until moved to the background (stopped with TaskStop; no file written).
- Read-only compiles of owner / witness / control rows before reading their locks: 0x00CE1000, 0x00414DE0, 0x00414EA0, 0x00E15500,
  0x00D3CBD0, 0x00ED7320, 0x00D08790, 0x00D7F460, 0x0058578B (into my pO dir; none is acctB or another agent's; none edited).
- Locks: 0x00F0CAA0 (NF192) and 0x00D1A740 (acctB) are REFS rows of my entries; both already bind the right spellings, neither was edited.

## Locks
Claimed beyond rows.txt: 0x00EE2A80 (held draft only), 0x00D71B30 0x00D701C0 0x00E0F4E0 0x00E158F0 0x00D37530 0x00E8DFA0 0x00F051D0
0x00F04CC0 0x00F045E0 0x00E7E640 0x00E49980 (q6466 family, all ported).

## Gate notes
- 35 q5992 self-leaf changes are stand-in -> stand-in (hidden-result type namespace); 0x00CE10B0 callee spelling moves stand-in -> stand-in
  per row (count-neutral). 0x00E0A510 adds FABLE_FORCEINLINE CTimer::operator= after its include, copied from the same-TU 0x00E07640.
- Two of the 61 rows change no binding: 0x00E7E640 (dead declaration fixed) and 0x00D3CBD0 (Leaf line + catalog module only).
