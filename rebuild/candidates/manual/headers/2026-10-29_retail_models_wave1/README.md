# Retail class-model migration, wave 1 (2026-10-29/30): 613 landed rows onto shared retail headers, bytes unchanged

Owner rule: a class with a retail vtable is modelled with its real virtuals from a shared header, never a row-local
stand-in. Agent brief BRIEF.md; per-row ROWS.tsv (family, change, grade before/after, byte check vs HEAD); REVERTED.tsv (8);
HEADERS.md (evidence per slot and member, unproven items); NEXT.md (wave 2 plan and counts).

Headers (rebuild/include/engine/): new fable_nscript_forward.h, CGameScriptInterfaceBase.h (917/918 slots probe-matched),
CScriptBase.h, CLevelScriptBase.h, CEntityScriptBase.h, CTimer.h, CPredicateBase.h, CIsThingAlive.h, CPredicate_And.h,
CEntityScriptBindingBase.h, CEntityScriptBinding.h, CSubDefClassBase.h; retail opt-in branches (FABLE_RETAIL_NSCRIPT) in
CGameScriptInterface.h and CScriptThing.h (78/78 slots); CBaseClass.h rewritten; CResource.h / CDefPointeeBase.h /
CIVCountedPointeeBase.h touched up. **Correction:** retail CDefClassBase is 0x25 bytes packed, not 0x28 (landed
CDegradableDef::operator= 0x00454434 copies a bool at +0x25 then a dword at +0x28); the earlier 0x28 evidence could not
distinguish the two. Slot 14 is now proven the const GetPSubDefInfo. Interface slot 308 AddLogBookEntry takes a `long`
stand-in for a nested enum that cannot be forward-declared.

Rows by family: chunk-579-flagged 7, DEF-HE 35, local CSpawnedFuncBase 331, Part A 8, local CDefClassBase 125, trivial local
CBaseClass 105, HEAD opt-in rows re-aligned 2. Agent recheck 613/613 PASS (9 MATCH, 604 RELOCATION_MATCH).
Coordinator independent spot regrade after cherry-pick onto 8e6501ca5+: 75/75 PASS (coordinator_spot_recheck.log);
bindings_multi_check 0; no binding changes (two vtable renames already bound to 0x01230BA0).

Reverted: 00461d2c, 004e1225, 004e6f0b, 00675c00 (do not compile on the shared headers: abstract derived def, second Copy
spelling, local CVectorMap clashes), 004e02f2 / 004e5f51 (bytes differ), 0067c980 (same bytes, leaf must respell QAE->UAE
first), 00f05f30 (partial: keeps local CScriptThing / CEntityScriptBase; retail inlines the copy at one site and calls
0x004ABE90 at another).

Unproven (HEADERS.md): base access of CEntityScriptBase::Main; `novtable` on the binding base (inferred); CScriptBase slot 1
RegisterMain (by elimination); ~CGameScriptInterface body address. Open for naming: invented reloc spellings
??0CScriptThing@@QAE@PAX@Z, ??1CScriptThing@@QAE@XZ, ?IsActiveThreadTerminating@CEntityScriptBase@@QAE_NXZ (PDB: QBE),
the SetPredicate variants, ??0CPredicateBase_00c26c10; 0x0143E8F8 carries 7 names.
