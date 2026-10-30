# Retail class-model migration, wave 9 (2026-10-30): 1,578 landed rows onto shared retail headers, bytes unchanged

Brief BRIEF.md; per-row ROWS.tsv; REVERTED.tsv (176 family edits, each from its own pre-image); HEADERS.md; NEXT.md (wave 10
counts and plan; the CCountedPointer remainder by shape); LESSONS.md; NAME_FIXES.tsv (5); bind_add.tsv (empty: every new
spelling was already bound to the same address); cp_deferred.tsv (1,317 CP rows proven SAME / same-target but restored to
keep the wave in budget); logs/ (recheck1..5.log, mcheck and apply logs); scripts/.

Header added: `engine/CCountedPointer.h` (CCPPointerInfo 12 bytes {RefCount, DeleteFunc __fastcall, Data}, ctor
`??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z`; CCountedPointer<T> 8 bytes {Data, Info} with the full FableWin member set incl. the
converting ctor / operator= templates and SetStaticCast; DeleteData is `static __fastcall delete static_cast<T*>(p)`, proven
by 0x00CDEE00 / 0x00674E80). The dtor and ReleaseData are separate bodies: the dtor zeroes Data then Info (732 out-of-line
COMDATs in retail), ReleaseData zeroes Info then Data and is never out of line; Reset 0x008AB2B0 / ShareData 0x00CE1040 are
ReleaseData expanded with dead stores removed. Three dtor models were compared on 955 rows; the one with the right callee
names (15 DIFFER) was chosen over the one matching more bytes with a wrong `ReleaseData` callee. Unproven: Reset's store
order; the bodies of GetReferenceCount / Get / IsNull (FableWin names only). No existing header changed.
Rows per family: CP 1,548 (local pointer model replaced by the header, rows under 00/d* and 00/e*; 530 unused
`Destroy_<addr>` deleter declarations removed), CPMP 12 (const-only CPlayerManager rows reach the QBE GetMainPlayer
0x00449960 through FableWin's `CMainGameComponent::PeekPlayerManager() const`; wave 8 had to revert these), CPMX 13
(pointer-returning stand-ins respelled to the header's reference accessors), CPS 5 (SetStaticCast proof), MB 1.
Agent recheck 1,578/1,578 PASS (1,515 RELOCATION_MATCH, 63 MATCH); mcheck 1,041 SAME + 537 RELOC_RENAMED (530 `Destroy_<addr>`
-> `DeleteData<T>`, 7 CPlayerManager accessor spellings, 1 `$L`), all same target; payload_check 0 stale; multi-check 0.
Coordinator: clean cherry-pick onto 024a33d5b; independent spot regrade 181/181 PASS (all 31 non-CP rows + 150 random CP rows;
coordinator_spot_recheck.log). 5 NAME_FIXES rows queued for naming wave 12 (prefix mig9: 0x00CDEE00 `Destroy_00cdee00` ->
`DeleteData<CEntityScriptBase>` in 61 rows; 0x0042F974 / 0x005C04BF / 0x005C0563 / 0x005C0511 are bound as ReleaseData but
their bytes are the dtor).
Reverted 176: CPA 85 (CreateActiveEntityScript: retail calls the (T*) ctor 0x00CE10B0, ShareData 0x00CE1040 and dtor
0x00CE1000 out of line, VC7.1 expands all three on the shared header, 347v272), ST2 40 (wave 8 ScriptThing residue retry, all
fail to compile), CP 27 (a called operator= 0x00CF8970/0x00CF8A70/0x00D36620/0x00D374A0, a called Reset in MovieBase::Release
0x00CF8850, ten per-site differences, the retail_asm row 0x00E81B40), MB 19 (header-shaped MovieBase rows DIFFER), CPMX 4
(non-const call sites would bind the QAE GetMainPlayer 0x00449970).
Open: `WARN SHARED_HEADER_AVAILABLE CCountedPointer` now prints for the 2,843 rows still defining a local pointer class
(debt signal; strict mode would refuse them; new chunk rows should include the header, CreateActiveEntityScript excepted);
MovieBase destruction form follows ctor expansion in the same function (35/35 vs 26/26: a per-function budget, unproven);
CGSI / CTimer / CScriptBase not reached.
Remaining local models: CCountedPointer 2,843, CScriptThing 428, CEntityScriptBase 293, CGameScriptInterface 220,
CIsThingAlive 154, CPredicateBase 142, CScriptBase 88, MovieBase 84, CPlayerManager 79, CIsThingConscious 26, CTimer 16.
